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

bool AddressV3::parseAndCheckV3(const std::string& addr, Discrimination& discrimination, Kind& kind, Data& raw) {
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
        /* TODO update tests, enforce 57 length
        //if (conv.size() != 33 && conv.size() != 65) {
        //if (conv.size() != 57) {
            return false;
        }
        */
        discrimination = (Discrimination)((conv[0] & 0b10000000) >> 7);
        kind = (Kind)(conv[0] & 0b01111111);
        /* TODO update tests, enforce 57 length
        if (kind <= Kind_Sentinel_Low || kind >= Kind_Sentinel_High) {
            return false;
        }
        if ((kind == Kind_Group && conv.size() != 65) ||
            (kind != Kind_Group && conv.size() != 33)) {
            return false;
        }
        */

        raw = Data(conv.size() - 1);
        std::copy(conv.begin() + 1, conv.end(), raw.begin());
        return true;
    } catch (...) {
        return false;
    }
}

bool AddressV3::isValid(const std::string& addr) {
    Discrimination discrimination;
    Kind kind;
    Data key;
    if (parseAndCheckV3(addr, discrimination, kind, key)) {
        return true;
    }
    // not V3, try older
    return AddressV2::isValid(addr);
}

Data blakeHash(Data d) {
    assert(d.size() == 32);
    return Hash::blake2b(d.data(), d.size(), 28);
}

AddressV3 AddressV3::createBase(Discrimination discrimination_in, const TW::Data& spendingKey, const TW::Data& stakingKey) {
    if (spendingKey.size() != 32) {
        throw std::invalid_argument("Wrong spending key size");
    }
    if (stakingKey.size() != 32) {
        throw std::invalid_argument("Wrong spending key size");
    }
    auto addr = AddressV3();
    addr.discrimination = discrimination_in;
    addr.kind = Kind_Base;

    const Data hash1 = blakeHash(spendingKey);
    const Data hash2 = blakeHash(stakingKey);

    addr.bytes = Data();
    append(addr.bytes, hash1);
    append(addr.bytes, hash2);

    return addr;
}

/*
AddressV3 AddressV3::createGroup(Discrimination discrimination_in, const Data& spendingKey, const Data& groupKey) {
    if (spendingKey.size() != 32) {
        throw std::invalid_argument("Wrong spending key size");
    }
    if (groupKey.size() != 32) {
        throw std::invalid_argument("Wrong group key size");
    }
    auto addr = AddressV3();
    addr.discrimination = discrimination_in;
    addr.kind = Kind_Group;
    addr.key1 = spendingKey;
    addr.groupKey = groupKey;
    return addr;
}

AddressV3 AddressV3::createAccount(Discrimination discrimination_in, const Data& accountKey) {
    if (accountKey.size() != 32) {
        throw std::invalid_argument("Wrong spending key size");
    }
    auto addr = AddressV3();
    addr.discrimination = discrimination_in;
    addr.kind = Kind_Account;
    addr.key1 = accountKey;
    return addr;
}
*/

AddressV3::AddressV3(const std::string& addr) {
    if (parseAndCheckV3(addr, discrimination, kind, bytes)) {
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
    discrimination = Discrim_Production;
    kind = Kind_Base;

    *this = createBase(Discrim_Production, subData(publicKey.bytes, 0, 32), subData(publicKey.bytes, 64, 32));
}

AddressV3::AddressV3(const Data& data) {
    // TODO check
    /*
    // min 4 bytes, 2 prefix + 2 len
    if (data.size() < 4) { throw std::invalid_argument("Address data too short"); }
    assert(data.size() >= 4);
    int index = 0;
    discrimination = (Discrimination)data[index++];
    kind = (Kind)data[index++];
    // key1:
    byte len1 = data[index++];
    if (data.size() < 4 + len1) { throw std::invalid_argument("Address data too short"); }
    assert(data.size() >= 4 + len1);
    key1 = Data(len1);
    std::copy(data.begin() + index, data.begin() + index + len1, key1.begin());
    index += len1;
    // groupKey:
    byte len2 = data[index++];
    if (data.size() < 4 + len1 + len2) { throw std::invalid_argument("Address data too short"); }
    assert(data.size() >= 4 + len1 + len2);
    groupKey = Data(len2);
    std::copy(data.begin() + index, data.begin() + index + len2, groupKey.begin());
    */
    discrimination = Discrim_Production; // TODO from byte 0
    kind = Kind_Base; // TODO from byte 0
    bytes = subData(data, 1);
}

AddressV3::AddressV3(const AddressV3& other) = default;

void AddressV3::operator=(const AddressV3& other)
{
    discrimination = other.discrimination;
    kind = other.kind;
    bytes = other.bytes;
    legacyAddressV2 = other.legacyAddressV2;
}

string AddressV3::string() const {
    std::string hrp;
    switch (kind) {
        case Kind_Base:
        case Kind_Single:
        case Kind_Group:
        case Kind_Account:
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

    const byte first = 01; // TODO from networkId, kind
    /*
    (byte)kind;
    if (discrimination == Discrim_Test) {
        first = first | 0b10000000;
    }
    */
    Data raw;
    append(raw, first);
    append(raw, bytes);

    // bech
    Data bech;
    if (!Bech32::convertBits<8, 5, true>(bech, raw)) {
        return "";
    }
    return Bech32::encode(hrp, bech, Bech32::ChecksumVariant::Bech32);
}

string AddressV3::stringBase32() const {
    if (legacyAddressV2.has_value()) {
        return legacyAddressV2->string();
    }

    // TODO to common
    byte first = (byte)kind;
    if (discrimination == Discrim_Test) {
        first = first | 0b10000000;
    }
    Data raw;
    TW::append(raw, first);
    TW::append(raw, bytes);
    std::string base32 = Base32::encode(raw, "abcdefghijklmnopqrstuvwxyz23456789");
    return base32;
}

Data AddressV3::data() const {
    // TODO check

    // TODO to common
    byte first = (byte)kind;
    if (discrimination == Discrim_Test) {
        first = first | 0b10000000;
    }
    Data raw;
    TW::append(raw, first);
    TW::append(raw, bytes);
    return raw;
    /*
    Data data;
    TW::append(data, (uint8_t)discrimination);
    TW::append(data, (uint8_t)kind);
    TW::append(data, (uint8_t)bytes.size());
    TW::append(data, bytes);
    //TW::append(data, (uint8_t)groupKey.size());
    //TW::append(data, groupKey);
    return data;
    */
}
