// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWIoTeXSigner.h>

#include "../IoTeX/Signer.h"
#include "../proto/Common.pb.h"
#include "../proto/IoTeX.pb.h"

using namespace TW::IoTeX;

TW_Proto_Result TWIoTeXSignerSign(TW_IoTeX_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto signer = new TWIoTeXSigner{ Signer(std::move(input)) };
    Proto::SigningOutput output = signer->impl.build();

    auto serialized = output.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
