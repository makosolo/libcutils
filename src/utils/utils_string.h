#ifndef H_UTILS_STRING_H
#define H_UTILS_STRING_H

#include <stdint.h>
#include <stdbool.h>

#define util_string_isspace(x)               (((x) == 0x20) || ((x) > 0x8 && (x) < 0xe))                // 空白字符
#define util_string_isgraph(x)               ((x) > 0x1f && (x) < 0x7f)                                 // 有效字符
#define util_string_isalpha(x)               (((x) > 0x40 && (x) < 0x5b) || ((x) > 0x60 && (x) < 0x7b)) // 字母
#define util_string_isupper(x)               ((x) > 0x40 && (x) < 0x5b)                                 // 大写字母
#define util_string_islower(x)               ((x) > 0x60 && (x) < 0x7b)                                 // 小写字母
#define util_string_isascii(x)               ((x) >= 0x0 && (x) < 0x80)                                 // ascii码
#define util_string_isdigit(x)               ((x) > 0x2f && (x) < 0x3a)                                 // 数字
#define util_string_isdigit2(x)              ((x) == '0' || (x) == '1')                                 // 二进制
#define util_string_isdigit8(x)              (((x) > 0x2f && (x) < 0x38))                               // 八进制
#define util_string_isdigit10(x)             (util_string_isdigit(x))                                   // 十进制
#define util_string_isdigit16(x)             (((x) > 0x2f && (x) < 0x3a) || ((x) > 0x40 && (x) < 0x47) || ((x) > 0x60 && (x) < 0x67)) // 十六进制

// 去掉空白字符
const char * util_string_trim(char *str);
const char * util_string_trim_left(char *str);
const char * util_string_trim_right(char *str);

// 去掉特定字符
const char * util_string_trim_char(char *str, char character);
const char * util_string_trim_char_left(char *str, char character);
const char * util_string_trim_char_right(char *str, char character);

// 替换字符
const char * util_string_replace(char *str, const char *sample, const char *target);

// 查找字符串
const char * util_string_find_str(const char *str, const char *sample);
// 查找字符串，第n次的位置
const char * util_string_find_str_nth(const char *str, uint32_t n, const char *sample);
// 统计字符串出现的次数
int util_string_find_str_count(const char *str, const char *sample);

// 字符串转大写
char *util_string_supper(char *str);
// 字符串转小写
char *util_string_lower(char *str);

#endif // H_UTILS_STRING_H
