// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Codec.h"

#include "HexCoding.h"

using namespace TW;
using namespace TW::Elrond;

std::string Codec::encodeString(const std::string& value) {
    std::string encoded = hex(TW::data(value));
    return encoded;
}

std::string Codec::encodeUint64(uint64_t value) {
    std::string encoded = normalizeHex(hex(value));
    return encoded;
}

std::string Codec::encodeBigInt(const std::string& value) {
    return encodeBigInt(uint256_t(value));
}

// For reference, see https://docs.elrond.com/developers/developer-reference/elrond-serialization-format/#arbitrary-width-big-numbers.
std::string Codec::encodeBigInt(uint256_t value) {
    // First, load the 256-bit value into a "Data" object.
    auto valueAsData = Data();
    encode256BE(valueAsData, value, 256);
    
    std::string encoded = normalizeHex(hex(valueAsData));
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
