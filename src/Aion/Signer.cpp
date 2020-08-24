// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Signer.h"

#include "../Hash.h"
#include "../uint256.h"
#include <boost/multiprecision/cpp_int.hpp>

using namespace TW;
using namespace TW::Aion;

Proto::SigningOutput Signer::sign(const Proto::SigningInput& input) noexcept {
    using boost::multiprecision::uint128_t;

    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto transaction = Transaction(
        /* nonce: */ static_cast<uint128_t>(load(input.nonce())),
        /* gasPrice: */ static_cast<uint128_t>(load(input.gas_price())),
        /* gasLimit: */ static_cast<uint128_t>(load(input.gas_limit())),
        /* to: */ Address(input.to_address()),
        /* amount: */ static_cast<uint128_t>(load(input.amount())),
        /* timestamp */ static_cast<uint128_t>(input.timestamp()),
        /* payload: */ Data(input.payload().begin(), input.payload().end()));
    Signer::sign(key, transaction);

    auto output = Proto::SigningOutput();
    auto encoded = transaction.encode();
    output.set_encoded(encoded.data(), encoded.size());
    output.set_signature(transaction.signature.data(), transaction.signature.size());
    return output;
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
