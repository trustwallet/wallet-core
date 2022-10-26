// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "HexCoding.h"

namespace TW::Hedera {

bool Address::isValid(const std::string& string) {
    // TODO: Finalize implementation
    return false;
}

Address::Address(const std::string& string) {
    // TODO: Finalize implementation

    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address string");
    }
}

Address::Address(const PublicKey& publicKey) {
    // TODO: Finalize implementation
}

std::string Address::string() const {
   std::string out = std::to_string(mShard) + "." + std::to_string(mRealm) + ".";
   if (mAlias.has_value()) {
       return out + hex(mAlias->bytesWithHederaDerPrefix());
   }
   return out + std::to_string(mNum);
}

Address::Address(std::size_t shard, std::size_t realm, std::size_t num, std::optional<PublicKey> alias)
    : mShard(shard), mRealm(realm), mNum(num), mAlias(alias) {
}

} // namespace TW::Hedera
