// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Ethereum/Entry.h"
#include "HexCoding.h"

namespace TW::Ronin {

bool Entry::validateAddress(TWCoinType coin, const std::string& address, const PrefixVariant& addressPrefix) const {
    return validateAddressRust(coin, address, addressPrefix);
}

std::string Entry::normalizeAddress(TWCoinType coin, const std::string& address) const {
    return normalizeAddressRust(coin, address);
}

std::string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TWDerivation derivation, const PrefixVariant& addressPrefix) const {
    return deriveAddressRust(coin, publicKey, derivation, addressPrefix);
}

Data Entry::addressToData(TWCoinType coin, const std::string& address) const {
    return addressToDataRust(coin, address);
}

void Entry::sign(TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signRust(dataIn, coin, dataOut);
}

// TODO call `signRustJSON` when it's done.
std::string Entry::signJSON(TWCoinType coin, const std::string& json, const Data& key) const {
    return Ethereum::Entry().signJSON(coin, json, key);
}

} // namespace TW::Ronin
