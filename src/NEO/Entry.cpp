// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Address.h"
#include "Signer.h"

//using namespace TW::NEO;
using namespace TW;
using namespace std;

bool NEO::Entry::validateAddress([[maybe_unused]] TWCoinType coin, [[maybe_unused]] const string& address, [[maybe_unused]] TW::byte p2pkh, [[maybe_unused]] TW::byte p2sh, [[maybe_unused]] const char* hrp) const {
    return Address::isValid(address);
}

string NEO::Entry::deriveAddress([[maybe_unused]] TWCoinType coin, const PublicKey& publicKey, [[maybe_unused]] TW::byte p2pkh, [[maybe_unused]] const char* hrp) const {
    return Address(publicKey).string();
}

Data NEO::Entry::addressToData([[maybe_unused]] TWCoinType coin, const std::string& address) const {
    const auto addr = Address(address);
    return {addr.bytes.begin(), addr.bytes.end()};
}

void NEO::Entry::sign([[maybe_unused]] TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

void NEO::Entry::plan([[maybe_unused]] TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    planTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}
