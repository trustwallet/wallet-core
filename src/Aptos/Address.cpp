// Copyright © 2017-2022 Trust Wallet.
// Author: Clement Doumergue
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "HexCoding.h"

namespace {

std::string normalize(const std::string& string, std::size_t hexLen) {
    std::string hexStr((TW::Aptos::Address::size * 2) - hexLen, '0');
    hexStr.append(string);
    return hexStr;
}

} // namespace

namespace TW::Aptos {

bool Address::isValid(const std::string& string) {
    auto address = string;
    if (address.starts_with("0x")) {
        address = address.substr(2);
        if (std::size_t hexLen = address.size(); hexLen < Address::size * 2) {
            address = normalize(address, hexLen);
        }
    }
    if (address.size() != 2 * Address::size) {
        return false;
    }
    const auto data = parse_hex(address);
    return isValid(data);
}

Address::Address(const std::string& string) {
    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address string");
    }
    auto hexFunctor = [&string]() {
        if (std::size_t hexLen = string.size() - 2; string.starts_with("0x") && hexLen < Address::size * 2) {
            //! We have specific address like 0x1, padding it.
            return parse_hex(normalize(string.substr(2), hexLen));
        } else {
            return parse_hex(string);
        }
    };

    const auto data = hexFunctor();
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const Data& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    if (publicKey.type != TWPublicKeyTypeED25519) {
        throw std::invalid_argument("Invalid public key type");
    }
    auto key_data = publicKey.bytes;
    append(key_data, 0x00);
    const auto data = Hash::sha3_256(key_data);
    std::copy(data.begin(), data.end(), bytes.begin());
}

std::string Address::string(bool withPrefix) const {
    std::string output = withPrefix ? "0x" : "";
    return output + hex(bytes);
}

std::string Address::shortString() const {
    std::string s = hex(bytes);
    s.erase(0, s.find_first_not_of('0'));
    return s;
}

BCS::Serializer& operator<<(BCS::Serializer& stream, Address addr) noexcept {
    stream.add_bytes(addr.bytes.begin(), addr.bytes.end());
    return stream;
}

} // namespace TW::Aptos
