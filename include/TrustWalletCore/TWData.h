// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

typedef const void TWString;

/// Defines a resizable block of data.
///
/// The implementantion of these methods should be language-specific to minimize translation overhead. For instance it
/// should be a `jbyteArray` for Java and an `NSData` for Swift.
typedef const void TWData;

/// Creates a block of data from a byte array.
TWData *_Nonnull TWDataCreateWithBytes(const uint8_t *_Nonnull bytes, size_t size) TW_VISIBILITY_DEFAULT;

/// Creates an uninitialized block of data with the provided size.
TWData *_Nonnull TWDataCreateWithSize(size_t size) TW_VISIBILITY_DEFAULT;

/// Creates a block of data by copying another block of data.
TWData *_Nonnull TWDataCreateWithData(TWData *_Nonnull data) TW_VISIBILITY_DEFAULT;

/// Creates a block of data from a hexadecimal string.  Odd length is invalid (intended grouping to bytes is not obvious).
TWData *_Nullable TWDataCreateWithHexString(const TWString *_Nonnull hex) TW_VISIBILITY_DEFAULT;

/// Returns the size in bytes.
size_t TWDataSize(TWData *_Nonnull data) TW_VISIBILITY_DEFAULT;

/// Returns the raw pointer to the contents of data.
uint8_t *_Nonnull TWDataBytes(TWData *_Nonnull data) TW_VISIBILITY_DEFAULT;

/// Returns the byte at the provided index.
uint8_t TWDataGet(TWData *_Nonnull data, size_t index) TW_VISIBILITY_DEFAULT;

/// Sets the byte at the provided index.
void TWDataSet(TWData *_Nonnull data, size_t index, uint8_t byte) TW_VISIBILITY_DEFAULT;

/// Copies a range of bytes into the provided buffer.
void TWDataCopyBytes(TWData *_Nonnull data, size_t start, size_t size, uint8_t *_Nonnull output) TW_VISIBILITY_DEFAULT;

/// Replaces a range of bytes with the contents of the provided buffer.
void TWDataReplaceBytes(TWData *_Nonnull data, size_t start, size_t size, const uint8_t *_Nonnull bytes) TW_VISIBILITY_DEFAULT;

/// Appends data from a byte array.
void TWDataAppendBytes(TWData *_Nonnull data, const uint8_t *_Nonnull bytes, size_t size) TW_VISIBILITY_DEFAULT;

/// Appends a single byte.
void TWDataAppendByte(TWData *_Nonnull data, uint8_t byte) TW_VISIBILITY_DEFAULT;

/// Appends a block of data.
void TWDataAppendData(TWData *_Nonnull data, TWData *_Nonnull append) TW_VISIBILITY_DEFAULT;

/// Revereses the bytes.
void TWDataReverse(TWData *_Nonnull data) TW_VISIBILITY_DEFAULT;

/// Sets all bytes to the given value.
void TWDataReset(TWData *_Nonnull data) TW_VISIBILITY_DEFAULT;

/// Deletes a block of data created with a `TWDataCreate*` method.
void TWDataDelete(TWData *_Nonnull data) TW_VISIBILITY_DEFAULT;

/// Determines whether two data blocks are equal.
bool TWDataEqual(TWData *_Nonnull lhs, TWData *_Nonnull rhs) TW_VISIBILITY_DEFAULT;

TW_EXTERN_C_END
