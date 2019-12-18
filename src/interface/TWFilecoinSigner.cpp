// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWFilecoinSigner.h>

#include "../Filecoin/Signer.h"
#include "../Filecoin/Transaction.h"
#include "../PrivateKey.h"

using namespace TW;
using namespace TW::Filecoin;

TW_Filecoin_Proto_SigningOutput TWFilecoinSignerSign(TW_Filecoin_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto protoOutput = Signer::sign(input);

    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serialized.data()),
                                 serialized.size());
}
