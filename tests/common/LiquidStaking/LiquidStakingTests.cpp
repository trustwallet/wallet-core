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
#include <gtest/gtest.h>
#include "TestUtilities.h"

namespace TW::LiquidStaking::tests {
    TEST(LiquidStaking, Coverage) {
        auto output = LiquidStaking::generateError(Proto::OK);
        ASSERT_EQ(output.code(), Proto::OK);
    }

    TEST(LiquidStaking, ErrorActionNotSet) {
        Proto::LiquidStakingInput input;
        auto output = build(input);
        ASSERT_EQ(output.status().code(), Proto::ERROR_ACTION_NOT_SET);
    }

    TEST(LiquidStaking, PolygonStraderStakeMatic) {
        Proto::LiquidStakingInput input;
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

        Ethereum::Proto::SigningOutput output;
        ANY_SIGN(tx, TWCoinTypePolygon);
        EXPECT_EQ(hex(output.encoded()), "02f87a81890185085e42c7c0858fbcc8fcd88301c52094fd225c9e6601c9d38d8f98d8731bf59efcf8c0e3880de0b6b3a764000084c78cf1a0c001a04bcf92394d53d4908130cc6d4f7b2491967f9d6c59292b84c1f56adc49f6c458a073e09f45d64078c41a7946ffdb1dee8e604eb76f318088490f8f661bb7ddfc54");
        // Successfully broadcasted https://polygonscan.com/tx/0x0f6c4f7a893c3f08be30d2ea24479d7ed4bdba40875d07cfd607cf97980b7cf0
    }
}
