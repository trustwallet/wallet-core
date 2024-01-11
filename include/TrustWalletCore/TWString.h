// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

typedef const void TWData;

/// Defines a resizable string.
///
/// The implementantion of these methods should be language-specific to minimize translation
/// overhead. For instance it should be a `jstring` for Java and an `NSString` for Swift. Create
/// allocates memory, the delete call should be called at the end to release memory.
typedef const void TWString;

/// Creates a TWString from a null-terminated UTF8 byte array. It must be deleted at the end.
///
/// \param bytes a null-terminated UTF8 byte array.
TWString* _Nonnull TWStringCreateWithUTF8Bytes(const char* _Nonnull bytes) TW_VISIBILITY_DEFAULT;

/// Creates a string from a raw byte array and size. It must be deleted at the end.
///
/// \param bytes a raw byte array.
/// \param size the size of the byte array.
TWString* _Nonnull TWStringCreateWithRawBytes(const uint8_t* _Nonnull bytes, size_t size) TW_VISIBILITY_DEFAULT;

/// Creates a hexadecimal string from a block of data. It must be deleted at the end.
///
/// \param data a block of data.
TWString* _Nonnull TWStringCreateWithHexData(TWData* _Nonnull data) TW_VISIBILITY_DEFAULT;

/// Returns the string size in bytes.
///
/// \param string a TWString pointer.
size_t TWStringSize(TWString* _Nonnull string) TW_VISIBILITY_DEFAULT;

/// Returns the byte at the provided index.
///
/// \param string a TWString pointer.
/// \param index the index of the byte.
char TWStringGet(TWString* _Nonnull string, size_t index) TW_VISIBILITY_DEFAULT;

/// Returns the raw pointer to the string's UTF8 bytes (null-terminated).
///
/// \param string a TWString pointer.
const char* _Nonnull TWStringUTF8Bytes(TWString* _Nonnull string) TW_VISIBILITY_DEFAULT;

/// Deletes a string created with a `TWStringCreate*` method and frees the memory.
///
/// \param string a TWString pointer.
void TWStringDelete(TWString* _Nonnull string) TW_VISIBILITY_DEFAULT;

/// Determines whether two string blocks are equal.
///
/// \param lhs a TWString pointer.
/// \param rhs another TWString pointer.
bool TWStringEqual(TWString* _Nonnull lhs, TWString* _Nonnull rhs) TW_VISIBILITY_DEFAULT;

TW_EXTERN_C_END
