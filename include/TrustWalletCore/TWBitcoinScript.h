// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"
#include "TWBitcoinSigHashType.h"
#include "TWCoinType.h"
#include "TWData.h"
#include "TWPublicKey.h"

TW_EXTERN_C_BEGIN

/// Bitcoin script manipulating functions
TW_EXPORT_CLASS
struct TWBitcoinScript;

/// Creates an empty script.
///
/// \return A pointer to the script
TW_EXPORT_STATIC_METHOD
struct TWBitcoinScript* _Nonnull TWBitcoinScriptCreate();

/// Creates a script from a raw data representation.
///
/// \param data The data buffer
/// \note Must be deleted with \TWBitcoinScriptDelete
/// \return A pointer to the script
TW_EXPORT_STATIC_METHOD
struct TWBitcoinScript* _Nonnull TWBitcoinScriptCreateWithData(TWData* _Nonnull data);

/// Creates a script from a raw bytes and size.
///
/// \param bytes The buffer
/// \param size The size of the buffer
/// \note Must be deleted with \TWBitcoinScriptDelete
/// \return A pointer to the script
struct TWBitcoinScript* _Nonnull TWBitcoinScriptCreateWithBytes(uint8_t* _Nonnull bytes, size_t size);

/// Creates a script by copying an existing script.
///
/// \param script Non-null pointer to a script
/// \note Must be deleted with \TWBitcoinScriptDelete
/// \return A pointer to the script
TW_EXPORT_STATIC_METHOD
struct TWBitcoinScript* _Nonnull TWBitcoinScriptCreateCopy(const struct TWBitcoinScript* _Nonnull script);

/// Delete/Deallocate a given script.
///
/// \param script Non-null pointer to a script
TW_EXPORT_METHOD
void TWBitcoinScriptDelete(struct TWBitcoinScript* _Nonnull script);

/// Get size of a script
///
/// \param script Non-null pointer to a script
/// \return size of the script
TW_EXPORT_PROPERTY
size_t TWBitcoinScriptSize(const struct TWBitcoinScript* _Nonnull script);

/// Get data of a script
///
/// \param script Non-null pointer to a script
/// \return data of the given script
TW_EXPORT_PROPERTY
TWData* _Nonnull TWBitcoinScriptData(const struct TWBitcoinScript* _Nonnull script);

/// Return script hash of a script
///
/// \param script Non-null pointer to a script
/// \return script hash of the given script
TW_EXPORT_PROPERTY
TWData* _Nonnull TWBitcoinScriptScriptHash(const struct TWBitcoinScript* _Nonnull script);

/// Determines whether this is a pay-to-script-hash (P2SH) script.
///
/// \param script Non-null pointer to a script
/// \return true if this is a pay-to-script-hash (P2SH) script, false otherwise
TW_EXPORT_PROPERTY
bool TWBitcoinScriptIsPayToScriptHash(const struct TWBitcoinScript* _Nonnull script);

/// Determines whether this is a pay-to-witness-script-hash (P2WSH) script.
///
/// \param script Non-null pointer to a script
/// \return true if this is a pay-to-witness-script-hash (P2WSH) script, false otherwise
TW_EXPORT_PROPERTY
bool TWBitcoinScriptIsPayToWitnessScriptHash(const struct TWBitcoinScript* _Nonnull script);

/// Determines whether this is a pay-to-witness-public-key-hash (P2WPKH) script.
///
/// \param script Non-null pointer to a script
/// \return true if this is a pay-to-witness-public-key-hash (P2WPKH) script, false otherwise
TW_EXPORT_PROPERTY
bool TWBitcoinScriptIsPayToWitnessPublicKeyHash(const struct TWBitcoinScript* _Nonnull script);

/// Determines whether this is a witness program script.
///
/// \param script Non-null pointer to a script
/// \return true if this is a witness program script, false otherwise
TW_EXPORT_PROPERTY
bool TWBitcoinScriptIsWitnessProgram(const struct TWBitcoinScript* _Nonnull script);

/// Determines whether 2 scripts have the same content
///
/// \param lhs Non-null pointer to the first script
/// \param rhs Non-null pointer to the second script
/// \return true if both script have the same content
TW_EXPORT_STATIC_METHOD
bool TWBitcoinScriptEqual(const struct TWBitcoinScript* _Nonnull lhs, const struct TWBitcoinScript* _Nonnull rhs);

/// Matches the script to a pay-to-public-key (P2PK) script.
///
/// \param script Non-null pointer to a script
/// \return The public key.
TW_EXPORT_METHOD
TWData* _Nullable TWBitcoinScriptMatchPayToPubkey(const struct TWBitcoinScript* _Nonnull script);

/// Matches the script to a pay-to-public-key-hash (P2PKH).
///
/// \param script Non-null pointer to a script
/// \return the key hash.
TW_EXPORT_METHOD
TWData* _Nullable TWBitcoinScriptMatchPayToPubkeyHash(const struct TWBitcoinScript* _Nonnull script);

/// Matches the script to a pay-to-script-hash (P2SH).
///
/// \param script Non-null pointer to a script
/// \return the script hash.
TW_EXPORT_METHOD
TWData* _Nullable TWBitcoinScriptMatchPayToScriptHash(const struct TWBitcoinScript* _Nonnull script);

/// Matches the script to a pay-to-witness-public-key-hash (P2WPKH).
///
/// \param script Non-null pointer to a script
/// \return the key hash.
TW_EXPORT_METHOD
TWData* _Nullable TWBitcoinScriptMatchPayToWitnessPublicKeyHash(const struct TWBitcoinScript* _Nonnull script);

/// Matches the script to a pay-to-witness-script-hash (P2WSH).
///
/// \param script Non-null pointer to a script
/// \return the script hash, a SHA256 of the witness script..
TW_EXPORT_METHOD
TWData* _Nullable TWBitcoinScriptMatchPayToWitnessScriptHash(const struct TWBitcoinScript* _Nonnull script);

/// Encodes the script.
///
/// \param script Non-null pointer to a script
/// \return The encoded script
TW_EXPORT_METHOD
TWData* _Nonnull TWBitcoinScriptEncode(const struct TWBitcoinScript* _Nonnull script);

/// Builds a standard 'pay to public key' script.
///
/// \param pubkey Non-null pointer to a pubkey
/// \note Must be deleted with \TWBitcoinScriptDelete
/// \return A pointer to the built script
TW_EXPORT_STATIC_METHOD
struct TWBitcoinScript* _Nonnull TWBitcoinScriptBuildPayToPublicKey(TWData* _Nonnull pubkey);

/// Builds a standard 'pay to public key hash' script.
///
/// \param hash Non-null pointer to a PublicKey hash
/// \note Must be deleted with \TWBitcoinScriptDelete
/// \return A pointer to the built script
TW_EXPORT_STATIC_METHOD
struct TWBitcoinScript* _Nonnull TWBitcoinScriptBuildPayToPublicKeyHash(TWData* _Nonnull hash);

/// Builds a standard 'pay to script hash' script.
///
/// \param scriptHash Non-null pointer to a script hash
/// \note Must be deleted with \TWBitcoinScriptDelete
/// \return A pointer to the built script
TW_EXPORT_STATIC_METHOD
struct TWBitcoinScript* _Nonnull TWBitcoinScriptBuildPayToScriptHash(TWData* _Nonnull scriptHash);

/// Builds a pay-to-witness-public-key-hash (P2WPKH) script..
///
/// \param hash Non-null pointer to a witness public key hash
/// \note Must be deleted with \TWBitcoinScriptDelete
/// \return A pointer to the built script
TW_EXPORT_STATIC_METHOD
struct TWBitcoinScript* _Nonnull TWBitcoinScriptBuildPayToWitnessPubkeyHash(TWData* _Nonnull hash);

/// Builds a pay-to-witness-script-hash (P2WSH) script.
///
/// \param scriptHash Non-null pointer to a script hash
/// \note Must be deleted with \TWBitcoinScriptDelete
/// \return A pointer to the built script
TW_EXPORT_STATIC_METHOD
struct TWBitcoinScript* _Nonnull TWBitcoinScriptBuildPayToWitnessScriptHash(TWData* _Nonnull scriptHash);

/// Builds the Ordinals inscripton for BRC20 transfer.
///
/// \param ticker ticker of the brc20
/// \param amount uint64 transfer amount
/// \param pubkey Non-null pointer to a pubkey
/// \note Must be deleted with \TWBitcoinScriptDelete
/// \return A pointer to the built script
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWBitcoinScriptBuildBRC20InscribeTransfer(TWString* _Nonnull ticker, TWString* _Nonnull amount, TWData* _Nonnull pubkey);

/// Builds the Ordinals inscripton for NFT construction.
///
/// \param mimeType the MIME type of the payload
/// \param payload the payload to inscribe
/// \param pubkey Non-null pointer to a pubkey
/// \note Must be deleted with \TWBitcoinScriptDelete
/// \return A pointer to the built script
TW_EXPORT_STATIC_METHOD
TWData* _Nonnull TWBitcoinScriptBuildOrdinalNftInscription(TWString* _Nonnull mimeType, TWData* _Nonnull payload, TWData* _Nonnull pubkey);

/// Builds a appropriate lock script for the given address..
///
/// \param address Non-null pointer to an address
/// \param coin coin type
/// \note Must be deleted with \TWBitcoinScriptDelete
/// \return A pointer to the built script
TW_EXPORT_STATIC_METHOD
struct TWBitcoinScript *_Nonnull TWBitcoinScriptLockScriptForAddress(TWString* _Nonnull address, enum TWCoinType coin);

/// Builds a appropriate lock script for the given address with replay.
TW_EXPORT_STATIC_METHOD
struct TWBitcoinScript *_Nonnull TWBitcoinScriptLockScriptForAddressReplay(TWString *_Nonnull address, enum TWCoinType coin, TWData *_Nonnull blockHash, int64_t blockHeight);

/// Return the default HashType for the given coin, such as TWBitcoinSigHashTypeAll.
///
/// \param coinType coin type
/// \return default HashType for the given coin
TW_EXPORT_STATIC_METHOD
uint32_t TWBitcoinScriptHashTypeForCoin(enum TWCoinType coinType);

TW_EXTERN_C_END
