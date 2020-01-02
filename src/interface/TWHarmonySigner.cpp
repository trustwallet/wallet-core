// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWHarmonySigner.h>

#include "../Harmony/Signer.h"
#include "../proto/Harmony.pb.h"
#include "../uint256.h"

using namespace TW;
using namespace TW::Harmony;

TW_Harmony_Proto_SigningOutput TWHarmonySignerSign(TW_Harmony_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    auto signer = Signer(load(input.chain_id()));
    auto protoOutput = signer.sign(input);

    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()),
                                 serialized.size());
}
