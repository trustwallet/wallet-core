// Copyright © 2019 Mart Roosmaa.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../proto/Nano.pb.h"
#include "../Nano/Signer.h"

#include <TrustWalletCore/TWNanoSigner.h>

using namespace TW;
using namespace TW::Nano;

TW_Nano_Proto_SigningOutput TWNanoSignerSign(TW_Nano_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    const auto signer = Signer(input);
    const auto signature = signer.sign();

    auto protoOutput = Proto::SigningOutput();
    protoOutput.set_block_hash(signer.blockHash.data(), signer.blockHash.size());
    protoOutput.set_signature(reinterpret_cast<const char *>(signature.data()), signature.size());

    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
