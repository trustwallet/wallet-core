// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "AddressV3.h"
#include "AddressV2.h"
#include "../Data.h"
#include "../Bech32.h"
#include "../Base32.h"
#include "../Crc.h"
#include "../HexCoding.h"
#include "../Hash.h"

#include <array>
#include <iostream>

using namespace TW;
using namespace TW::Cardano;
using namespace std;

bool AddressV3::parseAndCheckV3(const std::string& addr, Discrimination& discrimination, Kind& kind, Data& key1, Data& key2) {
    auto bech = Bech32::decode(addr);
    if (bech.second.size() == 0) {
        // empty Bech data
        return false;
    }
    // Bech bits conversion
    Data conv;
    auto success = Bech32::convertBits<5, 8, false>(conv, bech.second);
    if (!success) {
        return false;
    }
    if (conv.size() != 33 && conv.size() != 65) {
        return false;
    }
    discrimination = (Discrimination)((conv[0] & 0b10000000) >> 7);
    kind = (Kind)(conv[0] & 0b01111111);
    if (kind <= Kind_Sentinel_Low || kind >= Kind_Sentinel_High) {
        return false;
    }
    if ((kind == Kind_Group && conv.size() != 65) ||
        (kind != Kind_Group && conv.size() != 33)) {
        return false;
    }

    switch (kind) {
        case Kind_Single:
        case Kind_Account:
        case Kind_Multisig:
            assert(conv.size() == 33);
            key1 = Data(32);
            std::copy(conv.begin() + 1, conv.begin() + 33, key1.begin());
            return true;

        case Kind_Group:
            assert(conv.size() == 65);
            key1 = Data(32);
            key2 = Data(32);
            std::copy(conv.begin() + 1, conv.begin() + 33, key1.begin());
            std::copy(conv.begin() + 33, conv.begin() + 65, key2.begin());
            return true;

        default:
            return false;
    }
}

bool AddressV3::isValid(const std::string& addr) {
    try {
        Discrimination discrimination;
        Kind kind;
        Data key1;
        Data key2;
        if (parseAndCheckV3(addr, discrimination, kind, key1, key2)) {
            return true;
        }
    } catch (exception& ex) {
    }
    // not V3, try older
    return TW::Cardano::AddressV2::isValid(addr);
}

AddressV3 AddressV3::createSingle(Discrimination discrimination_in, const Data& spendingKey) {
    if (spendingKey.size() != 32) {
        throw std::invalid_argument("Wrong spending key size");
    }
    auto addr = AddressV3();
    addr.discrimination = discrimination_in;
    addr.kind = Kind_Single;
    addr.key1 = spendingKey;
    return addr;
}

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

AddressV3::AddressV3(const std::string& addr) : legacyAddressV2(nullptr) {
    if (parseAndCheckV3(addr, discrimination, kind, key1, groupKey)) {
        // values stored
        return;
    }
    // try legacy
    // throw on error
    legacyAddressV2 = new TW::Cardano::AddressV2(addr);
}

AddressV3::AddressV3(const PublicKey& publicKey) : legacyAddressV2(nullptr) {
    // input is extended pubkey, 64-byte
    if (publicKey.type != TWPublicKeyTypeED25519Extended) {
        throw std::invalid_argument("Invalid public key type");
    }
    discrimination = Discrim_Test;
    kind = Kind_Group;
    key1 = Data(32);
    groupKey = Data(32);
    std::copy(publicKey.bytes.begin(), publicKey.bytes.begin() + 32, key1.begin());
    std::copy(publicKey.bytes.begin() + 32, publicKey.bytes.begin() + 64, groupKey.begin());
}

string AddressV3::string() const {
    std::string hrp;
    switch (kind) {
        case Kind_Single:
        case Kind_Group:
        case Kind_Account:
            hrp = "addr"; break;
        default:
            hrp = ""; break;
    }
    return string(hrp);
}

string AddressV3::string(const std::string& hrp) const {
    if (legacyAddressV2 != nullptr) {
        return legacyAddressV2->string();
    }

    byte first = (byte)kind;
    if (discrimination == Discrim_Test) first = first | 0b10000000;
    Data keys;
    TW::append(keys, first);
    TW::append(keys, key1);
    if (groupKey.size() > 0) {
        TW::append(keys, groupKey);
    }
    // bech
    Data bech;
    if (!Bech32::convertBits<8, 5, true>(bech, keys)) {
        return "";
    }
    return Bech32::encode(hrp, bech);
}

string AddressV3::stringBase32() const {
    if (legacyAddressV2 != nullptr) {
        return legacyAddressV2->string();
    }

    byte first = (byte)kind;
    if (discrimination == Discrim_Test) first = first | 0b10000000;
    Data keys;
    TW::append(keys, first);
    TW::append(keys, key1);
    if (groupKey.size() > 0) {
        TW::append(keys, groupKey);
    }
    std::string base32 = Base32::encode(keys, "abcdefghijklmnopqrstuvwxyz23456789");
    return base32;
}