// Copyright © 201§-2019 Trust Wallet.
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
#include <cassert>

namespace TW::Ethereum::ABI {

/// Non-generic version of Function, templated version is impossible to pass around to and back over C interface
/// (void* looses the temaplate parameters).
class Function {
public:
    std::string name;
    ParamSet _inParams;
    ParamSet _outParams;

    Function(std::string name) : name(std::move(name)) {}
    Function(std::string name, std::vector<std::shared_ptr<ParamBase>> inParams)
        : name(std::move(name)), _inParams(ParamSet(inParams)) {}
    virtual ~Function() {}
    /// Returns the index of the parameter
    int addInParam(std::shared_ptr<ParamBase> param) {
        return _inParams.addParam(param);
    }
    /// Returns the index of the parameter
    int addOutParam(std::shared_ptr<ParamBase> param) {
        return _outParams.addParam(param);
    }
    bool getInParam(int paramIndex, std::shared_ptr<ParamBase>& param_out) {
        return _inParams.getParam(paramIndex, param_out);
    }
    bool getOutParam(int paramIndex, std::shared_ptr<ParamBase>& param_out) {
        return _outParams.getParam(paramIndex, param_out);
    }
    /// Return the function type signature, of the form "baz(int32,uint256)"
    std::string getType() const {
        return name + _inParams.getType();
    }

    /// Return the 4-byte function signature
    Data getSignature() const;

    virtual void encode(Data& data) const;

    bool decode(const Data& encoded, size_t& offset_inout);
};

inline void encode(const Function& func, Data& data) {
    func.encode(data);
}

} // namespace TW::Ethereum::ABI

/// Wrapper for C interface.
struct TWEthereumAbiFunction {
    TW::Ethereum::ABI::Function impl;
};
