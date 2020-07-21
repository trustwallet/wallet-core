// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ParamFactory.h"
#include "HexCoding.h"

using namespace std;

namespace TW::Ethereum::ABI {

std::shared_ptr<ParamBase> ParamFactory::make(const std::string& type) {
    shared_ptr<ParamBase> param;
    if (type == "address") {
        param = make_shared<ParamAddress>();
    } else if (type == "uint256") {
        param = make_shared<ParamUInt256>();
    } else if (type == "bool") {
        param = make_shared<ParamBool>();
    } else if (type == "bytes") {
        param =  make_shared<ParamByteArray>();
    } else if (type == "string") {
        param = make_shared<ParamString>();
    }
    return param;
}

std::string ParamFactory::getValue(const std::shared_ptr<ParamBase>& param, const std::string& type) {
    std::string result = "";
    if (type == "address") {
        auto value = dynamic_pointer_cast<ParamAddress>(param);
        result = hexEncoded(value->getData());
    } else if (type == "uint256") {
        auto value = dynamic_pointer_cast<ParamUInt256>(param);
        result = toString(value->getVal());
    } else if (type == "bool") {
        auto value = dynamic_pointer_cast<ParamBool>(param);
        result = value->getVal() ? "true" : "false";
    } else if (type == "bytes") {
        auto value = dynamic_pointer_cast<ParamByteArray>(param);
        result = hexEncoded(value->getVal());
    }  else if (type == "string") {
        auto value = dynamic_pointer_cast<ParamString>(param);
        result = value->getVal();
    }
    return result;
}

} // namespace TW::Ethereum::ABI
