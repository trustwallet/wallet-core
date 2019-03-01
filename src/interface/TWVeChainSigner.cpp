// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWVeChainSigner.h>

#include "../VeChain/Signer.h"
#include "../proto/VeChain.pb.h"
#include "../uint256.h"

using namespace TW;
using namespace TW::VeChain;

TW_VeChain_Proto_SigningOutput TWVeChainSignerSign(TW_VeChain_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), TWDataSize(data));

    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto transaction = Transaction();
    transaction.chainTag = input.chain_tag();
    transaction.blockRef = input.block_ref();
    transaction.expiration = input.expiration();
    for (auto& clause : input.clauses()) {
        transaction.clauses.push_back(clause);
    }
    transaction.gasPriceCoef = input.gas_price_coef();
    transaction.gas = input.gas();
    transaction.dependsOn = Data(input.depends_on().begin(), input.depends_on().end());
    transaction.nonce = input.nonce();
    transaction.signature = Signer::sign(key, transaction);

    auto protoOutput = Proto::SigningOutput();

    auto encoded = transaction.encode();
    protoOutput.set_encoded(encoded.data(), encoded.size());
    protoOutput.set_signature(transaction.signature.data(), transaction.signature.size());
    
    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}
