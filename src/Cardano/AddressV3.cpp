// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "AddressV3.h"
#include "AddressV2.h"
#include <TrustWalletCore/TWCoinType.h>
#include "../Data.h"
#include "../Bech32.h"
#include "../Base32.h"
#include "../Crc.h"
#include "../HexCoding.h"
#include "../Hash.h"

#include <array>

using namespace TW;
using namespace TW::Cardano;
using namespace std;

bool AddressV3::parseAndCheckV3(const std::string& addr, NetworkId& networkId, Kind& kind, Data& raw) {
    try {
        auto bech = Bech32::decode(addr);
        if (std::get<1>(bech).size() == 0) {
            // empty Bech data
            return false;
        }
        // Bech bits conversion
        Data conv;
        auto success = Bech32::convertBits<5, 8, false>(conv, std::get<1>(bech));
        if (!success) {
            return false;
        }
        if (conv.size() != EncodedSize) {
            return false;
        }
        kind = kindFromFirstByte(conv[0]);
        networkId = networkIdFromFirstByte(conv[0]);

        raw = Data(conv.size() - 1);
        std::copy(conv.begin() + 1, conv.end(), raw.begin());
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
    if (publicKey.type != TWPublicKeyTypeED25519Extended || publicKey.bytes.size() != PublicKey::ed25519DoubleExtendedSize) {
        throw std::invalid_argument("Invalid public key type");
    }
    kind = Kind_Base;

    *this = createBase(Network_Production, PublicKey(subData(publicKey.bytes, 0, 32), TWPublicKeyTypeED25519), PublicKey(subData(publicKey.bytes, 64, 32), TWPublicKeyTypeED25519));
}

AddressV3::AddressV3(const Data& data) {
    if (data.size() != EncodedSize) {
        throw std::invalid_argument("Address data too short");
    }
    networkId = networkIdFromFirstByte(data[0]);
    kind = kindFromFirstByte(data[0]);
    bytes = subData(data, 1);
}

AddressV3::AddressV3(const AddressV3& other) = default;

uint8_t AddressV3::firstByte(NetworkId networkId, Kind kind) {
    byte first = (byte)(((byte)kind << 4) + networkId);
    return first;
}

AddressV3::NetworkId AddressV3::networkIdFromFirstByte(uint8_t first) {
    return (NetworkId)(first & 0x0F);
}

AddressV3::Kind AddressV3::kindFromFirstByte(uint8_t first) {
    return (Kind)((first & 0xF0) >> 4);
}

void AddressV3::operator=(const AddressV3& other)
{
    networkId = other.networkId;
    kind = other.kind;
    bytes = other.bytes;
    legacyAddressV2 = other.legacyAddressV2;
}

string AddressV3::string() const {
    std::string hrp;
    switch (kind) {
        case Kind_Base:
            hrp = stringForHRP(TWHRPCardano); break;
        default:
            hrp = ""; break;
    }
    return string(hrp);
}

string AddressV3::string(const std::string& hrp) const {
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

Data AddressV3::data() const {
    if (legacyAddressV2.has_value()) {
        return legacyAddressV2->getCborData();
    }

    const byte first = firstByte(networkId, kind);
    Data raw;
    TW::append(raw, first);
    TW::append(raw, bytes);
    return raw;
}
