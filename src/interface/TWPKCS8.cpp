// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWPKCS8.h>

#include "../PrivateKey.h"

using namespace TW;

TWData *_Nullable TWPKCS8EncodeED25519PrivateKey(TWData *_Nonnull privateKey) {
    uint8_t* privateKeyBytes = TWDataBytes(privateKey);
    size_t privateKeySize = TWDataSize(privateKey);

    if (privateKeySize != PrivateKey::size) {
        return nullptr;
    }

    size_t totlen = 16 + privateKeySize;
    auto rv = TWDataCreateWithSize(totlen);
    size_t idx = 0;
    // sequence
    TWDataSet(rv, idx++, 0x30);
    TWDataSet(rv, idx++, (byte) (totlen - 2));
    // version
    TWDataSet(rv, idx++, 0x02);
    TWDataSet(rv, idx++, 1);
    // v1 - no public key included
    TWDataSet(rv, idx++, 0);
    // Algorithm Identifier
    // sequence
    TWDataSet(rv, idx++, 0x30);
    TWDataSet(rv, idx++, 5);
    // OID
    // https://msdn.microsoft.com/en-us/library/windows/desktop/bb540809%28v=vs.85%29.aspx
    TWDataSet(rv, idx++, 0x06);
    TWDataSet(rv, idx++, 3);
    TWDataSet(rv, idx++, (1 * 40) + 3);
    TWDataSet(rv, idx++, 101);
    TWDataSet(rv, idx++, (uint8_t) 112);
    // params - absent
    // PrivateKey
    TWDataSet(rv, idx++, 0x04);  // octet string
    TWDataSet(rv, idx++, (uint8_t) (2 + privateKeySize));
    // CurvePrivateKey
    TWDataSet(rv, idx++, 0x04);  // octet string
    TWDataSet(rv, idx++, (uint8_t) privateKeySize);
    // the key
    TWDataReplaceBytes(rv, idx, privateKeySize, privateKeyBytes);
    return rv;
}

TWData *_Nullable TWPKCS8DecodeED25519PrivateKey(TWData *_Nonnull data) {
    uint8_t* dataBytes = TWDataBytes(data);
    size_t dataSize = TWDataSize(data);

    //
    // Setup
    //
    size_t totlen = 48;
    size_t idlen = 5;

    //
    // Pre-decoding check
    //
    if (dataSize != totlen) {
        return nullptr;
    }

    int doid = dataBytes[11];
    if (doid != 112) {
        return nullptr;
    }

    //
    // Decoding
    //
    int idx = 0;
    if (dataBytes[idx++] != 0x30 ||
        dataBytes[idx++] != (totlen - 2) ||
        dataBytes[idx++] != 0x02 ||
        dataBytes[idx++] != 1 ||
        dataBytes[idx++] != 0 ||
        dataBytes[idx++] != 0x30 ||
        dataBytes[idx++] != idlen ||
        dataBytes[idx++] != 0x06 ||
        dataBytes[idx++] != 3 ||
        dataBytes[idx++] != (1 * 40) + 3 ||
        dataBytes[idx++] != 101) {
        return nullptr;
    }
    idx++; // OID, checked above
    if (dataBytes[idx++] != 0x04 ||
        dataBytes[idx++] != 34) {
        return nullptr;
    }
    if (dataBytes[idx++] != 0x04 ||
        dataBytes[idx++] != 32) {
        return nullptr;
    }
    TWData* rv = TWDataCreateWithBytes(dataBytes + idx, PrivateKey::size);
    return rv;
}
