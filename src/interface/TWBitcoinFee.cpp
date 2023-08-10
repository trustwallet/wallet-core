// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
