// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWBitcoinFeeCalculator.h>
#include "Bitcoin/Transaction.h"
#include "Data.h"

using namespace TW;

namespace internal {

std::optional<uint64_t> TWBitcoinCalculateFee(TWData *_Nonnull data, uint64_t satVb) {
    auto* encoded = reinterpret_cast<const TW::Data*>(data);
	return TW::Bitcoin::Transaction::calculateFee(*encoded, satVb);
}

}