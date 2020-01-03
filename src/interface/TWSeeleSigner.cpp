// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWSeeleSigner.h>

#include "../Seele/Signer.h"
#include "../proto/Seele.pb.h"

using namespace TW;
using namespace TW::Seele;

TW_Seele_Proto_SigningOutput TWSeeleSignerSign(TW_Seele_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto signer = new TWSeeleSigner{ Signer(std::move(input)) };
    Proto::SigningOutput output = signer->impl.build();

    auto serialized = output.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
