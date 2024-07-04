// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWTONAddressConverter.h>

#include "Base64.h"
#include "TheOpenNetwork/Address.h"

using namespace TW;

TWString *_Nullable TWTONAddressConverterToBoc(TWString *_Nonnull address) {
    auto& addressString = *reinterpret_cast<const std::string*>(address);

    try {
        const TheOpenNetwork::Address addressTon(addressString);
        auto bocEncoded = addressTon.toBoc();
        return TWStringCreateWithUTF8Bytes(bocEncoded.c_str());
    } catch (...) {
        return nullptr;
    }
}

TWString *_Nullable TWTONAddressConverterFromBoc(TWString *_Nonnull boc) {
    auto& bocEncoded = *reinterpret_cast<const std::string*>(boc);

    try {
        auto address = TheOpenNetwork::Address::fromBoc(bocEncoded);
        if (!address) {
            return nullptr;
        }

        auto userFriendly = true;
        auto bounceable = false;
        auto addressStr = address->string(userFriendly, bounceable);

        return TWStringCreateWithUTF8Bytes(addressStr.c_str());
    } catch (...) {
        return nullptr;
    }
}

TWString *_Nullable TWTONAddressConverterToUserFriendly(TWString *_Nonnull address, bool bounceable, bool testnet) {
    auto& addressString = *reinterpret_cast<const std::string*>(address);

    try {
        const TheOpenNetwork::Address addressTon(addressString);

        auto userFriendly = true;
        const auto addressFormatted = addressTon.string(userFriendly, bounceable, testnet);

        return TWStringCreateWithUTF8Bytes(addressFormatted.c_str());
    } catch (...) {
        return nullptr;
    }
}
