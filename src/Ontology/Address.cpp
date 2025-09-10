// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Address.h"
#include "OpCode.h"
#include "ParamsBuilder.h"

#include "../Hash.h"
#include "../Base58.h"

#include <stdexcept>
#include <string>

using namespace TW;

namespace TW::Ontology {

Address::Address(const PublicKey& publicKey) {
    std::vector<uint8_t> builder(publicKey.bytes);
    builder.insert(builder.begin(), PUSH_BYTE_33);
    builder.push_back(CHECK_SIG);
    auto builderData = toScriptHash(builder);
    std::copy(builderData.begin(), builderData.end(), _data.begin());
}

Address::Address(const std::string& b58Address) {
    if (!Address::isValid(b58Address)) {
        throw std::runtime_error("Invalid base58 encode address.");
    }
    const auto addressWithVersion = Base58::decodeCheck(b58Address, Rust::Base58Alphabet::Bitcoin, Hash::HasherSha256d);
    std::copy(addressWithVersion.begin() + 1, addressWithVersion.end(), _data.begin());
}

Address::Address(const std::vector<uint8_t>& bytes) {
    if (bytes.size() != size) {
        throw std::runtime_error("Invalid bytes data.");
    }
    std::copy(bytes.begin(), bytes.end(), _data.begin());
}

Address::Address(uint8_t m, const std::vector<Data>& publicKeys) {
    auto builderData = toScriptHash(ParamsBuilder::fromMultiPubkey(m, publicKeys));
    std::copy(builderData.begin(), builderData.end(), _data.begin());
}

Data Address::toScriptHash(const Data& data) {
    return Hash::ripemd(Hash::sha256(data));
}

bool Address::isValid(const std::string& b58Address) noexcept {
    if (b58Address.length() != 34) {
        return false;
    }
    const auto addressWithVersion =
        Base58::decodeCheck(b58Address, Rust::Base58Alphabet::Bitcoin, Hash::HasherSha256d);
    return addressWithVersion.size() == size + 1;
}

std::string Address::string() const {
    std::vector<uint8_t> encodeData(size + 1);
    encodeData[0] = version;
    std::copy(_data.begin(), _data.end(), encodeData.begin() + 1);
    return Base58::encodeCheck(encodeData, Rust::Base58Alphabet::Bitcoin, Hash::HasherSha256d);
}

} // namespace TW::Ontology
