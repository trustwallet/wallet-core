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
/// - EVM coins (every coin whose blockchain is Ethereum, e.g. Ethereum, Smart Chain, Polygon, Ethereum
///   Classic): `Ethereum.Proto.SigningInput` with an empty `private_key`. The key is taken from the
///   account of `coin`; the transaction is always signed under Ethereum (chain selected by `chain_id`).
/// - UTXO coins (Bitcoin, Litecoin, Dogecoin, Bitcoin Cash, Dash, Viacoin, DigiByte, Ravencoin, Firo, Qtum,
///   Zcash, Flux, Groestlcoin, Decred): `Bitcoin.Proto.SigningInput` with `signing_v2` set, `coin_type == coin`,
///   and no keys in either `private_key` or `signing_v2.private_keys`.
///
/// Returns the serialized SigningOutput, or an empty `Data` if `coin` is not supported by the sealed
/// path, the input violates its contract, or the stored key has no account for (`coin`, `address`)
/// whose derived key reproduces `address` (caller should treat as error). Never mutates `storedKey`.
/// Throws if `password` cannot decrypt the stored key.
///
/// Design invariant: the keystore is decrypted at most once per call (see SealedSigner.cpp).
Data sealedSign(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password, const Data& signingInput);

/// Sealed EVM message signing. Each function selects the key exactly like `sealedSign` (the account of
/// (`coin`, `address`), whose derived key must reproduce `address`), accepts only EVM coins, computes
/// the digest itself from structured input, and never signs a caller-supplied raw hash.
///
/// The `Data` results are a 65-byte r || s || v signature with v = 27/28, or empty on failure.
/// All throw if `password` cannot decrypt the stored key, and never mutate `storedKey`.

/// EIP-191 personal_sign of `message`: keccak256("\x19Ethereum Signed Message:\n" + len + message).
Data sealedSignEthereumPersonalMessage(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password, const Data& message);

/// EIP-712 signature over the typed data in `typedDataJson`.
Data sealedSignEthereumTypedData(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password, const std::string& typedDataJson);

/// Biz smart-account signature: signs `Biz.getEncodedHash(...)` with `sender` fixed to `address`, so
/// the result can only authorize operations of the signing account itself.
Data sealedSignBizUserOpHash(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password,
                             const Data& chainId, const std::string& codeAddress, const std::string& codeName, const std::string& codeVersion,
                             const std::string& typeHash, const std::string& domainSeparatorHash, const std::string& userOpHash);

/// EIP-7702 authorization delegating the account to `contractAddress`; returns the signed authorization
/// JSON (same format as `TWEip7702SignAuthorization`), or an empty string on failure.
std::string sealedSignEip7702Authorization(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password,
                                           const Data& chainId, const std::string& contractAddress, const Data& nonce);

} // namespace TW::Keystore
