// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Address.h"
#include "HexCoding.h"

namespace TW::Sui {

Address::Address(const std::string& string) : Address::SuiAddress(string) {
}

Address::Address(const PublicKey& publicKey): Address::SuiAddress(publicKey, TW::Hash::HasherBlake2b) {
}

Data Address::getDigest(const PublicKey& publicKey) {
    auto key_data = Data{0x00};
    append(key_data, publicKey.bytes);
    return key_data;
}

} // namespace TW::Sui
