// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "uint256.h"
#include "proto/Aion.pb.h"
#include <TrustWalletCore/TWAnySigner.h>

#include "../interface/TWTestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Aion;

TEST(TWAnySignerAion, SignTransfer) {
    auto privateKey = parse_hex("db33ffdf82c7ba903daf68d961d3c23c20471a8ce6b408e52d579fd8add80cc9");
    
    Proto::SigningInput input;
    input.set_to_address("0xa082c3de528b7807dc27ad66debb16d4cfe4054209398cee619dd95955063d1e");
    auto amount = store(uint256_t(10000));
    input.mutable_payload()->mutable_payload_transfer()->set_amount(amount.data(), amount.size());
    auto gasPrice = store(uint256_t(20000000000));
    input.set_gas_price(gasPrice.data(), gasPrice.size());
    auto gasLimit = store(uint256_t(21000));
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    auto nonce = store(uint256_t(9));
    input.set_nonce(nonce.data(), nonce.size());
    input.set_timestamp(155157377101);
    input.set_private_key(privateKey.data(), privateKey.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeAion);

    ASSERT_EQ(hex(output.encoded()), "f89b09a0a082c3de528b7807dc27ad66debb16d4cfe4054209398cee619dd95955063d1e8227108085242019b04d8252088800000004a817c80001b860a775daa30b33fda3091768f0561c8042ee23cb48a6a3e5d7e8248b13d04a48a7d3d3386742c2716031b79950cef5fcb49c079a5cab095c8b08915e126b9741389924ba2d5c00036a3b39c2a8562fa0800f1a13a566ce6e027274ce63a41dec07");
}

TEST(TWAnySignerAion, SignContractCall) {
    auto privateKey = parse_hex("db33ffdf82c7ba903daf68d961d3c23c20471a8ce6b408e52d579fd8add80cc9");
    
    Proto::SigningInput input;
    input.set_to_address("0xa082c3de528b7807dc27ad66debb16d4cfe4054209398cee619dd95955063d1e");
    auto amount = store(uint256_t(10000));
    input.mutable_payload()->mutable_payload_contract_generic()->set_payload("Hello World");
    auto gasPrice = store(uint256_t(20000000000));
    input.set_gas_price(gasPrice.data(), gasPrice.size());
    auto gasLimit = store(uint256_t(21000));
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    auto nonce = store(uint256_t(9));
    input.set_nonce(nonce.data(), nonce.size());
    input.set_timestamp(155157377101);
    input.set_private_key(privateKey.data(), privateKey.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeAion);

    ASSERT_EQ(hex(output.encoded()), "f8a409a0a082c3de528b7807dc27ad66debb16d4cfe4054209398cee619dd95955063d1e808b48656c6c6f20576f726c6485242019b04d8252088800000004a817c80001b860a775daa30b33fda3091768f0561c8042ee23cb48a6a3e5d7e8248b13d04a48a7b17b6a319c9834dfd46d028187b2ff37f912c0e556e1c4eae0d5bd4d125133d20da23c0a158dd646311b3747dbc01d5d7b66dda52770f774a1c13acf7296c203");
}
