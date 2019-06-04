// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"

#include "../Hash.h"

using namespace TW;
using namespace TW::Waves;

Data Signer::sign(const PrivateKey &privateKey, Transaction &transaction) noexcept {
    try {
        auto bytesToSign = transaction.serializeToSign();
        auto signature = privateKey.sign(bytesToSign, TWCurveCurve25519);
        return signature;
    } catch (...) {
        return Data();
    }
}
