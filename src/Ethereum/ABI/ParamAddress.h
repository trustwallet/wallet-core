// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "Bytes.h"
#include "Util.h"
#include "../../Data.h"

namespace TW::Ethereum::ABI {

/// 160-bit Address parameter, "address"
class ParamAddress: public ParamByteArrayFix
{
public:
    static const size_t bytes = 160/8;
    ParamAddress(): ParamByteArrayFix(bytes) {}
    ParamAddress(const Data& val): ParamByteArrayFix(bytes, val) {}
    virtual std::string getType() const { return "address"; };
};

} // namespace TW::Ethereum::ABI
