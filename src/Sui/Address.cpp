// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
