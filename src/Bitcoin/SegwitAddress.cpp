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
    return std::get<2>(decode(string));
}

bool SegwitAddress::isValid(const std::string& string, const std::string& hrp) {
    auto decoded = decode(string);
    if (!std::get<2>(decoded)) {
        return false;
    }
    // extra step to check hrp
    auto dec = Bech32::decode(string);
    if (dec.first != hrp) {
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

std::tuple<SegwitAddress, std::string, bool> SegwitAddress::decode(const std::string& addr) {
    auto resp = std::make_tuple(SegwitAddress(), "", false);
    auto dec = Bech32::decode(addr);
    if (dec.second.empty()) {
        // bech32 decode fails, or decoded data is empty
        return resp;
    }
    assert(dec.second.size() >= 1);

    // First byte is Segwit version
    // Only version 0 is currently supported; BIP173 BIP350
    if (dec.second[0] != 0) {
        return resp;
    }

    auto raw = fromRaw(dec.first, dec.second);
    return std::make_tuple(raw.first, dec.first, raw.second);
}

std::string SegwitAddress::string() const {
    Data enc;
    enc.push_back(static_cast<uint8_t>(witnessVersion));
    Bech32::convertBits<8, 5, true>(enc, witnessProgram);
    std::string result = Bech32::encode(hrp, enc);
    if (!std::get<2>(decode(result))) {
        return {};
    }
    return result;
}

std::pair<SegwitAddress, bool> SegwitAddress::fromRaw(const std::string& hrp,
                                                      const std::vector<uint8_t>& data) {
    auto resp = std::make_pair(SegwitAddress(), false);
    if (data.size() == 0) {
        return resp;
    }
    byte segwitVersion = data[0];
    Data conv;
    if (!Bech32::convertBits<5, 8, false>(conv, Data(data.begin() + 1, data.end())) ||
        conv.size() < 2 || conv.size() > 40 || segwitVersion > 16 ||
        (segwitVersion == 0 && conv.size() != 20 && conv.size() != 32)) {
        return resp;
    }

    return std::make_pair(SegwitAddress(hrp, data[0], conv), true);
}
