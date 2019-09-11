// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWEthereumSigner.h>

#include "../Ethereum/Signer.h"
#include "../proto/Ethereum.pb.h"
#include "../uint256.h"

using namespace TW;
using namespace TW::Ethereum;

TW_Ethereum_Proto_SigningOutput TWEthereumSignerSign(TW_Ethereum_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto signer = Signer(load(input.chain_id()));
    auto protoOutput = signer.sign(input);

    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}

TWData *_Nonnull TWEthereumMessage(TW_Ethereum_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto signer = Signer(load(input.chain_id()));
    auto transaction = Signer::build(input);
    auto encoded = signer.hash(transaction);
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(encoded.data()), encoded.size());
}

TWData *_Nonnull TWEthereumTransaction(TW_Ethereum_Proto_SigningInput data, TWData *_Nonnull signature) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    auto chainId = load(input.chain_id());
    auto signer = Signer(chainId);
    Data sig(TWDataBytes(signature), TWDataBytes(signature) + TWDataSize(signature));
    auto tuple = signer.values(chainId, sig);
    auto transaction = Signer::build(input);
    transaction.r = std::get<0>(tuple);
    transaction.s = std::get<1>(tuple);
    transaction.v = std::get<2>(tuple);

    auto encoded = RLP::encode(transaction);
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(encoded.data()), encoded.size());
}