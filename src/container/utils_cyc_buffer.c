#include "utils_cyc_buffer.h"

static uint32_t __ringbuf_rounddown_pow_of_two(uint32_t n)
{
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    return (n + 1) >> 1;
}

void util_cycbuf_init(util_cyc_buf_t* cycbuf, uint32_t max_size)
{
    ring->b    = buf;
    ring->esz  = esize;
    ring->m    = __ringbuf_rounddown_pow_of_two(bsize / esize) - 1;
    ring->w    = 0;
    ring->r    = 0;
}

uint32_t util_cycbuf_push(util_cyc_buf_t* cycbuf, void* data, uint32_t size)
{

}

uint32_t util_cycbuf_pop(util_cyc_buf_t* cycbuf, void* buf, uint32_t len)
{

}

uint32_t util_cycbuf_peek(util_cyc_buf_t* cycbuf, void* buf, uint32_t len)
{

}

uint32_t util_cycbuf_size(util_cyc_buf_t* cycbuf)
{

}

uint32_t util_cycbuf_lose(util_cyc_buf_t* cycbuf, uint32_t len)
{

}
