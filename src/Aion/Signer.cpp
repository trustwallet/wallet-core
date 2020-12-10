// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"

#include "../Ethereum/Transaction.h"
#include "../Hash.h"
#include "../uint256.h"
#include <boost/multiprecision/cpp_int.hpp>

using namespace TW;
using namespace TW::Aion;
using boost::multiprecision::uint128_t;

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    try {
        auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
        auto transaction = build(input);
        Signer::sign(key, transaction);
        auto output = Proto::SigningOutput();
        auto encoded = transaction.encode();
        output.set_encoded(encoded.data(), encoded.size());
        output.set_signature(transaction.signature.data(), transaction.signature.size());
        return output;
    } catch (std::exception&) {
        return Proto::SigningOutput();
    }
}

Transaction Signer::build(const Proto::SigningInput& input) {
    uint128_t amount;
    Data payload;

    switch (input.payload().payload_oneof_case()) {
        case Proto::Payload::kPayloadTransfer:
            amount = static_cast<uint128_t>(load(input.payload().payload_transfer().amount()));
            payload = Data(input.payload().payload_transfer().payload().begin(), input.payload().payload_transfer().payload().end());
            break;

        case Proto::Payload::kPayloadContractGeneric:
        default:
            amount = static_cast<uint128_t>(0);
            payload = Data(input.payload().payload_contract_generic().payload().begin(), input.payload().payload_contract_generic().payload().end());
            break;
    }

    auto transaction = Transaction(
        /* nonce: */ static_cast<uint128_t>(load(input.nonce())),
        /* gasPrice: */ static_cast<uint128_t>(load(input.gas_price())),
        /* gasLimit: */ static_cast<uint128_t>(load(input.gas_limit())),
        /* to: */ Address(input.to_address()),
        /* amount: */ amount,
        /* timestamp */ static_cast<uint128_t>(input.timestamp()),
        /* payload: */ payload);

    return transaction;
}

void Signer::sign(const PrivateKey& privateKey, Transaction& transaction) noexcept {
    auto encoded = transaction.encode();
    auto hashData = Hash::blake2b(encoded, 32);
    auto hashSignature = privateKey.sign(hashData, TWCurveED25519);
    auto publicKeyData = privateKey.getPublicKey(TWPublicKeyTypeED25519).bytes;

    // Aion signature = pubKeyBytes + signatureBytes
    Data result(publicKeyData.begin(), publicKeyData.end());
    result.insert(result.end(), hashSignature.begin(), hashSignature.end());

    transaction.signature = result;
}
