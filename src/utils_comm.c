#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "build/build_config.h"

#include "utils_comm.h"

static char *get_build_info(void) 
{
#define BUILD_INFO(a,b,c,d)     "Build: " a " " b " Repo: ***" c "(" d ")"
    return BUILD_INFO(BUILD_DATA, BUILD_TIME, BUILD_BRANCH, BUILD_VERSION);
}

static char *get_version(void) 
{
#define APP_VERSION(a,b,c)      "V" a "." b "." c
    return APP_VERSION(VERSION_MAJOR, VERSION_MINOR, VERSION_REVISED);
}

const char *util_get_version(void)
{
    printf("cutils_version:%s\t %s\n", get_version(), get_build_info());
    return get_version();
}
