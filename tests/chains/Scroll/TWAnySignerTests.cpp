// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "proto/Ethereum.pb.h"
#include "uint256.h"
#include "TestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>

#include <gtest/gtest.h>

namespace TW::Scroll::tests {

/// Successfully broadcasted:
/// https://blockscout.scroll.io/tx/0x5a7ba291e0490079bddda54ca5592e5990d6b0eb49f8d239202941e3f63d32bc
TEST(TWAnySignerScroll, Sign) {
    Ethereum::Proto::SigningInput input;
    auto chainId = store(uint256_t(534352));
    auto nonce = store(uint256_t(1));
    auto gasPrice = store(uint256_t(1000000));
    auto gasLimit = store(uint256_t(200000));
    auto key = parse_hex("ba4c9bceece0677d2c92be11c2338652e34b10675dc4cec5546a20a314fe7a73");

    input.set_chain_id(chainId.data(), chainId.size());
    input.set_nonce(nonce.data(), nonce.size());
    input.set_gas_price(gasPrice.data(), gasPrice.size());
    input.set_gas_limit(gasLimit.data(), gasLimit.size());
    input.set_private_key(key.data(), key.size());
    input.set_to_address("0xa6BC5EE0B1e904DD0773c5555D2F6833fE937A68");

    auto& transfer = *input.mutable_transaction()->mutable_transfer();
    auto amount = store(uint256_t(200000000000000));
    transfer.set_amount(amount.data(), amount.size());

    std::string expected = "f86c01830f424083030d4094a6bc5ee0b1e904dd0773c5555d2f6833fe937a6886b5e620f480008083104ec3a0c43ee3d34f7758e05e2f54df227eb7780ad97d06e91e03ef6a3c91d4bea6e42fa07d075f20776f7f485faca6f057110fd2745a5cdd6cf121682ef7791619a03ade";

    // sign test
    Ethereum::Proto::SigningOutput output;

    ANY_SIGN(input, TWCoinTypeScroll);

    ASSERT_EQ(hex(output.encoded()), expected);
    EXPECT_TRUE(TWAnySignerSupportsJSON(TWCoinTypeScroll));
}

} // namespace TW::Scroll::tests
