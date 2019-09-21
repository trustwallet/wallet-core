// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWNEARSigner.h>

#include "../NEAR/Signer.h"
#include "../proto/NEAR.pb.h"

using namespace TW;
using namespace TW::NEAR;

TW_NEAR_Proto_SigningOutput TWNEARSignerSign(TW_NEAR_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    Proto::SigningOutput output = Signer::sign(std::move(input));

    auto serialized = output.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
