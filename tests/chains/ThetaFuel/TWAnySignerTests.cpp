// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>
#include "HexCoding.h"
#include "uint256.h"
#include "proto/Ethereum.pb.h"

#include <gtest/gtest.h>

namespace TW::ThetaFuel::tests {

/// Successfully broadcasted:
/// https://explorer.thetatoken.org/txs/0x0e7b0642f89855bf591d094cb7648c325fcef669add66dd273c4e16170fbca01
TEST(TWAnySignerThetaFuel, TfuelTransfer) {
    auto chainId = store(uint256_t(361));
    auto nonce = store(uint256_t(5));
    auto gasLimit = store(uint256_t(79883));
    auto gasPrice = store(uint256_t(4000000000000)); // 0.000004
    auto toAddress = "0x8dbD6c7Ede90646a61Bbc649831b7c298BFd37A0";
    auto amount = uint256_t(100000000000000000); // 0.1
    auto amountData = store(amount);
    auto key = parse_hex("0xc99dd0045dff0c1594c383658c07b4b75f39b90af7f8b592d1a7b461e03cc34b");

    Ethereum::Proto::SigningInput input;
    input.set_chain_id(chainId.data(), chainId.size());
    input.set_nonce(nonce.data(), nonce.size());
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_gas_price(gasPrice.data(), gasPrice.size());
    input.set_to_address(toAddress);
    input.set_private_key(key.data(), key.size());
    auto &transfer = *input.mutable_transaction()->mutable_transfer();
    transfer.set_amount(amountData.data(), amountData.size());

    // sign test
    Ethereum::Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeThetaFuel);

    ASSERT_EQ(hex(output.encoded()),
              "f870058603a3529440008301380b948dbd6c7ede90646a61bbc649831b7c298bfd37a088016345785d8a0000808202f5a0b1857121d66a484798ad0cd0fed0e205ee2e1f7f7f60b45cf84a2dbeb25c8c9fa06ffedd5df33a38f7de958c2800482432b6a8546913fc145f2615cc93f7a7647d");
}

/// Successfully broadcasted:
/// https://explorer.thetatoken.org/txs/0x2c38163d84f031d4276dedc4e4424a6443208f7b22e1bfe6fd2ba0f607af5100
TEST(TWAnySignerThetaFuel, TdropTokenTransfer) {
    auto chainId = store(uint256_t(361));
    auto nonce = store(uint256_t(4));
    auto gasLimit = store(uint256_t(79883));
    auto gasPrice = store(uint256_t(4000000000000)); // 0.000004
    auto toAddress = "0x8dbD6c7Ede90646a61Bbc649831b7c298BFd37A0";
    auto token = "0x1336739b05c7ab8a526d40dcc0d04a826b5f8b03"; // TDROP
    auto amount = uint256_t(4000000000000000000); // 4 TDROP
    auto amountData = store(amount);
    auto key = parse_hex("0xc99dd0045dff0c1594c383658c07b4b75f39b90af7f8b592d1a7b461e03cc34b");

    Ethereum::Proto::SigningInput input;
    input.set_chain_id(chainId.data(), chainId.size());
    input.set_nonce(nonce.data(), nonce.size());
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_gas_price(gasPrice.data(), gasPrice.size());
    input.set_to_address(token);
    input.set_private_key(key.data(), key.size());
    auto &erc20 = *input.mutable_transaction()->mutable_erc20_transfer();
    erc20.set_to(toAddress);
    erc20.set_amount(amountData.data(), amountData.size());

    // sign test
    Ethereum::Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeThetaFuel);

    ASSERT_EQ(hex(output.encoded()),
              "f8ad048603a3529440008301380b941336739b05c7ab8a526d40dcc0d04a826b5f8b0380b844a9059cbb0000000000000000000000008dbd6c7ede90646a61bbc649831b7c298bfd37a00000000000000000000000000000000000000000000000003782dace9d9000008202f6a03c1d37f5fc6adaa018c4ba41e13b9983e91500e7cfa8bc3731bb6365dd28d61ba07500748e46febcb781d6f37dad2479e1bd172479d108614c986122e1c6a4441e");
}

} // namespace TW::ThetaFuel
