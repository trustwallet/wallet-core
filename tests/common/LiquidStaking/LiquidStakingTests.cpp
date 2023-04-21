// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "LiquidStaking/LiquidStaking.h"
#include "HexCoding.h"
#include "uint256.h"
#include <TrustWalletCore/TWCoinType.h>
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWLiquidStaking.h>
#include <gtest/gtest.h>
#include "TestUtilities.h"

namespace TW::LiquidStaking::tests {
    TEST(LiquidStaking, Coverage) {
        auto output = LiquidStaking::generateError(Proto::OK);
        ASSERT_EQ(output.code(), Proto::OK);
    }

    TEST(LiquidStaking, ErrorActionNotSet) {
        Proto::Input input;
        auto output = build(input);
        ASSERT_EQ(output.status().code(), Proto::ERROR_ACTION_NOT_SET);
    }

    TEST(LiquidStaking, ErrorCanDeserializeInput) {
        const auto inputData_ = data("Invalid");
        const auto inputTWData_ = WRAPD(TWDataCreateWithBytes((const uint8_t *)inputData_.data(), inputData_.size()));
        const auto outputTWData_ = WRAPD(TWLiquidStakingBuildRequest(inputTWData_.get()));
        Proto::Output outputProto;
        const auto outputData = data(TWDataBytes(outputTWData_.get()), TWDataSize(outputTWData_.get()));
        EXPECT_TRUE(outputProto.ParseFromArray(outputData.data(), static_cast<int>(outputData.size())));
        ASSERT_EQ(outputProto.status().code(), Proto::ERROR_INPUT_PROTO_DESERIALIZATION);
    }

    TEST(LiquidStaking, PolygonStrideWithdraw) {
        // TODO: code logic
        Proto::Input input;
        input.set_blockchain(Proto::STRIDE);
        input.set_protocol(Proto::Stride);
        Proto::Withdraw withdraw;
        Proto::Asset asset;
        asset.set_staking_token(Proto::ATOM);
        *withdraw.mutable_asset() = asset;
        withdraw.set_amount("1000000");
        *input.mutable_withdraw() = withdraw;

        auto ls_output = build(input);
        ASSERT_EQ(ls_output.status().code(), Proto::OK);
    }

    TEST(LiquidStaking, PolygonStride) {
        // TODO: code logic
        Proto::Input input;
        input.set_blockchain(Proto::STRIDE);
        input.set_protocol(Proto::Stride);
        Proto::Stake stake;
        Proto::Asset asset;
        asset.set_staking_token(Proto::ATOM);
        *stake.mutable_asset() = asset;
        stake.set_amount("1000000");
        *input.mutable_stake() = stake;

        auto ls_output = build(input);
        ASSERT_EQ(ls_output.status().code(), Proto::OK);
    }

    TEST(LiquidStaking, PolygonStraderSmartContractAddressNotSet) {
        Proto::Input input;
        input.set_blockchain(Proto::POLYGON);
        input.set_protocol(Proto::Strader);
        Proto::Stake stake;
        Proto::Asset asset;
        asset.set_staking_token(Proto::MATIC);
        *stake.mutable_asset() = asset;
        stake.set_amount("1000000000000000000");
        *input.mutable_stake() = stake;

        auto ls_output = build(input);
        ASSERT_EQ(ls_output.status().code(), Proto::ERROR_SMART_CONTRACT_ADDRESS_NOT_SET);
    }

    TEST(LiquidStaking, PolygonStraderStakeInvalidBlockchain) {
        Proto::Input input;
        input.set_blockchain(Proto::Blockchain::STRIDE);
        input.set_protocol(Proto::Strader);
        input.set_smart_contract_address("0xfd225c9e6601c9d38d8f98d8731bf59efcf8c0e3");
        Proto::Stake stake;
        Proto::Asset asset;
        *stake.mutable_asset() = asset;
        stake.set_amount("1000000");
        *input.mutable_stake() = stake;

        auto ls_output = build(input);
        ASSERT_EQ(ls_output.status().code(), Proto::ERROR_TARGETED_BLOCKCHAIN_NOT_SUPPORTED_BY_PROTOCOL);
    }

    TEST(LiquidStaking, PolygonStraderStakeMatic) {
        Proto::Input input;
        input.set_blockchain(Proto::POLYGON);
        input.set_protocol(Proto::Strader);
        input.set_smart_contract_address("0xfd225c9e6601c9d38d8f98d8731bf59efcf8c0e3");
        Proto::Stake stake;
        Proto::Asset asset;
        asset.set_staking_token(Proto::MATIC);
        *stake.mutable_asset() = asset;
        stake.set_amount("1000000000000000000");
        *input.mutable_stake() = stake;

        auto ls_output = build(input);
        ASSERT_EQ(ls_output.status().code(), Proto::OK);
        auto tx = *ls_output.mutable_ethereum();
        ASSERT_TRUE(tx.transaction().has_contract_generic());
        ASSERT_EQ(hex(tx.transaction().contract_generic().data(), true), "0xc78cf1a0");

        auto fill_tx_functor = [](auto& tx){
            // Following fields must be set afterwards, before signing ...
            const auto chainId = store(uint256_t(137));
            tx.set_chain_id(chainId.data(), chainId.size());
            auto nonce = store(uint256_t(1));
            tx.set_nonce(nonce.data(), nonce.size());
            auto maxInclusionFeePerGas = store(uint256_t(35941173184));
            auto maxFeePerGas = store(uint256_t(617347611864));
            tx.set_max_inclusion_fee_per_gas(maxInclusionFeePerGas.data(), maxInclusionFeePerGas.size());
            tx.set_max_fee_per_gas(maxFeePerGas.data(), maxFeePerGas.size());
            auto gasLimit = store(uint256_t(116000));
            tx.set_gas_limit(gasLimit.data(), gasLimit.size());
            auto privKey = parse_hex("4a160b803c4392ea54865d0c5286846e7ad5e68fbd78880547697472b22ea7ab");
            tx.set_private_key(privKey.data(), privKey.size());
            // ... end
        };

        {
            fill_tx_functor(tx);
            Ethereum::Proto::SigningOutput output;
            ANY_SIGN(tx, TWCoinTypePolygon);
            EXPECT_EQ(hex(output.encoded()), "02f87a81890185085e42c7c0858fbcc8fcd88301c52094fd225c9e6601c9d38d8f98d8731bf59efcf8c0e3880de0b6b3a764000084c78cf1a0c001a04bcf92394d53d4908130cc6d4f7b2491967f9d6c59292b84c1f56adc49f6c458a073e09f45d64078c41a7946ffdb1dee8e604eb76f318088490f8f661bb7ddfc54");
            // Successfully broadcasted https://polygonscan.com/tx/0x0f6c4f7a893c3f08be30d2ea24479d7ed4bdba40875d07cfd607cf97980b7cf0
        }

        // TW interface
        {
            const auto inputData_ = input.SerializeAsString();
            EXPECT_EQ(hex(inputData_), "0a170a00121331303030303030303030303030303030303030222a3078666432323563396536363031633964333864386639386438373331626635396566636638633065333001");
            const auto inputTWData_ = WRAPD(TWDataCreateWithBytes((const uint8_t *)inputData_.data(), inputData_.size()));
            const auto outputTWData_ = WRAPD(TWLiquidStakingBuildRequest(inputTWData_.get()));
            const auto outputData = data(TWDataBytes(outputTWData_.get()), TWDataSize(outputTWData_.get()));
            EXPECT_EQ(outputData.size(), 68ul);
            Proto::Output outputProto;
            EXPECT_TRUE(outputProto.ParseFromArray(outputData.data(), static_cast<int>(outputData.size())));
            ASSERT_TRUE(outputProto.has_ethereum());
            ASSERT_EQ(outputProto.status().code(), Proto::OK);
            auto eth_tx = *ls_output.mutable_ethereum();
            ASSERT_TRUE(eth_tx.transaction().has_contract_generic());
            ASSERT_EQ(hex(eth_tx.transaction().contract_generic().data(), true), "0xc78cf1a0");
            fill_tx_functor(eth_tx);
            Ethereum::Proto::SigningOutput output;
            ANY_SIGN(tx, TWCoinTypePolygon);
            EXPECT_EQ(hex(output.encoded()), "02f87a81890185085e42c7c0858fbcc8fcd88301c52094fd225c9e6601c9d38d8f98d8731bf59efcf8c0e3880de0b6b3a764000084c78cf1a0c001a04bcf92394d53d4908130cc6d4f7b2491967f9d6c59292b84c1f56adc49f6c458a073e09f45d64078c41a7946ffdb1dee8e604eb76f318088490f8f661bb7ddfc54");
            // Successfully broadcasted https://polygonscan.com/tx/0x0f6c4f7a893c3f08be30d2ea24479d7ed4bdba40875d07cfd607cf97980b7cf0
        }
    }

    TEST(LiquidStaking, PolygonStraderUnStakeMatic) {
        Proto::Input input;
        input.set_blockchain(Proto::POLYGON);
        input.set_protocol(Proto::Strader);
        input.set_smart_contract_address("0xfd225c9e6601c9d38d8f98d8731bf59efcf8c0e3");
        Proto::Unstake unstake;
        Proto::Asset asset;
        *unstake.mutable_asset() = asset;
        unstake.set_amount("1000000000000000000");
        *input.mutable_unstake() = unstake;

        auto ls_output = build(input);
        ASSERT_EQ(ls_output.status().code(), Proto::OK);
        auto tx = *ls_output.mutable_ethereum();
        ASSERT_TRUE(tx.transaction().has_contract_generic());
        ASSERT_EQ(hex(tx.transaction().contract_generic().data(), true), "0x48eaf6d60000000000000000000000000000000000000000000000000de0b6b3a7640000");

        // Following fields must be set afterwards, before signing ...
        const auto chainId = store(uint256_t(137));
        tx.set_chain_id(chainId.data(), chainId.size());
        auto nonce = store(uint256_t(4));
        tx.set_nonce(nonce.data(), nonce.size());
        auto maxInclusionFeePerGas = store(uint256_t(35941173184));
        auto maxFeePerGas = store(uint256_t(617347611864));
        tx.set_max_inclusion_fee_per_gas(maxInclusionFeePerGas.data(), maxInclusionFeePerGas.size());
        tx.set_max_fee_per_gas(maxFeePerGas.data(), maxFeePerGas.size());
        auto gasLimit = store(uint256_t(200000));
        tx.set_gas_limit(gasLimit.data(), gasLimit.size());
        auto privKey = parse_hex("4a160b803c4392ea54865d0c5286846e7ad5e68fbd78880547697472b22ea7ab");
        tx.set_private_key(privKey.data(), privKey.size());
        // ... end

        Ethereum::Proto::SigningOutput output;
        ANY_SIGN(tx, TWCoinTypePolygon);
        EXPECT_EQ(hex(output.encoded()), "02f89281890485085e42c7c0858fbcc8fcd883030d4094fd225c9e6601c9d38d8f98d8731bf59efcf8c0e380a448eaf6d60000000000000000000000000000000000000000000000000de0b6b3a7640000c001a0a0dd3f23758fbcc6f25c8e4396881ab6a1fb444e5a9531b1028b121407d4b79ca0618908f0f1aa79ce3f9e25cfe24a86fd8870c85e78b3730115c033f4f6678531");
        // Successfully broadcasted https://polygonscan.com/tx/0xa66855e4af8e654e458915f59acd77e88706c01b59a3e4aed1363a665458368a
    }

    TEST(LiquidStaking, PolygonStraderStakeBnb) {
        Proto::Input input;
        input.set_blockchain(Proto::BNB_BSC);
        input.set_protocol(Proto::Strader);
        input.set_smart_contract_address("0x7276241a669489e4bbb76f63d2a43bfe63080f2f");
        Proto::Stake stake;
        Proto::Asset asset;
        asset.set_staking_token(Proto::BNB);
        *stake.mutable_asset() = asset;
        stake.set_amount("20000000000000000");
        *input.mutable_stake() = stake;

        auto ls_output = build(input);
        ASSERT_EQ(ls_output.status().code(), Proto::OK);
        auto tx = *ls_output.mutable_ethereum();
        ASSERT_TRUE(tx.transaction().has_contract_generic());
        ASSERT_EQ(hex(tx.transaction().contract_generic().data(), true), "0xd0e30db0");

        // Following fields must be set afterwards, before signing ...
        const auto chainId = store(uint256_t(56));
        tx.set_chain_id(chainId.data(), chainId.size());
        auto nonce = store(uint256_t(1));
        tx.set_nonce(nonce.data(), nonce.size());
        auto gasPrice = store(uint256_t(20000000000));
        tx.set_gas_price(gasPrice.data(), gasPrice.size());
        auto gasLimit = store(uint256_t(250000));
        tx.set_gas_limit(gasLimit.data(), gasLimit.size());
        auto privKey = parse_hex("4a160b803c4392ea54865d0c5286846e7ad5e68fbd78880547697472b22ea7ab");
        tx.set_private_key(privKey.data(), privKey.size());
        // ... end

        Ethereum::Proto::SigningOutput output;
        ANY_SIGN(tx, TWCoinTypeSmartChain);
        EXPECT_EQ(hex(output.encoded()), "f871018504a817c8008303d090947276241a669489e4bbb76f63d2a43bfe63080f2f87470de4df82000084d0e30db08193a0ec9bcc1b203477b9e5af8d0f9338de2af2553bb34ba693e79183708d6025e5c9a01e1c1f5d724fa2aa55464451bc0eee45b8522091048e6ac377db0b181e412a15");
        // Successfully broadcasted https://bscscan.com/tx/0x17014f06b267f683d03d4d9cc2e5c1b182be05c14c3b9ffa0eaa3060bc859ba6
    }

    TEST(LiquidStaking, PolygonStraderUnStakeBnb) {
        Proto::Input input;
        input.set_blockchain(Proto::BNB_BSC);
        input.set_protocol(Proto::Strader);
        input.set_smart_contract_address("0x7276241a669489e4bbb76f63d2a43bfe63080f2f");
        Proto::Unstake unstake;
        Proto::Asset asset;
        asset.set_staking_token(Proto::BNB);
        *unstake.mutable_asset() = asset;
        unstake.set_amount("10000000000000000");
        *input.mutable_unstake() = unstake;

        auto ls_output = build(input);
        ASSERT_EQ(ls_output.status().code(), Proto::OK);
        auto tx = *ls_output.mutable_ethereum();
        ASSERT_TRUE(tx.transaction().has_contract_generic());
        ASSERT_EQ(hex(tx.transaction().contract_generic().data(), true), "0x745400c9000000000000000000000000000000000000000000000000002386f26fc10000");

        // Following fields must be set afterwards, before signing ...
        const auto chainId = store(uint256_t(56));
        tx.set_chain_id(chainId.data(), chainId.size());
        auto nonce = store(uint256_t(7));
        tx.set_nonce(nonce.data(), nonce.size());
        auto gasPrice = store(uint256_t(20000000000));
        tx.set_gas_price(gasPrice.data(), gasPrice.size());
        auto gasLimit = store(uint256_t(250000));
        tx.set_gas_limit(gasLimit.data(), gasLimit.size());
        auto privKey = parse_hex("4a160b803c4392ea54865d0c5286846e7ad5e68fbd78880547697472b22ea7ab");
        tx.set_private_key(privKey.data(), privKey.size());
        // ... end

        Ethereum::Proto::SigningOutput output;
        ANY_SIGN(tx, TWCoinTypeSmartChain);
        EXPECT_EQ(hex(output.encoded()), "f889078504a817c8008303d090947276241a669489e4bbb76f63d2a43bfe63080f2f80a4745400c9000000000000000000000000000000000000000000000000002386f26fc100008193a0a1b72a5c368e0591c488094e5f9a431b1be915310fb47c1c9312c247044310279f5fffeaf2e1659c841f31927b0e60870b455fa35e041ae29006472c87550c9d");
        // Successfully broadcasted https://bscscan.com/tx/0x420b203b998d4de40e78ab7c6e80399d45a20620368c11c7d7d45820eeef3096
    }
}
