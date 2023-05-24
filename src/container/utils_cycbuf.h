#ifndef H_UTILS_CYCBUF_H
#define H_UTILS_CYCBUF_H

typedef struct util_cycbuf_s {
	char*    buffer;  /* buffer pointer */
    int      size;    /* buffer size */
	int 	 w_pos;   /* write pos */
	int 	 r_pos;   /* read pos */
} util_cycbuf_t;

int      util_cycbuf_init(util_cycbuf_t* cycbuf, uint32_t max_size);
void     util_cycbuf_deinit(util_cycbuf_t* cycbuf);
uint32_t util_cycbuf_size(util_cycbuf_t* cycbuf);
uint32_t util_cycbuf_push(util_cycbuf_t* cycbuf, void* data, uint32_t len);
uint32_t util_cycbuf_pop(util_cycbuf_t* cycbuf, void* buf, uint32_t len);
uint32_t util_cycbuf_peek(util_cycbuf_t* cycbuf, void* buf, uint32_t len);
uint32_t util_cycbuf_lose(util_cycbuf_t* cycbuf, uint32_t len);
void 	 util_cycbuf_print(util_cycbuf_t* cycbuf);

#endif // H_UTILS_CYC_BUFFER_H
