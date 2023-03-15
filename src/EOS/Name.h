// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
