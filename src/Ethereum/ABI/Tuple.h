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
    bool _isDynamic = false;

    ParamTuple() {}
    ParamTuple(const std::vector<std::shared_ptr<ParamBase>>& params) : _params(ParamSet(params)) {}

    /// Add a parameter. Returns the index of the parameter.
    int addParam(std::shared_ptr<ParamBase> param) {
        if (!_isDynamic && param->isDynamic()) {
            _isDynamic = true;
        }
        return _params.addParam(param);
    }
    /// Get a parameter.
    bool getParam(int paramIndex, std::shared_ptr<ParamBase>& param_out) {
        return _params.getParam(paramIndex, param_out);
    }
    /// Return the function type signature, of the form "baz(int32,uint256)"
    std::string getType() const { return _params.getType(); }

    virtual size_t getSize() const;
    virtual bool isDynamic() const { return _isDynamic; }
    virtual void encode(Data& data) const;
    virtual bool decode(const Data& encoded, size_t& offset_inout);
    virtual bool setValueJson(const std::string& value);
    virtual size_t getCount() const { return _params.getCount(); }
};

} // namespace TW::Ethereum::ABI
