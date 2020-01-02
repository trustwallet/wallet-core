// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWIconSigner.h>

#include "../Icon/Signer.h"
#include "../proto/Icon.pb.h"
#include "../uint256.h"

using namespace TW;
using namespace TW::Icon;

TW_Icon_Proto_SigningOutput TWIconSignerSign(TW_Icon_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    const auto signer = Signer(input);
    const auto output = signer.sign();

    auto serialized = output.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
