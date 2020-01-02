// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWNULSSigner.h>

#include "../NULS/Signer.h"
#include "../PrivateKey.h"
#include "../proto/NULS.pb.h"

using namespace TW;
using namespace TW::NULS;

TW_NULS_Proto_SigningOutput TWNULSSignerSign(TW_NULS_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    auto output = Proto::SigningOutput();
    try {
        const auto signer = Signer(input);
        const auto data = signer.sign();
        output.set_encoded(data.data(), data.size());
    }
    catch(...) {}
    auto serialized = output.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serialized.data()), serialized.size());
}
