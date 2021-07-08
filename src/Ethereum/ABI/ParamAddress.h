// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "ParamNumber.h"
#include <Data.h>

namespace TW::Ethereum::ABI {

/// 160-bit Address parameter, "address".  Padded to the right, treated like ParamUInt160
class ParamAddress: public ParamUIntN
{
public:
    static const size_t bytes = 20;
    ParamAddress(): ParamUIntN(bytes * 8) {}
    ParamAddress(const Data& val): ParamUIntN(bytes * 8, load(val)) {}
    virtual std::string getType() const { return "address"; };
    // get the value as (20-byte) byte array (as opposed to uint256_t)
    Data getData() const;
    virtual bool setValueJson(const std::string& value);
};

} // namespace TW::Ethereum::ABI
