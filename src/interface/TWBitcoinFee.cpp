// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWBitcoinFee.h>
#include "Bitcoin/Transaction.h"
#include "Data.h"

uint64_t TWBitcoinFeeCalculateFee(TWData* _Nonnull data, uint64_t satVb) {
    auto* encoded = reinterpret_cast<const TW::Data*>(data);
    auto fee = TW::Bitcoin::Transaction::calculateFee(*encoded, satVb);
    if (!fee) {
        return 0;
    }

    return fee.value();
}
