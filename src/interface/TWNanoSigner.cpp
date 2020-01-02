// Copyright © 2019 Mart Roosmaa.
// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../Nano/Signer.h"
#include "../proto/Nano.pb.h"

#include <TrustWalletCore/TWNanoSigner.h>

using namespace TW;
using namespace TW::Nano;

TW_Nano_Proto_SigningOutput TWNanoSignerSign(TW_Nano_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    auto output = Proto::SigningOutput();
    try {
        const auto signer = Signer(input);
        const auto signature = signer.sign();

        output.set_block_hash(signer.blockHash.data(), signer.blockHash.size());
        output.set_signature(reinterpret_cast<const char *>(signature.data()), signature.size());
    }
    catch (...) {
    }
    auto serialized = output.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()),
                                 serialized.size());
}
