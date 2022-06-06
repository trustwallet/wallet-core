// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Address.h"

#include "../Bech32.h"
#include "../Coin.h"

#include <TrustWalletCore/TWCoinType.h>

using namespace TW::Nervos;
using namespace TW;

bool Address::isValid(const std::string& string) {
    return Address::isValid(string, TW::p2pkhPrefix(TWCoinTypeNervos),
                            TW::p2shPrefix(TWCoinTypeNervos),
                            stringForHRP(TW::hrp(TWCoinTypeNervos)));
}

bool Address::isValid(const std::string& string, byte p2pkh, byte p2sh, const char* hrp) {
    try {
        Address(string, p2pkh, p2sh, hrp);
        return true;
    } catch (std::exception e) {
        return false;
    }
}

Address::Address(const std::string& string)
    : Address(string, TW::p2pkhPrefix(TWCoinTypeNervos), TW::p2shPrefix(TWCoinTypeNervos),
              stringForHRP(TW::hrp(TWCoinTypeNervos))) {}

Address::Address(const std::string& string, byte p2pkh, byte p2sh, const char* hrp) : hrp(hrp) {
    bool isValid = false;

    do {
        auto decoded = Bech32::decode(string);
        auto& decodedHrp = std::get<0>(decoded);
        auto& decodedData = std::get<1>(decoded);
        auto& decodedVariant = std::get<2>(decoded);
        if (decodedHrp.compare(hrp)) {
            break;
        }
        Data decodedPayload;
        if (!Bech32::convertBits<5, 8, false>(decodedPayload, decodedData)) {
            break;
        }
        if (decodedPayload.size() == 0) {
            break;
        }
        addressType = (AddressType)decodedPayload[0];
        switch (addressType) {
        case AddressType::FullVersion: {
            if (decodedVariant != Bech32::ChecksumVariant::Bech32M) {
                break;
            }
            if (decodedPayload.size() < 34) {
                break;
            }
            codeHashIndex = -1;
            codeHash = Data(decodedPayload.begin() + 1, decodedPayload.begin() + 33);
            hashType = (HashType)decodedPayload[33];
            args = Data(decodedPayload.begin() + 34, decodedPayload.end());
            isValid = true;
            break;
        }
        case AddressType::HashIdx: {
            if (decodedVariant != Bech32::ChecksumVariant::Bech32) {
                break;
            }
            if (decodedPayload.size() != 22) {
                break;
            }
            codeHashIndex = decodedPayload[1];
            if (codeHashIndex != 0) {
                break;
            }
            codeHash = CommonFunc::getSecp256k1CodeHash();
            hashType = HashType::Type1;
            args = Data(decodedPayload.begin() + 2, decodedPayload.end());
            isValid = true;
            break;
        }
        case AddressType::DataCodeHash:
        case AddressType::TypeCodeHash: {
            if (decodedVariant != Bech32::ChecksumVariant::Bech32) {
                break;
            }
            if (decodedPayload.size() < 33) {
                break;
            }
            codeHashIndex = -1;
            codeHash = Data(decodedPayload.begin() + 1, decodedPayload.begin() + 33);
            hashType = addressType == AddressType::DataCodeHash ? HashType::Data0 : HashType::Type1;
            args = Data(decodedPayload.begin() + 33, decodedPayload.end());
            isValid = true;
            break;
        }
        }
    } while (false);

    if (!isValid) {
        throw std::invalid_argument("Invalid address string");
    }
}

Address::Address(const PublicKey& publicKey)
    : Address(publicKey, TW::p2pkhPrefix(TWCoinTypeNervos),
              stringForHRP(TW::hrp(TWCoinTypeNervos))) {}

Address::Address(const PublicKey& publicKey, byte p2pkh, const char* hrp) : hrp(hrp) {
    if (publicKey.type != TWPublicKeyTypeSECP256k1) {
        throw std::invalid_argument("Nervos::Address needs a SECP256k1 public key.");
    }
    addressType = AddressType::FullVersion;
    codeHashIndex = -1;
    codeHash = CommonFunc::getSecp256k1CodeHash();
    hashType = HashType::Type1;
    args = TW::Hash::blake2b(publicKey.bytes, 32, CommonFunc::getHashPersonalization());
    args = Data(args.begin(), args.begin() + 20);
}

std::string Address::string() const {
    auto data = Data();
    data.push_back((byte)addressType);
    Bech32::ChecksumVariant checksumVariant;
    switch (addressType) {
    case AddressType::FullVersion: {
        data.insert(data.end(), codeHash.begin(), codeHash.end());
        data.push_back((byte)hashType);
        data.insert(data.end(), args.begin(), args.end());
        checksumVariant = Bech32::ChecksumVariant::Bech32M;
        break;
    }
    case AddressType::HashIdx: {
        data.push_back(codeHashIndex);
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
        throw std::invalid_argument("Invalid address string");
    }
    }
    Data payload;
    if (!Bech32::convertBits<8, 5, true>(payload, data)) {
        return "";
    }
    return Bech32::encode(hrp, payload, checksumVariant);
}
