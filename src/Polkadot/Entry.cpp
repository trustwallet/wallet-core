// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Entry.h"

#include "Address.h"
#include "Signer.h"

namespace TW::Polkadot {

// Note: avoid business logic from here, rather just call into classes like Address, Signer, etc.

bool Entry::validateAddress([[maybe_unused]] TWCoinType coin, const std::string& address, const PrefixVariant& addressPrefix) const {
    if (auto* prefix = std::get_if<SS58Prefix>(&addressPrefix); prefix) {
        return Address::isValid(address, *prefix);
    }
    return Address::isValid(address);
}

std::string Entry::deriveAddress([[maybe_unused]] TWCoinType coin, const PublicKey& publicKey, [[maybe_unused]] TWDerivation derivation, const PrefixVariant& addressPrefix) const {
    if (auto* ss58Prefix = std::get_if<SS58Prefix>(&addressPrefix); ss58Prefix) {
        return Address(publicKey, *ss58Prefix).string();
    }
    return Address(publicKey).string();
}

Data Entry::addressToData([[maybe_unused]] TWCoinType coin, const std::string& address) const {
    const auto addr = Address(address);
    return {addr.bytes.begin() + 1, addr.bytes.end()};
}

void Entry::sign([[maybe_unused]] TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

} // namespace TW::Polkadot
