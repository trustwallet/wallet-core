// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWIconSigner.h>

#include "../Icon/Signer.h"
#include "../proto/Icon.pb.h"
#include "../uint256.h"
#include "../Hash.h"

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

TWString *_Nonnull TWIconSignerMessage(TW_Icon_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    const auto signer = Signer(input);
    const auto preImage = signer.preImage();

    return TWStringCreateWithUTF8Bytes(preImage.c_str());
}

TWData *_Nonnull TWIconSignerTransaction(TW_Icon_Proto_SigningInput data, TWData *_Nonnull signature) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    Data sig(TWDataBytes(signature), TWDataBytes(signature) + TWDataSize(signature));

    const auto signer = Signer(input);

    auto output = Proto::SigningOutput();
    output.set_signature(sig.data(), sig.size());

    auto encoded = signer.encode(Data(sig.begin(), sig.end()));
    output.set_encoded(encoded.data(), encoded.size());

    auto serialized = output.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}