// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Represents a derivation path index in C++ with value and hardened flag.
TW_EXPORT_CLASS
struct TWDerivationPathIndex;

/// Creates a new Index with a value and hardened flag.
/// Must be deleted with TWDerivationPathIndexDelete after use.
///
/// \param value Index value
/// \param hardened Indicates if the Index is hardened.
/// \return A new Index.
TW_EXPORT_STATIC_METHOD
struct TWDerivationPathIndex* _Nonnull TWDerivationPathIndexCreate(uint32_t value, bool hardened);

/// Deletes an Index.
///
/// \param index Index to delete.
TW_EXPORT_METHOD
void TWDerivationPathIndexDelete(struct TWDerivationPathIndex* _Nonnull index);

/// Returns numeric value of an Index.
///
/// \param index Index to get the numeric value of.
TW_EXPORT_PROPERTY
uint32_t TWDerivationPathIndexValue(struct TWDerivationPathIndex* _Nonnull index);

/// Returns hardened flag of an Index.
///
/// \param index Index to get hardened flag.
/// \return true if hardened, false otherwise.
TW_EXPORT_PROPERTY
bool TWDerivationPathIndexHardened(struct TWDerivationPathIndex* _Nonnull index);

/// Returns the string description of a derivation path index.
///
/// \param path Index to get the address of.
/// \return The string description of the derivation path index.
TW_EXPORT_PROPERTY
TWString* _Nonnull TWDerivationPathIndexDescription(struct TWDerivationPathIndex* _Nonnull index);

TW_EXTERN_C_END
