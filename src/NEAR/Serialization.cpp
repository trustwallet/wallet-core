// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Serialization.h"

#include "../NEAR/Address.h"
#include "../Base64.h"
#include <TrustWalletCore/TWHRP.h>

using namespace TW;
using namespace TW::NEAR;
using namespace TW::NEAR::Proto;

Data TW::NEAR::transactionData(const Proto::SigningInput& input) {
    return Data();
}

Data TW::NEAR::signedTransactionData(const Data& transactionData, const Data& signatureData) {
    return Data();
}