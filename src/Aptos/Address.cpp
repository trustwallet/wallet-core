// Copyright © 2017-2023 Trust Wallet.
// Author: Clement Doumergue
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "HexCoding.h"

namespace TW::Aptos {

Address::Address(const std::string& string) : Address::AptosAddress(string) {
}

Address::Address(const PublicKey& publicKey): Address::AptosAddress(publicKey) {
}

Data Address::getDigest(const PublicKey& publicKey) {
    auto key_data = publicKey.bytes;
    append(key_data, 0x00);
    return key_data;
}

BCS::Serializer& operator<<(BCS::Serializer& stream, Address addr) noexcept {
    stream.add_bytes(addr.bytes.begin(), addr.bytes.end());
    return stream;
}

} // namespace TW::Aptos
