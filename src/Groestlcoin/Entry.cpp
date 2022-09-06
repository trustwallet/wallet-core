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

bool Entry::validateAddress([[maybe_unused]] TWCoinType coin, const std::string& address, TW::byte p2pkh, TW::byte p2sh, const char* hrp) const {
    return TW::Bitcoin::SegwitAddress::isValid(address, hrp) || Address::isValid(address, {p2pkh, p2sh});
}

std::string Entry::deriveAddress([[maybe_unused]] TWCoinType coin, const PublicKey& publicKey, [[maybe_unused]] TW::byte p2pkh, const char* hrp) const {
    return TW::Bitcoin::SegwitAddress(publicKey, hrp).string();
}

void Entry::sign([[maybe_unused]] TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    signTemplate<Signer, Bitcoin::Proto::SigningInput>(dataIn, dataOut);
}

void Entry::plan([[maybe_unused]] TWCoinType coin, const TW::Data& dataIn, TW::Data& dataOut) const {
    planTemplate<Signer, Bitcoin::Proto::SigningInput>(dataIn, dataOut);
}

} // namespace TW::Groestlcoin
