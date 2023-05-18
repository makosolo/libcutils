#ifndef H_UTILS_STRING_H
#define H_UTILS_STRING_H

/*! strip the string
 *
 * @param string        the string
 * @param n             the striped size
 *
 * @return              the c-string
 */
char const* util_string_strip(char *str, uint32_t n);

/*! trim the left character for string
 *
 * @param string        the string
 *
 * @return              the c-string
 */
char const* util_string_trim_left(char *str, const char *character);

/*! trim the right character for string
 *
 * @param string        the string
 *
 * @return              the c-string
 */
char const* util_string_trim_right(char *str, const char *character);

int util_string_find_str(const char *str, const char *sample);
int util_string_find_str_range(const char *str, uint32_t len, const char *sample);
int util_string_find_str_n(const char *str, uint32_t n, const char *sample);
int util_string_find_str_count(const char *str, const char *sample);

int util_string_strcmp(const char *str1, const char *str2, uint32_t len);
int util_string_strcmp_tail(const char *str1, const char *str2, uint32_t len);

#endif // H_UTILS_STRING_H
