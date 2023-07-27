// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "CoinEntry.h"
#include "Coin.h"
#include "HexCoding.h"
#include "rust/Wrapper.h"
#include <variant>
#include <google/protobuf/util/json_util.h>

namespace TW {

const char* getFromPrefixHrpOrDefault(const PrefixVariant &prefix, TWCoinType coin) {
    if (std::holds_alternative<Bech32Prefix>(prefix)) {
        const char* fromPrefix = std::get<Bech32Prefix>(prefix);
        if (fromPrefix != nullptr && *fromPrefix != 0) {
            return fromPrefix;
        }
    }
    // Prefix contains no hrp or empty, return coin-default
    return stringForHRP(TW::hrp(coin));
}

byte getFromPrefixPkhOrDefault(const PrefixVariant &prefix, TWCoinType coin) {
    if (std::holds_alternative<Base58Prefix>(prefix)) {
        return std::get<Base58Prefix>(prefix).p2pkh;
    }
    // Prefix contains no base58 prefixes, return coin-default
    return TW::p2pkhPrefix(coin);
}

bool validateAddressRust(TWCoinType coin, const std::string& address, const PrefixVariant& addressPrefix) {
    if (!std::holds_alternative<std::monostate>(addressPrefix)) {
        throw std::invalid_argument("`Rust::tw_any_address_is_valid_bech32`, `Rust::tw_any_address_is_valid_ss58` are not supported yet");
    }

    Rust::TWStringWrapper addressStr = address;
    return Rust::tw_any_address_is_valid(addressStr.get(), static_cast<uint32_t>(coin));
}

std::string normalizeAddressRust(TWCoinType coin, const std::string& address) {
    Rust::TWStringWrapper addressStr = address;

    auto anyAddress = Rust::wrapTWAnyAddress(
        Rust::tw_any_address_create_with_string(addressStr.get(), static_cast<uint32_t>(coin)));
    if (!anyAddress) {
        return {};
    }

    Rust::TWStringWrapper normalized = Rust::tw_any_address_description(anyAddress.get());
    return normalized.toStringOrDefault();
}

std::string deriveAddressRust(TWCoinType coin, const PublicKey& publicKey, TWDerivation derivation, const PrefixVariant& addressPrefix) {
    if (!std::holds_alternative<std::monostate>(addressPrefix)) {
        throw std::invalid_argument("`Rust::tw_any_address_create_bech32_with_public_key`, "
                                    "`Rust::tw_any_address_create_ss58_with_public_key`, "
                                    "`Rust::tw_any_address_create_with_public_key_filecoin_address_type` are not supported yet");
    }

    auto *twPublicKeyRaw = Rust::tw_public_key_create_with_data(publicKey.bytes.data(),
                                                             publicKey.bytes.size(),
                                                             static_cast<uint32_t>(publicKey.type));
    auto twPublicKey = Rust::wrapTWPublicKey(twPublicKeyRaw);
    if (!twPublicKey) {
        return {};
    }

    auto *anyAddressRaw = Rust::tw_any_address_create_with_public_key_derivation(twPublicKey.get(),
                                                                                 static_cast<uint32_t>(coin),
                                                                                 static_cast<uint32_t>(derivation));
    auto anyAddress = Rust::wrapTWAnyAddress(anyAddressRaw);
    if (!anyAddress) {
        return {};
    }

    Rust::TWStringWrapper derivedAddress = Rust::tw_any_address_description(anyAddress.get());
    return derivedAddress.toStringOrDefault();
}

Data addressToDataRust(TWCoinType coin, const std::string& address) {
    Rust::TWStringWrapper addressStr = address;

    auto anyAddress = Rust::wrapTWAnyAddress(
        Rust::tw_any_address_create_with_string(addressStr.get(), static_cast<uint32_t>(coin)));
    if (!anyAddress) {
        return {};
    }

    Rust::TWDataWrapper data = Rust::tw_any_address_data(anyAddress.get());
    return data.toDataOrDefault();
}

void signRust(const Data& dataIn, TWCoinType coin, Data& dataOut)  {
    Rust::TWDataWrapper input = Rust::tw_data_create_with_bytes(dataIn.data(), dataIn.size());
    Rust::TWDataWrapper output = Rust::tw_any_signer_sign(input.get(), static_cast<uint32_t>(coin));

    dataOut = output.toDataOrDefault();
}

Data preImageHashesRust(TWCoinType coin, const Data& dataIn) {
    Rust::TWDataWrapper input = dataIn;
    Rust::TWDataWrapper output = Rust::tw_transaction_compiler_pre_image_hashes(static_cast<uint32_t>(coin), input.get());

    return output.toDataOrDefault();
}

void compileRust(
    TWCoinType coin,
    const Data& dataIn,
    const std::vector<Data>& signatures,
    const std::vector<PublicKey>& publicKeys,
    Data& dataOut
) {
    Rust::TWDataWrapper input = dataIn;

    std::vector<Data> publicKeysData;
    for (const auto& publicKey : publicKeys) {
        publicKeysData.push_back(publicKey.bytes);
    }

    Rust::TWDataVectorWrapper signaturesVec = signatures;
    Rust::TWDataVectorWrapper publicKeysVec = publicKeysData;

    Rust::TWDataWrapper output = Rust::tw_transaction_compiler_compile(static_cast<uint32_t>(coin), input.get(), signaturesVec.get(), publicKeysVec.get());
    dataOut = output.toDataOrDefault();
}

} // namespace TW
