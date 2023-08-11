// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWData.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWBitcoinFee;

/// Calculates the fee of any Bitcoin transaction.
///
/// \param data: the signed transaction in its final form.
/// \param satVb: the satoshis per vbyte amount. The passed on string is interpreted as a unit64_t.
/// \returns the fee denominated in satoshis or nullptr if the transaction failed to be decoded.
TW_EXPORT_STATIC_METHOD
TWString* _Nullable TWBitcoinFeeCalculateFee(TWData* _Nonnull data, TWString* _Nonnull satVb);

TW_EXTERN_C_END
