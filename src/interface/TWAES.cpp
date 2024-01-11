// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWAES.h>

#include <../../src/Encrypt.h>

using namespace TW;

TWData *_Nullable TWAESEncryptCBC(TWData *_Nonnull key, TWData *_Nonnull data, TWData *_Nonnull iv, enum TWAESPaddingMode mode) {
    try {
        Data encrypted = Encrypt::AESCBCEncrypt(*((Data*)key), *((Data*)data), *((Data*)iv), mode);
        return TWDataCreateWithData(&encrypted);
    } catch (...) {
        return nullptr;
    }
}

TWData *_Nullable TWAESDecryptCBC(TWData *_Nonnull key, TWData *_Nonnull data, TWData *_Nonnull iv, enum TWAESPaddingMode mode) {
    try {
        Data decrypted = Encrypt::AESCBCDecrypt(*((Data*)key), *((Data*)data), *((Data*)iv), mode);
        return TWDataCreateWithData(&decrypted);
    } catch (...) {
        return nullptr;
    }
}

TWData *_Nullable TWAESEncryptCTR(TWData *_Nonnull key, TWData *_Nonnull data, TWData *_Nonnull iv) {
    try {
        Data encrypted = Encrypt::AESCTREncrypt(*((Data*)key), *((Data*)data), *((Data*)iv));
        return TWDataCreateWithData(&encrypted);
    } catch (...) {
        return nullptr;
    }
}

TWData *_Nullable TWAESDecryptCTR(TWData *_Nonnull key, TWData *_Nonnull data, TWData *_Nonnull iv) {
    try {
        Data decrypted = Encrypt::AESCTRDecrypt(*((Data*)key), *((Data*)data), *((Data*)iv));
        return TWDataCreateWithData(&decrypted);
    } catch (...) {
        return nullptr;
    }
}
