// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Ethereum/Address.h"
#include "Ethereum/MessageSigner.h"
#include "Ethereum/RLP.h"
#include "Ethereum/Signer.h"
#include "Ethereum/Transaction.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

namespace TW::Ethereum {

using boost::multiprecision::uint256_t;

TEST(EthereumTransaction, encodeTransactionNonTyped) {
    const auto transaction = TransactionNonTyped::buildERC20Transfer(
        /* nonce: */ 0,
        /* gasPrice: */ 42000000000, // 0x09c7652400
        /* gasLimit: */ 78009, // 130B9
        /* tokenContract: */ parse_hex("0x6b175474e89094c44da98b954eedeac495271d0f"), // DAI
        /* toAddress: */ parse_hex("0x5322b34c88ed0691971bf52a7047448f0f4efc84"),
        /* amount: */ 2000000000000000000
    );
    const uint256_t dummyChain = 0x34;
    const auto dummySignature = Signature{{}, 0, 0, 0};

    const auto preHash = transaction->preHash(dummyChain);
    EXPECT_EQ(hex(preHash), "b3525019dc367d3ecac48905f9a95ff3550c25a24823db765f92cae2dec7ebfd");
    
    const auto encoded = transaction->encoded(dummySignature, dummyChain);
    EXPECT_EQ(hex(encoded), "f86a808509c7652400830130b9946b175474e89094c44da98b954eedeac495271d0f80b844a9059cbb0000000000000000000000005322b34c88ed0691971bf52a7047448f0f4efc840000000000000000000000000000000000000000000000001bc16d674ec80000808080");
}

TEST(EthereumSigner, PreHash) {
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
    const auto signature = Signer::sign(key, 1, transaction);

    ASSERT_EQ(signature.v, 37);
    ASSERT_EQ(signature.r, uint256_t("18515461264373351373200002665853028612451056578545711640558177340181847433846"));
    ASSERT_EQ(signature.s, uint256_t("46948507304638947509940763649030358759909902576025900602547168820602576006531"));
}

TEST(EthereumSigner, SignERC20Transfer) {
    const auto transaction = TransactionNonTyped::buildERC20Transfer(
        /* nonce: */ 0,
        /* gasPrice: */ 42000000000, // 0x09c7652400
        /* gasLimit: */ 78009, // 130B9
        /* tokenContract: */ parse_hex("0x6b175474e89094c44da98b954eedeac495271d0f"), // DAI
        /* toAddress: */ parse_hex("0x5322b34c88ed0691971bf52a7047448f0f4efc84"),
        /* amount: */ 2000000000000000000
    );

    const auto key = PrivateKey(parse_hex("0x608dcb1742bb3fb7aec002074e3420e4fab7d00cced79ccdac53ed5b27138151"));
    const auto signature = Signer::sign(key, 1, transaction);

    ASSERT_EQ(signature.v, 37);
    ASSERT_EQ(hex(store(signature.r)), "724c62ad4fbf47346b02de06e603e013f26f26b56fdc0be7ba3d6273401d98ce");
    ASSERT_EQ(hex(store(signature.s)), "032131cae15da7ddcda66963e8bef51ca0d9962bfef0547d3f02597a4a58c931");
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
    const auto signature = Signer::sign(key, chainID, transaction);
    EXPECT_EQ(signature.v, 0);
    EXPECT_EQ(hex(store(signature.r)), "92c336138f7d0231fe9422bb30ee9ef10bf222761fe9e04442e3a11e88880c64");
    EXPECT_EQ(hex(store(signature.s)), "6487026011dae03dc281bc21c7d7ede5c2226d197befb813a4ecad686b559e58");

    const auto encoded = transaction->encoded(signature, chainID);
    // https://ropsten.etherscan.io/tx/0x14429509307efebfdaa05227d84c147450d168c68539351fbc01ed87c916ab2e
    EXPECT_EQ(hex(encoded), "02f8710306847735940084b2d05e0082526c94b9f5771c27664bf2282d98e09d7f50cec7cb01a78701ee0c29f50cb180c080a092c336138f7d0231fe9422bb30ee9ef10bf222761fe9e04442e3a11e88880c64a06487026011dae03dc281bc21c7d7ede5c2226d197befb813a4ecad686b559e58");
}

TEST(EthereumSigner, EIP4337_NativeTransfer_Account_Not_Deployed) {
    const uint256_t chainID = 5;
    const auto transaction = TransactionEip4337::buildNativeTransfer(
        parse_hex("0x1306b01bC3e4AD202612D3843387e94737673F53"),
        parse_hex("0x5A87209b755781cF65fEeEdd3855ade0317f4a92"),
        parse_hex("0x21cc27d7db4fa19857a3702653a7a67ee30ca620"),
        parse_hex("0x78d9C32b96Bb872D66D51818227563f44e67E238"),
        parse_hex("0xce642355Fa553f408C34a2650Ad2F4A1634d033a"),
        0x2386f26fc10000,
        0x00,
        false,
        0x5580,
        0x073272,
        0x01952f1f85,
        0x0f,
        0xbc18
        );

    const auto serialized = transaction->serialize(chainID);
    const auto key = PrivateKey(parse_hex("f9fb27c90dcaa5631f373330eeef62ae7931587a19bd8215d0c2addf28e439c8"));
    const auto signature = Signer::sign(key, chainID, transaction);
    const auto encoded = transaction->encoded(signature, chainID);
    const auto result = std::string(encoded.begin(), encoded.end());

    // https://goerli.etherscan.io/tx/0xf4e9c9899da7d083f260fd8d0d326a6a0e965f03444a32c73e30cd30ccc609f7
    EXPECT_EQ(result, "{\"callData\":\"0xb61d27f6000000000000000000000000ce642355fa553f408c34a2650ad2f4a1634d033a000000000000000000000000000000000000000000000000002386f26fc1000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000000\",\"callGasLimit\":\"21888\",\"initCode\":\"0x5a87209b755781cf65feeedd3855ade0317f4a925fbfb9cf00000000000000000000000078d9c32b96bb872d66d51818227563f44e67e2380000000000000000000000000000000000000000000000000000000000000000\",\"maxFeePerGas\":\"6797860741\",\"maxPriorityFeePerGas\":\"15\",\"nonce\":\"0\",\"paymasterAndData\":\"0x\",\"preVerificationGas\":\"48152\",\"sender\":\"0x8ce23b8769ac01d0df0d5f47be1a38fea97f3879\",\"signature\":\"0x1560b19d17613ec8580cb0feaf7ac2953771404c5bd7830f585e5062e6ddd4b82ae3bb8dbddb659c0300e8009857b5c77501e1cfd5bbab48d03de0ea7207d07c1b\",\"verificationGasLimit\":\"471666\"}");
}

TEST(EthereumSigner, EIP4337_NativeTransfer_Account_Deployed) {
    const uint256_t chainID = 5;
    const auto transaction = TransactionEip4337::buildNativeTransfer(
        parse_hex("0x1306b01bC3e4AD202612D3843387e94737673F53"),
        parse_hex("0x5A87209b755781cF65fEeEdd3855ade0317f4a92"),
        parse_hex("0x21cc27d7db4fa19857a3702653a7a67ee30ca620"),
        parse_hex("0x78d9C32b96Bb872D66D51818227563f44e67E238"),
        parse_hex("0xce642355Fa553f408C34a2650Ad2F4A1634d033a"),
        0x2386f26fc10000,
        0x1,
        true,
        0x9d55,
        0x186a0,
        0x1a339c9e9,
        0xf,
        0xb708
    );

    const auto serialized = transaction->serialize(chainID);
    const auto key = PrivateKey(parse_hex("f9fb27c90dcaa5631f373330eeef62ae7931587a19bd8215d0c2addf28e439c8"));
    const auto signature = Signer::sign(key, chainID, transaction);
    const auto encoded = transaction->encoded(signature, chainID);
    const auto result = std::string(encoded.begin(), encoded.end());

    // https://goerli.etherscan.io/tx/0x707ee622b87a35eb2ffc3762553db8ba0efc5053cfdbeb44a841562df2a7c2bf
    EXPECT_EQ(result, "{\"callData\":\"0xb61d27f6000000000000000000000000ce642355fa553f408c34a2650ad2f4a1634d033a000000000000000000000000000000000000000000000000002386f26fc1000000000000000000000000000000000000000000000000000000000000000000600000000000000000000000000000000000000000000000000000000000000000\",\"callGasLimit\":\"40277\",\"initCode\":\"0x\",\"maxFeePerGas\":\"7033440745\",\"maxPriorityFeePerGas\":\"15\",\"nonce\":\"1\",\"paymasterAndData\":\"0x\",\"preVerificationGas\":\"46856\",\"sender\":\"0x8ce23b8769ac01d0df0d5f47be1a38fea97f3879\",\"signature\":\"0xaed2011e5cf267de495b38ecf86ad6f1d4c05217a99e59f47e8d52ba3d41c10144785893fa3e7c116a054999e3902fc2771064d0545148bc49f6d7c827fc7a9a1c\",\"verificationGasLimit\":\"100000\"}");
}

} // namespace TW::Ethereum
