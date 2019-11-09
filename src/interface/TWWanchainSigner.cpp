// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWWanchainSigner.h>

#include "../Wanchain/Signer.h"
#include "../proto/Ethereum.pb.h"
#include "../uint256.h"

using namespace TW;
using namespace TW::Wanchain;

TW_Ethereum_Proto_SigningOutput TWWanchainSignerSign(TW_Ethereum_Proto_SigningInput data) {
    Ethereum::Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto signer = Signer(load(input.chain_id()));
    auto protoOutput = signer.sign(input);

    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
