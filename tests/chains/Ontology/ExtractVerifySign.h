// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"
#include "PrivateKey.h"
#include "PublicKeyLegacy.h"

namespace TW::Ontology::tests {

inline bool extractVerifySignature(const PrivateKey &priv, const Data &txEncoded, const Data &txHash,
                                   size_t signStartsAt) {
    size_t signatureLen {64};

    auto pub = priv.getPublicKey(TWPublicKeyTypeNIST256p1);
    auto sign = subData(txEncoded, signStartsAt, signatureLen);
    return TrezorCrypto::verifyNist256p1Signature(pub.bytes, sign, txHash);
}

}
