// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"

#include "../Ethereum/RLP.h"
#include "../Hash.h"

using namespace TW;
using namespace TW::Theta;
using RLP = Ethereum::RLP;

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
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

    auto output = Proto::SigningOutput();

    transaction.setSignature(from, signature);
    auto encoded = transaction.encode();
    output.set_encoded(encoded.data(), encoded.size());
    output.set_signature(signature.data(), signature.size());
    return output;
}

Data Signer::encode(const Transaction& transaction) noexcept {
    const uint64_t nonce = 0;
    const uint256_t gasPrice = 0;
    const uint64_t gasLimit = 0;
    const Ethereum::Address to = Ethereum::Address("0x0000000000000000000000000000000000000000");
    const uint256_t amount = 0;

    auto encoded = Data();
    /// Need to add the following prefix to the tx signbytes to be compatible with
    /// the Ethereum tx format
    append(encoded, RLP::encode(nonce));
    append(encoded, RLP::encode(gasPrice));
    append(encoded, RLP::encode(gasLimit));
    append(encoded, RLP::encode(to.bytes));
    append(encoded, RLP::encode(amount));
    /// Chain ID
    auto payload = Data();
    append(payload, RLP::encode(chainID));
    append(payload, transaction.encode());
    append(encoded, RLP::encode(payload));
    return RLP::encodeList(encoded);
}

Data Signer::sign(const PrivateKey& privateKey, const Transaction& transaction) noexcept {
    auto encoded = encode(transaction);
    auto hash = Hash::keccak256(encoded);
    auto signature = privateKey.sign(hash, TWCurveSECP256k1);
    return signature;
}
