// Copyright © 2017-2020 Trust Wallet.
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
    bool getParam(int paramIndex, std::shared_ptr<ParamBase>& param_out) const;
    std::shared_ptr<ParamBase> getParamUnsafe(int paramIndex) const;
    size_t getCount() const { return _params.size(); }
    std::vector<std::shared_ptr<ParamBase>> const& getParams() const { return _params; }
    /// Return the function type signature, of the form "baz(int32,uint256)"
    std::string getType() const;
    bool isDynamic() const;
    size_t getSize() const;
    virtual void encode(Data& data) const;
    virtual bool decode(const Data& encoded, size_t& offset_inout);
    Data encodeHashes() const;

private:
    size_t getHeadSize() const;
};

/// Collection of different parameters, dynamic length, "(<par1>,<par2>,...)".
class Parameters: public ParamCollection
{
private:
    ParamSet _params;

public:
    Parameters() = default;
    Parameters(const std::vector<std::shared_ptr<ParamBase>>& params) : ParamCollection(), _params(ParamSet(params)) {}
    void addParam(const std::shared_ptr<ParamBase>& param) { _params.addParam(param); }
    void addParams(const std::vector<std::shared_ptr<ParamBase>>& params) { _params.addParams(params); }
    std::shared_ptr<ParamBase> getParam(int paramIndex) const { return _params.getParamUnsafe(paramIndex); }
    virtual std::string getType() const { return _params.getType(); }
    virtual size_t getSize() const { return _params.getSize(); }
    virtual bool isDynamic() const { return true; }
    virtual size_t getCount() const { return _params.getCount(); }
    virtual void encode(Data& data) const { _params.encode(data); }
    virtual bool decode(const Data& encoded, size_t& offset_inout) { return _params.decode(encoded, offset_inout); }
    virtual bool setValueJson(const std::string& value) { return false; }
    virtual Data hashStruct() const;
};

} // namespace TW::Ethereum::ABI
