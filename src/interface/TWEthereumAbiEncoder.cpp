// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWEthereumAbiEncoder.h>
#include <TrustWalletCore/TWEthereumAbiFunction.h>

#include "Ethereum/ABI.h"
#include "Data.h"
#include "HexCoding.h"
#include "../uint256.h"

#include <string>
#include <vector>
#include <cassert>

using namespace TW;
using namespace TW::Ethereum;
using namespace TW::Ethereum::ABI;

/// Wrapper for C interface, empty as all methods are static
struct TWEthereumAbiEncoder {
    //TW::Ethereum::ABI::Encoder impl;
};

struct TWEthereumAbiFunction *_Nullable TWEthereumAbiEncoderBuildFunction(TWString *_Nonnull name) {
    auto func = Function(TWStringUTF8Bytes(name));
    return new TWEthereumAbiFunction{ func };
}

void TWEthereumAbiEncoderDeleteFunction(struct TWEthereumAbiFunction *_Nonnull func) {
    assert(func != nullptr);
    delete func;
}

///// Encode

TWData*_Nonnull TWEthereumAbiEncoderEncode(struct TWEthereumAbiFunction *_Nonnull func) {
    assert(func != nullptr);
    Function& function = func->impl;

    Data encoded;
    function.encode(encoded);
    return TWDataCreateWithData(&encoded);
}
