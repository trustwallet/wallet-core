// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../proto/NEAR.pb.h"
#include "Data.h"

namespace TW::NEAR {

Data transactionData(const Proto::SigningInput& input);
Data signedTransactionData(const Data& transactionData, const Data& signatureData);
Data transactionDataWithPublicKey(const Proto::SigningInput& input);
} // namespace TW::NEAR
