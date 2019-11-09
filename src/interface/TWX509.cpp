// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWX509.h>

#include "../PublicKey.h"

using namespace TW;

TWData *_Nullable TWX509EncodeED25519PublicKey(TWData *_Nonnull publicKey) {
    size_t dataSize = TWDataSize(publicKey);
    uint8_t* dataBytes = TWDataBytes(publicKey);

    if (dataSize != PublicKey::ed25519Size) {
        return nullptr;
    }

    size_t totlen = 12 + dataSize;
    TWData* rv = TWDataCreateWithSize(totlen);
    size_t idx = 0;
    // sequence
    TWDataSet(rv, idx++, 0x30);
    TWDataSet(rv, idx++, (uint8_t)(totlen - 2));
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
    // the key
    TWDataSet(rv, idx++, 0x03); // bit string
    TWDataSet(rv, idx++, (byte) (1 + dataSize));
    TWDataSet(rv, idx++, 0); // number of trailing unused bits
    TWDataReplaceBytes(rv, idx, dataSize, dataBytes);
    return rv;
}

TWData *_Nullable TWX509DecodeED25519PublicKey(TWData *_Nonnull data) {
    size_t dataSize = TWDataSize(data);
    uint8_t* dataBytes = TWDataBytes(data);

    //
    // Setup
    //
    size_t totlen = 44;
    size_t idlen = 5;

    //
    // Pre-decoding check
    //
    if (dataSize != totlen) {
        return nullptr;
    }

    uint8_t doid = dataBytes[8];
    if (doid != 112) {
        return nullptr;
    }

    //
    // Decoding
    //
    size_t idx = 0;
    if (dataBytes[idx++] != 0x30 ||
        dataBytes[idx++] != (totlen - 2) ||
        dataBytes[idx++] != 0x30 ||
        dataBytes[idx++] != idlen ||
        dataBytes[idx++] != 0x06 ||
        dataBytes[idx++] != 3 ||
        dataBytes[idx++] != (1 * 40) + 3 ||
        dataBytes[idx++] != 101) {
        return nullptr;
    }
    idx++; // OID, checked above
    if (dataBytes[idx++] != 0x03 ||
        dataBytes[idx++] != 33 ||
        dataBytes[idx++] != 0) {
        return nullptr;
    }
    TWData* rv = TWDataCreateWithBytes(dataBytes + idx, PublicKey::ed25519Size);
    return rv;
}
