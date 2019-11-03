// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
    if (dec.second.empty()) {
        return false;
    }
    // check hrp prefix (if given)
    if (hrp.length() > 0 && dec.first.compare(0, hrp.length(), hrp) != 0) {
        return false;
    }

    Data conv;
    auto success = Bech32::convertBits<5, 8, false>(conv, dec.second);
    if (!success || conv.size() < 2 || conv.size() > 40) {
        return false;
    }

    return true;
}

bool Bech32Address::decode(const std::string& addr, Bech32Address& obj_out, const std::string& hrp) {
    auto dec = Bech32::decode(addr);
    if (dec.second.empty()) {
        return false;
    }
    // check hrp prefix (if given)
    if (hrp.length() > 0 && dec.first.compare(0, hrp.length(), hrp) != 0) {
        return false;
    }

    Data conv;
    auto success = Bech32::convertBits<5, 8, false>(conv, dec.second);
    if (!success || conv.size() < 2 || conv.size() > 40) {
        return false;
    }

    obj_out.setHrp(dec.first);
    obj_out.setKey(conv);
    return true;
}

Bech32Address::Bech32Address(std::string hrp, HasherType hasher, const PublicKey& publicKey)
: _hrp(hrp) {
    switch (hasher) {
        case HASHER_SHA2_RIPEMD:
            {
                auto key = Data(20);
                ecdsa_get_pubkeyhash(publicKey.compressed().bytes.data(), HASHER_SHA2_RIPEMD, key.data());
                setKey(key);
            }
            break;
        
        case HASHER_SHA2:
            {
                const auto hashed = Hash::sha256(publicKey.bytes);
                auto key = Data(20);
                std::copy(hashed.end() - 20, hashed.end(), key.begin());
                setKey(key);
            }
            break;

        default:
            throw std::invalid_argument("Invalid HasherType in Bech32Address");
    }
}

std::string Bech32Address::string() const {
    Data enc;
    if (!Bech32::convertBits<8, 5, true>(enc, _keyHash)) {
        return "";
    }
    std::string result = Bech32::encode(_hrp, enc);
    // check back
    Bech32Address obj;
    if (!decode(result, obj, _hrp)) {
        return "";
    }
    return result;
}
