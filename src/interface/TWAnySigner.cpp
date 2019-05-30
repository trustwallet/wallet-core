// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnySigner.h>

#include "AnySigner.h"

using namespace TW;
using namespace TW::Signer;

TW_Signer_Proto_SigningOutput TWAnySignerSign(TW_Signer_Proto_SigningInput data)
{
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto signer = new TWAnySigner{ AnySigner(input) };
    Proto::SigningOutput output = signer->impl.sign();

    auto serialized = output.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
