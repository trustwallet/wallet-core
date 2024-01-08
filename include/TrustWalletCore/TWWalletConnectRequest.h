// Copyright © 2017-2024 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWCoinType.h"
#include "TWData.h"

TW_EXTERN_C_BEGIN

/// Represents a WalletConnect signing request.
TW_EXPORT_CLASS
struct TWWalletConnectRequest;

/// Parses the WalletConnect signing request as a `SigningInput`.
///
/// \param coin The given coin type to plan the transaction for.
/// \param input The serialized data of a `WalletConnect::Proto::ParseRequestInput` proto object.
/// \return The serialized data of `WalletConnect::Proto::ParseRequestOutput` proto object.
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWWalletConnectRequestParse(enum TWCoinType coin, TWData* _Nonnull input);

TW_EXTERN_C_END
