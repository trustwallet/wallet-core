// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Codec.h"

#include "HexCoding.h"
#include "uint256.h"

namespace TW::MultiversX {

std::string Codec::encodeString(const std::string& value) {
    std::string encoded = hex(TW::data(value));
    return encoded;
}

std::string Codec::encodeUint64(uint64_t value) {
    std::string encoded = hex(store(uint256_t(value)));
    return encoded;
}

std::string Codec::encodeBigInt(const std::string& value) {
    return encodeBigInt(uint256_t(value));
}

// For reference, see https://docs.multiversx.com/developers/developer-reference/serialization-format#arbitrary-width-big-numbers.
std::string Codec::encodeBigInt(uint256_t value) {
    std::string encoded = hex(store(value));
    return encoded;
}

std::string Codec::encodeAddress(const std::string& bech32Address) {
    Address address;
    Address::decode(bech32Address, address);
    return encodeAddress(address);
}

std::string Codec::encodeAddress(const Address& address) {
    std::string encoded = hex(address.getKeyHash());
    return encoded;
}

} // namespace TW::MultiversX
