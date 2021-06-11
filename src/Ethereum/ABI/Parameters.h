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
    virtual Data hashStruct() const;
};

/// A named param.
class ParamNamed: public ParamBase
{
public:
    std::string _name;
    std::shared_ptr<ParamBase> _param;
    ParamNamed(const std::string& name, std::shared_ptr<ParamBase> param): _name(name), _param(param) {}

    virtual std::string getType() const;
    virtual size_t getSize() const { return _param->getSize(); }
    virtual bool isDynamic() const { return _param->isDynamic(); }
    virtual void encode(Data& data) const { return _param->encode(data); }
    virtual bool decode(const Data& encoded, size_t& offset_inout) { return _param->decode(encoded, offset_inout); }
    virtual Data hashStruct() const { return _param->hashStruct(); }
    virtual std::string getExtraTypes(std::vector<std::string>& ignoreList) const { return _param->getExtraTypes(ignoreList); }
};

/// A collection of named parameters.
class ParamSetNamed {
private:
    std::vector<std::shared_ptr<ParamNamed>> _params;

public:
    ParamSetNamed() = default;
    ParamSetNamed(const std::shared_ptr<ParamNamed>& param1) { addParam(param1); }
    ParamSetNamed(const std::vector<std::shared_ptr<ParamNamed>>& params) { addParams(params); }
    virtual ~ParamSetNamed();

    /// Returns the index of the parameter
    int addParam(const std::shared_ptr<ParamNamed>& param);
    void addParams(const std::vector<std::shared_ptr<ParamNamed>>& params);
    size_t getCount() const { return _params.size(); }
    std::string getType() const;
    Data encodeHashes() const;
    std::string getExtraTypes(std::vector<std::string>& ignoreList) const;
};

/// A named structure (set of parameters plus a type name).
class ParametersNamed: public ParamCollection
{
private:
    std::string _name;
    ParamSetNamed _params;

public:
    ParametersNamed() = default;
    ParametersNamed(const std::string& name, const std::vector<std::shared_ptr<ParamNamed>>& params) : ParamCollection(), _name(name), _params(ParamSetNamed(params)) {}

    std::string getType() const { return _name; }
    /// Get full type, extended by used sub-types, of the form 'Mail(Person from,Person to,string contents)Person(string name,address wallet)'
    std::string encodeType() const {
        std::vector<std::string> ignoreList;
        return getExtraTypes(ignoreList);
    }
    /// Get the hash of the full type.
    Data hashType() const;
    virtual size_t getSize() const { return _params.getCount(); }
    virtual bool isDynamic() const { return true; }
    virtual size_t getCount() const { return _params.getCount(); }
    virtual void encode(Data& data) const {}
    virtual bool decode(const Data& encoded, size_t& offset_inout) { return true; }
    Data encodeHashes() const;
    virtual Data hashStruct() const;
    virtual std::string getExtraTypes(std::vector<std::string>& ignoreList) const;
};

} // namespace TW::Ethereum::ABI
