// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWTronSigner.h>

#include "../Tron/Signer.h"
#include "../proto/Tron.pb.h"
#include "../uint256.h"

using namespace TW;
using namespace TW::Tron;

TW_Tron_Proto_SigningOutput TWTronSignerSign(TW_Tron_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    const auto output = Signer::sign(input);
    auto serialized = output.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
