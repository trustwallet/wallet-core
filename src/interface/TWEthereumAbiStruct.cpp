// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWEthereumAbiStruct.h>

#include <Ethereum/ABI/ParamStruct.h>

using namespace TW::Ethereum::ABI;
using namespace TW;

TWData* _Nonnull TWEthereumAbiStructHashStruct(TWString* _Nonnull messageJson) {
    Data data;
    try {
        data = ParamStruct::hashStructJson(TWStringUTF8Bytes(messageJson));
    } catch (const std::invalid_argument& ex) {
        // return empty
    } catch (...) {
        // return empty
    }
    return TWDataCreateWithBytes(data.data(), data.size());
}
