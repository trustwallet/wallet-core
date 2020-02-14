// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAES.h>

#include <../../src/Encrypt.h>

using namespace TW;

TWData *_Nullable TWAESCBCEncrypt(TWData *_Nonnull key, TWData *_Nonnull data, TWData *_Nonnull iv) {
    try {
        Data encrypted = Encrypt::AESCBCEncrypt(*((Data*)key), *((Data*)data), *((Data*)iv));
        return TWDataCreateWithData(&encrypted);
    } catch (...) {
        return nullptr;
    }
}

TWData *_Nullable TWAESCBCDecrypt(TWData *_Nonnull key, TWData *_Nonnull data, TWData *_Nonnull iv) {
    try {
        Data decrypted = Encrypt::AESCBCDecrypt(*((Data*)key), *((Data*)data), *((Data*)iv));
        return TWDataCreateWithData(&decrypted);
    } catch (...) {
        return nullptr;
    }
}

TWData *_Nullable TWAESCTREncrypt(TWData *_Nonnull key, TWData *_Nonnull data, TWData *_Nonnull iv) {
    try {
        Data encrypted = Encrypt::AESCTREncrypt(*((Data*)key), *((Data*)data), *((Data*)iv));
        return TWDataCreateWithData(&encrypted);
    } catch (...) {
        return nullptr;
    }
}

TWData *_Nullable TWAESCTRDecrypt(TWData *_Nonnull key, TWData *_Nonnull data, TWData *_Nonnull iv) {
    try {
        Data decrypted = Encrypt::AESCTRDecrypt(*((Data*)key), *((Data*)data), *((Data*)iv));
        return TWDataCreateWithData(&decrypted);
    } catch (...) {
        return nullptr;
    }
}
