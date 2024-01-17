// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "RustCoinEntry.h"
#include "Wrapper.h"

namespace TW::Rust {

bool RustCoinEntry::validateAddress(TWCoinType coin, const std::string &address, const PrefixVariant &addressPrefix) const {
    Rust::TWStringWrapper addressStr = address;

    if (std::holds_alternative<std::monostate>(addressPrefix)) {
        return Rust::tw_any_address_is_valid(addressStr.get(), static_cast<uint32_t>(coin));
    } else if (const auto* hrpPrefix = std::get_if<Bech32Prefix>(&addressPrefix); hrpPrefix) {
        Rust::TWStringWrapper hrpStr = std::string(*hrpPrefix);
        return Rust::tw_any_address_is_valid_bech32(addressStr.get(), static_cast<uint32_t>(coin), hrpStr.get());
    } else {
        throw std::invalid_argument("`Rust::tw_any_address_is_valid_ss58`, `Rust::tw_any_address_create_with_public_key_filecoin_address_type` are not supported yet");
    }
}

std::string RustCoinEntry::normalizeAddress(TWCoinType coin, const std::string& address) const {
    Rust::TWStringWrapper addressStr = address;

    // `CoinEntry::normalizeAddress` is used when a `TWAnyAddress` has been created already, therefore validated.
    auto anyAddress = Rust::wrapTWAnyAddress(
        Rust::tw_any_address_create_with_string_unchecked(addressStr.get(), static_cast<uint32_t>(coin)));
    if (!anyAddress) {
        return {};
    }

    Rust::TWStringWrapper normalized = Rust::tw_any_address_description(anyAddress.get());
    return normalized.toStringOrDefault();
}

std::string RustCoinEntry::deriveAddress(TWCoinType coin, const PublicKey& publicKey, TWDerivation derivation, const PrefixVariant& addressPrefix) const {
    auto *twPublicKeyRaw = Rust::tw_public_key_create_with_data(publicKey.bytes.data(),
                                                                publicKey.bytes.size(),
                                                                static_cast<uint32_t>(publicKey.type));
    auto twPublicKey = Rust::wrapTWPublicKey(twPublicKeyRaw);
    if (!twPublicKey) {
        return {};
    }

    Rust::TWAnyAddress* anyAddressRaw = nullptr;
    if (std::holds_alternative<std::monostate>(addressPrefix)) {
        anyAddressRaw = Rust::tw_any_address_create_with_public_key_derivation(twPublicKey.get(),
                                                                               static_cast<uint32_t>(coin),
                                                                               static_cast<uint32_t>(derivation));
    } else if (const auto* hrpPrefix = std::get_if<Bech32Prefix>(&addressPrefix); hrpPrefix) {
        Rust::TWStringWrapper hrpStr = std::string(*hrpPrefix);
        anyAddressRaw = Rust::tw_any_address_create_bech32_with_public_key(twPublicKey.get(),
                                                                           static_cast<uint32_t>(coin),
                                                                           hrpStr.get());
    } else {
        throw std::invalid_argument("`Rust::tw_any_address_is_valid_ss58`, `Rust::tw_any_address_create_with_public_key_filecoin_address_type` are not supported yet");
    }

    auto anyAddress = Rust::wrapTWAnyAddress(anyAddressRaw);
    if (!anyAddress) {
        return {};
    }

    Rust::TWStringWrapper derivedAddress = Rust::tw_any_address_description(anyAddress.get());
    return derivedAddress.toStringOrDefault();
}

Data RustCoinEntry::addressToData(TWCoinType coin, const std::string& address) const {
    Rust::TWStringWrapper addressStr = address;

    // `CoinEntry::normalizeAddress` is used when a `TWAnyAddress` has been created already, therefore validated.
    auto anyAddress = Rust::wrapTWAnyAddress(
        Rust::tw_any_address_create_with_string_unchecked(addressStr.get(), static_cast<uint32_t>(coin)));
    if (!anyAddress) {
        return {};
    }

    Rust::TWDataWrapper data = Rust::tw_any_address_data(anyAddress.get());
    return data.toDataOrDefault();
}

void RustCoinEntry::sign(TWCoinType coin, const Data& dataIn, Data& dataOut) const {
    Rust::TWDataWrapper input = Rust::tw_data_create_with_bytes(dataIn.data(), dataIn.size());
    Rust::TWDataWrapper output = Rust::tw_any_signer_sign(input.get(), static_cast<uint32_t>(coin));

    dataOut = output.toDataOrDefault();
}

Data RustCoinEntry::preImageHashes(TWCoinType coin, const Data& txInputData) const {
    Rust::TWDataWrapper input = txInputData;
    Rust::TWDataWrapper output = Rust::tw_transaction_compiler_pre_image_hashes(static_cast<uint32_t>(coin), input.get());

    return output.toDataOrDefault();
}

void RustCoinEntry::compile(TWCoinType coin, const Data& txInputData, const std::vector<Data>& signatures, const std::vector<PublicKey>& publicKeys, Data& dataOut) const {
    Rust::TWDataWrapper input = txInputData;

    std::vector<Data> publicKeysData;
    for (const auto& publicKey : publicKeys) {
        publicKeysData.push_back(publicKey.bytes);
    }

    Rust::TWDataVectorWrapper signaturesVec = signatures;
    Rust::TWDataVectorWrapper publicKeysVec = publicKeysData;

    Rust::TWDataWrapper output = Rust::tw_transaction_compiler_compile(static_cast<uint32_t>(coin), input.get(), signaturesVec.get(), publicKeysVec.get());
    dataOut = output.toDataOrDefault();
}

} // namespace TW::Rust
