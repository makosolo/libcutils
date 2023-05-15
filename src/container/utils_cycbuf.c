#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "utils_cycbuf.h"

static uint32_t cycbuf_size(util_cycbuf_t* cycbuf)
{
    return (cycbuf->w_pos >= cycbuf->r_pos? 
           cycbuf->w_pos - cycbuf->r_pos: 
           cycbuf->size - cycbuf->r_pos + cycbuf->w_pos);
}

static uint32_t cycbuf_avail(util_cycbuf_t* cycbuf)
{
    return (cycbuf->size - cycbuf_size(cycbuf));
}

static uint32_t cycbuf_write(util_cycbuf_t* cycbuf, void* data, uint32_t len)
{
    uint32_t l = len < (cycbuf->size - cycbuf->w_pos) ? len : (cycbuf->size - cycbuf->w_pos);
    uint32_t pos = 0;

    memcpy(cycbuf->buffer + cycbuf->w_pos, data, l);
    if (len - l > 0) {
        memcpy(cycbuf->buffer, (uint8_t*)data + l, len - l);
        pos = len - l;
    }
    else {
        pos = cycbuf->r_pos + l;
    }
    
    return pos;
}

static uint32_t cycbuf_read(util_cycbuf_t* cycbuf, void* buf, uint32_t len)
{
    uint32_t l = cycbuf->w_pos - cycbuf->r_pos;
    uint32_t pos = 0;

    if (len > l) {
        len = l;
    }

    l = len < (cycbuf->size - cycbuf->r_pos) ? len : (cycbuf->size - cycbuf->r_pos);

    memcpy(buf, cycbuf->buffer + cycbuf->r_pos, l);
    if (len - l > 0) {
        memcpy(buf + l, cycbuf->buffer, len - l);
        pos = len - l;
    }
    else {
        pos = cycbuf->r_pos + l;
    }
    
    return pos;
}

int util_cycbuf_init(util_cycbuf_t* cycbuf, uint32_t max_size)
{
    if (NULL == cycbuf) {
        return -1;
    }

    cycbuf->buffer  = (char *)malloc(max_size);
    cycbuf->size    = max_size;
    cycbuf->r_pos   = 0;
    cycbuf->w_pos   = 0;

    return (NULL != cycbuf->buffer)? 0: -1;
}

void util_cycbuf_deinit(util_cycbuf_t* cycbuf)
{
    if (NULL == cycbuf || NULL == cycbuf->buffer) {
        return;
    }

    free(cycbuf->buffer);
}

uint32_t util_cycbuf_size(util_cycbuf_t* cycbuf)
{
    if (NULL == cycbuf) {
        return 0;
    }

    return cycbuf_size(cycbuf);
}

uint32_t util_cycbuf_push(util_cycbuf_t* cycbuf, void* data, uint32_t len)
{
    if (NULL == cycbuf || NULL == cycbuf->buffer || NULL == data) {
        return 0;
    }

    uint32_t avail = cycbuf_avail(cycbuf);

    if (len > avail) {
        len = avail;
    }

    cycbuf->w_pos = cycbuf_write(cycbuf, data, len);

    return len;
}

uint32_t util_cycbuf_pop(util_cycbuf_t* cycbuf, void* buf, uint32_t len)
{
    if (NULL == cycbuf || NULL == cycbuf->buffer || NULL == buf) {
        return 0;
    }

    uint32_t l = cycbuf->w_pos - cycbuf->r_pos;

    if (len > l) {
        len = l;
    }

    cycbuf->r_pos = cycbuf_read(cycbuf, buf, len);

    return len;
}

uint32_t util_cycbuf_peek(util_cycbuf_t* cycbuf, void* buf, uint32_t len)
{
    if (NULL == cycbuf || NULL == cycbuf->buffer || NULL == buf) {
        return 0;
    }

    uint32_t l = cycbuf->w_pos - cycbuf->r_pos;

    if (len > l) {
        len = l;
    }

    cycbuf_read(cycbuf, buf, len);

    return len;
}

uint32_t util_cycbuf_lose(util_cycbuf_t* cycbuf, uint32_t len)
{
    if (NULL == cycbuf || NULL == cycbuf->buffer) {
        return 0;
    }

    uint32_t avail = cycbuf_avail(cycbuf);

    if (len > avail) {
        len = avail;
    }

    if (cycbuf->w_pos > cycbuf->r_pos) {
        cycbuf->r_pos += len;
    }
    else {
        cycbuf->r_pos += len - (cycbuf->size - cycbuf->r_pos);
    }
    
    return len;
}
