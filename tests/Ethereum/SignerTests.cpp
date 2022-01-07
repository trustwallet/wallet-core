// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Ethereum/Address.h"
#include "Ethereum/RLP.h"
#include "Ethereum/Signer.h"
#include "Ethereum/Transaction.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

namespace TW::Ethereum {

using boost::multiprecision::uint256_t;

TEST(EthereumSigner, preHash) {
    const auto address = parse_hex("0x3535353535353535353535353535353535353535");
    const auto transaction = TransactionNonTyped::buildNativeTransfer(
        /* nonce: */ 9,
        /* gasPrice: */ 20000000000,
        /* gasLimit: */ 21000,
        /* to: */ address,
        /* amount: */ 1000000000000000000
    );
    const auto preHash = transaction->preHash(1);

    ASSERT_EQ(hex(preHash), "daf5a779ae972f972197303d7b574746c7ef83eadac0f2791ad23db92e4c8e53");
}

TEST(EthereumSigner, Sign) {
    const auto address = parse_hex("0x3535353535353535353535353535353535353535");
    const auto transaction = TransactionNonTyped::buildNativeTransfer(
        /* nonce: */ 9,
        /* gasPrice: */ 20000000000,
        /* gasLimit: */ 21000,
        /* to: */ address,
        /* amount: */ 1000000000000000000
    );

    const auto key = PrivateKey(parse_hex("0x4646464646464646464646464646464646464646464646464646464646464646"));
    auto signer = Signer(1);
    const auto signature = signer.sign(key, transaction);

    ASSERT_EQ(signature.v, 37);
    ASSERT_EQ(signature.r, uint256_t("18515461264373351373200002665853028612451056578545711640558177340181847433846"));
    ASSERT_EQ(signature.s, uint256_t("46948507304638947509940763649030358759909902576025900602547168820602576006531"));
}

TEST(EthereumSigner, EIP1559_1442) {
    const auto transaction = TransactionEip1559::buildNativeTransfer(
        6, // nonce
        2000000000, // maxInclusionFeePerGas
        3000000000, // maxFeePerGas
        21100, // gasLimit
        parse_hex("B9F5771C27664bF2282D98E09D7F50cEc7cB01a7"),
        543210987654321, // amount
        Data() // data
    );

    const uint256_t chainID = 3;
    const auto key = PrivateKey(parse_hex("4f96ed80e9a7555a6f74b3d658afdd9c756b0a40d4ca30c42c2039eb449bb904"));
    auto signer = Signer(chainID);
    const auto signature = signer.sign(key, transaction);
    EXPECT_EQ(signature.v, 0);
    EXPECT_EQ(hex(store(signature.r)), "92c336138f7d0231fe9422bb30ee9ef10bf222761fe9e04442e3a11e88880c64");
    EXPECT_EQ(hex(store(signature.s)), "6487026011dae03dc281bc21c7d7ede5c2226d197befb813a4ecad686b559e58");

    const auto encoded = transaction->encoded(signature, chainID);
    // https://ropsten.etherscan.io/tx/0x14429509307efebfdaa05227d84c147450d168c68539351fbc01ed87c916ab2e
    EXPECT_EQ(hex(encoded), "02f8710306847735940084b2d05e0082526c94b9f5771c27664bf2282d98e09d7f50cec7cb01a78701ee0c29f50cb180c080a092c336138f7d0231fe9422bb30ee9ef10bf222761fe9e04442e3a11e88880c64a06487026011dae03dc281bc21c7d7ede5c2226d197befb813a4ecad686b559e58");
}

} // namespace TW::Ethereum
