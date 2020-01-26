// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWZilliqaSigner.h>

#include "../Zilliqa/Signer.h"
#include "../proto/Zilliqa.pb.h"

using namespace TW;
using namespace TW::Zilliqa;

TW_Zilliqa_Proto_SigningOutput TWZilliqaSignerSign(TW_Zilliqa_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));
    auto output = Signer::sign(input);
    auto serialized = output.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
