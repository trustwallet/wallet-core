// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWString.h"
#include "TWPrivateKey.h"

TW_EXTERN_C_BEGIN

/// Bitcoin message signing and verification.
///
/// Bitcoin Core and some other wallets support a message signing & verification format, to create a proof (a signature)
/// that someone has access to the private keys of a specific address.
/// This feature currently works on old legacy addresses only.
TW_EXPORT_STRUCT
struct TWBitcoinMessageSigner;

/// Sign a message.
///
/// \param privateKey: the private key used for signing
/// \param address: the address that matches the privateKey, must be a legacy address (P2PKH)
/// \param message: A custom message which is input to the signing.
/// \note Address is derived assuming compressed public key format.
/// \returns the signature, Base64-encoded.  On invalid input empty string is returned. Returned object needs to be deleteed after use.
TW_EXPORT_STATIC_METHOD
TWString* _Nonnull TWBitcoinMessageSignerSignMessage(const struct TWPrivateKey* _Nonnull privateKey, TWString* _Nonnull address, TWString* _Nonnull message);

/// Verify signature for a message.
///
/// \param address: address to use, only legacy is supported
/// \param message: the message signed (without prefix)
/// \param signature: in Base64-encoded form.
/// \returns false on any invalid input (does not throw).
TW_EXPORT_STATIC_METHOD
bool TWBitcoinMessageSignerVerifyMessage(TWString* _Nonnull address, TWString* _Nonnull message, TWString* _Nonnull signature);

TW_EXTERN_C_END
