// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include <Base58.h>
#include <HexCoding.h>
#include <map>

#include "../Hash.h"
#include "Data.h"

using namespace TW;
using namespace TW::Polkadot;

std::map<int, int> ChecksumLengthMap = {
    // address size to checksum length
    {3, 1},
    {4, 1},
    {5, 2},
    {6, 1},
    {7, 2},
    {8, 3},
    {9, 4},
    {10, 1},
    {11, 2},
    {12, 3},
    {13, 4},
    {14, 5},
    {15, 6},
    {16, 7},
    {17, 8},
    {34, 2}
};

const Data ss58prefix = {0x53, 0x53, 0x35, 0x38, 0x50, 0x52, 0x45};

bool Address::isValid(const std::string &string) {

    auto decoded = Base58::bitcoin.decode(string);
    if (decoded.size() <= 0) {
        return false;
    }

    auto addrType = decoded[0];

    if (addrType != TWPolkaDotAddressTypePolkaDotLiveSS58 &&
        addrType != TWPolkaDotAddressTypePolkaDotLiveAccountID &&
        addrType != TWPolkaDotAddressTypePolkaDotCanarySS58 &&
        addrType != TWPolkaDotAddressTypePolkaDotLiveCanaryAccountID &&
        addrType != TWPolkaDotAddressTypeKulupuSS58 &&
        addrType != TWPolkaDotAddressTypeKulupu &&
        addrType != TWPolkaDotAddressTypeDothereumSS58 &&
        addrType != TWPolkaDotAddressTypeDothereumAccountID &&
        addrType != TWPolkaDotAddressTypeGenericSS58 &&
        addrType != TWPolkaDotAddressTypeGenericAccountID) {
        return false;
    }

    int checkLength = ChecksumLengthMap[decoded.size()-1];

    Data toHash(ss58prefix);
    append(toHash, Data(decoded.begin(), decoded.end() - checkLength));

    auto hashed = TW::Hash::blake2b(toHash, 64);

    for (int i = 0; i < checkLength; i++) {
        if (hashed[i] != decoded[decoded.size()-checkLength+i]) {
            return false;
        }
    }

    return true;
}

Address::Address(const std::string& string) {
    if (!isValid(string)) {
        throw std::invalid_argument("Invalid address string");
    }

    auto decoded = Base58::bitcoin.decode(string);

    checksumLength = ChecksumLengthMap[decoded.size()-1];
    addressType = static_cast<TWPolkadotAddressType>(decoded[0]);

    bytes = Data(decoded.begin()+1, decoded.end()-checksumLength);
}

Address::Address(const PublicKey& publicKey, TWPolkadotAddressType addrType, int checkSmLength) : addressType(addrType), checksumLength(checkSmLength) {
    if (publicKey.type != TWPublicKeyTypeED25519) {
        throw std::invalid_argument("Invalid public key type");
    }

    bytes = Data(publicKey.bytes.begin(), publicKey.bytes.end());
}

std::string Address::string() const {

    Data addr = {static_cast<byte>(addressType)};
    append(addr, bytes);

    Data toHash(ss58prefix);
    append(toHash, addr);

    auto hashed = TW::Hash::blake2b(toHash, 64);

    for (int i = 0; i < checksumLength; i++) {
        addr.push_back(hashed[i]);
    }

    return Base58::bitcoin.encode(addr);
}
