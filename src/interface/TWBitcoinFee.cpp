// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <TrustWalletCore/TWBitcoinFee.h>
#include "Bitcoin/Transaction.h"

TWString* _Nullable TWBitcoinFeeCalculateFee(TWData* _Nonnull data, TWString* _Nonnull satVb) {
    auto* encoded = reinterpret_cast<const TW::Data*>(data);
    const std::string& satVbStr = *reinterpret_cast<const std::string*>(satVb);
    uint64_t satVbInt = std::stoull(satVbStr);
    auto fee = TW::Bitcoin::Transaction::calculateFee(*encoded, satVbInt);
    if (!fee) {
        return nullptr;
    }

    return TWStringCreateWithUTF8Bytes(std::to_string(*fee).c_str());
}
