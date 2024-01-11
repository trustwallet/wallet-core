// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Entry.h"

#include "Address.h"
#include "Signer.h"

namespace TW::Nervos {
using namespace std;

bool Entry::validateAddress([[maybe_unused]] TWCoinType coin, const std::string& address, const PrefixVariant& addressPrefix) const {
    auto* hrpPrefix = std::get_if<Bech32Prefix>(&addressPrefix);
    return Address::isValid(address, hrpPrefix ? *hrpPrefix : HRP_NERVOS);
}

std::string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, [[maybe_unused]] TWDerivation derivation, const PrefixVariant& addressPrefix) const {
    const char* hrp = getFromPrefixHrpOrDefault(addressPrefix, coin);
    return Address(publicKey, hrp).string();
}

void Entry::sign([[maybe_unused]] TWCoinType coin, const Data& dataIn, Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

void Entry::plan([[maybe_unused]] TWCoinType coin, const Data& dataIn, Data& dataOut) const {
    planTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

} // namespace TW::Nervos
