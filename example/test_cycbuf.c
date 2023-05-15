#include "utils_cycbuf.h"

char *buf[8] = {"11111", "22222", "33333", "44444", "55555", "66666" "77777", "88888"};

void test_cycbuf(void)
{
    util_cycbuf_t cycbuf;

    util_cycbuf_init(&cycbuf, 32);

    printf("++++++++++++++++++++++++util_cycbuf_push++++++++++++++++++++++++\n");
    for (int i = 0; i < 10; i++)
    {
        printf("util_cycbuf_push=%d %d\n", util_cycbuf_push(&cycbuf, buf[i], 5), util_cycbuf_size(&cycbuf));
    }

    printf("++++++++++++++++++++++++util_cycbuf_pop++++++++++++++++++++++++\n");
    for (int i = 0; i < 10; i++)
    {
        char tmp[128] = {0};
        util_cycbuf_pop(&cycbuf, tmp, 10);
        printf("util_cycbuf_pop=%d %d tmp=%s\n", util_cycbuf_pop(&cycbuf, tmp, 5), util_cycbuf_size(&cycbuf), tmp);
    }

    printf("++++++++++++++++++++++++util_cycbuf_peek++++++++++++++++++++++++\n");
    for (int i = 0; i < 10; i++)
    {
        char tmp[128] = {0};
        printf("util_cycbuf_peek=%d %d tmp=%s \n", util_cycbuf_peek(&cycbuf, tmp, 5), util_cycbuf_size(&cycbuf), tmp);
    }

    printf("++++++++++++++++++++++++util_cycbuf_lose++++++++++++++++++++++++\n");
    for (int i = 0; i < 10; i++)
    {
        util_cycbuf_lose(&cycbuf, 10);
        printf("util_cycbuf_lose=%d %d \n", util_cycbuf_lose(&cycbuf, 10), util_cycbuf_size(&cycbuf));
    }

    util_cycbuf_deinit(&cycbuf);
}
