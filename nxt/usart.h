/**
 * Generic usart interface routines
 */
// Bit values used for events
#define US_READABLE   0x1
#define US_WRITEABLE  0x2
#define US_WRITEEMPTY 0x4

#define BUF_CNT 2
typedef struct
{
  AT91S_PDC *dma;
  AT91S_USART *dev;
  uint8_t *in_buf[BUF_CNT];
  uint8_t *out_buf[BUF_CNT];
  int in_offset;
  int in_size;
  int out_size;
  uint8_t in_base;
  uint8_t out_base;
} usart;

usart *usart_allocate(AT91S_USART *dev, AT91S_PDC *dma, int inSz, int outSz);
void usart_enable(usart *us);
void usart_disable(usart *us);
void usart_free(usart *us);
uint32_t usart_status(usart *us);
uint32_t usart_write(usart *us, uint8_t *buf, uint32_t off, uint32_t len);
uint32_t usart_read(usart *us, uint8_t * buf, uint32_t off, uint32_t length);
uint8_t * usart_get_write_buffer(usart *us);
void usart_write_buffer(usart *us, uint32_t len);





