#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include<sys/stat.h>
#include<sys/types.h>

#include "utils_file.h"

int util_file_mkdirs(char *path)
{
    char str[512];
    char *p = str;
    int result = 0;

    strncpy(str, path, 512);

    while (*p)
    {
        if(*p == '/' && str != p)
        {
            *p = '\0';
            if(0 != access(str, 0)) {
                result = mkdir(str, 0777);
                if(result != 0 && errno != EEXIST) {
                    printf("mkdir=%s\n", str);
                    perror("mkdir fail");
                    return -1;
                }
            }
            *p = '/';
        }
        p++;
    }

    if(0 != access(str, 0)) {
        result = mkdir(str, 0777);
        if(result != 0 && errno != EEXIST) {
            printf("mkdir=%s\n", str);
            perror("mkdir fail");
            return -1;
        }
    }

    return 0;
}
