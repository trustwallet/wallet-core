// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <charconv>
#include <system_error>

#include "Address.h"
#include "HexCoding.h"
#include "Wallet.h"

using namespace TW;

namespace TW::Everscale {

using AddressImpl = TW::CommonTON::RawAddress;

bool Address::isValid(const std::string& string) noexcept {
    return AddressImpl::isValid(string);
}

Address::Address(const std::string& string) {
    if (!Address::isValid(string)) {
        throw std::invalid_argument("Invalid address string!");
    }

    addressData = AddressImpl::splitAddress(string);
}

Address::Address(const PublicKey& publicKey, int8_t workchainId)
    : Address(InitData(publicKey).computeAddr(workchainId)) {
}

std::string Address::string() const {
    return AddressImpl::to_string(addressData);
}

} // namespace TW::Everscale
