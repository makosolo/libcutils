#ifndef H_UTILS_CYCBUF_H
#define H_UTILS_CYCBUF_H

struct util_cycbuf_s;
typedef struct util_cycbuf_s util_cycbuf_t;

util_cycbuf_t*  util_cycbuf_create(uint32_t max_size);
void            util_cycbuf_destroy(util_cycbuf_t* cycbuf);
uint32_t        util_cycbuf_size(util_cycbuf_t* cycbuf);
uint32_t        util_cycbuf_push(util_cycbuf_t* cycbuf, void* data, uint32_t len);
uint32_t        util_cycbuf_pop(util_cycbuf_t* cycbuf, void* buf, uint32_t len);
uint32_t        util_cycbuf_peek(util_cycbuf_t* cycbuf, void* buf, uint32_t len);
uint32_t        util_cycbuf_lose(util_cycbuf_t* cycbuf, uint32_t len);
void            util_cycbuf_print(util_cycbuf_t* cycbuf);

#endif // H_UTILS_CYC_BUFFER_H
