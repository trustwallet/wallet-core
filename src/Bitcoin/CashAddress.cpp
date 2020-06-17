// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "CashAddress.h"
#include "../Coin.h"

#include <TrezorCrypto/cash_addr.h>
#include <TrezorCrypto/ecdsa.h>

#include <array>
#include <cassert>
#include <cstring>

using namespace TW::Bitcoin;

/// Cash address human-readable part
static const std::string cashHRP = "bitcoincash";

/// From https://github.com/bitcoincashorg/bitcoincash.org/blob/master/spec/cashaddr.md

static const uint8_t p2khVersion = 0x00;
static const uint8_t p2shVersion = 0x08;

static constexpr size_t maxHRPSize = 20;
static constexpr size_t maxDataSize = 104;

bool CashAddress::isValid(const std::string& string) {
    auto withPrefix = string;
    if (!std::equal(cashHRP.begin(), cashHRP.end(), string.begin())) {
        withPrefix = cashHRP + ":" + string;
    }

    std::array<char, maxHRPSize + 1> hrp = {0};
    std::array<uint8_t, maxDataSize> data;
    size_t dataLen;
    if (cash_decode(hrp.data(), data.data(), &dataLen, withPrefix.c_str()) == 0 || dataLen != CashAddress::size) {
        return false;
    }
    if (std::strncmp(hrp.data(), cashHRP.c_str(), std::min(cashHRP.size(), maxHRPSize)) != 0) {
        return false;
    }
    return true;
}

CashAddress::CashAddress(const std::string& string) {
    auto withPrefix = string;
    if (!std::equal(cashHRP.begin(), cashHRP.end(), string.begin())) {
        withPrefix = cashHRP + ":" + string;
    }

    std::array<char, maxHRPSize + 1> hrp;
    std::array<uint8_t, maxDataSize> data;
    size_t dataLen;
    auto success = cash_decode(hrp.data(), data.data(), &dataLen, withPrefix.c_str()) != 0;
    if (!success || std::strncmp(hrp.data(), cashHRP.c_str(), std::min(cashHRP.size(), maxHRPSize)) != 0 || dataLen != CashAddress::size) {
        throw std::invalid_argument("Invalid address string");
    }
    std::copy(data.begin(), data.begin() + dataLen, bytes.begin());
}

CashAddress::CashAddress(const std::vector<uint8_t>& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address key data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

CashAddress::CashAddress(const PublicKey& publicKey) {
    if (publicKey.type != TWPublicKeyTypeSECP256k1) {
        throw std::invalid_argument("CashAddress needs a compressed SECP256k1 public key.");
    }
    std::array<uint8_t, 21> payload;
    payload[0] = 0;
    ecdsa_get_pubkeyhash(publicKey.bytes.data(), HASHER_SHA2_RIPEMD, payload.data() + 1);

    size_t outlen = 0;
    auto success = cash_addr_to_data(bytes.data(), &outlen, payload.data(), 21) != 0;
    if (!success || outlen != CashAddress::size) {
        throw std::invalid_argument("unable to cash_addr_to_data");
    }
}

std::string CashAddress::string() const {
    std::array<char, 129> result;
    cash_encode(result.data(), cashHRP.c_str(), bytes.data(), CashAddress::size);
    return result.data();
}

Address CashAddress::legacyAddress() const {
    std::vector<uint8_t> result(Address::size);
    size_t outlen = 0;
    cash_data_to_addr(result.data(), &outlen, bytes.data(), CashAddress::size);
    assert(outlen == 21 && "Invalid length");
    if (result[0] == p2khVersion) {
        result[0] = TW::p2pkhPrefix(TWCoinTypeBitcoinCash);
    } else if (result[0] == p2shVersion) {
        result[0] = TW::p2shPrefix(TWCoinTypeBitcoinCash);
    }
    return Address(result);
}
