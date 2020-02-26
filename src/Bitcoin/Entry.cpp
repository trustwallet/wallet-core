// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Address.h"
#include "SegwitAddress.h"
#include "Signer.h"

using namespace TW::Bitcoin;
using namespace std;

bool Entry::validateAddress(TWCoinType coin, const string& address, TW::byte p2pkh, TW::byte p2sh, const char* hrp) const {
    return SegwitAddress::isValid(address, hrp)
        || Address::isValid(address, {{p2pkh}, {p2sh}});
}

string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TW::byte, const char* hrp) const {
    return SegwitAddress(publicKey, 0, hrp).string();
}

void Entry::sign(TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    AnySignTempl<Signer, Proto::SigningInput>(dataIn, dataOut);
}

void Entry::plan(TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    planTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}
