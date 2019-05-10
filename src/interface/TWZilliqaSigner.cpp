// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWZilliqaSigner.h>

#include "../Zilliqa/Signer.h"
#include "../proto/Zilliqa.pb.h"
#include "../uint256.h"

using namespace TW;
using namespace TW::Zilliqa;

TW_Zilliqa_Proto_SigningOutput TWZilliqaSignerSign(TW_Zilliqa_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto preImage = Signer::getPreImage(input);
    auto output = Signer::sign(preImage, key);
    
    auto serialized = output.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
