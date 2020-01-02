// Copyright © 2017 Pieter Wuille
// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "SegwitAddress.h"
#include "../Bech32.h"

#include <TrezorCrypto/ecdsa.h>
#include <TrustWalletCore/TWHRP.h>

using namespace TW::Bitcoin;

bool SegwitAddress::isValid(const std::string& string) {
    auto dec = Bech32::decode(string);
    if (dec.second.empty()) {
        return false;
    }

    Data conv;
    if (!Bech32::convertBits<5, 8, false>(conv, Data(dec.second.begin() + 1, dec.second.end())) ||
        conv.size() < 2 || conv.size() > 40 || dec.second[0] > 16 ||
        (dec.second[0] == 0 && conv.size() != 20 && conv.size() != 32)) {
        return false;
    }

    return true;
}

bool SegwitAddress::isValid(const std::string& string, const std::string& hrp) {
    auto dec = Bech32::decode(string);
    if (dec.second.empty()) {
        return false;
    }
    if (dec.first != hrp) {
        return false;
    }

    Data conv;
    if (!Bech32::convertBits<5, 8, false>(conv, Data(dec.second.begin() + 1, dec.second.end())) ||
        conv.size() < 2 || conv.size() > 40 || dec.second[0] > 16 ||
        (dec.second[0] == 0 && conv.size() != 20 && conv.size() != 32)) {
        return false;
    }

    return true;
}

SegwitAddress::SegwitAddress(const PublicKey& publicKey, int witver, std::string hrp)
    : hrp(std::move(hrp)), witnessVersion(witver), witnessProgram() {
    if (publicKey.type != TWPublicKeyTypeSECP256k1) {
        throw std::invalid_argument("SegwitAddress needs a compressed SECP256k1 public key.");
    }
    witnessProgram.resize(20);
    ecdsa_get_pubkeyhash(publicKey.compressed().bytes.data(), HASHER_SHA2_RIPEMD,
                         witnessProgram.data());
}

std::pair<SegwitAddress, bool> SegwitAddress::decode(const std::string& addr) {
    auto dec = Bech32::decode(addr);
    if (dec.second.empty()) {
        return std::make_pair(SegwitAddress(), false);
    }

    Data conv;
    if (!Bech32::convertBits<5, 8, false>(conv, Data(dec.second.begin() + 1, dec.second.end())) ||
        conv.size() < 2 || conv.size() > 40 || dec.second[0] > 16 ||
        (dec.second[0] == 0 && conv.size() != 20 && conv.size() != 32)) {
        return std::make_pair(SegwitAddress(), false);
    }

    return std::make_pair(SegwitAddress(dec.first, dec.second[0], conv), true);
}

std::string SegwitAddress::string() const {
    Data enc;
    enc.push_back(static_cast<uint8_t>(witnessVersion));
    Bech32::convertBits<8, 5, true>(enc, witnessProgram);
    std::string result = Bech32::encode(hrp, enc);
    if (!decode(result).second) {
        return {};
    }
    return result;
}

std::pair<SegwitAddress, bool> SegwitAddress::fromRaw(const std::string& hrp,
                                                      const std::vector<uint8_t>& data) {
    Data conv;
    if (!Bech32::convertBits<5, 8, false>(conv, Data(data.begin() + 1, data.end())) ||
        conv.size() < 2 || conv.size() > 40 || data[0] > 16 ||
        (data[0] == 0 && conv.size() != 20 && conv.size() != 32)) {
        return std::make_pair(SegwitAddress(), false);
    }

    return std::make_pair(SegwitAddress(hrp, data[0], conv), true);
}
