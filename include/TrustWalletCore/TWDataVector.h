// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"

TW_EXTERN_C_BEGIN

// A vector of TWData byte arrays
TW_EXPORT_CLASS
struct TWDataVector;

TW_EXPORT_STATIC_METHOD
struct TWDataVector *_Nullable TWDataVectorCreate();

// Create with one element
TW_EXPORT_STATIC_METHOD
struct TWDataVector *_Nullable TWDataVectorCreateWithData(TWData *_Nonnull data);

// Delete must be called at the end
TW_EXPORT_METHOD
void TWDataVectorDelete(struct TWDataVector *_Nonnull dataVector);

// Add an element to the vector
TW_EXPORT_METHOD
void TWDataVectorAdd(struct TWDataVector *_Nonnull dataVector, TWData *_Nonnull data);

// Retrieve the number of elements
TW_EXPORT_PROPERTY
unsigned int TWDataVectorSize(struct TWDataVector *_Nonnull dataVector);

// Retrieve the n-th element. A clone is returned (must be freed).
TW_EXPORT_METHOD
TWData *_Nullable TWDataVectorGet(struct TWDataVector *_Nonnull dataVector, unsigned int index);

TW_EXTERN_C_END
