
#include "uart.h"
#include "interrupts.h"
#include "at91sam7.h"

#include "byte_fifo.h"

#include "aic.h"
#include "irq.h"



#define N_UARTS      1
#define TX_FIFO_SIZE 64
#define RX_FIFO_SIZE 32

struct soft_uart {
#ifndef UART_POLLED
  struct byte_fifo tx;
  struct byte_fifo rx;
  int transmitting;
  int sending_break;
  int tx_total;
  int rx_total;
#endif
  struct _AT91S_USART *uart;
};

#ifndef UART_POLLED
static uint8_t tx_buffer[N_UARTS][TX_FIFO_SIZE];
static uint8_t rx_buffer[N_UARTS][RX_FIFO_SIZE];
#endif

struct soft_uart uart[N_UARTS];



int
uart_get_byte(uint32_t u, uint8_t *b)
{

  int ret_val;
  int i_state;
  struct soft_uart *p;
  volatile struct _AT91S_USART *up;

  if (u >= N_UARTS)
    return -1;

  p = &uart[u];
  up = p->uart;

  i_state = interrupts_get_and_disable();

#ifndef UART_POLLED
  ret_val = byte_fifo_get(&p->rx, b);
#else
#endif

  if (i_state)
    interrupts_enable();

  return ret_val;

}

int
uart_put_byte(uint32_t u, uint8_t b)
{
  int ret_val;
  int i_state;
  struct soft_uart *p;
  volatile struct _AT91S_USART *up;

  if (u >= N_UARTS)
    return -1;

  p = &uart[u];
  up = p->uart;

  i_state = interrupts_get_and_disable();

#ifndef UART_POLLED
  ret_val = byte_fifo_put(&p->tx, 0, b);

  /* Enable transmitting and the transmit interrupt.
   * This will allow the UART tx chain to wake up and 
   * pick up the next byte for tx, if it was stopped.
   */
  p->transmitting = 1;
  up->US_IER = 0x02;
#else
#  warning Polled UART not supported!
#endif

  if (i_state)
    interrupts_enable();

  return ret_val;
}

void
uart_put_str(uint32_t u, const uint8_t *str)
{
  while (*str) {
    while (!uart_put_byte(u, *str)) {
    }
    str++;
  }
}

int
uart_set_break(uint32_t u)
{
  struct soft_uart *p;
  volatile struct _AT91S_USART *up;

  if (u >= N_UARTS)
    return 0;

  p = &uart[u];
  up = p->uart;
  up->US_CR = 0x200;		// set break;
  p->sending_break = 0;

  return 1;
}

int
uart_clear_break(uint32_t u)
{
  struct soft_uart *p;
  volatile struct _AT91S_USART *up;

  if (u >= N_UARTS)
    return 0;

  p = &uart[u];
  up = p->uart;

  up->US_CR = 0x400;		// stop break;
  p->sending_break = 0;
  return 1;
}


#ifndef UART_POLLED
static void
uart_process_isr(uint32_t u)
{
  struct soft_uart *p;
  volatile struct _AT91S_USART *up;

  uint8_t status;
  uint8_t b;

  if (u >= N_UARTS)
    return;

  p = &uart[u];
  up = p->uart;

  while (((status = up->US_CSR) & ((p->transmitting) ? 0x03 : 0x01)) != 0) {

    up->US_CR = 0x100;		/* clear error bits */

    if (status & 1) {
      /* Receiver holding. */
      b = up->US_RHR;
      byte_fifo_put(&p->rx, 1, b);
      p->rx_total++;

    }

    if (status & 2) {
      /* Transmitter empty */
      if (byte_fifo_get(&p->tx, &b)) {
	up->US_THR = b;
	p->tx_total++;
      } else {
	/* Turn off transmitting */
	up->US_IDR = 0x02;
	p->transmitting = 0;
      }
    }
  }
}


void
uart_isr_C_0(void)
{
  uart_process_isr(0);
}

void
uart_isr_C_1(void)
{
  uart_process_isr(1);
}
#endif

static int
uart_calc_divisor(int baudRate)
{

  return (CLOCK_FREQUENCY / baudRate + 8) / 16;
}




int
uart_CheckBreak(uint32_t u)
{
#if 0
  int isBreak;
  struct soft_uart *p;
  volatile struct _AT91S_USART *up;

  if (u >= N_UARTS)
    return 0;

  p = &uart[u];
  up = p->uart;

  up->US_CR = 0x100;		// clear status flags

  systimer_WaitMilliseconds(5);

  isBreak = (up->US_CSR & 4) ? 1 : 0;

  return isBreak;
#else
  return 0;
#endif
}



int
uart_init(uint32_t u, uint32_t baudRate, uint32_t dataBits, uint32_t stopBits, char parity)
{

  struct soft_uart *p = &uart[u];
  volatile struct _AT91S_USART *up;
  int i_state;
  uint32_t peripheral_id;
  uint32_t mode;
  uint8_t dummy;
  void (*isr)(void);
  uint32_t pinmask = 0;
  int error = 0;

  if (u >= N_UARTS)
    return 0;

  p = &uart[u];

  /* Initialise the uart structure */
  switch (u) {
  case 0:
    p->uart = AT91C_BASE_US0;
    peripheral_id = AT91C_ID_US0;
    pinmask = (1 << 5) | (1 << 6);
    isr = uart_isr_entry_0;
    break;
  case 1:
    p->uart = AT91C_BASE_US1;
    peripheral_id = AT91C_ID_US1;
    pinmask = (1 << 21) | (1 << 22);	// todo
    isr = uart_isr_entry_1;
    break;
  default:
    return 0;
  }
  byte_fifo_init(&p->tx, &tx_buffer[u][0], TX_FIFO_SIZE);
  byte_fifo_init(&p->rx, &rx_buffer[u][0], RX_FIFO_SIZE);
  p->transmitting = 0;


  up = p->uart;


  mode = 0;
  switch (dataBits) {
  case 7:
    mode |= 0x80;
    break;
  case 8:
    mode |= 0xc0;
    break;
  default:
    error = 1;
  }

  switch (stopBits) {
  case 1:
    mode |= 0x00000000;
    break;
  case 15:
    mode |= 0x00001000;
    break;
  case 2:
    mode |= 0x00002000;
    break;
  default:
    error = 1;
  }

  switch (parity) {
  case 'N':
    mode |= 0x00000800;
    break;
  case 'O':
    mode |= 0x00000200;
    break;
  case 'E':
    mode |= 0x00000000;
    break;
  case 'M':
    mode |= 0x00000600;
    break;
  case 'S':
    mode |= 0x00000400;
    break;
  default:
    error = 1;
  }

  if (error)
    return 0;

  i_state = interrupts_get_and_disable();



  /* Grab the clock we need */
  *AT91C_PMC_PCER = (1 << AT91C_ID_PIOA);	/* Need PIO too */
  *AT91C_PMC_PCER = (1 << peripheral_id);

  /* Grab the pins we need */
  *AT91C_PIOA_PDR = pinmask;
  *AT91C_PIOA_ASR = pinmask;

  up->US_CR = 0x5AC;		// Disable 
  up->US_MR = mode;
  up->US_IDR = 0xFFFFFFFF;
  up->US_BRGR = uart_calc_divisor(baudRate);	// rw Baud rate generator
  up->US_RTOR = 0;		// rw Receiver timeout
  up->US_TTGR = 0;		// rw Transmitter time guard
  up->US_RPR = 0;		// rw Receiver pointer
  up->US_RCR = 0;		// rw Receiver counter
  up->US_TPR = 0;		// rw Transmitter pointer
  up->US_TCR = 0;		// rw Transmitter counter

  // Set up UART interrupt


  aic_mask_off(peripheral_id);
  aic_set_vector(peripheral_id, AIC_INT_LEVEL_NORMAL, isr);
  aic_mask_on(peripheral_id);


  // Finally enable the UART
  up->US_CR = 0x50;		// enable tx and rx
  dummy = up->US_RHR;		// dummy read.
  up->US_IER = 1;		//Enable rx and tx interrupts. This should cause a bogus tx int

  if (i_state)
    interrupts_enable();

  return 1;
}

void
uart_close(uint32_t u)
{
  /* Nothing */
}
