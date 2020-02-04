// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "TWBase.h"
#include "TWData.h"
#include "TWPublicKey.h"
#include "TWCoinType.h"

TW_EXTERN_C_BEGIN

TW_EXPORT_CLASS
struct TWFreeCashScript;

/// Creates an empty script.
TW_EXPORT_STATIC_METHOD
struct TWFreeCashScript *_Nonnull TWFreeCashScriptCreate();

/// Creates a script from a raw data representation.
TW_EXPORT_STATIC_METHOD
struct TWFreeCashScript *_Nonnull TWFreeCashScriptCreateWithData(TWData *_Nonnull data);
struct TWFreeCashScript *_Nonnull TWFreeCashScriptCreateWithBytes(uint8_t *_Nonnull bytes, size_t size);

/// Creates a script by copying an existring script.
TW_EXPORT_STATIC_METHOD
struct TWFreeCashScript *_Nonnull TWFreeCashScriptCreateCopy(const struct TWFreeCashScript *_Nonnull script);

TW_EXPORT_METHOD
void TWFreeCashScriptDelete(struct TWFreeCashScript *_Nonnull script);

TW_EXPORT_PROPERTY
size_t TWFreeCashScriptSize(const struct TWFreeCashScript *_Nonnull script);

TW_EXPORT_PROPERTY
TWData *_Nonnull TWFreeCashScriptData(const struct TWFreeCashScript *_Nonnull script);

TW_EXPORT_PROPERTY
TWData *_Nonnull TWFreeCashScriptScriptHash(const struct TWFreeCashScript *_Nonnull script);

/// Determines whether this is a pay-to-script-hash (P2SH) script.
TW_EXPORT_PROPERTY
bool TWFreeCashScriptIsPayToScriptHash(const struct TWFreeCashScript *_Nonnull script);

/// Determines whether this is a pay-to-witness-script-hash (P2WSH) script.
TW_EXPORT_PROPERTY
bool TWFreeCashScriptIsPayToWitnessScriptHash(const struct TWFreeCashScript *_Nonnull script);

/// Determines whether this is a witness programm script.
TW_EXPORT_PROPERTY
bool TWFreeCashScriptIsWitnessProgram(const struct TWFreeCashScript *_Nonnull script);

TW_EXPORT_STATIC_METHOD
bool TWFreeCashScriptEqual(const struct TWFreeCashScript *_Nonnull lhs, const struct TWFreeCashScript *_Nonnull rhs);

/// Matches the script to a pay-to-public-key (P2PK) script.
///
/// - Returns: the public key.
TW_EXPORT_METHOD
TWData *_Nullable TWFreeCashScriptMatchPayToPubkey(const struct TWFreeCashScript *_Nonnull script);

/// Matches the script to a pay-to-public-key-hash (P2PKH).
///
/// - Returns: the key hash.
TW_EXPORT_METHOD
TWData *_Nullable TWFreeCashScriptMatchPayToPubkeyHash(const struct TWFreeCashScript *_Nonnull script);

/// Matches the script to a pay-to-script-hash (P2SH).
///
/// - Returns: the script hash.
TW_EXPORT_METHOD
TWData *_Nullable TWFreeCashScriptMatchPayToScriptHash(const struct TWFreeCashScript *_Nonnull script);

/// Matches the script to a pay-to-witness-public-key-hash (P2WPKH).
///
/// - Returns: the key hash.
TW_EXPORT_METHOD
TWData *_Nullable TWFreeCashScriptMatchPayToWitnessPublicKeyHash(const struct TWFreeCashScript *_Nonnull script);

/// Matches the script to a pay-to-witness-script-hash (P2WSH).
///
/// - Returns: the script hash, a SHA256 of the witness script.
TW_EXPORT_METHOD
TWData *_Nullable TWFreeCashScriptMatchPayToWitnessScriptHash(const struct TWFreeCashScript *_Nonnull script);

/// Encodes the script.
TW_EXPORT_METHOD
TWData *_Nonnull TWFreeCashScriptEncode(const struct TWFreeCashScript *_Nonnull script);

/// Builds a standard 'pay to public key hash' script.
TW_EXPORT_STATIC_METHOD
struct TWFreeCashScript *_Nonnull TWFreeCashScriptBuildPayToPublicKeyHash(TWData *_Nonnull hash);

/// Builds a standard 'pay to script hash' script.
TW_EXPORT_STATIC_METHOD
struct TWFreeCashScript *_Nonnull TWFreeCashScriptBuildPayToScriptHash(TWData *_Nonnull scriptHash);

/// Builds a pay-to-witness-public-key-hash (P2WPKH) script.
TW_EXPORT_STATIC_METHOD
struct TWFreeCashScript *_Nonnull TWFreeCashScriptBuildPayToWitnessPubkeyHash(TWData *_Nonnull hash);

/// Builds a pay-to-witness-script-hash (P2WSH) script.
TW_EXPORT_STATIC_METHOD
struct TWFreeCashScript *_Nonnull TWFreeCashScriptBuildPayToWitnessScriptHash(TWData *_Nonnull scriptHash);

/// Builds a pay-to-public-key-hash (P2PKH) script appropriate for the given address.
TW_EXPORT_STATIC_METHOD
struct TWFreeCashScript *_Nonnull TWFreeCashScriptBuildForAddress(TWString *_Nonnull address, enum TWCoinType coin);

TW_EXTERN_C_END
