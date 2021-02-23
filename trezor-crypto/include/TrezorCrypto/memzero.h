#ifndef __MEMZERO_H__
#define __MEMZERO_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void memzero(void *s, size_t n);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
