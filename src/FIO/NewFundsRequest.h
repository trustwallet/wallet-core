// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "../Data.h"

#include <string>

namespace TW::FIO {

/// Data class, data encoded in a new funds request
class NewFundsContent {
public:
    /// FIO name of the addressed payee, e.g. "mario@trust"
    std::string payeeFioName;
    /// Amount requested (string)
    std::string amount;
    /// Token code of the amount requested
    std::string tokenCode;
    /// Memo free text (optional, may be empty)
    std::string memo;
    /// Optional hash
    std::string hash;
    /// Optional url
    std::string offlineUrl;

    /// Serialize into binary stream (abi)
    void serialize(Data& out) const;

    /// Deserialize, create from binary stream.  Return default (empty fields) on error.
    static NewFundsContent deserialize(const Data& in, size_t& indexInOut);
};

} // namespace TW::FIO
