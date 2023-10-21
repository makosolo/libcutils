#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "string/utils_string.h"

void test_string(void)
{
    printf("++++++++++++++++++++++++string++++++++++++++++++++++++\n");
    {
        char string[] = {0x20, 0x20, 0x30, 0x31, 0x32, 0x20, 0x20, 0x00};
        char *pstr;

        printf("+++util_string_trim:%s len=%ld\n", string, strlen(string));
        pstr = (char*)util_string_trim(string);
        printf("---util_string_trim:%s len=%ld\n", pstr, strlen(pstr));
    }

    {
        char string[] = {0x20, 0x20, 0x30, 0x31, 0x32, 0x20, 0x20, 0x00};
        char *pstr;

        printf("+++util_string_trim_left:%s len=%ld\n", string, strlen(string));
        pstr = (char*)util_string_trim_left(string);
        printf("---util_string_trim_left:%s len=%ld\n", pstr, strlen(pstr));
    }

    {
        char string[] = {0x20, 0x20, 0x30, 0x31, 0x32, 0x20, 0x20, 0x00};
        char *pstr;

        printf("+++util_string_trim_right:%s len=%ld\n", string, strlen(string));
        pstr = (char*)util_string_trim_right(string);
        printf("---util_string_trim_right:%s len=%ld\n", pstr, strlen(pstr));
    }

    {
        char string[] = {0x24, 0x24, 0x30, 0x31, 0x32, 0x24, 0x24, 0x00};
        char *pstr;

        printf("+++util_string_trim_char:%s len=%ld\n", string, strlen(string));
        pstr = (char*)util_string_trim_char(string, '$');
        printf("---util_string_trim_char:%s len=%ld\n", pstr, strlen(pstr));
    }

    {
        char string[] = {0x24, 0x24, 0x30, 0x31, 0x32, 0x24, 0x24, 0x00};
        char *pstr;

        printf("+++util_string_trim_char_left:%s len=%ld\n", string, strlen(string));
        pstr = (char*)util_string_trim_char_left(string, '$');
        printf("---util_string_trim_char_left:%s len=%ld\n", pstr, strlen(pstr));
    }

    {
        char string[] = {0x24, 0x24, 0x30, 0x31, 0x32, 0x24, 0x24, 0x00};
        char *pstr;

        printf("+++util_string_trim_char_right:%s len=%ld\n", string, strlen(string));
        pstr = (char*)util_string_trim_char_right(string, '$');
        printf("---util_string_trim_char_right:%s len=%ld\n", pstr, strlen(pstr));
    }

    {
        char string[] = {0x24, 0x24, 0x30, 0x31, 0x32, 0x24, 0x24, 0x00};
        char *pstr;

        printf("+++util_string_replace:%s len=%ld\n", string, strlen(string));
        pstr = (char*)util_string_replace(string, "$", "!");
        printf("---util_string_replace:%s len=%ld\n", pstr, strlen(pstr));
    }

    {
        char string[] = {0x24, 0x24, 0x30, 0x31, 0x32, 0x24, 0x24, 0x00};
        printf("util_string_find_str:%s %s\n", string, util_string_find_str(string, "$"));
        printf("util_string_find_str_nth:%s %s\n", string, util_string_find_str_nth(string, 2, "$"));
        printf("util_string_find_str_nth:%s %s\n", string, util_string_find_str_nth(string, 3, "$"));
        printf("util_string_find_str_count:%s %d\n", string, util_string_find_str_count(string, "$"));
    }

    {
        char string[] = {0x24, 0x24, 0x30, 0x31, 0x32, 0x24, 0x24, 0x00};
        printf("util_string_supper:%s %s\n", string, util_string_supper(string));
        printf("util_string_lower:%s %s\n", string, util_string_lower(string));
    }

    {
        char string1[] = "abcdef";
        char string2[] = "ABCDEF";
        printf("util_string_supper:%s\n", util_string_supper(string1));
        printf("util_string_lower:%s\n", util_string_lower(string2));
    }
}
