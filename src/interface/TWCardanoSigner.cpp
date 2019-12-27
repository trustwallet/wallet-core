// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWCardanoSigner.h>

#include "../Cardano/Signer.h"
#include "../proto/Cardano.pb.h"

using namespace TW;
using namespace TW::Cardano;

TW_Cardano_Proto_SigningOutput TWCardanoSignerSign(TW_Cardano_Proto_SigningInput data) {
    Proto::SigningInput input;
    Proto::SigningOutput output;
    if (!input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)))) {
        // failed to parse input, return empty output with error
        output.set_error("Error: could not parse input");
    } else {
        output = Signer::sign(std::move(input));
    }
    auto serialized = output.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serialized.data()), serialized.size());
}

uint64_t TWCardanoSignerComputeFee(TW_Cardano_Proto_SigningInput data) {
    Proto::SigningInput input;
    if (!input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)))) {
        // failed to parse input, return 0
        return 0;
    }
    uint64_t fee = Signer::computeFee(std::move(input));
    return fee;
}
