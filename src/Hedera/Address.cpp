// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "HexCoding.h"
#include "algorithm/string.h"

#include <regex>

namespace TW::Hedera::internal {
    inline const std::regex gEntityIDRegex{"(0|[1-9]\\d*)\\.(0|[1-9]\\d*)\\.(0|[1-9]\\d*)(?:-([a-z]{5}))?$"};
}

namespace TW::Hedera {

bool Address::isValid(const std::string& string) {
    using namespace internal;
    std::smatch match;
    auto isValid = std::regex_match(string, match, gEntityIDRegex);
    if (!isValid) {
        auto parts = TW::split(string, '.');
        if (parts.size() != 3) {
            return false;
        }
        auto isNumberFunctor = [](std::string_view input){
            return input.find_first_not_of("0123456789") == std::string::npos;
        };
        if (!isNumberFunctor(parts[0]) || !isNumberFunctor(parts[1])) {
            return false;
        }
        try {
            [[maybe_unused]] auto pubKey = PublicKey::fromHederaDerPrefix(parts[2]);
            isValid = true;
        }
        catch ([[maybe_unused]] const std::runtime_error& error) {
            return false;
        }
    }
    return isValid;
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
