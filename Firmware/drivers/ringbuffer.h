#ifndef __RINGBUFFER_H
#define __RINGBUFFER_H

#include "stm32f1xx_hal.h"
#include <rtthread.h>

#define rt_ringbuffer_space_len(rb) ((rb)->buffer_size - rt_ringbuffer_data_len(rb))

struct rt_ringbuffer
{
  rt_uint8_t *buffer_ptr;

  rt_uint16_t read_mirror : 1;
  rt_uint16_t read_index : 15;
  rt_uint16_t write_mirror : 1;
  rt_uint16_t write_index : 15;

  rt_int16_t buffer_size;
};

enum rt_ringbuffer_state
{
  RT_RINGBUFFER_EMPTY,
  RT_RINGBUFFER_FULL,
  /* half full is neither full nor empty */
  RT_RINGBUFFER_HALFFULL,
};

/**
 * RingBuffer for DeviceDriver
 *
 * Please note that the ring buffer implementation of RT-Thread
 * has no thread wait or resume feature.
 */
void rt_ringbuffer_init(struct rt_ringbuffer *rb, rt_uint8_t *pool, rt_int16_t size);
void rt_ringbuffer_reset(struct rt_ringbuffer *rb);
rt_size_t rt_ringbuffer_put(struct rt_ringbuffer *rb, const rt_uint8_t *ptr, rt_uint16_t length);
rt_size_t rt_ringbuffer_put_force(struct rt_ringbuffer *rb, const rt_uint8_t *ptr, rt_uint16_t length);
rt_size_t rt_ringbuffer_putchar(struct rt_ringbuffer *rb, const rt_uint8_t ch);
rt_size_t rt_ringbuffer_putchar_force(struct rt_ringbuffer *rb, const rt_uint8_t ch);
rt_size_t rt_ringbuffer_get(struct rt_ringbuffer *rb, rt_uint8_t *ptr, rt_uint16_t length);
rt_size_t rt_ringbuffer_getchar(struct rt_ringbuffer *rb, rt_uint8_t *ch);
rt_size_t rt_ringbuffer_data_len(struct rt_ringbuffer *rb);

#ifdef RT_USING_HEAP
struct rt_ringbuffer* rt_ringbuffer_create(rt_uint16_t length);
void rt_ringbuffer_destroy(struct rt_ringbuffer *rb);
#endif

rt_inline rt_uint16_t rt_ringbuffer_get_size(struct rt_ringbuffer *rb)
{
    RT_ASSERT(rb != RT_NULL);
    return rb->buffer_size;
}

/** return the size of empty space in rb */
#define rt_ringbuffer_space_len(rb) ((rb)->buffer_size - rt_ringbuffer_data_len(rb))




#define N 1024
typedef struct
{
  uint8_t buffer[N];       //buffer
  volatile uint16_t w_ptr; //
  volatile uint16_t r_ptr; //
  volatile uint16_t cap;   //
} fifo_buffer;

uint8_t ring_readbyte(fifo_buffer *p);

void ring_writebyte(fifo_buffer *p, uint8_t data);

void ring_readdata(fifo_buffer *p, uint8_t *data, uint16_t len);

void ring_writedata(fifo_buffer *p, uint8_t *data, uint16_t len);

#endif
