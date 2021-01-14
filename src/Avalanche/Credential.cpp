// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Credential.h"

#include "../BinaryCoding.h"

using namespace TW::Avalanche;

void Credential::encode(Data& data) const {
    encode32LE(TypeID, data);
    encode32LE(Signatures.size(), data);
    for (auto sig : Signatures) {
        for (auto byte : sig) {
            data.push_back(byte);
        }
    }
}