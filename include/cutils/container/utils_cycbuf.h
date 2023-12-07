#ifndef H_UTILS_CYCBUF_H
#define H_UTILS_CYCBUF_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct util_cycbuf_s {
    uint8_t  flag;
    uint8_t* buffer;  /* buffer pointer */
    int      size;    /* buffer size */
    int      w_pos;   /* write pos */
    int      r_pos;   /* read pos */
};

typedef struct util_cycbuf_s util_cycbuf_t;

int     util_cycbuf_create(util_cycbuf_t* cycbuf, uint32_t max_size, uintptr_t *buf_memory);
int     util_cycbuf_destroy(util_cycbuf_t* cycbuf);
int     util_cycbuf_size(util_cycbuf_t* cycbuf);
int     util_cycbuf_push(util_cycbuf_t* cycbuf, void* data, uint32_t len);
int     util_cycbuf_pop(util_cycbuf_t* cycbuf, void* buf, uint32_t len);
int     util_cycbuf_peek(util_cycbuf_t* cycbuf, void* buf, uint32_t len);
int     util_cycbuf_lose(util_cycbuf_t* cycbuf, uint32_t len);
void    util_cycbuf_print(util_cycbuf_t* cycbuf);

#ifdef __cplusplus
}
#endif

#endif // H_UTILS_CYC_BUFFER_H
