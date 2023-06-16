// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
#include "Script.h"

#include "BinaryCoding.h"
#include "OpCode.h"

namespace TW::NEO {

Data Script::CreateSignatureRedeemScript(const Data& publicKey) {
    Data result;
    result.push_back((byte)PUSHBYTES21);
    result.insert(result.end(), publicKey.begin(), publicKey.end());
    result.push_back((byte)CHECKSIG);
    return result;
}

Data Script::CreateInvocationScript(const Data& signature) {
    Data result;
    result.push_back((byte)PUSHBYTES40);
    result.insert(result.end(), signature.begin(), signature.end());
    return result;
}

Data Script::CreateNep5TransferScript(const Data& assetId, const Data& from, const Data& to,
                                      uint256_t value, bool withRet /*= false*/) {
    Data result;

    // handle value
    if (value == uint256_t(0)) {
        result.push_back(PUSH0);
    } else if (value >= uint256_t(1) && value <= uint256_t(16)) {
        result.push_back(PUSH1 - 1 + (byte)value);
    } else {
        Data v;
        encode256LE(v, value);
        result.push_back((byte)v.size());
        result.insert(result.end(), v.begin(), v.end());
    }

    encodeBytes(result, to);
    encodeBytes(result, from);

    // args length
    result.push_back(PUSH3);

    result.push_back(PACK);

    std::string operation = "transfer";
    encodeBytes(result, {operation.begin(), operation.end()});

    result.push_back(APPCALL);
    result.insert(result.end(), assetId.begin(), assetId.end());

    if (withRet) {
        result.push_back(THROWIFNOT);
        result.push_back(RET);
    }
    return result;
}

} // namespace TW::NEO
