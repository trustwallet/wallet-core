// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include <TrustWalletCore/TWData.h>
#include <memory>
#include "Data.h"

inline std::shared_ptr<TWData> wrapTWData(TWData* data) {
    return std::shared_ptr<TWData>(data, TWDataDelete);
}

inline TW::Data dataFromTWData(const std::shared_ptr<TWData>& data) {
    return TW::Data(TWDataBytes(data.get()), TWDataBytes(data.get()) + TWDataSize(data.get()));
}
