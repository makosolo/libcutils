#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <signal.h>

#include "utils_file.h"
#include "utils_log.h"

static int exit_flag = 0;
static int run_count = 0;
static char *g_log_levels[UTIL_LOG_MAX] = {"DEBUG", "INFO", "WARN", "ERROR", "OFF"};

static int OnLogPrint(void *arg, util_log_level_e level, const char *fmt, va_list va)
{
    if (level >= UTIL_LOG_MAX) {
        return -1;
    }

	fprintf(stdout, " received log[%p] : level = [%s] ", arg, g_log_levels[level]);
	vfprintf(stdout, fmt, va);
}

static void app_int_sig_handler(int sig)
{
    exit_flag = 1;

    printf("app_int_sig_handler exit\n");
    // exit(0);
}

void test_log(void)
{
    int status = 0;

    /* Register the signal handler. */
    signal(SIGINT, app_int_sig_handler);

    printf("++++++++++++++++++++++++log++++++++++++++++++++++++\n");

    util_log_cfg_t cfg;

    memset(&cfg, 0, sizeof(util_log_cfg_t));

    snprintf(cfg.tags, sizeof(cfg.tags), "UTIL_LOG");

    // cfg.stdout_enabled  = true;
    cfg.arg             = NULL;
    cfg.onLogCallback   = OnLogPrint;

    cfg.file_count = 5;
    cfg.file_size  = 1024;
    snprintf(cfg.file_path, sizeof(cfg.file_path), "/tmp/test");

    UTIL_ASSERT_RET(0 == util_log_init(&cfg), "util_log_init fail!\n");

    UTIL_ASSERT_RET(0 == util_log_set_level(UTIL_LOG_OFF), "util_log_set_level fail!\n");

    while (!exit_flag) {
        util_log_error("util_log_error[%04d] \n", run_count);
        util_log_warn("util_log_warn [%04d] \n", run_count);
        util_log_info("util_log_info [%04d] \n", run_count);
        util_log_debug("util_log_debug[%04d] \n", run_count);

        if (run_count >= 50) {
            // UTIL_ASSERT_RET(0 == util_log_set_level(UTIL_LOG_OFF), "util_log_set_level fail!\n");
        }
        else if (run_count >= 40) {
            UTIL_ASSERT_RET(0 == util_log_set_level(UTIL_LOG_DEBUG), "util_log_set_level fail!\n");
        }
        else if (run_count >= 30) {
            UTIL_ASSERT_RET(0 == util_log_set_level(UTIL_LOG_INFO), "util_log_set_level fail!\n");
        }
        else if (run_count >= 20) {
            UTIL_ASSERT_RET(0 == util_log_set_level(UTIL_LOG_WARN), "util_log_set_level fail!\n");
        }
        else if (run_count >= 10) {
            UTIL_ASSERT_RET(0 == util_log_set_level(UTIL_LOG_ERROR), "util_log_set_level fail!\n");
        }

        run_count++;

        usleep(200*1000);
    }

    printf("test_log leave\n");
}
