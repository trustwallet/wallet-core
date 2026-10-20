// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "StoredKey.h"

#include <TrustWalletCore/TWCoinType.h>

#include <string>

namespace TW::Keystore {

/// Sealed signing (Security Layer, Phase 2).
///
/// Signs `signingInput` for `coin` with the key of the stored account whose address is `address`,
/// decrypting it with `password`. The private key is derived, injected into the signing input, used,
/// and wiped entirely inside the library — it is NEVER returned to the caller.
///
/// Supported coins and their input contract:
/// - Ethereum: `Ethereum.Proto.SigningInput` with an empty `private_key`.
/// - Bitcoin, Litecoin, Dogecoin: `Bitcoin.Proto.SigningInput` with `signing_v2` set, `coin_type == coin`,
///   and no keys in either `private_key` or `signing_v2.private_keys`.
///
/// Returns the serialized SigningOutput, or an empty `Data` if `coin` is not supported by the sealed
/// path, the input violates its contract, or the stored key has no account for (`coin`, `address`)
/// whose derived key reproduces `address` (caller should treat as error). Never mutates `storedKey`.
/// Throws if `password` cannot decrypt the stored key.
///
/// Design invariant: the keystore is decrypted at most once per call (see SealedSigner.cpp).
Data sealedSign(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password, const Data& signingInput);

} // namespace TW::Keystore
