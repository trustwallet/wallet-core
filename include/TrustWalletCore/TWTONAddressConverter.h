// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWString.h"

TW_EXTERN_C_BEGIN

/// TON address operations.
TW_EXPORT_CLASS
struct TWTONAddressConverter;

/// Converts a TON user address into a Bag of Cells (BoC) with a single root Cell.
/// The function is mostly used to request a Jetton user address via `get_wallet_address` RPC.
/// https://docs.ton.org/develop/dapps/asset-processing/jettons#retrieving-jetton-wallet-addresses-for-a-given-user
///
/// \param address Address to be converted into a Bag Of Cells (BoC).
/// \return Pointer to a base64 encoded Bag Of Cells (BoC). Null if invalid address provided.
TW_EXPORT_STATIC_METHOD
TWString *_Nullable TWTONAddressConverterToBoc(TWString *_Nonnull address);

/// Parses a TON address from a Bag of Cells (BoC) with a single root Cell.
/// The function is mostly used to parse a Jetton user address received on `get_wallet_address` RPC.
/// https://docs.ton.org/develop/dapps/asset-processing/jettons#retrieving-jetton-wallet-addresses-for-a-given-user
///
/// \param boc Base64 encoded Bag Of Cells (BoC).
/// \return Pointer to a Jetton address.
TW_EXPORT_STATIC_METHOD
TWString *_Nullable TWTONAddressConverterFromBoc(TWString *_Nonnull boc);

/// Converts any TON address format to user friendly with the given parameters.
///
/// \param address raw or user-friendly address to be converted.
/// \param bounceable whether the result address should be bounceable.
/// \param testnet whether the result address should be testnet.
/// \return user-friendly address str.
TW_EXPORT_STATIC_METHOD
TWString *_Nullable TWTONAddressConverterToUserFriendly(TWString *_Nonnull address, bool bounceable, bool testnet);

TW_EXTERN_C_END
