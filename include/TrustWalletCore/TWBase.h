// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#if !defined(TW_EXTERN_C_BEGIN)
#if defined(__cplusplus)
#define TW_EXTERN_C_BEGIN extern "C" {
#define TW_EXTERN_C_END   }
#else
#define TW_EXTERN_C_BEGIN
#define TW_EXTERN_C_END
#endif
#endif

// Marker for default visibility
#define TW_VISIBILITY_DEFAULT __attribute__((visibility("default")))

// Marker for exported classes
#define TW_EXPORT_CLASS

// Marker for exported structs
#define TW_EXPORT_STRUCT

// Marker for exported enums
#define TW_EXPORT_ENUM(type)

// Marker for exported functions
#define TW_EXPORT_FUNC extern

// Marker for exported methods
#define TW_EXPORT_METHOD extern

// Marker for exported properties
#define TW_EXPORT_PROPERTY extern

// Marker for exported static methods
#define TW_EXPORT_STATIC_METHOD extern

// Marker for exported static properties
#define TW_EXPORT_STATIC_PROPERTY extern

// Marker for discardable result (static) method
#define TW_METHOD_DISCARDABLE_RESULT

// Marker for Protobuf types to be serialized across the interface
#define PROTO(x) TWData *

#if __has_feature(assume_nonnull)
#define TW_ASSUME_NONNULL_BEGIN _Pragma("clang assume_nonnull begin")
#define TW_ASSUME_NONNULL_END   _Pragma("clang assume_nonnull end")
#else
#define TW_ASSUME_NONNULL_BEGIN
#define TW_ASSUME_NONNULL_END
#endif

#if defined(__cplusplus) && (__cplusplus >= 201402L)
#  define TW_DEPRECATED(since) [[deprecated("Since " #since)]]
#  define TW_DEPRECATED_FOR(since, replacement) [[deprecated("Since " #since "; use " #replacement)]]
#else
#  define TW_DEPRECATED(since)
#  define TW_DEPRECATED_FOR(since, replacement)
#endif

#if !__has_feature(nullability)
#ifndef _Nullable
#define _Nullable
#endif
#ifndef _Nonnull
#define _Nonnull
#endif
#ifndef _Null_unspecified
#define _Null_unspecified
#endif
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
