// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Address.h"
#include "../Bitcoin/SegwitAddress.h"
#include "Signer.h"

namespace TW::Groestlcoin {

bool Entry::validateAddress([[maybe_unused]] TWCoinType coin, const std::string& address, const PrefixVariant& addressPrefix) const {
    if (auto* prefix = std::get_if<Base58Prefix>(&addressPrefix); prefix) {
        return Address::isValid(address, {prefix->p2pkh, prefix->p2sh});
    }
    return TW::Bitcoin::SegwitAddress::isValid(address, std::get<Bech32Prefix>(addressPrefix));
}

std::string Entry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, [[maybe_unused]] TWDerivation derivation, const PrefixVariant& addressPrefix) const {
    std::string hrp = getFromPrefixHrpOrDefault(addressPrefix, coin);
    return TW::Bitcoin::SegwitAddress(publicKey, hrp).string();
}

void Entry::sign([[maybe_unused]] TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Bitcoin::Proto::SigningInput>(dataIn, dataOut);
}

void Entry::plan([[maybe_unused]] TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    planTemplate<Signer, Bitcoin::Proto::SigningInput>(dataIn, dataOut);
}

} // namespace TW::Groestlcoin
