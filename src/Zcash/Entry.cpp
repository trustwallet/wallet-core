// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Signer.h"
#include "TAddress.h"

namespace TW::Zcash {

bool Entry::validateAddress([[maybe_unused]] TWCoinType coin, const std::string& address, [[maybe_unused]] const PrefixVariant& addressPrefix) const {
    return TAddress::isValid(address);
}

std::string Entry::deriveAddress([[maybe_unused]] TWCoinType coin, const PublicKey& publicKey, [[maybe_unused]] TWDerivation derivation, const PrefixVariant& addressPrefix) const {
    byte p2pkh = getFromPrefixPkhOrDefault(addressPrefix, coin);
    return TAddress(publicKey, p2pkh).string();
}

Data Entry::addressToData([[maybe_unused]] TWCoinType coin, const std::string& address) const {
    const auto addr = TAddress(address);
    return {addr.bytes.begin() + 2, addr.bytes.end()};
}

void Entry::sign([[maybe_unused]] TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Bitcoin::Proto::SigningInput>(dataIn, dataOut);
}

void Entry::plan([[maybe_unused]] TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    planTemplate<Signer, Bitcoin::Proto::SigningInput>(dataIn, dataOut);
}

} // namespace TW::Zcash
