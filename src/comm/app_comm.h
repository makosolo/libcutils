#ifndef H_FT_REMOTE_DEBUG_COMM_H
#define H_FT_REMOTE_DEBUG_COMM_H

#ifdef __cplusplus
extern "C" {
#endif

#define 	HANDLE_MAGIC    (0x4654)
#define 	HANDLE_CREATE(CHID) ((HANDLE_MAGIC << 16) | (CHID & 0xff))
#define 	HANDLE_GET_CHID(HANDLEID) ((((HANDLEID >> 16) & 0xffff) == HANDLE_MAGIC)? (HANDLEID & 0xff):-1)

#include "utils_net.h"
#include "utils_list.h"

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif
