// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Entry.h"

#include "Address.h"
#include "Signer.h"

#include "TheOpenNetwork/wallet/WalletV4R2.h"
#include "WorkchainType.h"

namespace TW::TheOpenNetwork {

bool Entry::validateAddress([[maybe_unused]] TWCoinType coin, [[maybe_unused]] const std::string& address, [[maybe_unused]] const PrefixVariant& addressPrefix) const {
    return Address::isValid(address);
}

std::string Entry::normalizeAddress([[maybe_unused]] TWCoinType coin, const std::string& address) const {
    return Address(address).string(true, true, false);
}

std::string Entry::deriveAddress([[maybe_unused]] TWCoinType coin, const PublicKey& publicKey, [[maybe_unused]] TWDerivation derivation, [[maybe_unused]] const PrefixVariant& addressPrefix) const {
    return WalletV4R2(publicKey, WorkchainType::Basechain).getAddress().string();
}

void Entry::sign([[maybe_unused]] TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Proto::SigningInput>(dataIn, dataOut);
}

} // namespace TW::TheOpenNetwork
