// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Address.h"
#include "Signer.h"

using namespace TW;
using namespace TW::Nervos;
using namespace std;

bool Entry::validateAddress(TWCoinType coin, const string& address, byte p2pkh, byte p2sh,
                            const char* hrp) const {
    return Address::isValid(address, hrp);
}

string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, byte p2pkh,
                            const char* hrp) const {
    return Address(publicKey, hrp).string();
}

void Entry::sign(TWCoinType coin, const Data& dataIn, Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}
