// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWPublicKey.h"
#include "TWCoinType.h"
#include "TWBitcoinSigHashType.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWBitcoinScript;

/// Creates an empty script.
TW_EXPORT_STATIC_METHOD
struct TWBitcoinScript *_Nonnull TWBitcoinScriptCreate();

/// Creates a script from a raw data representation.
TW_EXPORT_STATIC_METHOD
struct TWBitcoinScript *_Nonnull TWBitcoinScriptCreateWithData(TWData *_Nonnull data);
struct TWBitcoinScript *_Nonnull TWBitcoinScriptCreateWithBytes(uint8_t *_Nonnull bytes, size_t size);

/// Creates a script by copying an existring script.
TW_EXPORT_STATIC_METHOD
struct TWBitcoinScript *_Nonnull TWBitcoinScriptCreateCopy(const struct TWBitcoinScript *_Nonnull script);

TW_EXPORT_METHOD
void TWBitcoinScriptDelete(struct TWBitcoinScript *_Nonnull script);

TW_EXPORT_PROPERTY
size_t TWBitcoinScriptSize(const struct TWBitcoinScript *_Nonnull script);

TW_EXPORT_PROPERTY
TWData *_Nonnull TWBitcoinScriptData(const struct TWBitcoinScript *_Nonnull script);

TW_EXPORT_PROPERTY
TWData *_Nonnull TWBitcoinScriptScriptHash(const struct TWBitcoinScript *_Nonnull script);

/// Determines whether this is a pay-to-script-hash (P2SH) script.
TW_EXPORT_PROPERTY
bool TWBitcoinScriptIsPayToScriptHash(const struct TWBitcoinScript *_Nonnull script);

/// Determines whether this is a pay-to-witness-script-hash (P2WSH) script.
TW_EXPORT_PROPERTY
bool TWBitcoinScriptIsPayToWitnessScriptHash(const struct TWBitcoinScript *_Nonnull script);

/// Determines whether this is a pay-to-witness-public-key-hash (P2WPKH) script.
TW_EXPORT_PROPERTY
bool TWBitcoinScriptIsPayToWitnessPublicKeyHash(const struct TWBitcoinScript *_Nonnull script);

/// Determines whether this is a witness programm script.
TW_EXPORT_PROPERTY
bool TWBitcoinScriptIsWitnessProgram(const struct TWBitcoinScript *_Nonnull script);

TW_EXPORT_STATIC_METHOD
bool TWBitcoinScriptEqual(const struct TWBitcoinScript *_Nonnull lhs, const struct TWBitcoinScript *_Nonnull rhs);

/// Matches the script to a pay-to-public-key (P2PK) script.
///
/// - Returns: the public key.
TW_EXPORT_METHOD
TWData *_Nullable TWBitcoinScriptMatchPayToPubkey(const struct TWBitcoinScript *_Nonnull script);

/// Matches the script to a pay-to-public-key-hash (P2PKH).
///
/// - Returns: the key hash.
TW_EXPORT_METHOD
TWData *_Nullable TWBitcoinScriptMatchPayToPubkeyHash(const struct TWBitcoinScript *_Nonnull script);

/// Matches the script to a pay-to-script-hash (P2SH).
///
/// - Returns: the script hash.
TW_EXPORT_METHOD
TWData *_Nullable TWBitcoinScriptMatchPayToScriptHash(const struct TWBitcoinScript *_Nonnull script);

/// Matches the script to a pay-to-witness-public-key-hash (P2WPKH).
///
/// - Returns: the key hash.
TW_EXPORT_METHOD
TWData *_Nullable TWBitcoinScriptMatchPayToWitnessPublicKeyHash(const struct TWBitcoinScript *_Nonnull script);

/// Matches the script to a pay-to-witness-script-hash (P2WSH).
///
/// - Returns: the script hash, a SHA256 of the witness script.
TW_EXPORT_METHOD
TWData *_Nullable TWBitcoinScriptMatchPayToWitnessScriptHash(const struct TWBitcoinScript *_Nonnull script);

/// Encodes the script.
TW_EXPORT_METHOD
TWData *_Nonnull TWBitcoinScriptEncode(const struct TWBitcoinScript *_Nonnull script);

/// Builds a standard 'pay to public key' script.
TW_EXPORT_STATIC_METHOD
struct TWBitcoinScript *_Nonnull TWBitcoinScriptBuildPayToPublicKey(TWData *_Nonnull pubkey);

/// Builds a standard 'pay to public key hash' script.
TW_EXPORT_STATIC_METHOD
struct TWBitcoinScript *_Nonnull TWBitcoinScriptBuildPayToPublicKeyHash(TWData *_Nonnull hash);

/// Builds a standard 'pay to script hash' script.
TW_EXPORT_STATIC_METHOD
struct TWBitcoinScript *_Nonnull TWBitcoinScriptBuildPayToScriptHash(TWData *_Nonnull scriptHash);

/// Builds a pay-to-witness-public-key-hash (P2WPKH) script.
TW_EXPORT_STATIC_METHOD
struct TWBitcoinScript *_Nonnull TWBitcoinScriptBuildPayToWitnessPubkeyHash(TWData *_Nonnull hash);

/// Builds a pay-to-witness-script-hash (P2WSH) script.
TW_EXPORT_STATIC_METHOD
struct TWBitcoinScript *_Nonnull TWBitcoinScriptBuildPayToWitnessScriptHash(TWData *_Nonnull scriptHash);

/// Builds a appropriate lock script for the given address.
TW_EXPORT_STATIC_METHOD
struct TWBitcoinScript *_Nonnull TWBitcoinScriptLockScriptForAddress(TWString *_Nonnull address, enum TWCoinType coin);

// Return the default HashType for the given coin, such as TWBitcoinSigHashTypeAll.
TW_EXPORT_STATIC_METHOD
uint32_t TWBitcoinScriptHashTypeForCoin(enum TWCoinType coinType);

TW_EXTERN_C_END
