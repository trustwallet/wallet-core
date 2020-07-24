// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "ParamBase.h"
#include "ParamNumber.h"
#include "Parameters.h"

namespace TW::Ethereum::ABI {

/// Dynamic array of the same types, "<type>[]"
class ParamArray: public ParamCollection
{
private:
    ParamSet _params;

public:
    ParamArray() = default;
    ParamArray(const std::shared_ptr<ParamBase>& param1) : ParamCollection() { addParam(param1); }
    ParamArray(const std::vector<std::shared_ptr<ParamBase>>& params) : ParamCollection() { setVal(params); }
    void setVal(const std::vector<std::shared_ptr<ParamBase>>& params) { addParams(params); }
    std::vector<std::shared_ptr<ParamBase>> const& getVal() const { return _params.getParams(); }
    int addParam(const std::shared_ptr<ParamBase>& param);
    void addParams(const std::vector<std::shared_ptr<ParamBase>>& params);
    std::string getFirstType() const;
    std::shared_ptr<ParamBase> getParam(int paramIndex) { return _params.getParamUnsafe(paramIndex); }
    virtual std::string getType() const { return getFirstType() + "[]"; }
    virtual size_t getSize() const;
    virtual bool isDynamic() const { return true; }
    virtual size_t getCount() const { return _params.getCount(); }
    virtual void encode(Data& data) const;
    virtual bool decode(const Data& encoded, size_t& offset_inout);
};

} // namespace TW::Ethereum::ABI
