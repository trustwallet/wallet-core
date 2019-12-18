// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"

using namespace TW;
using namespace TW::Filecoin;

void Signer::sign(const PrivateKey& privateKey, Transaction& transaction) const noexcept {
    Data to_sign = Hash::blake2b(transaction.cid(), 32);
    transaction.signature = privateKey.sign(to_sign, TWCurveSECP256k1);
}
