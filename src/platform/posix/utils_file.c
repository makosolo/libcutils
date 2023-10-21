#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include<sys/stat.h>
#include<sys/types.h>

#include "utils_file.h"

int util_file_mkdirs(const char *path)
{
    if (0 == access(path, F_OK)) {
        return 0;
    }

    char *p = (char *)path;

    while (*p && (p = strchr(p, '/')))
    {
        *p = '\0';
        if(path != p && 0 != access(path, F_OK) && 0 != mkdir(path, 0777)) {
            if (errno != EEXIST) {
                fprintf(stderr, "Failed to create directory %s: %s\n", path, strerror(errno));
            }
        }
        *p = '/';

        p++;
    }

    if (0 != access(path, F_OK) && 0 != mkdir(path, 0777)) {
        if (errno != EEXIST) {
            fprintf(stderr, "Failed to create directory %s: %s\n", path, strerror(errno));
        }
    }

    return 0;
}
