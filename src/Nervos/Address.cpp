// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"
#include "Constants.h"

#include "../Bech32.h"
#include "../Coin.h"

#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWHRP.h>

namespace TW::Nervos {

[[nodiscard]] bool Address::isValid(const std::string& string) noexcept {
    return Address::isValid(string, HRP_NERVOS);
}

[[nodiscard]] bool Address::isValid(const std::string& string, const char* hrp) noexcept {
    return Address().decode(string, hrp);
}

Address::Address(const std::string& string) : Address(string, HRP_NERVOS) {}

Address::Address(const std::string& string, const char* hrp) {
    if (!decode(string, hrp)) {
        throw std::invalid_argument("Invalid address string");
    }
}

bool Address::decode(const std::string& string, const char* hrp) noexcept {
    this->hrp = hrp;
    auto decoded = Bech32::decode(string);
    auto&& [decodedHrp, decodedData, decodedVariant] = decoded;
    if (decodedHrp.compare(hrp)) {
        return false;
    }
    Data decodedPayload;
    if (!Bech32::convertBits<5, 8, false>(decodedPayload, decodedData)) {
        return false;
    }
    if (decodedPayload.empty()) {
        return false;
    }
    addressType = AddressType(decodedPayload[0]);
    switch (addressType) {
    case AddressType::FullVersion: {
        int codeHashOffset = 1;
        int codeHashSize = 32;
        int hashTypeOffset = codeHashOffset + codeHashSize;
        int hashTypeSize = 1;
        int argsOffset = hashTypeOffset + hashTypeSize;
        if (decodedVariant != Bech32::ChecksumVariant::Bech32M) {
            return false;
        }
        if (decodedPayload.size() < argsOffset) {
            return false;
        }
        codeHashIndex = -1;
        codeHash = Data(decodedPayload.begin() + codeHashOffset,
                        decodedPayload.begin() + codeHashOffset + codeHashSize);
        hashType = HashType(decodedPayload[hashTypeOffset]);
        args = Data(decodedPayload.begin() + argsOffset, decodedPayload.end());
        break;
    }
    case AddressType::HashIdx: {
        int codeHashIndexOffset = 1;
        int codeHashIndexSize = 1;
        int argsOffset = codeHashIndexOffset + codeHashIndexSize;
        int argsSize = 20;
        if (decodedVariant != Bech32::ChecksumVariant::Bech32) {
            return false;
        }
        if (decodedPayload.size() != argsOffset + argsSize) {
            return false;
        }
        codeHashIndex = decodedPayload[codeHashIndexOffset];
        if (codeHashIndex != 0) {
            return false;
        }
        codeHash = Constants::gSecp256k1CodeHash;
        hashType = HashType::Type1;
        args = Data(decodedPayload.begin() + argsOffset, decodedPayload.end());
        break;
    }
    case AddressType::DataCodeHash:
    case AddressType::TypeCodeHash: {
        int codeHashOffset = 1;
        int codeHashSize = 32;
        int argsOffset = codeHashOffset + codeHashSize;
        if (decodedVariant != Bech32::ChecksumVariant::Bech32) {
            return false;
        }
        if (decodedPayload.size() < argsOffset) {
            return false;
        }
        codeHashIndex = -1;
        codeHash = Data(decodedPayload.begin() + codeHashOffset,
                        decodedPayload.begin() + codeHashOffset + codeHashSize);
        hashType = addressType == AddressType::DataCodeHash ? HashType::Data0 : HashType::Type1;
        args = Data(decodedPayload.begin() + argsOffset, decodedPayload.end());
        break;
    }
    default: {
        return false;
    }
    }
    return true;
}

Address::Address(const PublicKey& publicKey) : Address(publicKey, HRP_NERVOS) {}

Address::Address(const PublicKey& publicKey, const char* hrp) : hrp(hrp) {
    if (publicKey.type != TWPublicKeyTypeSECP256k1) {
        throw std::invalid_argument("Nervos::Address needs a SECP256k1 public key.");
    }
    addressType = AddressType::FullVersion;
    codeHashIndex = -1;
    codeHash = Constants::gSecp256k1CodeHash;
    hashType = HashType::Type1;
    Data publicKeyHash = Hash::blake2b(publicKey.bytes, 32, Constants::gHashPersonalization);
    Data truncatedPublicKeyHash = Data(publicKeyHash.begin(), publicKeyHash.begin() + 20);
    args = truncatedPublicKeyHash;
}

std::string Address::string() const {
    auto data = Data();
    data.emplace_back(addressType);
    Bech32::ChecksumVariant checksumVariant;
    switch (addressType) {
    case AddressType::FullVersion: {
        data.insert(data.end(), codeHash.begin(), codeHash.end());
        data.emplace_back(hashType);
        data.insert(data.end(), args.begin(), args.end());
        checksumVariant = Bech32::ChecksumVariant::Bech32M;
        break;
    }
    case AddressType::HashIdx: {
        data.emplace_back(codeHashIndex);
        data.insert(data.end(), args.begin(), args.end());
        checksumVariant = Bech32::ChecksumVariant::Bech32;
        break;
    }
    case AddressType::DataCodeHash:
    case AddressType::TypeCodeHash: {
        data.insert(data.end(), codeHash.begin(), codeHash.end());
        data.insert(data.end(), args.begin(), args.end());
        checksumVariant = Bech32::ChecksumVariant::Bech32;
        break;
    }
    default: {
        return "";
    }
    }
    Data payload;
    if (!Bech32::convertBits<8, 5, true>(payload, data)) {
        return "";
    }
    return Bech32::encode(hrp, payload, checksumVariant);
}

} // namespace Nervos
