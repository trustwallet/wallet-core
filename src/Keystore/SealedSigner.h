// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "StoredKey.h"

#include <TrustWalletCore/TWCoinType.h>

namespace TW::Keystore {

/// Sealed signing (Security Layer, Phase 2).
///
/// Signs `signingInput` for `coin` using the key held inside `storedKey`, decrypting it with
/// `password`. The private key is derived, injected into the signing input, used, and wiped
/// entirely inside the library — it is NEVER returned to the caller.
///
/// Returns the serialized SigningOutput, or an empty `Data` if `coin` is not yet supported by the
/// sealed path, or (for mnemonic keys) the stored key does not hold exactly one account for `coin`
/// at its default derivation (caller should fall back / treat as error). Never mutates `storedKey`.
///
/// Design invariant: the keystore is decrypted at most once per call (see SealedSigner.cpp).
Data sealedSign(const StoredKey& storedKey, TWCoinType coin, const Data& password, const Data& signingInput);

} // namespace TW::Keystore
