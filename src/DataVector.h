// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "TrustWalletCore/TWDataVector.h"
#include "Data.h"

namespace TW {

static std::vector<Data> createFromTWDataVector(const struct TWDataVector* _Nonnull dataVector) {
    std::vector<Data> ret;
    const auto n = TWDataVectorSize(dataVector);
    for (auto i = 0uL; i < n; ++i) {
        const auto* const elem = TWDataVectorGet(dataVector, i);
        if (const auto* const data = reinterpret_cast<const Data *>(elem); data) {
            ret.emplace_back(*data);
            TWDataDelete(elem);
        }
    }
    return ret;
}

} // namespace TW
