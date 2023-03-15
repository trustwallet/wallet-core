// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once

#include "ParamBase.h"
#include "Parameters.h"
#include "Bytes.h"

#include "../../uint256.h"
#include "../../Hash.h"

#include <string>

namespace TW::Ethereum::ABI {

/// Non-generic version of Function, templated version is impossible to pass around to and back over C interface
/// (void* looses the template parameters).
class Function {
public:
    std::string name;
    ParamSet _inParams;
    ParamSet _outParams;

    Function(std::string name) : name(std::move(name)) {}
    Function(std::string name, const std::vector<std::shared_ptr<ParamBase>>& inParams)
        : name(std::move(name)), _inParams(ParamSet(inParams)) {}
    virtual ~Function() {}
    /// Add an input parameter. Returns the index of the parameter.
    int addInParam(std::shared_ptr<ParamBase> param) {
        return _inParams.addParam(param);
    }
    /// Add an output parameter. Returns the index of the parameter.
    int addOutParam(std::shared_ptr<ParamBase> param) {
        return _outParams.addParam(param);
    }
    /// Add an input or output parameter. Returns the index of the parameter.
    int addParam(std::shared_ptr<ParamBase> param, bool isOutput = false) {
        return isOutput ? _outParams.addParam(param) : _inParams.addParam(param);
    }
    /// Get an input parameter.
    bool getInParam(int paramIndex, std::shared_ptr<ParamBase>& param_out) {
        return _inParams.getParam(paramIndex, param_out);
    }
    /// Get an output parameter.
    bool getOutParam(int paramIndex, std::shared_ptr<ParamBase>& param_out) {
        return _outParams.getParam(paramIndex, param_out);
    }
    /// Get an input or output parameter.
    bool getParam(int paramIndex, std::shared_ptr<ParamBase>& param_out, bool isOutput = false) {
        return isOutput ? _outParams.getParam(paramIndex, param_out) : _inParams.getParam(paramIndex, param_out);
    }
    /// Return the function type signature, of the form "baz(int32,uint256)"
    std::string getType() const {
        return name + _inParams.getType();
    }

    /// Return the 4-byte function signature
    Data getSignature() const;

    virtual void encode(Data& data) const;

    /// Decode binary, fill output parameters
    bool decodeOutput(const Data& encoded, size_t& offset_inout);
    /// Decode binary, fill input parameters
    bool decodeInput(const Data& encoded, size_t& offset_inout);
};

inline void encode(const Function& func, Data& data) {
    func.encode(data);
}

} // namespace TW::Ethereum::ABI

/// Wrapper for C interface.
struct TWEthereumAbiFunction {
    TW::Ethereum::ABI::Function impl;
};
