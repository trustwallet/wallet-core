// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "ParamBase.h"
#include "ParamNumber.h"

#include <vector>
#include <string>
#include <memory>

namespace TW::Ethereum::ABI {

/// A set of parameters
class ParamSet {
private:
    std::vector<std::shared_ptr<ParamBase>> _params;

public:
    ParamSet() = default;
    ParamSet(const std::shared_ptr<ParamBase>& param1) { addParam(param1); }
    ParamSet(const std::vector<std::shared_ptr<ParamBase>>& params) { addParams(params); }
    virtual ~ParamSet();

    /// Returns the index of the parameter
    int addParam(const std::shared_ptr<ParamBase>& param);
    void addParams(const std::vector<std::shared_ptr<ParamBase>>& params);
    bool getParam(size_t paramIndex, std::shared_ptr<ParamBase>& param_out) const;
    std::shared_ptr<ParamBase> getParamUnsafe(size_t paramIndex) const;
    size_t getCount() const { return _params.size(); }
    std::vector<std::shared_ptr<ParamBase>> const& getParams() const { return _params; }
    /// Return the function type signature, of the form "baz(int32,uint256)"
    std::string getType() const;
    bool isDynamic() const;
    size_t getSize() const;
    virtual void encode(Data& data) const;
    virtual bool decode(const Data& encoded, size_t& offset_inout);
    Data encodeHashes() const;
    /// Creates a copy of this set with exactly the same params.
    ParamSet clone() const;

private:
    size_t getHeadSize() const;
};

/// Collection of different parameters, dynamic length, "(<par1>,<par2>,...)".
class Parameters final : public ParamCollection
{
private:
    ParamSet _params;

public:
    Parameters() = default;
    Parameters(const std::vector<std::shared_ptr<ParamBase>>& params) : ParamCollection(), _params(ParamSet(params)) {}
    void addParam(const std::shared_ptr<ParamBase>& param) { _params.addParam(param); }
    void addParams(const std::vector<std::shared_ptr<ParamBase>>& params) { _params.addParams(params); }
    std::shared_ptr<ParamBase> getParam(size_t paramIndex) const { return _params.getParamUnsafe(paramIndex); }
    std::string getType() const override { return _params.getType(); }
    size_t getSize() const override { return _params.getSize(); }
    bool isDynamic() const override { return true; }
    size_t getCount() const override { return _params.getCount(); }
    void encode(Data& data) const override { _params.encode(data); }
    bool decode(const Data& encoded, size_t& offset_inout) override { return _params.decode(encoded, offset_inout); }
    bool setValueJson([[maybe_unused]] const std::string& value) override { return false; }
    Data hashStruct() const override;
    std::shared_ptr<ParamBase> clone() const override;
};

} // namespace TW::Ethereum::ABI
