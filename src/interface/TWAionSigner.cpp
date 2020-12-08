// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAionSigner.h>
#include <TrustWalletCore/TWAionAddress.h>

#include "../Aion/Signer.h"
#include "../proto/Aion.pb.h"
#include "../uint256.h"
#include "../HexCoding.h"
#include <boost/multiprecision/cpp_int.hpp>

using namespace TW;
using namespace TW::Aion;

TW_Aion_Proto_SigningOutput TWAionSignerSign(TW_Aion_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    using boost::multiprecision::uint128_t;

    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));

    auto transaction = Transaction(
        /* nonce: */ static_cast<uint128_t>(load(input.nonce())),
        /* gasPrice: */ static_cast<uint128_t>(load(input.gas_price())),
        /* gasLimit: */ static_cast<uint128_t>(load(input.gas_limit())),
        /* to: */ Address(input.to_address()),
        /* amount: */ static_cast<uint128_t>(load(input.amount())),
        /* payload: */ Data(input.payload().begin(), input.payload().end())
    );
    Signer::sign(key, transaction);

    auto protoOutput = Proto::SigningOutput();
    auto encoded = transaction.encode();
    protoOutput.set_encoded(encoded.data(), encoded.size());
    protoOutput.set_signature(transaction.signature.data(), transaction.signature.size());

    auto serialized = protoOutput.SerializeAsString();
    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(serialized.data()), serialized.size());
}

TWData *_Nonnull TWAionSignerMessage(TW_Aion_Proto_SigningInput data) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    using boost::multiprecision::uint128_t;

    auto transaction = Transaction(
            /* nonce: */ static_cast<uint128_t>(load(input.nonce())),
            /* gasPrice: */ static_cast<uint128_t>(load(input.gas_price())),
            /* gasLimit: */ static_cast<uint128_t>(load(input.gas_limit())),
            /* to: */ Address(input.to_address()),
            /* amount: */ static_cast<uint128_t>(load(input.amount())),
            /* payload: */ Data(input.payload().begin(), input.payload().end())
    );

    auto encoded = transaction.encode();
    auto hashData = Hash::blake2b(encoded, 32);

    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(hashData.data()), hashData.size());
}

TWData *_Nonnull TWAionSignerTransaction(TW_Aion_Proto_SigningInput data, TWData *_Nonnull pubKey, TWData *_Nonnull signature) {
    Proto::SigningInput input;
    input.ParseFromArray(TWDataBytes(data), static_cast<int>(TWDataSize(data)));

    using boost::multiprecision::uint128_t;

    auto transaction = Transaction(
            /* nonce: */ static_cast<uint128_t>(load(input.nonce())),
            /* gasPrice: */ static_cast<uint128_t>(load(input.gas_price())),
            /* gasLimit: */ static_cast<uint128_t>(load(input.gas_limit())),
            /* to: */ Address(input.to_address()),
            /* amount: */ static_cast<uint128_t>(load(input.amount())),
            /* payload: */ Data(input.payload().begin(), input.payload().end())
    );

    auto key = PrivateKey(Data(input.private_key().begin(), input.private_key().end()));
    auto pubkey = key.getPublicKey(TWPublicKeyTypeED25519);
    auto twPubKey = TWPublicKey{pubkey};
    auto address = TWAionAddressCreateWithPublicKey(&twPubKey);

    Signer::sign(key, transaction);
    auto encoded = transaction.encode();
    auto hexEncoded = hex(encoded.begin(), encoded.end());

    return TWDataCreateWithBytes(reinterpret_cast<const uint8_t *>(encoded.data()), encoded.size());
}