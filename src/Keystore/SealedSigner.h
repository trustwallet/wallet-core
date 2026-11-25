// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"
#include "StoredKey.h"

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWStoredKeySignError.h>

#include <string>

namespace TW::Keystore {

/// Outcome of a sealed signing call: `error` (see TWStoredKeySignError.h for the cases and the order
/// they are checked in) and, when it is `TWStoredKeySignErrorOk`, the output in `data`.
///
/// `message` is a fixed description of the failure, for diagnostics. It is a string literal by
/// construction, so it can never carry the input, an address, a password or key material.
struct SealedSignResult {
    TWStoredKeySignError error = TWStoredKeySignErrorOk;
    Data data;
    const char* message = "";

    bool ok() const { return error == TWStoredKeySignErrorOk; }

    static SealedSignResult success(Data output) { return {TWStoredKeySignErrorOk, std::move(output), ""}; }
    static SealedSignResult failure(TWStoredKeySignError error, const char* message) { return {error, {}, message}; }
};

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
/// - Tron: `Tron.Proto.SigningInput` with `transaction` or `raw_json` and an empty `private_key`. A
///   `raw_json` input keeps the Tron signer's checks (`txID` must be the hash of the transaction rebuilt
///   from `raw_data`, and of `raw_data_hex` when present); a mismatch yields an output with
///   `Error_tx_hash_mismatch`, exactly as on the normal path.
/// - Solana: `Solana.Proto.SigningInput` for a library-built transaction or with `raw_message` (dApp
///   transaction), with `private_key`, `fee_payer_private_key` and
///   `create_nonce_account.nonce_account_private_key` all empty.
///
/// On success `data` is the serialized SigningOutput. Errors the chain signer itself reports (e.g. a
/// Tron `raw_json` txID mismatch) are a successful call whose SigningOutput carries the error, as with
/// AnySigner. Never mutates `storedKey`; never throws.
///
/// Design invariant: the keystore is decrypted at most once per call (see SealedSigner.cpp).
SealedSignResult sealedSign(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password, const Data& signingInput);

/// Sealed EVM message signing. Each function selects the key exactly like `sealedSign` (the account of
/// (`coin`, `address`), whose derived key must reproduce `address`), accepts only EVM coins, computes
/// the digest itself from structured input, and never signs a caller-supplied raw hash.
///
/// On success `data` is a 65-byte r || s || v signature with v = 27/28 (EIP-7702: the signed
/// authorization JSON as UTF-8). Non-EVM coins are `TWStoredKeySignErrorUnsupportedCoin`. All never
/// mutate `storedKey` and never throw.

/// EIP-191 personal_sign of `message`: keccak256("\x19Ethereum Signed Message:\n" + len + message).
SealedSignResult sealedSignEthereumPersonalMessage(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password, const Data& message);

/// EIP-712 signature over the typed data in `typedDataJson`.
SealedSignResult sealedSignEthereumTypedData(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password, const std::string& typedDataJson);

/// Biz smart-account signature: signs `Biz.getEncodedHash(...)` with `sender` fixed to `address`, so
/// the result can only authorize operations of the signing account itself.
SealedSignResult sealedSignBizUserOpHash(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password,
                                         const Data& chainId, const std::string& codeAddress, const std::string& codeName, const std::string& codeVersion,
                                         const std::string& typeHash, const std::string& domainSeparatorHash, const std::string& userOpHash);

/// EIP-7702 authorization delegating the account to `contractAddress`; `data` is the signed
/// authorization JSON as UTF-8 (same JSON as `TWEip7702SignAuthorization`).
SealedSignResult sealedSignEip7702Authorization(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password,
                                                const Data& chainId, const std::string& contractAddress, const Data& nonce);

/// Sealed Tron message signing, same as `Tron::MessageSigner::signMessage`:
/// keccak256("\x19TRON Signed Message:\n" + len + message), as a 65-byte r || s || v (v = 27/28).
/// Any `coin` other than Tron is `TWStoredKeySignErrorUnsupportedCoin`. Account selection and errors as
/// for the EVM message functions.
SealedSignResult sealedSignTronMessage(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password, const Data& message);

/// Sealed Solana message signing: 64-byte ed25519 signature over the raw `message` bytes (same as the
/// Solana message signer for a UTF-8 message). Bytes for which `isSolanaTransactionMessage` holds are
/// `TWStoredKeySignErrorTransactionBytesRefused` (so it never signs a transaction); any `coin` other than
/// Solana is `TWStoredKeySignErrorUnsupportedCoin`. Account selection and errors as for the EVM message
/// functions.
SealedSignResult sealedSignSolanaMessage(const StoredKey& storedKey, TWCoinType coin, const std::string& address, const Data& password, const Data& message);

/// Whether `message` is exactly a Solana transaction message: a complete legacy or v0 message that
/// passes the validator's header / index rules and uses up every byte. Same rule as the App
/// (SC-140951, `solana-transaction-message.ts`). Exposed for tests.
bool isSolanaTransactionMessage(const Data& message);

} // namespace TW::Keystore

/// Wrapper for C interface.
struct TWStoredKeySignResult {
    TW::Keystore::SealedSignResult impl;
};
