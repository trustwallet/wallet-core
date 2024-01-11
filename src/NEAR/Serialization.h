// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "../proto/NEAR.pb.h"
#include "Data.h"

namespace TW::NEAR {

Data transactionData(const Proto::SigningInput& input);
Data signedTransactionData(const Data& transactionData, const Data& signatureData);
Data transactionDataWithPublicKey(const Proto::SigningInput& input);
} // namespace TW::NEAR
