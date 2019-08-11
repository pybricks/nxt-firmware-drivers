/* Byte FIFO queue implementation.
 *
 * Implements a simple byte FIFO on top of a data buffer, that lets
 * you enqueue and dequeue single bytes.
 */

#ifndef __BFIFO_H__
#define __BFIFO_H__

#include <stdint.h>

/* The byte fifo control structure. */
struct byte_fifo {
  /* The buffer containing the fifo. */
  uint8_t *buffer;
  /* Pointer to the end of the buffer. */
  uint8_t *buffer_end;
  /* Size in bytes of the buffer. */
  uint32_t buffer_size;

  /* Current head (freshest data) of the fifo in the buffer. */
  uint8_t *head;
  /* Current tail (oldest data) of the fifo in the buffer. */
  uint8_t *tail;
  /* Number of bytes currently enqueued in the fifo. */
  uint32_t holding;
};

void byte_fifo_clear(struct byte_fifo *f);
void byte_fifo_init(struct byte_fifo *f, uint8_t *buffer, uint32_t buff_size);
int byte_fifo_put(struct byte_fifo *f, uint32_t force, uint8_t b);
int byte_fifo_get(struct byte_fifo *f, uint8_t *b);

#endif
