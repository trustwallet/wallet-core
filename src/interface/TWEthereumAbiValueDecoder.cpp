// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWEthereumAbiValueDecoder.h>

#include <Ethereum/ABI/ValueDecoder.h>
#include <Data.h>

using namespace TW::Ethereum;
using namespace TW;

TWString* _Nonnull TWEthereumAbiValueDecoderDecodeUInt256(TWData* _Nonnull input) {
    auto data = TW::data(TWDataBytes(input), TWDataSize(input));
    auto decoded = Ethereum::ABI::ValueDecoder::decodeUInt256(data);
    return TWStringCreateWithUTF8Bytes(TW::toString(decoded).c_str());
}
