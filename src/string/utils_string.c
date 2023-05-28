#include <string.h>

#include "utils_string.h"

const char * util_string_trim(char *str)
{
    char *s, *t;

    if (NULL == str) {
        return NULL;
    }

    for (s = str; util_string_isspace(*s); s++)
        ;
 
    if (*s == 0)
        return (s);
 
    t = s + strlen(s) - 1;
    while (t > s && util_string_isspace(*t))
        t--;
 
    *++t = '\0';
 
    return s;
}

const char * util_string_trim_left(char *str)
{
    char *s;

    if (NULL == str) {
        return NULL;
    }

    for (s = str; util_string_isspace(*s); s++)
        ;
 
    return s;
}

const char * util_string_trim_right(char *str)
{
    char *t;

    if (NULL == str) {
        return NULL;
    }

    t = str + strlen(str) - 1;
    while (t > str && util_string_isspace(*t))
        t--;
 
    *++t = '\0';
 
    return str;
}

const char * util_string_trim_char(char *str, char character)
{
    char *s, *t;

    if (NULL == str) {
        return NULL;
    }

    for (s = str; character == *s; s++)
        ;
 
    if (*s == 0)
        return (s);
 
    t = s + strlen(s) - 1;
    while (t > s && character == *t)
        t--;
 
    *++t = '\0';
 
    return s;
}

const char * util_string_trim_char_left(char *str, char character)
{
    char *s;

    if (NULL == str) {
        return NULL;
    }

    for (s = str; character == *s; s++)
        ;
 
    return s;
}

const char * util_string_trim_char_right(char *str, char character)
{
    char *s, *t;

    if (NULL == str) {
        return NULL;
    }

    s = str;
    t = s + strlen(s) - 1;
    while (t > s && character == *t)
        t--;
 
    *++t = '\0';
 
    return s;
}

const char * util_string_replace(char *str, const char *sample, const char *target)
{
    char *s;
    size_t len;

    if (NULL == str || NULL == sample) {
        return NULL;
    }

    s   = str;
    len = strlen(sample);

    if (len != strlen(target)) {
        return NULL;
    }

    while (*s != '\0') {
        if (0 == strncmp(s, sample, len)) {
            strncpy(s, target, len);
            s += len;
        }
        else {
            s++;
        }
    }
 
    return str;
}

const char * util_string_find_str(const char *str, const char *sample)
{
    char *s;
    size_t len;

    if (NULL == str || NULL == sample) {
        return NULL;
    }

    s   = (char*)str;
    len = strlen(sample);

    if (len > strlen(str)) {
        return NULL;
    }

    while (*s != '\0') {
        if (0 == strncmp(s, sample, len)) {
            return s;
        }
        s++;
    }

    return NULL;
}

const char * util_string_find_str_nth(const char *str, uint32_t n, const char *sample)
{
    char *s;
    size_t len;
    uint32_t cnt = 0;

    if (NULL == str || NULL == sample) {
        return NULL;
    }

    s   = (char*)str;
    len = strlen(sample);

    if (len > strlen(str)) {
        return NULL;
    }

    while (*s != '\0') {
        if (0 == strncmp(s, sample, len)) {
            if (n == (++cnt)) {
                return s;
            }
            s += len;
        }
        else {
            s++;
        }
    }

    return NULL;
}

int util_string_find_str_count(const char *str, const char *sample)
{
    char *s;
    size_t len;
    uint32_t cnt = 0;

    if (NULL == str || NULL == sample) {
        return 0;
    }

    s   = (char*)str;
    len = strlen(sample);

    if (len > strlen(str)) {
        return 0;
    }

    while (*s != '\0') {
        if (0 == strncmp(s, sample, len)) {
            cnt++;
            s += len;
        }
        else {
            s++;
        }
    }

    return cnt;
}

char *util_string_supper(char *str)
{
    char *s;

    if (NULL == str) {
        return NULL;
    }

    s = str;

    while (*s != '\0') {
        if (util_string_islower(*s)) {
            *s -= 32; 
        }
        s++;
    }

    return str;
}

char *util_string_lower(char *str)
{
    char *s;

    if (NULL == str) {
        return NULL;
    }

    s = str;

    while (*s != '\0') {
        if (util_string_isupper(*s)) {
            *s += 32; 
        }
        s++;
    }

    return str;
}
