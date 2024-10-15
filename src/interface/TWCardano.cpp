// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <algorithm>
#include <TrustWalletCore/TWCardano.h>

#include "Cardano/Transaction.h"
#include "Cardano/AddressV3.h"
#include "proto/Cardano.pb.h"

using namespace TW;

namespace internal {

/// May throw an exception if `tokenBundle` can't be deserialzied as `TW::Cardano::Proto::TokenBundle`.
std::optional<uint64_t> cardanoMinAdaAmount(const std::string& toAddress, const Data *tokenBundle, uint64_t coinsPerUtxoByte) {
    // Set the initial amount to 0.
    const uint64_t amount = 0;
    const TW::Cardano::AddressV3 cardanoAddress(toAddress);

    TW::Cardano::Proto::TokenBundle bundleProto;
    if (!tokenBundle || !bundleProto.ParseFromArray(tokenBundle->data(), (int)tokenBundle->size())) {
        // Expect at least an empty `TokenBundle`.
        return std::nullopt;
    }
    const auto tokens = TW::Cardano::TokenBundle::fromProto(bundleProto);

    const TW::Cardano::TxOutput output(cardanoAddress.data(), amount, tokens);
    return output.minAdaAmount(coinsPerUtxoByte);
}

}

std::optional<uint64_t> parseAdaAmount(const std::string &amount) {
    const bool onlyDigits = std::all_of(
        amount.begin(),
        amount.end(),
        [](unsigned char c)->bool { return std::isdigit(c); }
    );
    if (!onlyDigits) {
        return std::nullopt;
    }

    std::size_t chars;
    uint64_t amountInt = std::stoull(amount, &chars);
    if (chars != amount.size()) {
        return std::nullopt;
    }

    return amountInt;
}

uint64_t TWCardanoMinAdaAmount(TWData *_Nonnull tokenBundle) {
    const Data* bundleData = static_cast<const Data*>(tokenBundle);
    TW::Cardano::Proto::TokenBundle bundleProto;
    if (bundleData && bundleProto.ParseFromArray(bundleData->data(), (int)bundleData->size())) {
        return TW::Cardano::TokenBundle::fromProto(bundleProto).minAdaAmount();
    }
    return 0;
}

TWString *_Nullable TWCardanoOutputMinAdaAmount(TWString *_Nonnull toAddress, TWData *_Nonnull tokenBundle, TWString *_Nonnull coinsPerUtxoByte) {
    const std::string& address = *reinterpret_cast<const std::string*>(toAddress);
    const std::string& coinsPerUtxoByteStr = *reinterpret_cast<const std::string*>(coinsPerUtxoByte);
    const auto* bundleData = static_cast<const Data*>(tokenBundle);

    try {
        const auto coinsPerUtxoByteInt = parseAdaAmount(coinsPerUtxoByteStr);
        if (!coinsPerUtxoByteInt) {
            // Couldn't parse the `coinsPerUtxoByte` string.
            return nullptr;
        }

        const auto minAdaAmount = internal::cardanoMinAdaAmount(address, bundleData, *coinsPerUtxoByteInt);
        if (!minAdaAmount) {
            return nullptr;
        }
        return TWStringCreateWithUTF8Bytes(std::to_string(*minAdaAmount).c_str());
    } catch (...) {
        return nullptr;
    }
}

TWString *_Nonnull TWCardanoGetStakingAddress(TWString *_Nonnull baseAddress) {
    const auto& address = *reinterpret_cast<const std::string*>(baseAddress);
    try {
        return TWStringCreateWithUTF8Bytes(TW::Cardano::AddressV3(address).getStakingAddress().c_str());
    } catch (...) {
        return TWStringCreateWithUTF8Bytes("");
    }
}

TWString *_Nonnull TWCardanoGetByronAddress(struct TWPublicKey *_Nonnull publicKey) {
    try {
        return TWStringCreateWithUTF8Bytes(TW::Cardano::AddressV2({ publicKey->impl }).string().c_str());
    } catch (...) {
        return TWStringCreateWithUTF8Bytes("");
    }
}
