// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWTONAddressConverter.h>

#include "Base64.h"
#include "rust/Wrapper.h"

using namespace TW;

TWString *_Nullable TWTONAddressConverterToBoc(TWString *_Nonnull address) {
    auto& addressString = *reinterpret_cast<const std::string*>(address);

    const Rust::TWStringWrapper addressRustStr = addressString;
    const Rust::TWStringWrapper bocRustStr = Rust::tw_ton_address_converter_to_boc(addressRustStr.get());
    if (!bocRustStr) {
        return nullptr;
    }

    return TWStringCreateWithUTF8Bytes(bocRustStr.c_str());
}

TWString *_Nullable TWTONAddressConverterFromBoc(TWString *_Nonnull boc) {
    auto& bocEncoded = *reinterpret_cast<const std::string*>(boc);

    const Rust::TWStringWrapper bocRustStr = bocEncoded;
    const Rust::TWStringWrapper addressRustStr = Rust::tw_ton_address_converter_from_boc(bocRustStr.get());
    if (!addressRustStr) {
        return nullptr;
    }

    return TWStringCreateWithUTF8Bytes(addressRustStr.c_str());
}

TWString *_Nullable TWTONAddressConverterToUserFriendly(TWString *_Nonnull address, bool bounceable, bool testnet) {
    auto& addressString = *reinterpret_cast<const std::string*>(address);

    const Rust::TWStringWrapper addressRustStr = addressString;
    const Rust::TWStringWrapper userFriendlyRustStr = Rust::tw_ton_address_converter_to_user_friendly(addressRustStr.get(), bounceable, testnet);
    if (!userFriendlyRustStr) {
        return nullptr;
    }

    return TWStringCreateWithUTF8Bytes(userFriendlyRustStr.c_str());
}
