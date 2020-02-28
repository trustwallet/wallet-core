// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "NewFundsRequest.h"
#include "../BinaryCoding.h"

#include <string>

namespace TW::FIO {

using namespace std;

void NewFundsContent::serialize(Data& out) const {
    encodeString(payeeFioName, out);
    encodeString(amount, out);
    encodeString(coinSymbol, out);
    encodeString(memo, out);
    encodeString(hash, out);
    encodeString(offlineUrl, out);
}

NewFundsContent NewFundsContent::deserialize(const Data& in, size_t& indexInOut) {
    NewFundsContent newFunds;

    auto result = decodeString(in, indexInOut);
    if (!get<0>(result)) { return newFunds; }
    newFunds.payeeFioName = get<1>(result);
    result = decodeString(in, indexInOut);
    if (!get<0>(result)) { return newFunds; }
    newFunds.amount = get<1>(result);
    result = decodeString(in, indexInOut);
    if (!get<0>(result)) { return newFunds; }
    newFunds.coinSymbol = get<1>(result);
    result = decodeString(in, indexInOut);
    if (!get<0>(result)) { return newFunds; }
    newFunds.memo = get<1>(result);
    result = decodeString(in, indexInOut);
    if (!get<0>(result)) { return newFunds; }
    newFunds.hash = get<1>(result);
    result = decodeString(in, indexInOut);
    if (!get<0>(result)) { return newFunds; }
    newFunds.offlineUrl = get<1>(result);

    return newFunds;
}

} // namespace TW::FIO
