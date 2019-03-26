// Copyright © 2017 Pieter Wuille
// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include "../Bech32.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrustWalletCore/TWHRP.h>

using namespace TW::Tendermint;

bool Address::isValid(const std::string& addr) {
    auto dec = Bech32::decode(addr);
    if (dec.second.empty()) {
        return false;
    }

    Data conv;
    auto success =
        Bech32::convertBits<5, 8, false>(conv, Data(dec.second.begin(), dec.second.end()));
    if (!success || conv.size() < 2 || conv.size() > 40) {
        return false;
    }

    return true;
}

bool Address::isValid(const std::string& addr, const std::string& hrp) {
    auto dec = Bech32::decode(addr);
    if (dec.second.empty()) {
        return false;
    }
    if (dec.first != hrp) {
        return false;
    }

    Data conv;
    auto success =
        Bech32::convertBits<5, 8, false>(conv, Data(dec.second.begin(), dec.second.end()));
    if (!success || conv.size() < 2 || conv.size() > 40) {
        return false;
    }

    return true;
}

Address::Address(const std::string& hrp, const PublicKey& publicKey) : hrp(hrp), keyHash() {
    keyHash.resize(20);
    ecdsa_get_pubkeyhash(publicKey.compressed().bytes.data(), HASHER_SHA2_RIPEMD, keyHash.data());
}

std::pair<Address, bool> Address::decode(const std::string& addr) {
    auto dec = Bech32::decode(addr);
    if (dec.second.empty()) {
        return std::make_pair(Address(), false);
    }

    Data conv;
    auto success =
        Bech32::convertBits<5, 8, false>(conv, Data(dec.second.begin(), dec.second.end()));
    if (!success || conv.size() < 2 || conv.size() > 40) {
        return std::make_pair(Address(), false);
    }

    return std::make_pair(Address(dec.first, std::move(conv)), true);
}

std::string Address::string() const {
    Data enc;
    Bech32::convertBits<8, 5, true>(enc, keyHash);
    std::string result = Bech32::encode(hrp, enc);
    if (!decode(result).second) {
        return {};
    }
    return result;
}
