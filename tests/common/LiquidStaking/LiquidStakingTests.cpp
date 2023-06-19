// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "LiquidStaking/LiquidStaking.h"
#include "HexCoding.h"
#include "Base64.h"
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

    TEST(LiquidStaking, PolygonStraderWithdrawMatic) {
        Proto::Input input;
        input.set_blockchain(Proto::POLYGON);
        input.set_protocol(Proto::Strader);
        input.set_smart_contract_address("0xfd225c9e6601c9d38d8f98d8731bf59efcf8c0e3");
        Proto::Withdraw withdraw;
        Proto::Asset asset;
        *withdraw.mutable_asset() = asset;
        withdraw.set_idx("0");
        *input.mutable_withdraw() = withdraw;

        auto ls_output = build(input);
        ASSERT_EQ(ls_output.status().code(), Proto::OK);
        auto tx = *ls_output.mutable_ethereum();
        ASSERT_TRUE(tx.transaction().has_contract_generic());
        ASSERT_EQ(hex(tx.transaction().contract_generic().data(), true), "0x77baf2090000000000000000000000000000000000000000000000000000000000000000");

        // Following fields must be set afterwards, before signing ...
        const auto chainId = store(uint256_t(137));
        tx.set_chain_id(chainId.data(), chainId.size());
        auto nonce = store(uint256_t(6));
        tx.set_nonce(nonce.data(), nonce.size());
        auto maxInclusionFeePerGas = store(uint256_t(35941173184));
        auto maxFeePerGas = store(uint256_t(678347611864));
        tx.set_max_inclusion_fee_per_gas(maxInclusionFeePerGas.data(), maxInclusionFeePerGas.size());
        tx.set_max_fee_per_gas(maxFeePerGas.data(), maxFeePerGas.size());
        auto gasLimit = store(uint256_t(200000));
        tx.set_gas_limit(gasLimit.data(), gasLimit.size());
        auto privKey = parse_hex("4a160b803c4392ea54865d0c5286846e7ad5e68fbd78880547697472b22ea7ab");
        tx.set_private_key(privKey.data(), privKey.size());
        // ... end

        Ethereum::Proto::SigningOutput output;
        ANY_SIGN(tx, TWCoinTypePolygon);
        EXPECT_EQ(hex(output.encoded()), "02f89281890685085e42c7c0859df0ab1ed883030d4094fd225c9e6601c9d38d8f98d8731bf59efcf8c0e380a477baf2090000000000000000000000000000000000000000000000000000000000000000c080a02c2440ded34fc9930e4d07a7c16d5a6d865b17e37dba61f3a94a3b78cd269a35a055cb8ba79645063f99e999b6ca44cd0ac26d6fd2b3acc8453a1c1c61de767559");
        // Successfully broadcasted https://polygonscan.com/tx/0x61fa7b9051ddb9e2906130b0c5d94b8e3ecfc89b1fdfeff86042fbea851d8ba4
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

    TEST(LiquidStaking, AptosTortugaStakeApt) {
        // Successfully broadcasted: https://explorer.aptoslabs.com/txn/0x25dca849cb4ebacbff223139f7ad5d24c37c225d9506b8b12a925de70429e685/userTxnOverview?network=mainnet
        Proto::Input input;
        input.set_blockchain(Proto::APTOS);
        input.set_protocol(Proto::Tortuga);
        input.set_smart_contract_address("0x8f396e4246b2ba87b51c0739ef5ea4f26515a98375308c31ac2ec1e42142a57f");
        Proto::Stake stake;
        Proto::Asset asset;
        asset.set_staking_token(Proto::APT);
        *stake.mutable_asset() = asset;
        stake.set_amount("100000000");
        *input.mutable_stake() = stake;

        auto ls_output = build(input);
        ASSERT_EQ(ls_output.status().code(), Proto::OK);
        auto& tx = *ls_output.mutable_aptos();

        auto fill_tx_functor = [](auto& tx){
            // Following fields must be set afterwards, before signing ...
            tx.set_sender("0xf3d7f364dd7705824a5ebda9c7aab6cb3fc7bb5b58718249f12defec240b36cc");
            tx.set_sequence_number(19);
            tx.set_max_gas_amount(5554);
            tx.set_gas_unit_price(100);
            tx.set_expiration_timestamp_secs(1670240203);
            tx.set_chain_id(1);
            auto privateKey = parse_hex("786fc7ceca43b4c1da018fea5d96f35dfdf5605f220b1205ff29c5c6d9eccf05");
            tx.set_private_key(privateKey.data(), privateKey.size());
        };

        auto verify_tx_functor = [](auto& tx) {
            EXPECT_EQ(hex(tx.raw_txn()), "f3d7f364dd7705824a5ebda9c7aab6cb3fc7bb5b58718249f12defec240b36cc1300000000000000028f396e4246b2ba87b51c0739ef5ea4f26515a98375308c31ac2ec1e42142a57f0c7374616b655f726f75746572057374616b6500010800e1f50500000000b2150000000000006400000000000000cbd78d630000000001");
            EXPECT_EQ(hex(tx.authenticator().signature()), "22d3166c3003f9c24a35fd39c71eb27e0d2bb82541be610822165c9283f56fefe5a9d46421b9caf174995bd8f83141e60ea8cff521ecf4741fe19e6ae9a5680d");
            EXPECT_EQ(hex(tx.encoded()), "f3d7f364dd7705824a5ebda9c7aab6cb3fc7bb5b58718249f12defec240b36cc1300000000000000028f396e4246b2ba87b51c0739ef5ea4f26515a98375308c31ac2ec1e42142a57f0c7374616b655f726f75746572057374616b6500010800e1f50500000000b2150000000000006400000000000000cbd78d630000000001002089e0211d7e19c7d3a8e2030fe16c936a690ca9b95569098c5d2bf1031ff44bc44022d3166c3003f9c24a35fd39c71eb27e0d2bb82541be610822165c9283f56fefe5a9d46421b9caf174995bd8f83141e60ea8cff521ecf4741fe19e6ae9a5680d");
            nlohmann::json expectedJson = R"(
                {
                    "sender": "0xf3d7f364dd7705824a5ebda9c7aab6cb3fc7bb5b58718249f12defec240b36cc",
                    "sequence_number": "19",
                    "max_gas_amount": "5554",
                    "gas_unit_price": "100",
                    "expiration_timestamp_secs": "1670240203",
                    "payload": {
                        "function": "0x8f396e4246b2ba87b51c0739ef5ea4f26515a98375308c31ac2ec1e42142a57f::stake_router::stake",
                        "type_arguments": [],
                        "arguments": [
                            "100000000"
                        ],
                    "type": "entry_function_payload"
                    },
                    "signature": {
                        "public_key": "0x89e0211d7e19c7d3a8e2030fe16c936a690ca9b95569098c5d2bf1031ff44bc4",
                        "signature": "0x22d3166c3003f9c24a35fd39c71eb27e0d2bb82541be610822165c9283f56fefe5a9d46421b9caf174995bd8f83141e60ea8cff521ecf4741fe19e6ae9a5680d",
                        "type": "ed25519_signature"
                    }
                }
        )"_json;
            nlohmann::json parsedJson = nlohmann::json::parse(tx.json());
            assertJSONEqual(expectedJson, parsedJson);
        };

        {
            fill_tx_functor(tx);
            Aptos::Proto::SigningOutput output;
            ANY_SIGN(tx, TWCoinTypeAptos);
            verify_tx_functor(output);
        }

        // TW interface
        {
            const auto inputData_ = input.SerializeAsString();
            EXPECT_EQ(hex(inputData_), "0a0f0a0208031209313030303030303030224230783866333936653432343662326261383762353163303733396566356561346632363531356139383337353330386333316163326563316534323134326135376628023004");
            const auto inputTWData_ = WRAPD(TWDataCreateWithBytes((const uint8_t *)inputData_.data(), inputData_.size()));
            const auto outputTWData_ = WRAPD(TWLiquidStakingBuildRequest(inputTWData_.get()));
            const auto outputData = data(TWDataBytes(outputTWData_.get()), TWDataSize(outputTWData_.get()));
            EXPECT_EQ(outputData.size(), 79ul);
            Proto::Output outputProto;
            EXPECT_TRUE(outputProto.ParseFromArray(outputData.data(), static_cast<int>(outputData.size())));
            ASSERT_TRUE(outputProto.has_aptos());
            ASSERT_EQ(outputProto.status().code(), Proto::OK);
            auto aptos_tx = *ls_output.mutable_aptos();
            fill_tx_functor(aptos_tx);
            Aptos::Proto::SigningOutput output;
            ANY_SIGN(tx, TWCoinTypeAptos);
            verify_tx_functor(output);
        }
    }

    TEST(LiquidStaking, AptosTortugaUnstakeApt) {
        // Successfully broadcasted: https://explorer.aptoslabs.com/txn/0x92edb4f756fe86118e34a0e64746c70260ee02c2ae2cf402b3e39f6a282ce968?network=mainnet
        Proto::Input input;
        input.set_blockchain(Proto::APTOS);
        input.set_protocol(Proto::Tortuga);
        input.set_smart_contract_address("0x8f396e4246b2ba87b51c0739ef5ea4f26515a98375308c31ac2ec1e42142a57f");
        Proto::Unstake unstake;
        Proto::Asset asset;
        asset.set_staking_token(Proto::APT);
        *unstake.mutable_asset() = asset;
        unstake.set_amount("99178100");
        *input.mutable_unstake() = unstake;

        auto ls_output = build(input);
        ASSERT_EQ(ls_output.status().code(), Proto::OK);
        auto& tx = *ls_output.mutable_aptos();

        auto fill_tx_functor = [](auto& tx){
            // Following fields must be set afterwards, before signing ...
            tx.set_sender("0xf3d7f364dd7705824a5ebda9c7aab6cb3fc7bb5b58718249f12defec240b36cc");
            tx.set_sequence_number(20);
            tx.set_max_gas_amount(2371);
            tx.set_gas_unit_price(120);
            tx.set_expiration_timestamp_secs(1670304949);
            tx.set_chain_id(1);
            auto privateKey = parse_hex("786fc7ceca43b4c1da018fea5d96f35dfdf5605f220b1205ff29c5c6d9eccf05");
            tx.set_private_key(privateKey.data(), privateKey.size());
        };

        auto verify_tx_functor = [](auto& tx) {
            EXPECT_EQ(hex(tx.raw_txn()), "f3d7f364dd7705824a5ebda9c7aab6cb3fc7bb5b58718249f12defec240b36cc1400000000000000028f396e4246b2ba87b51c0739ef5ea4f26515a98375308c31ac2ec1e42142a57f0c7374616b655f726f7574657207756e7374616b650001087456e9050000000043090000000000007800000000000000b5d48e630000000001");
            EXPECT_EQ(hex(tx.authenticator().signature()), "6994b917432ad70ae84d2ce1484e6aece589a68aad1b7c6e38c9697f2a012a083a3a755c5e010fd3d0f149a75dd8d257acbd09f10800e890074e5ad384314d0c");
            EXPECT_EQ(hex(tx.encoded()), "f3d7f364dd7705824a5ebda9c7aab6cb3fc7bb5b58718249f12defec240b36cc1400000000000000028f396e4246b2ba87b51c0739ef5ea4f26515a98375308c31ac2ec1e42142a57f0c7374616b655f726f7574657207756e7374616b650001087456e9050000000043090000000000007800000000000000b5d48e630000000001002089e0211d7e19c7d3a8e2030fe16c936a690ca9b95569098c5d2bf1031ff44bc4406994b917432ad70ae84d2ce1484e6aece589a68aad1b7c6e38c9697f2a012a083a3a755c5e010fd3d0f149a75dd8d257acbd09f10800e890074e5ad384314d0c");
            nlohmann::json expectedJson = R"(
                {
                    "sender": "0xf3d7f364dd7705824a5ebda9c7aab6cb3fc7bb5b58718249f12defec240b36cc",
                    "sequence_number": "20",
                    "max_gas_amount": "2371",
                    "gas_unit_price": "120",
                    "expiration_timestamp_secs": "1670304949",
                    "payload": {
                        "function": "0x8f396e4246b2ba87b51c0739ef5ea4f26515a98375308c31ac2ec1e42142a57f::stake_router::unstake",
                        "type_arguments": [],
                        "arguments": [
                            "99178100"
                        ],
                    "type": "entry_function_payload"
                    },
                    "signature": {
                        "public_key": "0x89e0211d7e19c7d3a8e2030fe16c936a690ca9b95569098c5d2bf1031ff44bc4",
                        "signature": "0x6994b917432ad70ae84d2ce1484e6aece589a68aad1b7c6e38c9697f2a012a083a3a755c5e010fd3d0f149a75dd8d257acbd09f10800e890074e5ad384314d0c",
                        "type": "ed25519_signature"
                    }
                }
        )"_json;
            nlohmann::json parsedJson = nlohmann::json::parse(tx.json());
            assertJSONEqual(expectedJson, parsedJson);
        };

        {
            fill_tx_functor(tx);
            Aptos::Proto::SigningOutput output;
            ANY_SIGN(tx, TWCoinTypeAptos);
            verify_tx_functor(output);
        }

        // TW interface
        {
            const auto inputData_ = input.SerializeAsString();
            EXPECT_EQ(hex(inputData_), "120e0a02080312083939313738313030224230783866333936653432343662326261383762353163303733396566356561346632363531356139383337353330386333316163326563316534323134326135376628023004");
            const auto inputTWData_ = WRAPD(TWDataCreateWithBytes((const uint8_t *)inputData_.data(), inputData_.size()));
            const auto outputTWData_ = WRAPD(TWLiquidStakingBuildRequest(inputTWData_.get()));
            const auto outputData = data(TWDataBytes(outputTWData_.get()), TWDataSize(outputTWData_.get()));
            EXPECT_EQ(outputData.size(), 79ul);
            Proto::Output outputProto;
            EXPECT_TRUE(outputProto.ParseFromArray(outputData.data(), static_cast<int>(outputData.size())));
            ASSERT_TRUE(outputProto.has_aptos());
            ASSERT_EQ(outputProto.status().code(), Proto::OK);
            auto aptos_tx = *ls_output.mutable_aptos();
            fill_tx_functor(aptos_tx);
            Aptos::Proto::SigningOutput output;
            ANY_SIGN(tx, TWCoinTypeAptos);
            verify_tx_functor(output);
        }
    }

    TEST(LiquidStaking, AptosTortugaWithdrawApt) {
        // Successfully broadcasted: https://explorer.aptoslabs.com/txn/0x9fc874de7a7d3e813d9a1658d896023de270a0096a5e258c196005656ace7d54?network=mainnet
        Proto::Input input;
        input.set_blockchain(Proto::APTOS);
        input.set_protocol(Proto::Tortuga);
        input.set_smart_contract_address("0x8f396e4246b2ba87b51c0739ef5ea4f26515a98375308c31ac2ec1e42142a57f");
        Proto::Withdraw withdraw;
        Proto::Asset asset;
        asset.set_staking_token(Proto::APT);
        *withdraw.mutable_asset() = asset;
        withdraw.set_idx("0");
        *input.mutable_withdraw() = withdraw;

        auto ls_output = build(input);
        ASSERT_EQ(ls_output.status().code(), Proto::OK);
        auto& tx = *ls_output.mutable_aptos();

        auto fill_tx_functor = [](auto& tx){
            // Following fields must be set afterwards, before signing ...
            tx.set_sender("0xf3d7f364dd7705824a5ebda9c7aab6cb3fc7bb5b58718249f12defec240b36cc");
            tx.set_sequence_number(28);
            tx.set_max_gas_amount(10);
            tx.set_gas_unit_price(148);
            tx.set_expiration_timestamp_secs(1682066783);
            tx.set_chain_id(1);
            auto privateKey = parse_hex("786fc7ceca43b4c1da018fea5d96f35dfdf5605f220b1205ff29c5c6d9eccf05");
            tx.set_private_key(privateKey.data(), privateKey.size());
        };

        auto verify_tx_functor = [](auto& tx) {
            EXPECT_EQ(hex(tx.raw_txn()), "f3d7f364dd7705824a5ebda9c7aab6cb3fc7bb5b58718249f12defec240b36cc1c00000000000000028f396e4246b2ba87b51c0739ef5ea4f26515a98375308c31ac2ec1e42142a57f0c7374616b655f726f7574657205636c61696d00010800000000000000000a0000000000000094000000000000005f4d42640000000001");
            EXPECT_EQ(hex(tx.authenticator().signature()), "c936584f89777e1fe2d5dd75cd8d9c514efc445810ba22f462b6fe7229c6ec7fc1c8b25d3e233eafaa8306433b3220235e563498ba647be38cac87ff618e3d03");
            EXPECT_EQ(hex(tx.encoded()), "f3d7f364dd7705824a5ebda9c7aab6cb3fc7bb5b58718249f12defec240b36cc1c00000000000000028f396e4246b2ba87b51c0739ef5ea4f26515a98375308c31ac2ec1e42142a57f0c7374616b655f726f7574657205636c61696d00010800000000000000000a0000000000000094000000000000005f4d42640000000001002089e0211d7e19c7d3a8e2030fe16c936a690ca9b95569098c5d2bf1031ff44bc440c936584f89777e1fe2d5dd75cd8d9c514efc445810ba22f462b6fe7229c6ec7fc1c8b25d3e233eafaa8306433b3220235e563498ba647be38cac87ff618e3d03");
            nlohmann::json expectedJson = R"(
                {
                    "sender": "0xf3d7f364dd7705824a5ebda9c7aab6cb3fc7bb5b58718249f12defec240b36cc",
                    "sequence_number": "28",
                    "max_gas_amount": "10",
                    "gas_unit_price": "148",
                    "expiration_timestamp_secs": "1682066783",
                    "payload": {
                        "function": "0x8f396e4246b2ba87b51c0739ef5ea4f26515a98375308c31ac2ec1e42142a57f::stake_router::claim",
                        "type_arguments": [],
                        "arguments": [
                            "0"
                        ],
                        "type": "entry_function_payload"
                    },
                    "signature": {
                        "public_key": "0x89e0211d7e19c7d3a8e2030fe16c936a690ca9b95569098c5d2bf1031ff44bc4",
                        "signature": "0xc936584f89777e1fe2d5dd75cd8d9c514efc445810ba22f462b6fe7229c6ec7fc1c8b25d3e233eafaa8306433b3220235e563498ba647be38cac87ff618e3d03",
                        "type": "ed25519_signature"
                    }
                }
        )"_json;
            nlohmann::json parsedJson = nlohmann::json::parse(tx.json());
            assertJSONEqual(expectedJson, parsedJson);
        };

        {
            fill_tx_functor(tx);
            Aptos::Proto::SigningOutput output;
            ANY_SIGN(tx, TWCoinTypeAptos);
            verify_tx_functor(output);
        }

        // TW interface
        {
            const auto inputData_ = input.SerializeAsString();
            EXPECT_EQ(hex(inputData_), "1a070a0208031a0130224230783866333936653432343662326261383762353163303733396566356561346632363531356139383337353330386333316163326563316534323134326135376628023004");
            const auto inputTWData_ = WRAPD(TWDataCreateWithBytes((const uint8_t *)inputData_.data(), inputData_.size()));
            const auto outputTWData_ = WRAPD(TWLiquidStakingBuildRequest(inputTWData_.get()));
            const auto outputData = data(TWDataBytes(outputTWData_.get()), TWDataSize(outputTWData_.get()));
            EXPECT_EQ(outputData.size(), 74ul);
            Proto::Output outputProto;
            EXPECT_TRUE(outputProto.ParseFromArray(outputData.data(), static_cast<int>(outputData.size())));
            ASSERT_TRUE(outputProto.has_aptos());
            ASSERT_EQ(outputProto.status().code(), Proto::OK);
            auto aptos_tx = *ls_output.mutable_aptos();
            fill_tx_functor(aptos_tx);
            Aptos::Proto::SigningOutput output;
            ANY_SIGN(tx, TWCoinTypeAptos);
            verify_tx_functor(output);
        }
    }

    TEST(LiquidStaking, StrideStakeAtom) {
        Proto::Input input;
        input.set_blockchain(Proto::STRIDE);
        input.set_protocol(Proto::Stride);
        Proto::Stake stake;
        Proto::Asset asset;
        asset.set_staking_token(Proto::ATOM);
        asset.set_denom("uatom");
        asset.set_from_address("stride1mry47pkga5tdswtluy0m8teslpalkdq0a2sjge");
        *stake.mutable_asset() = asset;
        stake.set_amount("100000");
        *input.mutable_stake() = stake;

        auto ls_output = build(input);
        ASSERT_EQ(ls_output.status().code(), Proto::OK);
        ASSERT_TRUE(ls_output.has_cosmos());
        auto tx = *ls_output.mutable_cosmos();

        auto fill_tx_functor = [](auto& tx){
            // Following fields must be set afterwards, before signing ...tx
            auto privateKey = parse_hex("a498a9ee41af9bab5ef2a8be63d5c970135c3c109e70efc8c56c534e6636b433");
            tx.set_signing_mode(Cosmos::Proto::Protobuf);
            tx.set_account_number(136412);
            tx.set_chain_id("stride-1");
            tx.set_memo("");
            tx.set_sequence(0);
            tx.set_private_key(privateKey.data(), privateKey.size());

            auto& fee = *tx.mutable_fee();
            fee.set_gas(500000);
            auto amountOfFee = fee.add_amounts();
            amountOfFee->set_denom("ustrd");
            amountOfFee->set_amount("0");
        };

        auto verify_tx_functor = [](auto& tx) {
            // Successfully broadcasted: https://www.mintscan.io/stride/txs/48E51A2571D99453C4581B30CECA2A1156C0D1EBACCD3619729B5A35AD67CC94?height=3485243
            auto expectedJson = R"(
                {
                    "mode":"BROADCAST_MODE_BLOCK",
                    "tx_bytes":"CmMKYQofL3N0cmlkZS5zdGFrZWliYy5Nc2dMaXF1aWRTdGFrZRI+Ci1zdHJpZGUxbXJ5NDdwa2dhNXRkc3d0bHV5MG04dGVzbHBhbGtkcTBhMnNqZ2USBjEwMDAwMBoFdWF0b20SYgpOCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAsv9teRyiTMiKU5gzwiD1D30MeEInSnstEep5tVQRarlEgQKAggBEhAKCgoFdXN0cmQSATAQoMIeGkCDaZHV5/Z3CAQC5DXkaHmF6OKUiS5XKDsl3ZnBaaVuJjlSWV2vA7MPwGbC17P6jbVJt58ZLcxIWFt76UO3y1ix"
                })";
            assertJSONEqual(tx.serialized(), expectedJson);
            EXPECT_EQ(hex(tx.signature()), "836991d5e7f677080402e435e4687985e8e294892e57283b25dd99c169a56e263952595daf03b30fc066c2d7b3fa8db549b79f192dcc48585b7be943b7cb58b1");
            EXPECT_EQ(tx.json(), "");
            EXPECT_EQ(tx.error_message(), "");
        };

        {
            fill_tx_functor(tx);
            Cosmos::Proto::SigningOutput output;
            ANY_SIGN(tx, TWCoinTypeStride);
            verify_tx_functor(output);
        }

        {
            const auto inputData_ = input.SerializeAsString();
            EXPECT_EQ(hex(inputData_), "0a420a3808011a057561746f6d222d737472696465316d72793437706b67613574647377746c7579306d387465736c70616c6b6471306132736a6765120631303030303028013002");
            const auto inputTWData_ = WRAPD(TWDataCreateWithBytes((const uint8_t *)inputData_.data(), inputData_.size()));
            const auto outputTWData_ = WRAPD(TWLiquidStakingBuildRequest(inputTWData_.get()));
            const auto outputData = data(TWDataBytes(outputTWData_.get()), TWDataSize(outputTWData_.get()));
            EXPECT_EQ(outputData.size(), 69ul);
            Proto::Output outputProto;
            EXPECT_TRUE(outputProto.ParseFromArray(outputData.data(), static_cast<int>(outputData.size())));
            ASSERT_TRUE(outputProto.has_cosmos());
            ASSERT_EQ(outputProto.status().code(), Proto::OK);
            auto aptos_tx = *ls_output.mutable_cosmos();
            fill_tx_functor(aptos_tx);
            Cosmos::Proto::SigningOutput output;
            ANY_SIGN(tx, TWCoinTypeStride);
            verify_tx_functor(output);
        }
    }

    TEST(LiquidStaking, StrideUnstakeAtom) {
        Proto::Input input;
        input.set_blockchain(Proto::STRIDE);
        input.set_protocol(Proto::Stride);
        Proto::Unstake unstake;
        Proto::Asset asset;
        asset.set_staking_token(Proto::ATOM);
        asset.set_from_address("stride1mry47pkga5tdswtluy0m8teslpalkdq0a2sjge");
        *unstake.mutable_asset() = asset;
        unstake.set_amount("40000");
        unstake.set_receiver_chain_id("cosmoshub-4");
        unstake.set_receiver_address("cosmos1mry47pkga5tdswtluy0m8teslpalkdq07pswu4");
        *input.mutable_unstake() = unstake;

        auto ls_output = build(input);
        ASSERT_EQ(ls_output.status().code(), Proto::OK);
        ASSERT_TRUE(ls_output.has_cosmos());
        auto tx = *ls_output.mutable_cosmos();

        auto fill_tx_functor = [](auto& tx){
            // Following fields must be set afterwards, before signing ...tx
            auto privateKey = parse_hex("a498a9ee41af9bab5ef2a8be63d5c970135c3c109e70efc8c56c534e6636b433");
            tx.set_signing_mode(Cosmos::Proto::Protobuf);
            tx.set_account_number(136412);
            tx.set_chain_id("stride-1");
            tx.set_memo("");
            tx.set_sequence(1);
            tx.set_private_key(privateKey.data(), privateKey.size());

            auto& fee = *tx.mutable_fee();
            fee.set_gas(1000000);
            auto amountOfFee = fee.add_amounts();
            amountOfFee->set_denom("ustrd");
            amountOfFee->set_amount("0");
        };

        auto verify_tx_functor = [](auto& tx) {
            // Successfully broadcasted: https://www.mintscan.io/stride/txs/B3D3A92A2FFB92A480A4B547A4303E6932204972A965D687DB4FB6B4E16B2C42?height=3485343
            auto expectedJson = R"(
                {
                    "mode":"BROADCAST_MODE_BLOCK",
                    "tx_bytes":"CpgBCpUBCh8vc3RyaWRlLnN0YWtlaWJjLk1zZ1JlZGVlbVN0YWtlEnIKLXN0cmlkZTFtcnk0N3BrZ2E1dGRzd3RsdXkwbTh0ZXNscGFsa2RxMGEyc2pnZRIFNDAwMDAaC2Nvc21vc2h1Yi00Ii1jb3Ntb3MxbXJ5NDdwa2dhNXRkc3d0bHV5MG04dGVzbHBhbGtkcTA3cHN3dTQSZApQCkYKHy9jb3Ntb3MuY3J5cHRvLnNlY3AyNTZrMS5QdWJLZXkSIwohAsv9teRyiTMiKU5gzwiD1D30MeEInSnstEep5tVQRarlEgQKAggBGAESEAoKCgV1c3RyZBIBMBDAhD0aQKf84TYoPqwnXw22r0dok2fYplUFu003TlIfpoT+wqTZF1lHPC+RTAoJob6x50CnfvGlgJFBEQYPD+Ccv659VVA="
                })";
            assertJSONEqual(tx.serialized(), expectedJson);
            EXPECT_EQ(TW::Base64::encode(data(tx.signature())), "p/zhNig+rCdfDbavR2iTZ9imVQW7TTdOUh+mhP7CpNkXWUc8L5FMCgmhvrHnQKd+8aWAkUERBg8P4Jy/rn1VUA==");
            EXPECT_EQ(tx.json(), "");
            EXPECT_EQ(tx.error_message(), "");
        };

        {
            fill_tx_functor(tx);
            Cosmos::Proto::SigningOutput output;
            ANY_SIGN(tx, TWCoinTypeStride);
            verify_tx_functor(output);
        }

        {
            const auto inputData_ = input.SerializeAsString();
            EXPECT_EQ(hex(inputData_), "12760a310801222d737472696465316d72793437706b67613574647377746c7579306d387465736c70616c6b6471306132736a6765120534303030301a2d636f736d6f73316d72793437706b67613574647377746c7579306d387465736c70616c6b647130377073777534220b636f736d6f736875622d3428013002");
            const auto inputTWData_ = WRAPD(TWDataCreateWithBytes((const uint8_t *)inputData_.data(), inputData_.size()));
            const auto outputTWData_ = WRAPD(TWLiquidStakingBuildRequest(inputTWData_.get()));
            const auto outputData = data(TWDataBytes(outputTWData_.get()), TWDataSize(outputTWData_.get()));
            EXPECT_EQ(outputData.size(), 121ul);
            Proto::Output outputProto;
            EXPECT_TRUE(outputProto.ParseFromArray(outputData.data(), static_cast<int>(outputData.size())));
            ASSERT_TRUE(outputProto.has_cosmos());
            ASSERT_EQ(outputProto.status().code(), Proto::OK);
            auto aptos_tx = *ls_output.mutable_cosmos();
            fill_tx_functor(aptos_tx);
            Cosmos::Proto::SigningOutput output;
            ANY_SIGN(tx, TWCoinTypeStride);
            verify_tx_functor(output);
        }
    }

    TEST(LiquidStaking, EthereumLidoStakeEth) {
        Proto::Input input;
        input.set_blockchain(Proto::ETHEREUM);
        input.set_protocol(Proto::Lido);
        input.set_smart_contract_address("0xae7ab96520DE3A18E5e111B5EaAb095312D7fE84");
        Proto::Stake stake;
        Proto::Asset asset;
        asset.set_staking_token(Proto::ETH);
        *stake.mutable_asset() = asset;
        stake.set_amount("1000000000000000");
        *input.mutable_stake() = stake;

        auto ls_output = build(input);
        ASSERT_EQ(ls_output.status().code(), Proto::OK);
        auto tx = *ls_output.mutable_ethereum();
        ASSERT_TRUE(tx.transaction().has_contract_generic());
        ASSERT_EQ(hex(tx.transaction().contract_generic().data(), true), "0xa1903eab0000000000000000000000000000000000000000000000000000000000000000");

        auto fill_tx_functor = [](auto& tx){
            // Following fields must be set afterwards, before signing ...
            const auto chainId = store(uint256_t(1));
            tx.set_chain_id(chainId.data(), chainId.size());
            auto nonce = store(uint256_t(0));
            tx.set_nonce(nonce.data(), nonce.size());
            auto maxInclusionFeePerGas = store(uint256_t(1500000000));
            auto maxFeePerGas = store(uint256_t(109039719380));
            tx.set_max_inclusion_fee_per_gas(maxInclusionFeePerGas.data(), maxInclusionFeePerGas.size());
            tx.set_max_fee_per_gas(maxFeePerGas.data(), maxFeePerGas.size());
            auto gasLimit = store(uint256_t(117093));
            tx.set_gas_limit(gasLimit.data(), gasLimit.size());
            auto privKey = parse_hex("d68f3ae9c5b3974d27e606eb9fd8e03cf172380307dd0fe1273394b40c8b33aa");
            tx.set_private_key(privKey.data(), privKey.size());
            // ... end
        };

        {
            fill_tx_functor(tx);
            Ethereum::Proto::SigningOutput output;
            ANY_SIGN(tx, TWCoinTypeEthereum);
            EXPECT_EQ(hex(output.encoded()), "02f89701808459682f008519634613d48301c96594ae7ab96520de3a18e5e111b5eaab095312d7fe8487038d7ea4c68000a4a1903eab0000000000000000000000000000000000000000000000000000000000000000c001a0daace8c05277cf7eaff3f03a4e1ba7edadab20407674d1b659f5c13f89a04087a0528b5d4499b67a50989b6397e530be23515532732b54d60c4a4d726e499e046a");
            // Successfully broadcasted https://etherscan.io/tx/0x4d509fd50f474a568419ade4df13b43943b5c8233e980d2217784c512941b3bd
        }

        // TW interface
        {
            const auto inputData_ = input.SerializeAsString();
            EXPECT_EQ(hex(inputData_), "0a160a020804121031303030303030303030303030303030222a3078616537616239363532304445334131384535653131314235456141623039353331324437664538342803");
            const auto inputTWData_ = WRAPD(TWDataCreateWithBytes((const uint8_t *)inputData_.data(), inputData_.size()));
            const auto outputTWData_ = WRAPD(TWLiquidStakingBuildRequest(inputTWData_.get()));
            const auto outputData = data(TWDataBytes(outputTWData_.get()), TWDataSize(outputTWData_.get()));
            EXPECT_EQ(outputData.size(), 99ul);
            Proto::Output outputProto;
            EXPECT_TRUE(outputProto.ParseFromArray(outputData.data(), static_cast<int>(outputData.size())));
            ASSERT_TRUE(outputProto.has_ethereum());
            ASSERT_EQ(outputProto.status().code(), Proto::OK);
            auto eth_tx = *ls_output.mutable_ethereum();
            ASSERT_TRUE(eth_tx.transaction().has_contract_generic());
            ASSERT_EQ(hex(eth_tx.transaction().contract_generic().data(), true), "0xa1903eab0000000000000000000000000000000000000000000000000000000000000000");
            fill_tx_functor(eth_tx);
            Ethereum::Proto::SigningOutput output;
            ANY_SIGN(tx, TWCoinTypeEthereum);
            EXPECT_EQ(hex(output.encoded()), "02f89701808459682f008519634613d48301c96594ae7ab96520de3a18e5e111b5eaab095312d7fe8487038d7ea4c68000a4a1903eab0000000000000000000000000000000000000000000000000000000000000000c001a0daace8c05277cf7eaff3f03a4e1ba7edadab20407674d1b659f5c13f89a04087a0528b5d4499b67a50989b6397e530be23515532732b54d60c4a4d726e499e046a");
            // Successfully broadcasted https://etherscan.io/tx/0x4d509fd50f474a568419ade4df13b43943b5c8233e980d2217784c512941b3bd
        }
    }
}
