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
using namespace std;

TW_Ethereum_Proto_SigningOutput TWEthereumSignerSign(TW_Ethereum_Proto_SigningInput data) {
    try {
        Proto::SigningInput input;
        input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

        auto signer = Signer(load(input.chain_id()));
        auto protoOutput = signer.sign(input);

        auto serialized = protoOutput.SerializeAsString();
        return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
    } catch (const std::exception& e) {
        std::cerr<<e.what()<<std::endl;
    }

    return nullptr;
}

TWData *_Nonnull TWEthereumSignerMessage(TW_Ethereum_Proto_SigningInput data) {
    try {
        Proto::SigningInput input;
        input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

        auto signer = Signer(load(input.chain_id()));
        auto transaction = Signer::build(input);

        auto serialized = signer.serialize(transaction);
        return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
    } catch (const std::exception& e) {
        std::cerr<<e.what()<<std::endl;
    }

    return nullptr;
}

TWData *_Nonnull TWEthereumSignerTransaction(TW_Ethereum_Proto_SigningInput data, TWData *_Nonnull signature) {
    try {
        Proto::SigningInput input;
        input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

        auto chainID = load(input.chain_id());
        auto signer = Signer(chainID);
        Data sig(TWDataBytes(signature), TWDataBytes(signature) + TWDataSize(signature));

        auto transaction = Signer::build(input);
        auto sigObj = signer.values(chainID, sig, transaction->usesReplayProtection());

        auto encoded = transaction->encoded(sigObj, chainID);
        return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(encoded.data()), encoded.size());
    } catch (const std::exception& e) {
        std::cerr<<e.what()<<std::endl;
    }

    return nullptr;
}