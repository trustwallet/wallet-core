// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include <TrezorCrypto/ed25519.h>

#include <algorithm>

using namespace TW;
using namespace TW::Semux;

void Signer::sign(const PrivateKey &privateKey, Transaction &transaction) noexcept {
    auto hash = transaction.getHash();
    auto signature = privateKey.sign(hash, TWCurveED25519);
    auto pubkey = privateKey.getPublicKey(TWPublicKeyTypeED25519);
    std::copy(signature.begin(), signature.end(), transaction.signature.begin());
    std::copy(pubkey.bytes.begin(), pubkey.bytes.end(),
              transaction.signature.begin() + signature.size());
}
