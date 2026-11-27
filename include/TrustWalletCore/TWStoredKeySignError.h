// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TWBase.h"

TW_EXTERN_C_BEGIN

/// Outcome of a sealed signing call (\TWStoredKeySign and the TWStoredKeySign*Message functions).
///
/// Checks run in this order, so the first failing one is reported: input checks that need no key
/// (unsupported coin, invalid input, transaction bytes refused); the account lookup (before the keystore
/// is decrypted, so a call that cannot succeed never runs the KDF); decryption (key format, password,
/// payload); the derived address check; signing.
TW_EXPORT_ENUM(uint32_t)
enum TWStoredKeySignError {
    /// Signed; the result carries the output.
    TWStoredKeySignErrorOk = 0,
    /// The function does not sign for this coin.
    TWStoredKeySignErrorUnsupportedCoin = 1,
    /// The input fails to parse or breaks the function's input contract (e.g. it carries a private key).
    TWStoredKeySignErrorInvalidInput = 2,
    /// A Solana message is exactly a transaction message, so signing it would sign a transaction.
    TWStoredKeySignErrorTransactionBytesRefused = 3,
    /// The stored key has no account for the coin and address.
    TWStoredKeySignErrorAccountNotFound = 4,
    /// The account record is ambiguous (one address under several paths), or the key derived from it
    /// does not produce the address.
    TWStoredKeySignErrorAccountMismatch = 5,
    /// The keystore uses a KDF or cipher this library does not read.
    TWStoredKeySignErrorUnsupportedKeyFormat = 6,
    /// The password fails the keystore MAC check.
    TWStoredKeySignErrorWrongPassword = 7,
    /// The MAC check passes but the decrypted payload is not a valid mnemonic or private key.
    TWStoredKeySignErrorDecryptionFailed = 8,
    /// The signer produced no output, or a signature of the wrong length.
    TWStoredKeySignErrorSigningFailed = 9,
    /// Unexpected failure inside the library.
    TWStoredKeySignErrorInternal = 10,
};

TW_EXTERN_C_END
