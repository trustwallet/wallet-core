// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "AddressV3.h"
#include "AddressV2.h"
#include <TrustWalletCore/TWCoinType.h>
#include "../Bech32.h"
#include "../Base32.h"
#include "../HexCoding.h"

#include <array>

namespace TW::Cardano {

bool AddressV3::checkLength(Kind kind, size_t length) noexcept {
    switch (kind) {
    case Kind_Base:
        return (length == EncodedSize2);

    case Kind_Enterprise:
    case Kind_Reward:
        return (length == EncodedSize1);

    default:
        // accept other types as well
        return true;
    }
}

bool AddressV3::parseAndCheckV3(const Data& raw, NetworkId& networkId, Kind& kind, Data& bytes) noexcept {
    if (raw.empty()) {
        // too short, cannot extract kind and networkId
        return false;
    }
    kind = kindFromFirstByte(raw[0]);
    networkId = networkIdFromFirstByte(raw[0]);
    if (networkId != Network_Production) {
        return false;
    }

    bytes = Data();
    std::copy(cbegin(raw) + 1, cend(raw), std::back_inserter(bytes));

    return checkLength(kind, raw.size());
}

bool AddressV3::parseAndCheckV3(const std::string& addr, NetworkId& networkId, Kind& kind, Data& bytes) noexcept {
    try {
        auto bech = Bech32::decode(addr);
        if (std::get<1>(bech).empty()) {
            // empty Bech data
            return false;
        }
        // Bech bits conversion
        Data conv;
        auto success = Bech32::convertBits<5, 8, false>(conv, std::get<1>(bech));
        if (!success) {
            return false;
        }

        if (!parseAndCheckV3(conv, networkId, kind, bytes)) {
            return false;
        }

        // check prefix
        if (const auto expectedHrp = getHrp(kind); !addr.starts_with(expectedHrp)) {
            return false;
        }

        return true;
    } catch (...) {
        return false;
    }
}

bool AddressV3::isValid(const std::string& addr) {
    NetworkId networkId;
    Kind kind;
    Data key;
    return parseAndCheckV3(addr, networkId, kind, key);
}

bool AddressV3::isValidLegacy(const std::string& addr) {
    if (isValid(addr)) {
        return true;
    }
    // not V3, try older
    return AddressV2::isValid(addr);
}

Data blakeHash(Data d) {
    assert(d.size() == 32);
    return Hash::blake2b(d.data(), d.size(), AddressV3::HashSize);
}

AddressV3 AddressV3::createBase(NetworkId networkId, const TW::Data& spendingKeyHash, const TW::Data& stakingKeyHash) {
    if (spendingKeyHash.size() != HashSize) {
        throw std::invalid_argument("Wrong spending key hash size");
    }
    if (stakingKeyHash.size() != HashSize) {
        throw std::invalid_argument("Wrong spending key hash size");
    }
    auto addr = AddressV3();
    addr.networkId = networkId;
    addr.kind = Kind_Base;

    addr.bytes = Data();
    append(addr.bytes, spendingKeyHash);
    append(addr.bytes, stakingKeyHash);

    return addr;
}

AddressV3 AddressV3::createBase(NetworkId networkId, const PublicKey& spendingKey, const PublicKey& stakingKey) {
    if (spendingKey.bytes.size() != 32) {
        throw std::invalid_argument("Wrong spending key size");
    }
    if (stakingKey.bytes.size() != 32) {
        throw std::invalid_argument("Wrong spending key size");
    }

    const Data hash1 = blakeHash(spendingKey.bytes);
    const Data hash2 = blakeHash(stakingKey.bytes);
    return createBase(networkId, hash1, hash2);
}

AddressV3 AddressV3::createReward(NetworkId networkId, const TW::Data& stakingKeyHash) {
    if (stakingKeyHash.size() != HashSize) {
        throw std::invalid_argument("Wrong spending key hash size");
    }
    auto addr = AddressV3();
    addr.networkId = networkId;
    addr.kind = Kind_Reward;
    addr.bytes = stakingKeyHash;
    return addr;
}

AddressV3::AddressV3(const std::string& addr) {
    if (parseAndCheckV3(addr, networkId, kind, bytes)) {
        // values stored
        return;
    }
    // try legacy
    // throw on error
    legacyAddressV2 = AddressV2(addr);
}

AddressV3::AddressV3(const PublicKey& publicKey) {
    // input is double extended pubkey
    if (publicKey.type != TWPublicKeyTypeED25519Cardano || publicKey.bytes.size() != PublicKey::cardanoKeySize) {
        throw std::invalid_argument("Invalid public key type");
    }
    *this = createBase(Network_Production, PublicKey(subData(publicKey.bytes, 0, 32), TWPublicKeyTypeED25519), PublicKey(subData(publicKey.bytes, 64, 32), TWPublicKeyTypeED25519));
}

AddressV3::AddressV3(const Data& data) {
    parseAndCheckV3(data, networkId, kind, bytes);
}

AddressV3::AddressV3(const AddressV3& other) = default;

uint8_t AddressV3::firstByte(NetworkId networkId, Kind kind) {
    auto first = (byte)(((byte)kind << 4) + networkId);
    return first;
}

AddressV3::NetworkId AddressV3::networkIdFromFirstByte(uint8_t first) {
    return (NetworkId)(first & 0x0F);
}

AddressV3::Kind AddressV3::kindFromFirstByte(uint8_t first) {
    return (Kind)((first & 0xF0) >> 4);
}

std::string AddressV3::getHrp(Kind kind) noexcept {
    switch (kind) {
    case Kind_Base:
    case Kind_Enterprise:
    default:
        return stringForHRP(TWHRPCardano);
    case Kind_Reward:
        return "stake";
    }
}

std::string AddressV3::string() const {
    const auto hrp = getHrp(kind);
    return string(hrp);
}

std::string AddressV3::string(const std::string& hrp) const {
    if (legacyAddressV2.has_value()) {
        return legacyAddressV2->string();
    }

    const Data raw = data();
    // bech
    Data bech;
    if (!Bech32::convertBits<8, 5, true>(bech, raw)) {
        return "";
    }
    return Bech32::encode(hrp, bech, Bech32::ChecksumVariant::Bech32);
}

Data AddressV3::data() const noexcept {
    if (legacyAddressV2.has_value()) {
        return legacyAddressV2->getCborData();
    }

    const byte first = firstByte(networkId, kind);
    Data raw;
    TW::append(raw, first);
    TW::append(raw, bytes);
    return raw;
}

std::string AddressV3::getStakingAddress() const noexcept {
    if (kind != Kind_Base || bytes.size() != (2 * HashSize)) {
        return "";
    }
    const auto& stakingKeyHash = TW::subData(bytes, HashSize, HashSize);
    return createReward(this->networkId, stakingKeyHash).string();
}

} // namespace TW::Cardano
