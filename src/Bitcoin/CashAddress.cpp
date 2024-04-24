// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "CashAddress.h"
#include "../Coin.h"

#include <TrezorCrypto/cash_addr.h>
#include <TrezorCrypto/ecdsa.h>

#include <array>
#include <cassert>
#include <utility>

namespace TW::Bitcoin {

/// From https://github.com/bitcoincashorg/bitcoincash.org/blob/master/spec/cashaddr.md
namespace {

enum class Version : uint8_t {
    p2kh = 0x00,
    p2sh = 0x08
};
constexpr size_t maxHRPSize{20};
constexpr size_t maxDataSize{104};

} // namespace

namespace details {

inline std::string buildPrefix(const std::string& hrp, const std::string& string) noexcept {
    if (string.size() < hrp.size() || !std::equal(hrp.begin(), hrp.end(), string.begin())) {
        return hrp + ":" + string;
    }
    return string;
}

inline void determinePrefix(TW::Data& data) noexcept {
    auto& prefix = data.front();
    switch (static_cast<Version>(prefix)) {
    case Version::p2kh:
        prefix = TW::p2pkhPrefix(TWCoinTypeBitcoinCash);
        break;
    case Version::p2sh:
        prefix = TW::p2shPrefix(TWCoinTypeBitcoinCash);
        break;
    }
}

} // namespace details

bool CashAddress::isValid(const std::string& hrp, const std::string& string) noexcept {
    const auto withPrefix = details::buildPrefix(hrp, string);
    std::array<char, maxHRPSize + 1> decodedHRP{0};
    std::array<uint8_t, maxDataSize> data{};
    size_t dataLen;
    const bool decodeValid =
        cash_decode(decodedHRP.data(), data.data(), &dataLen, withPrefix.c_str()) != 0 &&
        dataLen == CashAddress::size;
    return decodeValid &&
           std::string(decodedHRP.data()).compare(0, std::min(hrp.size(), maxHRPSize), hrp) == 0;
}

CashAddress::CashAddress(const std::string& hrp, const std::string& string)
    : hrp(hrp) {
    const auto withPrefix = details::buildPrefix(hrp, string);
    std::array<char, maxHRPSize + 1> decodedHRP{};
    std::array<uint8_t, maxDataSize> data{};
    size_t dataLen;
    bool success = cash_decode(decodedHRP.data(), data.data(), &dataLen, withPrefix.c_str()) != 0;
    if (!success ||
        std::string(decodedHRP.data()).compare(0, std::min(hrp.size(), maxHRPSize), hrp) != 0 ||
        dataLen != CashAddress::size) {
        throw std::invalid_argument("Invalid address string");
    }
    std::copy(data.begin(), data.begin() + dataLen, bytes.begin());
}

CashAddress::CashAddress(std::string hrp, const PublicKey& publicKey)
    : hrp(std::move(hrp)) {
    if (publicKey.type != TWPublicKeyTypeSECP256k1) {
        throw std::invalid_argument("CashAddress needs a compressed SECP256k1 public key.");
    }
    std::array<uint8_t, 21> payload{};
    payload[0] = 0;
    ecdsa_get_pubkeyhash(publicKey.bytes.data(), HASHER_SHA2_RIPEMD, payload.data() + 1);

    size_t outlen = 0;
    auto success = cash_addr_to_data(bytes.data(), &outlen, payload.data(), 21) != 0;
    if (!success || outlen != CashAddress::size) {
        throw std::invalid_argument("unable to cash_addr_to_data");
    }
}

std::string CashAddress::string() const noexcept {
    std::array<char, 129> result{};
    cash_encode(result.data(), hrp.c_str(), bytes.data(), CashAddress::size);
    return result.data();
}

Address CashAddress::legacyAddress() const noexcept {
    Data result(Address::size);
    size_t outlen = 0;
    cash_data_to_addr(result.data(), &outlen, bytes.data(), CashAddress::size);
    assert(outlen == 21 && "Invalid length");
    details::determinePrefix(result);
    return Address(result);
}

Data CashAddress::getData() const {
    Data data(Address::size);
    size_t outlen = 0;
    cash_data_to_addr(data.data(), &outlen, bytes.data(), CashAddress::size);
    return data;
}

} // namespace TW::Bitcoin
