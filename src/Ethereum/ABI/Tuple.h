// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "ParamBase.h"
#include "Parameters.h"
#include "Data.h"

namespace TW::Ethereum::ABI {

/// A Tuple is a collection of parameters
class ParamTuple: public ParamCollection
{
public:
    ParamSet _params;

    ParamTuple() {}
    ParamTuple(const std::vector<std::shared_ptr<ParamBase>>& params) : _params(ParamSet(params)) {}

    /// Add a parameter. Returns the index of the parameter.
    int addParam(std::shared_ptr<ParamBase> param);
    /// Get a parameter.
    bool getParam(int paramIndex, std::shared_ptr<ParamBase>& param_out) {
        return _params.getParam(paramIndex, param_out);
    }
    /// Return the type signature, of the form "(int32,uint256)"
    std::string getType() const { return _params.getType(); }

    virtual size_t getSize() const { return _params.getSize(); }
    virtual bool isDynamic() const { return _params.isDynamic(); }
    virtual void encode(Data& data) const { return _params.encode(data); }
    virtual bool decode(const Data& encoded, size_t& offset_inout) { return _params.decode(encoded, offset_inout); }
    virtual bool setValueJson(const std::string& value) { return false; }
    virtual size_t getCount() const { return _params.getCount(); }
};

} // namespace TW::Ethereum::ABI
