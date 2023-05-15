#ifndef H_UTILS_CYCBUF_H
#define H_UTILS_CYCBUF_H

struct util_cycbuf_s {
	char* b;
	uint32_t w;   /* write pos */
	uint32_t r;   /* read pos */
	uint32_t m;   /* mask */
	uint32_t esz; /* entry size */
} util_cycbuf_t;

void     util_cycbuf_init(util_cyc_buf_t* cycbuf, uint32_t max_size);
uint32_t util_cycbuf_push(util_cyc_buf_t* cycbuf, void* data, uint32_t size);
uint32_t util_cycbuf_pop(util_cyc_buf_t* cycbuf, void* buf, uint32_t len);
uint32_t util_cycbuf_peek(util_cyc_buf_t* cycbuf, void* buf, uint32_t len);
uint32_t util_cycbuf_size(util_cyc_buf_t* cycbuf);
uint32_t util_cycbuf_lose(util_cyc_buf_t* cycbuf, uint32_t len);

#endif // H_UTILS_CYC_BUFFER_H
