// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWStoredKeySignError.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// Result of a sealed signing call: an error code and, on success, the signed output.
TW_EXPORT_CLASS
struct TWStoredKeySignResult;

/// Deletes a sealed signing result.
///
/// \param result Non-null pointer to a result
TW_EXPORT_METHOD
void TWStoredKeySignResultDelete(struct TWStoredKeySignResult* _Nonnull result);

/// The outcome of the call; \TWStoredKeySignErrorOk when it signed.
///
/// \param result Non-null pointer to a result
/// \return The error code
TW_EXPORT_PROPERTY
enum TWStoredKeySignError TWStoredKeySignResultError(struct TWStoredKeySignResult* _Nonnull result);

/// The signed output (its format is documented on each signing function), or null on failure.
///
/// \param result Non-null pointer to a result
/// \note Returned object needs to be deleted with \TWDataDelete
/// \return The output if the call succeeded, null otherwise
TW_EXPORT_PROPERTY
TWData* _Nullable TWStoredKeySignResultData(struct TWStoredKeySignResult* _Nonnull result);

/// A fixed description of the failure, for diagnostics; empty on success. It never contains the input,
/// an address, a password or key material.
///
/// \param result Non-null pointer to a result
/// \note Returned object needs to be deleted with \TWStringDelete
/// \return The description
TW_EXPORT_PROPERTY
TWString* _Nonnull TWStoredKeySignResultErrorMessage(struct TWStoredKeySignResult* _Nonnull result);

TW_EXTERN_C_END
