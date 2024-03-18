// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"

namespace TW {

template <std::size_t BYTES>
class Uint {
public:
    Uint() = default;

private:
    Data data;
};

} // namespace TW
