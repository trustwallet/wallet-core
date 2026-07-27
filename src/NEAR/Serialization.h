// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "../proto/NEAR.pb.h"
#include "../Result.h"
#include "Data.h"

namespace TW::NEAR {

Result<Data, Common::Proto::SigningError> transactionData(const Proto::SigningInput& input);
Data signedTransactionData(const Data& transactionData, const Data& signatureData);
Result<Data, Common::Proto::SigningError> transactionDataWithPublicKey(const Proto::SigningInput& input);
} // namespace TW::NEAR
