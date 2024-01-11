// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWData.h"

TW_EXTERN_C_BEGIN

/// A vector of TWData byte arrays
TW_EXPORT_CLASS
struct TWDataVector;

/// Creates a Vector of Data.
///
/// \note Must be deleted with \TWDataVectorDelete
/// \return a non-null Vector of Data.
TW_EXPORT_STATIC_METHOD
struct TWDataVector* _Nonnull TWDataVectorCreate();

/// Creates a Vector of Data with the given element
///
/// \param data A non-null valid block of data
/// \return A Vector of data with a single given element
TW_EXPORT_STATIC_METHOD
struct TWDataVector* _Nonnull TWDataVectorCreateWithData(TWData* _Nonnull data);

/// Delete/Deallocate a Vector of Data
///
/// \param dataVector A non-null Vector of data
TW_EXPORT_METHOD
void TWDataVectorDelete(struct TWDataVector* _Nonnull dataVector);

/// Add an element to a Vector of Data. Element is cloned
///
/// \param dataVector A non-null Vector of data
/// \param data A non-null valid block of data
/// \note data input parameter must be deleted on its own
TW_EXPORT_METHOD
void TWDataVectorAdd(struct TWDataVector* _Nonnull dataVector, TWData* _Nonnull data);

/// Retrieve the number of elements
///
/// \param dataVector A non-null Vector of data
/// \return the size of the given vector.
TW_EXPORT_PROPERTY
size_t TWDataVectorSize(const struct TWDataVector* _Nonnull dataVector);

/// Retrieve the n-th element.
///
/// \param dataVector A non-null Vector of data
/// \param index index element of the vector to be retrieved, need to be < TWDataVectorSize
/// \note Returned element must be freed with \TWDataDelete
/// \return A non-null block of data
TW_EXPORT_METHOD
TWData* _Nullable TWDataVectorGet(const struct TWDataVector* _Nonnull dataVector, size_t index);

TW_EXTERN_C_END
