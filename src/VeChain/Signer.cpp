// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"

#include "../Ethereum/RLP.h"
#include "../Ethereum/Signer.h"
#include "../Hash.h"

using namespace TW;
using namespace TW::VeChain;

Data Signer::sign(const PrivateKey& privateKey, Transaction& transaction) noexcept {
    auto encoded = transaction.encode();
    auto hash = Hash::blake2b(encoded, 32);
    auto signature = privateKey.sign(hash);
    return Data(signature.begin(), signature.end());
}
