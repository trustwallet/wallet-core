// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Bech32Address.h"
#include "Bech32.h"
#include "Data.h"
#include <TrezorCrypto/ecdsa.h>

using namespace TW;

bool Bech32Address::isValid(const std::string& addr) {
    return isValid(addr, "");
}

bool Bech32Address::isValid(const std::string& addr, const std::string& hrp) {
    auto dec = Bech32::decode(addr);
    // check hrp prefix (if given)
    if (hrp.length() > 0 && std::get<0>(dec).compare(0, hrp.length(), hrp) != 0) {
        return false;
    }
    if (std::get<1>(dec).empty()) {
        return false;
    }

    Data conv;
    auto success = Bech32::convertBits<5, 8, false>(conv, std::get<1>(dec));
    if (!success || conv.size() < 2 || conv.size() > 40) {
        return false;
    }

    return true;
}

bool Bech32Address::decode(const std::string& addr, Bech32Address& obj_out, const std::string& hrp) {
    auto dec = Bech32::decode(addr);
    // check hrp prefix (if given)
    if (hrp.length() > 0 && std::get<0>(dec).compare(0, hrp.length(), hrp) != 0) {
        return false;
    }
    if (std::get<1>(dec).empty()) {
        return false;
    }

    Data conv;
    auto success = Bech32::convertBits<5, 8, false>(conv, std::get<1>(dec));
    if (!success || conv.size() < 2 || conv.size() > 40) {
        return false;
    }

    obj_out.setHrp(std::get<0>(dec));
    obj_out.setKey(conv);
    return true;
}

Bech32Address::Bech32Address(const std::string& hrp, Hash::Hasher hasher, const PublicKey& publicKey)
: hrp(hrp) {
    bool skipTypeByte = false;
    // Extended-key / keccak-hash skips first byte (Evmos)
    if (publicKey.type == TWPublicKeyTypeSECP256k1Extended || hasher == Hash::HasherKeccak256) {
        skipTypeByte = true;
    }
    const auto hash = publicKey.hash({}, hasher, skipTypeByte);
    auto key = subData(hash, hash.size() - 20, 20);
    setKey(key);
}

std::string Bech32Address::string() const {
    Data enc;
    if (!Bech32::convertBits<8, 5, true>(enc, keyHash)) {
        return "";
    }
    std::string result = Bech32::encode(hrp, enc, Bech32::ChecksumVariant::Bech32);
    // check back
    Bech32Address obj;
    if (!decode(result, obj, hrp)) {
        return "";
    }
    return result;
}
