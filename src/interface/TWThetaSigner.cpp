// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWThetaSigner.h>

#include "../Ethereum/Address.h"
#include "../Theta/Signer.h"
#include "../proto/Theta.pb.h"
#include "../uint256.h"

using namespace TW;
using namespace TW::Theta;

TW_Theta_Proto_SigningOutput TWThetaSignerSign(TW_Theta_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto pkFrom = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto from = Ethereum::Address(pkFrom.getPublicKey(TWPublicKeyTypeSECP256k1Extended));

    auto transaction = Transaction(
        /* from: */ from,
        /* to: */ Ethereum::Address(input.to_address()),
        /* thetaAmount: */ load(input.theta_amount()),
        /* tfuelAmount: */ load(input.tfuel_amount()),
        /* sequence: */ input.sequence(),
        /* feeAmount: */ load(input.fee()));

    auto signer = Signer(input.chain_id());
    auto signature = signer.sign(pkFrom, transaction);

    auto protoOutput = Proto::SigningOutput();

    transaction.setSignature(from, signature);
    auto encoded = transaction.encode();
    protoOutput.set_encoded(encoded.data(), encoded.size());
    protoOutput.set_signature(signature.data(), signature.size());

    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t*>(serialized.data()),
                                 serialized.size());
}
