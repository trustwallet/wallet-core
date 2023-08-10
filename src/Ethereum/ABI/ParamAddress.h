// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "ParamNumber.h"
#include <Data.h>

namespace TW::Ethereum::ABI {

/// 160-bit Address parameter, "address".  Padded to the right, treated like ParamUInt160
class ParamAddress final: public ParamBase
{
private:
    ParamUIntN _val;
public:
    static const size_t bytes = 20;
    ParamAddress(): _val(bytes * 8) {}
    explicit ParamAddress(const Data& val): _val(bytes * 8, load(val)) {}
    std::string getType() const override { return "address"; };
    size_t getSize() const override { return _val.getSize(); }
    bool isDynamic() const override { return _val.isDynamic(); }
    void encode(Data& data) const override { _val.encode(data); }
    bool decode(const Data& encoded, size_t& offset_inout) override { return _val.decode(encoded, offset_inout); }
    // get the value as (20-byte) byte array (as opposed to uint256_t)
    Data getData() const;
    bool setValueJson(const std::string& value) override;
    std::shared_ptr<ParamBase> clone() const override;
};

} // namespace TW::Ethereum::ABI
