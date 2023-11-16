#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "utils_cycbuf.h"

static uint32_t cycbuf_size(util_cycbuf_t* cycbuf)
{
    uint32_t size = 0;

    if (cycbuf->w_pos >= 0) {
        size = (cycbuf->w_pos > cycbuf->r_pos?
                cycbuf->w_pos - cycbuf->r_pos:
                cycbuf->size - cycbuf->r_pos + cycbuf->w_pos);
    }

    return size;
}

static uint32_t cycbuf_remain(util_cycbuf_t* cycbuf)
{
    return (cycbuf->size - cycbuf_size(cycbuf));
}

static void cycbuf_write(util_cycbuf_t* cycbuf, void* data, uint32_t len)
{
    uint32_t writableLen = 0;
    uint32_t overFlowLen = 0;

    /* init pointer */
    if (cycbuf->w_pos < 0 || cycbuf->r_pos < 0) {
        cycbuf->w_pos = cycbuf->r_pos = 0;
    }

    writableLen = len < (cycbuf->size - cycbuf->w_pos) ? len : (cycbuf->size - cycbuf->w_pos);
    overFlowLen = len - writableLen;

    memcpy(cycbuf->buffer + cycbuf->w_pos, data, writableLen);

    if (overFlowLen > 0) {
        memcpy(cycbuf->buffer, (uint8_t*)data + writableLen, overFlowLen);
        cycbuf->w_pos = overFlowLen;
    }
    else {
        cycbuf->w_pos += writableLen;
    }

    /* reset w_pos */
    if (cycbuf->w_pos >= cycbuf->size) {
        cycbuf->w_pos = 0;
    }
}

static uint32_t cycbuf_read(util_cycbuf_t* cycbuf, void* buf, uint32_t len, bool remove)
{
    uint32_t readableLen = 0;
    uint32_t overFlowLen = 0;

    readableLen = len < (cycbuf->size - cycbuf->r_pos) ? len : (cycbuf->size - cycbuf->r_pos);
    overFlowLen = len - readableLen;

    if (NULL != buf) {
        memcpy(buf, cycbuf->buffer + cycbuf->r_pos, readableLen);
    }

    if (overFlowLen > 0) {
        if (NULL != buf) {
            memcpy(buf + readableLen, cycbuf->buffer, overFlowLen);
        }

        if (remove) {
            cycbuf->r_pos = overFlowLen;
        }
    }
    else if (remove) {
        cycbuf->r_pos += readableLen;
    }

    /* reset r_pos */
    if (remove) {
        if (cycbuf->r_pos >= cycbuf->size) {
            cycbuf->r_pos = 0;
        }

        if (cycbuf->r_pos == cycbuf->w_pos) {
            cycbuf->r_pos = cycbuf->w_pos = -1;
        }
    }
}

int util_cycbuf_create(util_cycbuf_t* cycbuf, uint32_t max_size, uintptr_t *buf_memory)
{
    uint8_t *temp_buf = NULL;
    int status = 0;

    if (NULL == cycbuf || 0 == max_size) {
        return -1;
    }

    if (NULL == buf_memory) {
        cycbuf->flag = 0;
        temp_buf = (uint8_t*)malloc(max_size);
    }
    else {
        cycbuf->flag = 1;
        temp_buf = (uint8_t*)buf_memory;
    }
    
    if (NULL != temp_buf) {
        cycbuf->buffer  = temp_buf;
        cycbuf->size    = max_size;
        cycbuf->r_pos   = -1;
        cycbuf->w_pos   = -1;
    }
    else {
        status  = -1;
        printf("cycbuf memory allocation failed\n");
    }

    return status;
}

int util_cycbuf_destroy(util_cycbuf_t* cycbuf)
{
    uint8_t *temp_buf = NULL;

    if (NULL == cycbuf) {
        return -1;
    }

    temp_buf = cycbuf->buffer;
    cycbuf->buffer = NULL;

    if (0 == cycbuf->flag) {
        free(temp_buf);
    }

    return 0;
}

int util_cycbuf_size(util_cycbuf_t* cycbuf)
{
    if (NULL == cycbuf) {
        return -1;
    }

    return cycbuf_size(cycbuf);
}

int util_cycbuf_push(util_cycbuf_t* cycbuf, void* data, uint32_t len)
{
    if (NULL == cycbuf || NULL == cycbuf->buffer || NULL == data) {
        return -1;
    }

    uint32_t remain = cycbuf_remain(cycbuf);

    if (len > remain) {
        len = remain;
    }

    if (len > 0) {
        cycbuf_write(cycbuf, data, len);
    }

    return len;
}

int util_cycbuf_pop(util_cycbuf_t* cycbuf, void* buf, uint32_t len)
{
    if (NULL == cycbuf || NULL == cycbuf->buffer || NULL == buf) {
        return -1;
    }

    uint32_t size = cycbuf_size(cycbuf);

    if (len > size) {
        len = size;
    }

    if (len > 0) {
        cycbuf_read(cycbuf, buf, len, true);
    }

    return len;
}

int util_cycbuf_peek(util_cycbuf_t* cycbuf, void* buf, uint32_t len)
{
    if (NULL == cycbuf || NULL == cycbuf->buffer || NULL == buf) {
        return -1;
    }

    uint32_t size = cycbuf_size(cycbuf);

    if (len > size) {
        len = size;
    }

    if (len > 0) {
        cycbuf_read(cycbuf, buf, len, false);
    }

    return len;
}

int util_cycbuf_lose(util_cycbuf_t* cycbuf, uint32_t len)
{
    if (NULL == cycbuf || NULL == cycbuf->buffer) {
        return -1;
    }

    uint32_t size = cycbuf_size(cycbuf);

    if (len > size) {
        len = size;
    }

    if (len > 0) {
        cycbuf_read(cycbuf, NULL, len, true);
    }

    return len;
}

void util_cycbuf_print(util_cycbuf_t* cycbuf)
{
    if (NULL == cycbuf || NULL == cycbuf->buffer) {
        return;
    }

    printf("util_cycbuf_print: r_pos=%d w_pos=%d size=%d\n", cycbuf->r_pos, cycbuf->w_pos, util_cycbuf_size(cycbuf));
    for (int i = 0; i < util_cycbuf_size(cycbuf); i++) {
        printf("%c ", cycbuf->buffer[(cycbuf->r_pos+i)%cycbuf->size]);
    }
    printf("\n");
}
