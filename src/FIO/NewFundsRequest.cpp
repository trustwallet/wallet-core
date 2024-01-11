// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "NewFundsRequest.h"
#include "../BinaryCoding.h"

#include <string>

namespace TW::FIO {

using namespace std;

void NewFundsContent::serialize(Data& out) const {
    encodeString(payeePublicAddress, out);
    encodeString(amount, out);
    encodeString(chainCode, out);
    encodeString(coinSymbol, out);
    encodeString(memo, out);
    encodeString(hash, out);
    encodeString(offlineUrl, out);
    encodeString("", out); // future_use1
    encodeString("", out); // future_use2
    encodeString("", out); // future_use3
    encodeString("", out); // future_use4
    encodeString("", out); // future_use5
}

NewFundsContent NewFundsContent::deserialize(const Data& in, size_t& indexInOut) {
    NewFundsContent newFunds;

    auto result = decodeString(in, indexInOut);
    if (!get<0>(result)) { return newFunds; }
    newFunds.payeePublicAddress = get<1>(result);
    result = decodeString(in, indexInOut);
    if (!get<0>(result)) { return newFunds; }
    newFunds.amount = get<1>(result);
    result = decodeString(in, indexInOut);
    if (!get<0>(result)) { return newFunds; }
    newFunds.chainCode = get<1>(result);
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
