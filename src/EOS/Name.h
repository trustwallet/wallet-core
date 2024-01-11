// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#pragma once

#include "Data.h"

namespace TW::EOS {

class Name {
public:
    uint64_t value = 0;

    Name() = default;
    Name(const std::string& str);
    static uint64_t toSymbol(char c) noexcept;
    std::string string() const noexcept;

    void serialize(TW::Data& o) const noexcept;
};

} // namespace TW::EOS
