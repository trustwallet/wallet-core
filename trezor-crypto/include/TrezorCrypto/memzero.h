#ifndef TC__MEMZERO_H__
#define TC__MEMZERO_H__

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif
void memzero(void* const pnt, const size_t len);

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif
