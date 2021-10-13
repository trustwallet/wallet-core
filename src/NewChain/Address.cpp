// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "../HexCoding.h"
#include "Ethereum/AddressChecksum.h"
#include "Ethereum/Address.h"

using EthAddress = TW::Ethereum::Address;

using namespace TW::NewChain;

bool Address::isValid(const std::string& string) {
    if (string.size() != 42 || string[0] != '0' || string[1] != 'x') {
        return false;
    }
    const auto data = parse_hex(string);
    return Address::isValid(data);
}

Address::Address(const std::string& string) {
    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address data");
    }
    const auto data = parse_hex(string);
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const Data& data) {
    if (!isValid(data)) {
        throw std::invalid_argument("Invalid address data");
    }
    std::copy(data.begin(), data.end(), bytes.begin());
}

Address::Address(const PublicKey& publicKey) {
    if (publicKey.type != TWPublicKeyTypeNIST256p1Extended) {
        throw std::invalid_argument("NewChain::Address needs an extended TWPublicKeyTypeNIST256p1Extended public key.");
    }
    const auto data = publicKey.hash({}, static_cast<Hash::HasherSimpleType>(Hash::keccak256), true);
    std::copy(data.end() - Address::size, data.end(), bytes.begin());
}

std::string Address::string() const {
    const auto addressString = hex(this->bytes);
    auto *const ethAddress = new EthAddress(addressString);
    return TW::Ethereum::checksumed(*ethAddress, TW::Ethereum::ChecksumType::eip55);
}