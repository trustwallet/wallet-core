// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Data.h"

namespace TW::Tezos {
    // implement format input as described in https://tezostaquito.io/docs/signing/
    std::string formatMessage(const std::string& message, const std::string& dAppUrl);
    // implement input to payload as described in: https://tezostaquito.io/docs/signing/
    std::string inputToPayload(const std::string& input);
}
