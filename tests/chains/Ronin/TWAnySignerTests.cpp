// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "proto/Ethereum.pb.h"
#include "uint256.h"
#include "TestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>

#include <gtest/gtest.h>

namespace TW::Ronin::tests {

TEST(TWAnySignerRonin, Sign) {
    // https://explorer.roninchain.com/tx/0xf13a2c4421700f8782ca73eaf16bb8baf82bcf093e23570a1ff062cdd8dbf6c3
    Ethereum::Proto::SigningInput input;
    auto chainId = store(uint256_t(2020));
    auto nonce = store(uint256_t(0));
    auto gasPrice = store(uint256_t(1000000000));
    auto gasLimit = store(uint256_t(21000));
    auto key = parse_hex("0x4646464646464646464646464646464646464646464646464646464646464646");

    input.set_chain_id(chainId.data(), chainId.size());
    input.set_nonce(nonce.data(), nonce.size());
    input.set_gas_price(gasPrice.data(), gasPrice.size());
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_private_key(key.data(), key.size());
    input.set_to_address("ronin:c36edf48e21cf395b206352a1819de658fd7f988");

    auto& transfer = *input.mutable_transaction()->mutable_transfer();
    auto amount = store(uint256_t(276447));
    transfer.set_amount(amount.data(), amount.size());

    std::string expected = "f86880843b9aca0082520894c36edf48e21cf395b206352a1819de658fd7f988830437df80820feca0442aa06b0d0465bfecf84b28e2ce614a32a1ccc12735dc03a5799517d6659d7aa004e1bf2efa30743f1b6d49dbec2671e9fb5ead1e7da15e352ca1df6fb86a8ba7";

    // sign test
    Ethereum::Proto::SigningOutput output;

    ANY_SIGN(input, TWCoinTypeRonin);

    ASSERT_EQ(hex(output.encoded()), expected);
    EXPECT_TRUE(TWAnySignerSupportsJSON(TWCoinTypeRonin));
}

} // namespace TW::Ronin::tests
