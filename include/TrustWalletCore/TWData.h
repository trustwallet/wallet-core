// Copyright © 2017-2023 Trust Wallet.
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
///
/// \param bytes Non-null raw bytes buffer
/// \param size size of the buffer
/// \return Non-null filled block of data.
TWData *_Nonnull TWDataCreateWithBytes(const uint8_t *_Nonnull bytes, size_t size) TW_VISIBILITY_DEFAULT;

/// Creates an uninitialized block of data with the provided size.
///
/// \param size size for the block of data
/// \return Non-null uninitialized block of data with the provided size
TWData *_Nonnull TWDataCreateWithSize(size_t size) TW_VISIBILITY_DEFAULT;

/// Creates a block of data by copying another block of data.
///
/// \param data buffer that need to be copied
/// \return Non-null filled block of data.
TWData *_Nonnull TWDataCreateWithData(TWData *_Nonnull data) TW_VISIBILITY_DEFAULT;

/// Creates a block of data from a hexadecimal string.  Odd length is invalid (intended grouping to bytes is not obvious).
///
/// \param hex input hex string
/// \return Non-null filled block of data
TWData *_Nullable TWDataCreateWithHexString(const TWString *_Nonnull hex) TW_VISIBILITY_DEFAULT;

/// Returns the size in bytes.
///
/// \param data A non-null valid block of data
/// \return the size of the given block of data
size_t TWDataSize(TWData *_Nonnull data) TW_VISIBILITY_DEFAULT;

/// Returns the raw pointer to the contents of data.
///
/// \param data A non-null valid block of data
/// \return the raw pointer to the contents of data
uint8_t *_Nonnull TWDataBytes(TWData *_Nonnull data) TW_VISIBILITY_DEFAULT;

/// Returns the byte at the provided index.
///
/// \param data A non-null valid block of data
/// \param index index of the byte that we want to fetch - index need to be < TWDataSize(data)
/// \return the byte at the provided index
uint8_t TWDataGet(TWData *_Nonnull data, size_t index) TW_VISIBILITY_DEFAULT;

/// Sets the byte at the provided index.
///
/// \param data A non-null valid block of data
/// \param index index of the byte that we want to set - index need to be < TWDataSize(data)
/// \param byte Given byte to be written in data
void TWDataSet(TWData *_Nonnull data, size_t index, uint8_t byte) TW_VISIBILITY_DEFAULT;

/// Copies a range of bytes into the provided buffer.
///
/// \param data A non-null valid block of data
/// \param start starting index of the range - index need to be < TWDataSize(data)
/// \param size size of the range we want to copy - size need to be < TWDataSize(data) - start
/// \param output The output buffer where we want to copy the data.
void TWDataCopyBytes(TWData *_Nonnull data, size_t start, size_t size, uint8_t *_Nonnull output) TW_VISIBILITY_DEFAULT;

/// Replaces a range of bytes with the contents of the provided buffer.
///
/// \param data A non-null valid block of data
/// \param start starting index of the range - index need to be < TWDataSize(data)
/// \param size size of the range we want to replace - size need to be < TWDataSize(data) - start
/// \param bytes The buffer that will replace the range of data
void TWDataReplaceBytes(TWData *_Nonnull data, size_t start, size_t size, const uint8_t *_Nonnull bytes) TW_VISIBILITY_DEFAULT;

/// Appends data from a byte array.
///
/// \param data A non-null valid block of data
/// \param bytes Non-null byte array
/// \param size The size of the byte array
void TWDataAppendBytes(TWData *_Nonnull data, const uint8_t *_Nonnull bytes, size_t size) TW_VISIBILITY_DEFAULT;

/// Appends a single byte.
///
/// \param data A non-null valid block of data
/// \param byte A single byte
void TWDataAppendByte(TWData *_Nonnull data, uint8_t byte) TW_VISIBILITY_DEFAULT;

/// Appends a block of data.
///
/// \param data A non-null valid block of data
/// \param append A non-null valid block of data
void TWDataAppendData(TWData *_Nonnull data, TWData *_Nonnull append) TW_VISIBILITY_DEFAULT;

/// Reverse the bytes.
///
/// \param data A non-null valid block of data
void TWDataReverse(TWData *_Nonnull data) TW_VISIBILITY_DEFAULT;

/// Sets all bytes to the given value.
///
/// \param data A non-null valid block of data
void TWDataReset(TWData *_Nonnull data) TW_VISIBILITY_DEFAULT;

/// Deletes a block of data created with a `TWDataCreate*` method.
///
/// \param data A non-null valid block of data
void TWDataDelete(TWData *_Nonnull data) TW_VISIBILITY_DEFAULT;

/// Determines whether two data blocks are equal.
///
/// \param lhs left non null block of data to be compared
/// \param rhs right non null block of data to be compared
/// \return true if both block of data are equal, false otherwise
bool TWDataEqual(TWData *_Nonnull lhs, TWData *_Nonnull rhs) TW_VISIBILITY_DEFAULT;

TW_EXTERN_C_END
