#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
// #include <ctype.h>
#include <errno.h>
#include <time.h>
#include <stdarg.h>
#include <dirent.h>

#include "utils_mutex.h"
#include "utils_list.h"
#include "utils_file.h"

#include "utils_log.h"

// #define UTIL_LOG_DEBUG

#ifdef UTIL_LOG_DEBUG
#define UTIL_LOG_PRINT(msg, ...)	printf("[%s:%d] "msg, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#else
#define UTIL_LOG_PRINT(msg, ...)
#endif

typedef struct {
    util_list_node_t    node;
    char            	file_name[512];
} list_node_data_t;

typedef struct {
    char                tags[64];

	FILE*               fp;
    char                file_path[128];
	char                file_name[512];
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

static int util_log_list_add(util_list_t *list, char *file_name)
{
	list_node_data_t *data = NULL;

	data = (list_node_data_t*)malloc(sizeof(list_node_data_t));
	if (NULL != data) {
		strncpy(data->file_name, file_name, sizeof(data->file_name)-1);
		util_list_insert_tail(list, &data->node);
		return 0;
	}

	return -1;
}

static int util_log_list_remove(util_list_t *list)
{
	list_node_data_t *data = NULL;
	util_list_node_t *node = util_list_head(list);

	if (NULL != node) {
		char link_name[128] = "";

		data = util_list_data(node, list_node_data_t, node);

		/* remove old file */
		unlink(data->file_name);
		util_list_remove(&data->node);

		return 0;
	}

	return -1;
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
			UTIL_LOG_PRINT("remove file=%s \n", data->file_name);
			util_list_remove(&data->node);
			free(data);
		}
	}
}

static int util_log_print_header(util_log_context_t *obj, FILE *fp, util_log_level_e level)
{
	util_log_context_t *pObj = obj;
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
		     g_log_levels[level], pObj->tags);

	if (result < 0) {
		return -1;
	}

	return result;
}

static int util_log_stdout(util_log_context_t *obj, util_log_level_e level, const char *fmt, va_list va)
{
	FILE *dest = (level == UTIL_LOG_ERROR) ? stderr : stdout;

	if (util_log_print_header(obj, dest, level) < 0) {
		return -1;
	}

	if (vfprintf(dest, fmt, va) < 0) {
		return -1;
	}

	return 0;
}

static int util_log_file(util_log_context_t *obj, util_log_level_e level, const char *fmt, va_list va)
{
	util_log_context_t *pObj = obj;
	int size = 0;
	bool remove = false;

start:
	if (NULL != pObj->fp && pObj->file_size < pObj->file_size_limit) {
		size = util_log_print_header(pObj, pObj->fp, level);
		if (size < 0) {
			return -1;
		}
		pObj->file_size += size;

		size = vfprintf(pObj->fp, fmt, va);
		if (size < 0) {
			return -1;
		}
		pObj->file_size += size;

		fflush(pObj->fp);
	}
	else {
		char log_file[512] = "";
		time_t logtime = time(0);
		struct tm t;

		/* close old file */
		if (NULL != pObj->fp) {
			fclose(pObj->fp);
		}

		/* create dir */
		if(0 != access(pObj->file_path, 0)) {
			util_file_mkdirs(pObj->file_path);
		}

		/* remove old file */
		if (pObj->file_count >= pObj->file_count_limit) {
			util_log_list_remove(&pObj->list);
		}
		else {
			pObj->file_count++;
		}

		localtime_r(&logtime, &t);

		// 格式化文件名 20180401T230404
		snprintf(log_file, sizeof(log_file), "%s/%s_%04d%02d%02d-%02d%02d%02d.ulog",
				pObj->file_path,
				pObj->tags,
				t.tm_year + 1900,
				t.tm_mon + 1,
				t.tm_mday,
				t.tm_hour,
				t.tm_min,
				t.tm_sec);

		/* create new file */
		FILE *fp = fopen(log_file, "w+");
		if (NULL != fp) {
			pObj->fp 		= fp;
			pObj->file_size	= 0;

			util_log_list_add(&pObj->list, log_file);

			/* create soft link */
			unlink(pObj->file_name);
			symlink(log_file, pObj->file_name);

			goto start;
		}
		else {
			UTIL_LOG_PRINT("fopen[%s] fail, errno=%d \n", log_file, errno);
		}
	}

	return 0;
}

static int util_log_load(util_log_context_t *obj)
{
	util_log_context_t *pObj = obj;
	struct dirent **namelist;
	int total = 0;
	char log_file[512] = "";

	/* scan directory */
	total = scandir(pObj->file_path, &namelist ,0, alphasort);
	if (total < 0) {
		UTIL_LOG_PRINT("scandir[%s] fail, errno=%d \n", pObj->file_path, errno);
		return -1;
	}

	for (int i = 0; i < total; i++) {
		snprintf(log_file, sizeof(log_file), "%s/%s", pObj->file_path, namelist[i]->d_name);

		if (0 == strcmp(namelist[i]->d_name, ".")  ||
			0 == strcmp(namelist[i]->d_name, "..") ||
			0 == strcmp(log_file, pObj->file_name)) {
			continue;
		}

		if (pObj->file_count < pObj->file_count_limit) {
			pObj->file_count++;
			util_log_list_add(&pObj->list, log_file);
			UTIL_LOG_PRINT("d_name:%s \n", log_file);
		}
		else {
			UTIL_LOG_PRINT("unlink=%s \n", namelist[i]->d_name);
			unlink(namelist[i]->d_name);
		}
	}

	/* open file */
	FILE *fp = fopen(pObj->file_name, "a+");

#if 0
	if (NULL == fp) {
		UTIL_LOG_PRINT("[%s] soft link error? \n", pObj->file_name);

		/* try delete */
		unlink(pObj->file_name);

		fp = fopen(namelist[pObj->file_count-1]->d_name, "a+");
		if (NULL != fp) {
			symlink(namelist[pObj->file_count-1]->d_name, pObj->file_name);
		}
	}
#endif

	if (NULL != fp) {
		long size = 0;

		if (0 != fseek(fp, 0, SEEK_END)) {
			UTIL_LOG_PRINT("fseek fail, errno=%d \n", errno);
		}

		size = ftell(fp);
		if (size < 0) {
			size = 0;
			UTIL_LOG_PRINT("ftell fail, errno=%d \n", errno);
		}

		pObj->fp 		= fp;
		pObj->file_size = size;
	}

	UTIL_LOG_PRINT("[%s]: fp=%p size=%ld \n", pObj->file_name, fp, pObj->file_size);

	return 0;
}

int util_log_init(util_log_cfg_t *cfg)
{
	util_log_context_t *pObj = &g_log_ctx_t;

    if (NULL == cfg) {
        return -1;
    }

    if (pObj->is_init) {
		return 0;
	}

	pObj->stdout_enabled  = cfg->stdout_enabled;
	pObj->onLogCallback   = cfg->onLogCallback;
	pObj->arg             = cfg->arg;

	strncpy(pObj->tags, cfg->tags, sizeof(pObj->tags)-1);

	if (0 != cfg->file_path[0]) {
		pObj->file_size_limit = cfg->file_size;
		if (pObj->file_size_limit > UTIL_LOG_FILE_SIZE_MAX) {
			pObj->file_size_limit = UTIL_LOG_FILE_SIZE_MAX;
		}

		if (pObj->file_size_limit < UTIL_LOG_FILE_SIZE_MIN) {
			pObj->file_size_limit = UTIL_LOG_FILE_SIZE_MIN;
		}

		pObj->file_count_limit = cfg->file_count;
		if (pObj->file_count_limit < UTIL_LOG_FILE_COUNT_MIN) {
			pObj->file_count_limit = UTIL_LOG_FILE_COUNT_MIN;
		}

		strncpy(pObj->file_path, cfg->file_path, sizeof(pObj->file_path));
		if (0 != util_file_mkdirs(cfg->file_path)) {
			return -1;
		}

		snprintf(pObj->file_name, sizeof(pObj->file_name), "%s/%s.ulog", pObj->file_path, pObj->tags);
	}

	/* create mutex */
	if (0 != util_mutex_create(&pObj->mutex)) {
		return -1;
	}

	util_log_list_init(&pObj->list);

	util_log_load(pObj);

	pObj->is_init = true;

    return 0;
}

int util_log_deinit(void)
{
	util_log_context_t *pObj = &g_log_ctx_t;
    int result = 0;

    if (!pObj->is_init) {
        return -1;
    }

	if (NULL != pObj->fp) {
		if (0 != fclose(pObj->fp)) {
			result = -1;
		}
	}

    util_mutex_delete(&pObj->mutex);
	util_log_list_clear(&pObj->list);

    memset(pObj, 0, sizeof(util_log_context_t));

    return result;
}

int util_log_set_level(util_log_level_e level)
{
	util_log_context_t *pObj = &g_log_ctx_t;

    util_mutex_lock(pObj->mutex);
    pObj->level = level;
    util_mutex_unlock(pObj->mutex);

	return 0;
}

int util_log_log(util_log_level_e level, const char *fmt, ...)
{
	util_log_context_t *pObj = &g_log_ctx_t;
    va_list va;
    int result = 0;

    util_mutex_lock(pObj->mutex);

	if (level < pObj->level) {
		util_mutex_unlock(pObj->mutex);
		return 0;
	}

	if (pObj->stdout_enabled) {
		va_start(va, fmt);
		result |= util_log_stdout(pObj, level, fmt, va);
		va_end(va);
	}

	if (0 != pObj->file_name[0]) {
		va_start(va, fmt);
		result |= util_log_file(pObj, level, fmt, va);
		va_end(va);
	}

	if (pObj->onLogCallback) {
		va_start(va, fmt);
		result |= pObj->onLogCallback(pObj->arg, level, fmt, va);
		va_end(va);
	}

    util_mutex_unlock(pObj->mutex);

    return result;
}
