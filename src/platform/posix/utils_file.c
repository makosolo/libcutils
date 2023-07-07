#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include<sys/stat.h>
#include<sys/types.h>

#include "utils_file.h"

int util_file_mkdirs(const char *path)
{
    if (access(path, F_OK) == 0) {
        return 0;
    }

    char *p = strchr(path, '/');

    while (*p)
    {
        *p = '\0';
        if(0 != access(path, F_OK)) {
            if (mkdir(path, 0777) != 0) {
                if (errno != EEXIST) {
                    fprintf(stderr, "Failed to create directory %s: %s\n", path, strerror(errno));
                    return -1;
                }
            }
            *p = '/';
        }

        p = strchr(p + 1, '/');
    }

    if (mkdir(path, 0777) != 0) {
        if (errno != EEXIST) {
            fprintf(stderr, "Failed to create directory %s: %s\n", path, strerror(errno));
            return -1;
        }
    }

    return 0;
}
