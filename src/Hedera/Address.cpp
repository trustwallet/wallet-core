// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Address.h"
#include "HexCoding.h"
#include "DER.h"
#include "algorithm/string.hpp"

#include <charconv>
#include <regex>

namespace TW::Hedera::internal {
    static const std::regex gEntityIDRegex{"(0|[1-9]\\d*)\\.(0|[1-9]\\d*)\\.(0|[1-9]\\d*)(?:-([a-z]{5}))?$"};
}

namespace TW::Hedera {


Alias::Alias(std::optional<PublicKey> alias) noexcept : mPubKey(std::move(alias)) {

}

std::string Alias::string() const noexcept {
    std::string pubkeyBytes = "";
    if (mPubKey.has_value()) {
        pubkeyBytes = hex(mPubKey.value().bytes);
    }
    return gHederaDerPrefixPublic + pubkeyBytes;
}

/// Parses one decimal component of an entity ID. Requires the whole component to be
/// consumed and to fit std::size_t, so that isValid() and the string constructor accept
/// exactly the same inputs.
static std::optional<std::size_t> parseEntityIdPart(std::string_view part) {
    std::size_t value = 0;
    const auto result = std::from_chars(part.data(), part.data() + part.size(), value);
    if (result.ec != std::errc{} || result.ptr != part.data() + part.size()) {
        return std::nullopt;
    }
    return value;
}

bool Address::isValid(const std::string& string) {
    using namespace internal;
    std::smatch match;
    if (std::regex_match(string, match, gEntityIDRegex)) {
        return parseEntityIdPart(match[1].str()).has_value() &&
               parseEntityIdPart(match[2].str()).has_value() &&
               parseEntityIdPart(match[3].str()).has_value();
    }

    auto parts = TW::ssplit(string, '.');
    if (parts.size() != 3) {
        return false;
    }
    if (!parseEntityIdPart(parts[0]).has_value() || !parseEntityIdPart(parts[1]).has_value()) {
        return false;
    }
    return hasDerPrefix(parts[2]);
}

Address::Address(const std::string& string) {
    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address string");
    }

    // isValid() has already checked every component with the same parser, so the
    // optionals below cannot be empty; the throw guards against the two drifting apart.
    auto toInt = [](std::string_view part) -> std::size_t {
        const auto value = parseEntityIdPart(part);
        if (!value.has_value()) {
            throw std::invalid_argument("Invalid entity ID");
        }
        return *value;
    };

    // Numeric `shard.realm.num` form (with optional checksum suffix). Full numeric
    // consumption is required: partial parsing would silently misdirect alias entity
    // IDs to a wrong account.
    std::smatch match;
    if (std::regex_match(string, match, internal::gEntityIDRegex)) {
        mShard = toInt(match[1].str());
        mRealm = toInt(match[2].str());
        mNum = toInt(match[3].str());
        return;
    }

    // Alias form also accepted by isValid(): `shard.realm.<DER-prefixed ed25519 public key hex>`.
    auto parts = TW::ssplit(string, '.');
    mShard = toInt(parts[0]);
    mRealm = toInt(parts[1]);
    const auto& aliasPart = parts[2];
    const auto keyOffset =
        aliasPart.find(gHederaDerPrefixPublic) + std::string(gHederaDerPrefixPublic).size();
    mAlias = Alias(PublicKey(parse_hex(aliasPart.substr(keyOffset)), TWPublicKeyTypeED25519));
}

Address::Address(const PublicKey& publicKey)
    : Address(0, 0, 0, publicKey) {
}

std::string Address::string() const {
    std::string out = std::to_string(mShard) + "." + std::to_string(mRealm) + ".";
    if (mAlias.mPubKey.has_value()) {
        return out + mAlias.string();
    }
    return out + std::to_string(mNum);
}

Address::Address(std::size_t shard, std::size_t realm, std::size_t num, std::optional<PublicKey> alias)
    : mShard(shard), mRealm(realm), mNum(num), mAlias(std::move(alias)) {
}

} // namespace TW::Hedera
