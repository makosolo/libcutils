#include "utils_cycbuf.h"

char *buf[10] = {"11111", "22222", "33333", "44444", "55555", "66666", "77777", "88888", "99999", "aaaaa"};

void test_cycbuf(void)
{
    util_cycbuf_t *cycbuf = NULL;

    if (0 == util_cycbuf_create(&cycbuf, 32)) {
        printf("util_cycbuf_create fail\n");
        return;
    }

    printf("++++++++++++++++++++++++util_cycbuf_push++++++++++++++++++++++++\n");
    for (int i = 0; i < 10; i++)
    {
        uint32_t len = util_cycbuf_push(cycbuf, buf[i], 5);
        printf("util_cycbuf_push=%d %d\n", len, util_cycbuf_size(cycbuf));
        util_cycbuf_print(cycbuf);
    }

    printf("++++++++++++++++++++++++util_cycbuf_peek++++++++++++++++++++++++\n");
    for (int i = 0; i < 10; i++)
    {
        char tmp[128] = {0};
        uint32_t len = util_cycbuf_peek(cycbuf, tmp, 5);
        printf("util_cycbuf_peek=%d %d tmp=%s \n", len, util_cycbuf_size(cycbuf), tmp);
        util_cycbuf_print(cycbuf);
    }

    printf("++++++++++++++++++++++++util_cycbuf_pop++++++++++++++++++++++++\n");
    for (int i = 0; i < 10; i++)
    {
        char tmp[128] = {0};
        uint32_t len = util_cycbuf_pop(cycbuf, tmp, 5);
        printf("util_cycbuf_pop=%d %d tmp=%s\n", len, util_cycbuf_size(cycbuf), tmp);
        util_cycbuf_print(cycbuf);
    }

    printf("++++++++++++++++++++++++util_cycbuf_pop++++++++++++++++++++++++\n");
    for (int i = 0; i < 10; i++)
    {
        uint32_t len = util_cycbuf_push(cycbuf, buf[i], 5);
        printf("util_cycbuf_push=%d %d\n", len, util_cycbuf_size(cycbuf));
        util_cycbuf_print(cycbuf);

        if (0 == i%2) {
        char tmp[128] = {0};
        uint32_t len = util_cycbuf_pop(cycbuf, tmp, 5);
        printf("util_cycbuf_pop=%d %d tmp=%s\n", len, util_cycbuf_size(cycbuf), tmp);
            util_cycbuf_print(cycbuf);
        }
    }

    printf("++++++++++++++++++++++++util_cycbuf_lose++++++++++++++++++++++++\n");
    for (int i = 0; i < 10; i++)
    {
        uint32_t len = util_cycbuf_push(cycbuf, buf[i], 5);
        printf("util_cycbuf_push=%d %d\n", len, util_cycbuf_size(cycbuf));
        util_cycbuf_print(cycbuf);

        if (0 == i%2) {
            uint32_t len = util_cycbuf_lose(cycbuf, 5);
            printf("util_cycbuf_lose=%d %d \n", len, util_cycbuf_size(cycbuf));
            util_cycbuf_print(cycbuf);
        }
    }

    util_cycbuf_delete(&cycbuf);
}
