// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Address.h"
#include "Signer.h"

namespace TW::Nervos {
using namespace std;

bool Entry::validateAddress([[maybe_unused]] TWCoinType coin, const string& address, byte, byte,
                            const char* hrp) const {
    return Address::isValid(address, hrp);
}

string Entry::deriveAddress([[maybe_unused]] TWCoinType coin, const PublicKey& publicKey, byte,
                            const char* hrp) const {
    return Address(publicKey, hrp).string();
}

void Entry::sign([[maybe_unused]] TWCoinType coin, const Data& dataIn, Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

void Entry::plan([[maybe_unused]] TWCoinType coin, const Data& dataIn, Data& dataOut) const {
    planTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

} // namespace TW::Nervos
