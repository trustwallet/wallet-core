// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

namespace TW::Aptos {

bool Entry::validateAddress(TWCoinType coin, const std::string& address, const PrefixVariant& addressPrefix) const {
    return validateAddressRust(coin, address, addressPrefix);
}

std::string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TWDerivation derivation, const PrefixVariant& addressPrefix) const {
    return deriveAddressRust(coin, publicKey, derivation, addressPrefix);
}

void Entry::sign([[maybe_unused]] TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signRust(dataIn, coin, dataOut);
}

Data Entry::preImageHashes([[maybe_unused]] TWCoinType coin, const Data& txInputData) const {
    return preImageHashesRust(coin, txInputData);
}

void Entry::compile([[maybe_unused]] TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    compileRust(coin, txInputData, signatures, publicKeys, dataOut);
}

} // namespace TW::Aptos
