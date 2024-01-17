// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "EIP1014.h"
#include "AddressChecksum.h"
#include "Hash.h"
#include "HexCoding.h"

namespace TW::Ethereum {

Data create2Address(const std::string& from, const Data& salt, const Data& initCodeHash) {
    if (salt.size() != 32) {
        throw std::runtime_error("Error: salt must be 32 bytes.");
    }
    if (initCodeHash.size() != 32) {
        throw std::runtime_error("Error: initCodeHash must be 32 bytes.");
    }
    Data input = {0xff};
    append(input, parse_hex(from));
    append(input, salt);
    append(input, initCodeHash);
    auto hash = Hash::keccak256(input);
    return Data(hash.end() - 20, hash.end());
}

} // namespace TW::Ethereum
