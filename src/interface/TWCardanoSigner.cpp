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
    try {
        if (!input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)))) {
            // TODO set error
        } else {
            output = Signer::sign(std::move(input));
        }
    } catch (...) {
        // TODO set error
    }
    auto serialized = output.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serialized.data()), serialized.size());
}
