// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Codec.h"

#include "HexCoding.h"

using namespace TW;
using namespace TW::Elrond;

void normalizeHexStringTopLevel(std::string& value);

std::string Codec::encodeStringTopLevel(std::string value) {
    std::string encoded = hex(TW::data(value));
    return encoded;
}

std::string Codec::encodeUint64TopLevel(uint64_t value) {
    std::string encoded = hex(value);
    normalizeHexStringTopLevel(encoded);
    return encoded;
}

// For reference, see https://docs.elrond.com/developers/developer-reference/elrond-serialization-format/#arbitrary-width-big-numbers.
std::string Codec::encodeBigIntTopLevel(uint256_t value) {
    // First, load the 256-bit value into a "Data" object.
    auto valueAsData = Data();
    encode256BE(valueAsData, value, 256);
    
    std::string encoded = hex(valueAsData);
    normalizeHexStringTopLevel(encoded);
    return encoded;
}

/// Normalizes a hex string. The passed string is mutated.
/// E.g.: "A" -> "0A"; "00A" -> "0A".
void normalizeHexStringTopLevel(std::string& value) {
    // Strip the redundant leading zeros (if any).
    value.erase(0, value.find_first_not_of('0'));

    // Make sure the result has even length (valid hex).
    if (value.size() % 2 != 0) {
        value.insert(0, 1, '0');
    }
}

std::string Codec::encodeAddressTopLevel(const Address& address) {
    std::string encoded = hex(address.getKeyHash());
    return encoded;
}
