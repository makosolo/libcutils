#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>
#include <stdarg.h>

#include "utils_mutex.h"
#include "utils_list.h"

#include "utils_log.h"

typedef struct {
    util_list_node_t    node;
    char            	file_name[UTIL_LOG_NAME_SIZE];
} list_node_data_t;

typedef struct {
    char                tags[UTIL_LOG_NAME_SIZE];

	FILE*               fp;
    char                file_name[UTIL_LOG_NAME_SIZE];
	size_t				file_size;
	size_t              file_size_limit;
	uint32_t			file_count;
	uint32_t 			file_count_limit;
	util_list_t 		list;

	util_mutex_t*       mutex;
	util_log_level_e    level;
	bool                is_init;
	bool                stdout_enabled;

	void*               arg;
	OnUtilLogCallback   onLogCallback;
} util_log_context_t;

static util_log_context_t g_log_ctx_t = {0};

static char *g_log_levels[UTIL_LOG_MAX] = {"DEBUG", "INFO", "WARN", "ERROR", "OFF"};

static int util_log_list_init(util_list_t *list)
{
	util_list_init(list);
}

static int util_log_list_add(util_list_t *list, char *file_name, bool remove)
{
	list_node_data_t *data = NULL;

	if (!remove) {
		data = (list_node_data_t*)malloc(sizeof(list_node_data_t));
	}
	else {
		util_list_node_t *node = util_list_head(list);

		if (NULL != node) {
			data = util_list_data(node, list_node_data_t, node);

			/* remove old file */
			unlink(data->file_name);
			util_list_remove(&data->node);			
		}
	}

	/* add new file */
	if (NULL != data) {
		strncpy(data->file_name, file_name, UTIL_LOG_NAME_SIZE-1);
		util_list_insert_tail(list, &data->node);
	}

	return 0;
}

static void util_log_list_clear(util_list_t *list)
{
    if (util_list_empty(list)) {
		return;
	}

	util_list_node_t *pos, *tmp;

	util_list_for_each_next(pos, tmp, list) {
		list_node_data_t *data = util_list_data(pos, list_node_data_t, node);

		if (NULL != data) {
			printf("util_list_remove: file=%s \n", data->file_name);
			util_list_remove(&data->node);
			free(data);				
		}
	}
}

static int util_log_print_header(FILE *fp, util_log_level_e level)
{
	int result = 0;
	time_t t;
	struct tm tt, *tm;

	t = time(NULL);
	tm = localtime_r(&t, &tt);

	if (tm == NULL) {
		return -1;
	}

	result = fprintf(fp, "[%d-%02d-%02d %02d:%02d:%02d][%-5s][%s] ",
		     tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
		     tm->tm_hour, tm->tm_min, tm->tm_sec,
		     g_log_levels[level], g_log_ctx_t.tags);
             
	if (result < 0) {
		return -1;
	}

	return result;
}

static int util_log_stdout(util_log_level_e level, const char *fmt, va_list va)
{
	FILE *dest = (level == UTIL_LOG_ERROR) ? stderr : stdout;

	if (util_log_print_header(dest, level) < 0) {
		return -1;
	}

	if (vfprintf(dest, fmt, va) < 0) {
		return -1;
	}

	return 0;
}

static int util_log_file(util_log_level_e level, const char *fmt, va_list va)
{
	int size = 0;
	bool remove = false;
	
start:
	if (NULL != g_log_ctx_t.fp && g_log_ctx_t.file_size < g_log_ctx_t.file_size_limit) {
		size = util_log_print_header(g_log_ctx_t.fp, level);
		if (size < 0) {
			return -1;
		}
		g_log_ctx_t.file_size += size;

		size = vfprintf(g_log_ctx_t.fp, fmt, va);
		if (size < 0) {
			return -1;
		}
		g_log_ctx_t.file_size += size;

		fflush(g_log_ctx_t.fp);
	}
	else {
		char log_file[256] = "";
		time_t logtime = time(0);
		struct tm t;

		if (NULL != g_log_ctx_t.fp) {
			fclose(g_log_ctx_t.fp);
		}

		localtime_r(&logtime, &t);

		// 格式化文件名 20180401T230404
		snprintf(log_file, sizeof(log_file), "%s_%04d%02d%02d-%02d%02d%02d.log",
				g_log_ctx_t.file_name,
				t.tm_year + 1900,
				t.tm_mon + 1,
				t.tm_mday,
				t.tm_hour,
				t.tm_min,
				t.tm_sec);
		
		FILE *fp = fopen(log_file, "w+");
		if (NULL == fp) {
			return -1;
		}

		if (ftell(fp) < 0) {
			fclose(fp);
			return -1;
		}

		if (g_log_ctx_t.file_count < g_log_ctx_t.file_count_limit) {
			g_log_ctx_t.file_count++;
		}
		else {
			remove = true;
		}

		util_log_list_add(&g_log_ctx_t.list, log_file, remove);
		g_log_ctx_t.fp 			= fp;
		g_log_ctx_t.file_size	= 0;

		/* create new file link */
		unlink(g_log_ctx_t.file_name);
		symlink(log_file, g_log_ctx_t.file_name);

		goto start;
	}

	return 0;
}

static void util_log_load(void)
{

}

int util_log_init(util_log_cfg_t *cfg)
{
    if (NULL == cfg) {
        return -1;
    }

    if (g_log_ctx_t.is_init) {
		return 0;
	}

	g_log_ctx_t.stdout_enabled  = cfg->stdout_enabled;
	g_log_ctx_t.onLogCallback   = cfg->onLogCallback;
	g_log_ctx_t.arg             = cfg->arg;

	strncpy(g_log_ctx_t.tags, cfg->tags, UTIL_LOG_NAME_SIZE);
	g_log_ctx_t.tags[UTIL_LOG_NAME_SIZE-1] = 0;

	if (0 != cfg->file_name[0]) {
		g_log_ctx_t.file_size_limit = cfg->file_size;
		if (g_log_ctx_t.file_size_limit > UTIL_LOG_FILE_SIZE_MAX) {
			g_log_ctx_t.file_size_limit = UTIL_LOG_FILE_SIZE_MAX;
		}

		if (g_log_ctx_t.file_size_limit < UTIL_LOG_FILE_SIZE_MIN) {
			g_log_ctx_t.file_size_limit = UTIL_LOG_FILE_SIZE_MIN;
		}

		g_log_ctx_t.file_count_limit = cfg->file_count;
		if (g_log_ctx_t.file_count_limit < UTIL_FILE_COUNT_MIN) {
			g_log_ctx_t.file_count_limit = UTIL_FILE_COUNT_MIN;
		}

		strncpy(g_log_ctx_t.file_name, cfg->file_name, UTIL_LOG_NAME_SIZE);
		g_log_ctx_t.file_name[UTIL_LOG_NAME_SIZE-1] = 0;			
	}

	/* create mutex */
	if (0 != util_mutex_create(&g_log_ctx_t.mutex)) {
		return -1;
	}

	util_log_list_init(&g_log_ctx_t.list);

	util_log_load();	

	g_log_ctx_t.is_init = true;

    return 0;
}

int util_log_deinit(void)
{
    int result = 0;

    if (!g_log_ctx_t.is_init) {
        return -1;
    }

	if (NULL != g_log_ctx_t.fp) {
		if (0 != fclose(g_log_ctx_t.fp)) {
			result = -1;
		}
	}

    util_mutex_delete(&g_log_ctx_t.mutex);
    memset(&g_log_ctx_t, 0, sizeof(util_log_context_t));

    return result;
}

int util_log_set_level(util_log_level_e level)
{
    util_mutex_lock(g_log_ctx_t.mutex);
    g_log_ctx_t.level = level;
    util_mutex_unlock(g_log_ctx_t.mutex);

	return 0;
}

int util_log_log(util_log_level_e level, const char *fmt, ...)
{
    va_list va;
    int result = 0;

    util_mutex_lock(g_log_ctx_t.mutex);

	if (level < g_log_ctx_t.level) {
		util_mutex_unlock(g_log_ctx_t.mutex);
		return 0;
	}

	if (g_log_ctx_t.stdout_enabled) {
		va_start(va, fmt);
		result |= util_log_stdout(level, fmt, va);
		va_end(va);
	}

	if (0 != g_log_ctx_t.file_name[0]) {
		va_start(va, fmt);
		result |= util_log_file(level, fmt, va);
		va_end(va);
	}

	if (g_log_ctx_t.onLogCallback) {
		va_start(va, fmt);
		result |= g_log_ctx_t.onLogCallback(g_log_ctx_t.arg, level, fmt, va);
		va_end(va);
	}

    util_mutex_unlock(g_log_ctx_t.mutex);

    return result;
}
