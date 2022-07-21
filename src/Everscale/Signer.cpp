// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"
#include "Address.h"
#include "../PublicKey.h"

using namespace TW;
using namespace TW::Everscale;


Proto::SigningOutput Signer::sign(const Proto::SigningInput &input) noexcept {
    auto protoOutput = Proto::SigningOutput();

    // auto privateKey = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    // auto signature = privateKey.sign(payload, TWCurveED25519);

    Data message;
    int32_t wc = 0;

    protoOutput.set_message(message.data(), message.size());
    protoOutput.set_to(wc);

    return protoOutput;
}
