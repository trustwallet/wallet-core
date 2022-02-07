// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Address.h"
#include "Ronin/Address.h"
#include "Signer.h"

using namespace TW::Ethereum;
using namespace std;

bool Entry::validateAddress(TWCoinType coin, const string& address, TW::byte, TW::byte, const char*) const {
    switch(coin) {
    case TWCoinTypeRonin:
        return Ronin::Address::isValid(address);
    default:
        return Address::isValid(address);
    }
}

string Entry::normalizeAddress(TWCoinType coin, const string& address) const {
    switch(coin) {
    case TWCoinTypeRonin:
        return Ronin::Address(address).string();
    default:
         // normalized with EIP55 checksum
        return Address(address).string();
    }
}

string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TW::byte, const char*) const {
    switch(coin) {
    case TWCoinTypeRonin:
        return Ronin::Address(publicKey).string();
    default:
        return Address(publicKey).string();
    }
}

void Entry::sign(TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

string Entry::signJSON(TWCoinType coin, const std::string& json, const Data& key) const { 
    return Signer::signJSON(json, key);
}
