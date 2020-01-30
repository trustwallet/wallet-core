// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWNEOSigner.h>

#include "../NEO/Signer.h"
#include "../proto/NEO.pb.h"

using namespace TW;
using namespace TW::NEO;

TW_NEO_Proto_SigningOutput TWNEOSignerSign(TW_NEO_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto protoOutput = Signer::sign(input);
    auto serialized = protoOutput.SerializeAsString();

    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
