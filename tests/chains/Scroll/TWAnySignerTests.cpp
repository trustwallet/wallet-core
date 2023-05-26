// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
    auto chainId = store(uint256_t(534353));
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

    std::string expected = "f86c01830f424083030d4094a6bc5ee0b1e904dd0773c5555d2f6833fe937a6886b5e620f480008083104ec6a0a30f1e29c3d1447d7f7f76c7112af3fb86eb3cb7cc536c932e720cd9813ae274a048ffcadba06d3645829d74fa1cf28744111cfc35893761239fa6b510c84c396e";

    // sign test
    Ethereum::Proto::SigningOutput output;

    ANY_SIGN(input, TWCoinTypeScroll);

    ASSERT_EQ(hex(output.encoded()), expected);
    EXPECT_TRUE(TWAnySignerSupportsJSON(TWCoinTypeScroll));
}

} // namespace TW::Scroll::tests
