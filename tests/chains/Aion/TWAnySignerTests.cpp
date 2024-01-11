// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "uint256.h"
#include "proto/Aion.pb.h"
#include <TrustWalletCore/TWAnySigner.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Aion::tests {

TEST(TWAnySignerAion, Sign) {
    auto privateKey = parse_hex("db33ffdf82c7ba903daf68d961d3c23c20471a8ce6b408e52d579fd8add80cc9");

    Proto::SigningInput input;
    input.set_to_address("0xa082c3de528b7807dc27ad66debb16d4cfe4054209398cee619dd95955063d1e");
    auto amount = store(uint256_t(10000));
    input.set_amount(amount.data(), amount.size());
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

} // namespace TW::Aion::tests
