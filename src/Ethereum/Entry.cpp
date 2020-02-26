// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Address.h"
#include "AnySigner.h"
#include "Signer.h"

using namespace TW::Ethereum;
using namespace std;

bool Entry::validateAddress(const string& address, TW::byte, TW::byte, const char*) const {
    return Address::isValid(address);
}

string Entry::normalizeAddress(const string& address) const {
    // normalized with EIP55 checksum
    return Address(address).string();
}

string Entry::deriveAddress(const PublicKey& publicKey, TW::byte, const char*) const {
    return Address(publicKey).string();
}

void Entry::sign(const TW::Data& dataIn, TW::Data& dataOut) const {
    AnySignTempl<Signer, Proto::SigningInput>(dataIn, dataOut);
}
