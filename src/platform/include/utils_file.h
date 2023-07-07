#ifndef H_UTILS_FILE_H
#define H_UTILS_FILE_H

#ifdef __cplusplus
extern "C" {
#endif

#define UTIL_FILE_NAME_MAX  (256)

int util_file_mkdirs(const char *path);

#ifdef __cplusplus
}
#endif

#endif // H_UTILS_FILE_H
