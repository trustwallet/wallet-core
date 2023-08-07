// Copyright © 2017-2023 Trust Wallet.
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
class ParamTuple final : public ParamCollection
{
public:
    ParamSet _params;

    ParamTuple() = default;
    explicit ParamTuple(const std::vector<std::shared_ptr<ParamBase>>& params) : _params(ParamSet(params)) {}

    /// Add a parameter. Returns the index of the parameter.
    int addParam(std::shared_ptr<ParamBase> param);
    /// Get a parameter.
    bool getParam(int paramIndex, std::shared_ptr<ParamBase>& param_out) {
        return _params.getParam(paramIndex, param_out);
    }
    /// Return the type signature, of the form "(int32,uint256)"
    std::string getType() const override  { return _params.getType(); }

    size_t getSize() const override { return _params.getSize(); }
    bool isDynamic() const override { return _params.isDynamic(); }
    void encode(Data& data) const override { return _params.encode(data); }
    bool decode(const Data& encoded, size_t& offset_inout) override { return _params.decode(encoded, offset_inout); }
    bool setValueJson([[maybe_unused]] const std::string& value) override { return false; }
    size_t getCount() const override { return _params.getCount(); }
    std::shared_ptr<ParamBase> clone() const override;
};

} // namespace TW::Ethereum::ABI
