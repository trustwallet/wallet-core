// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWStellarSigner.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <PrivateKey.h>

#include "../Stellar/Signer.h"
#include "../Stellar/Address.h"
#include "../proto/Stellar.pb.h"

using namespace TW;
using namespace TW::Stellar;

TW_Stellar_Proto_SigningOutput TWStellarSignerSign(TW_Stellar_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), TWDataSize(data));

    const auto signer = Signer(input);
    const auto output = signer.sign();

    auto protoOutput = Proto::SigningOutput();
    protoOutput.set_signature(output);

    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
