// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "OpCode.h"
#include "ParamsBuilder.h"

#include "../Hash.h"

#include <TrezorCrypto/base58.h>

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
    Data addressWithVersion(size + 1);
    base58_decode_check(b58Address.c_str(), HASHER_SHA2D, addressWithVersion.data(), size + 1);
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
    Data addressWithVersion(size + 1);
    auto len =
        base58_decode_check(b58Address.c_str(), HASHER_SHA2D, addressWithVersion.data(), size + 1);
    return len == size + 1;
}

std::string Address::string() const {
    std::vector<uint8_t> encodeData(size + 1);
    encodeData[0] = version;
    std::copy(_data.begin(), _data.end(), encodeData.begin() + 1);
    size_t b58StrSize = 34;
    std::string b58Str(b58StrSize, ' ');
    base58_encode_check(encodeData.data(), (int)encodeData.size(), HASHER_SHA2D, &b58Str[0],
                        (int)b58StrSize + 1);
    return b58Str;
}

} // namespace TW::Ontology
