// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Function.h"

#include <string>

namespace TW::Ethereum::ABI {

Data Function::getSignature() const {
    auto typ = getType();
    auto hash = Hash::keccak256(Data(typ.begin(), typ.end()));
    auto signature = Data(hash.begin(), hash.begin() + 4);
    return signature;
}

void Function::encode(Data& data) const {
    Data signature = getSignature();
    append(data, signature);
    _inParams.encode(data);
}

bool Function::decodeOutput(const Data& encoded, size_t& offset_inout) {
    // read parameter values
    if (!_outParams.decode(encoded, offset_inout)) {
        return false;
    }
    return true;
}

bool Function::decodeInput(const Data& encoded, size_t& offset_inout) {
    // read 4-byte hash
    auto p = ParamByteArrayFix(4);
    if (!p.decode(encoded, offset_inout)) {
        return false;
    }
    std::vector<byte> hash = p.getVal();
    // adjust offset; hash is NOT padded to 32 bytes
    offset_inout = offset_inout - 32 + 4;
    // verify hash
    Data hashExpect = getSignature();
    if (hash != hashExpect) {
        // invalid hash
        return false;
    }
    // read parameters
    if (!_inParams.decode(encoded, offset_inout)) {
        return false;
    }
    return true;
}

} // namespace TW::Ethereum::ABI
