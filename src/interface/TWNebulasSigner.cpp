// Copyright © 2019 Mart Roosmaa.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../proto/Nebulas.pb.h"
#include "../Nebulas/Signer.h"
#include "../Nebulas/Address.h"

#include <TrustWalletCore/TWNebulasSigner.h>

using namespace TW;
using namespace TW::Nebulas;

TW_Nebulas_Proto_SigningOutput TWNebulasSignerSign(TW_Nebulas_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    auto signer = Signer(load(input.chain_id()));
    auto protoOutput = signer.sign(input);
    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
