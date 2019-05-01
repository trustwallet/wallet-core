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

static TW_Proto_Result createErrorResult(const std::string& description) {
    auto result = TW::Proto::Result();
    result.set_success(false);
    result.set_error(description);
    auto serialized = result.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}

TW_Proto_Result TWIoTeXSignerSign(TW_IoTeX_Proto_SigningInput data) {
    Proto::SigningInput input;
    if (!input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)))) {
        return createErrorResult("IoTeX: invalid input, please check IoTeX.proto file for correct msg format");
    }

    try {
        auto signer = new TWIoTeXSigner{ Signer(std::move(input)) };
        Proto::SigningOutput output = signer->impl.build();

        auto result = TW::Proto::Result();
        result.set_success(true);
        result.add_objects()->PackFrom(output);
        auto serialized = result.SerializeAsString();
        return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
    } catch (const std::exception& e) {
        return createErrorResult(e.what());
    }
}
