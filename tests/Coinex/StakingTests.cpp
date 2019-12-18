// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"
#include "HDWallet.h"
#include "HexCoding.h"
#include "Base64.h"
#include "proto/Coinex.pb.h"
#include "Cosmos/Address.h"
#include "Coinex/Signer.h"

#include <TrustWalletCore/TWHRP.h>
#include <gtest/gtest.h>

namespace TW::Coinex {

TEST(CoinexStaking, Staking) {
    auto input = Proto::SigningInput();
    input.set_account_number(35);
    input.set_chain_id("coinexdex-test2001");
    input.set_memo("ifwallet");
    input.set_sequence(5);

    auto& message = *input.mutable_stake_message();
    message.set_delegator_address("cettest1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0dsez0j");
    message.set_validator_address("cettestvaloper1hpgvshhx4t3mm0t54v5d7xu0qmw675nnul5nhj");
    auto& amountOfTx = *message.mutable_amount();
    amountOfTx.set_denom("cet");
    amountOfTx.set_amount(300000000000); // 3000 CET

    auto &fee = *input.mutable_fee();
    fee.set_gas(100000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("cet");
    amountOfFee->set_amount(2000000);

    auto privateKey = parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto signer = Coinex::Signer(std::move(input));
    auto signature = signer.sign();
    auto signatureInBase64 = Base64::encode(signature);

    auto output = signer.build();

    ASSERT_EQ("", output.json());

    //ASSERT_EQ("a1627478a563666565a266616d6f756e7481a266616d6f756e74633430306564656e6f6d6363657463676173653130303030646d656d6f68696677616c6c6574636d736781a2647479706576636f736d6f732d73646b2f4d736744656c65676174656576616c7565a366616d6f756e74a266616d6f756e7464313030306564656e6f6d636365747164656c656761746f725f61646472657373782d636f696e657831686b6671337a616861716b6b7a78356d6a6e616d776a73667071326a6b377a306a75777876727176616c696461746f725f616464726573737834636f696e657876616c6f7065723170786633736763787075393965346861766d65757337676d6d743875336a396a7076376a34646a7369676e61747572657381a2677075625f6b6579a26474797065781a74656e6465726d696e742f5075624b6579536563703235366b316576616c7565782c416b76436f784a6c465438483577344c71776879546d7546346866347a574b4d36324b58516b653753544f43697369676e61747572657858577158537a61713476486b7a423341373161697a6b6474447867665867315057656e7065574e672f767263654b6b6a746637376a394372676b636d483762545643695a495670623952784a77574733754865564674513d3d647479706572636f736d6f732d73646b2f4d736753656e64", hex(output.encoded()));

    // https://testnet.coinex.org/transactions/0ECD0F2930F7EBDD8B8B0B3DCF641E94EC0A3C94539EFF14C9D828B6F31FF055
}

TEST(CoinexStaking, WithdrawStaking) {
    auto input = Proto::SigningInput();
    input.set_account_number(30);
    input.set_chain_id("coinexdex-test2000");
    input.set_memo("ifwallet");
    input.set_sequence(9);

    auto& message = *input.mutable_withdraw_stake_reward_message();
    message.set_delegator_address("cettest1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0dsez0j");
    message.set_validator_address("cettestvaloper1ykaa52ajdxqeu2248vhk5j5k5qqtwtwdtlew7c");

    auto &fee = *input.mutable_fee();
    fee.set_gas(100000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("cet");
    amountOfFee->set_amount(2000000);

    auto privateKey = parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto signer = Coinex::Signer(std::move(input));
    auto signature = signer.sign();
    auto signatureInBase64 = Base64::encode(signature);

    auto output = signer.build();

    ASSERT_EQ("", output.json());

}

TEST(CoinexStaking, UnStaking) {
    auto input = Proto::SigningInput();
    input.set_account_number(30);
    input.set_chain_id("coinexdex-test2000");
    input.set_memo("ifwallet");
    input.set_sequence(11);

    auto& message = *input.mutable_unstake_message();
    message.set_delegator_address("cettest1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0dsez0j");
    message.set_validator_address("cettestvaloper1ykaa52ajdxqeu2248vhk5j5k5qqtwtwdtlew7c");
    auto& amountOfTx = *message.mutable_amount();
    amountOfTx.set_denom("cet");
    amountOfTx.set_amount(10000000000);

    auto &fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("cet");
    amountOfFee->set_amount(4000000);

    auto privateKey = parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto signer = Coinex::Signer(std::move(input));
    auto signature = signer.sign();
    auto signatureInBase64 = Base64::encode(signature);

    auto output = signer.build();

    ASSERT_EQ("", output.json());

}

TEST(CoinexStaking, Redelegate) {
    auto input = Proto::SigningInput();
    input.set_account_number(35);
    input.set_chain_id("coinexdex-test2001");
    input.set_memo("ifwallet");
    input.set_sequence(6);

    auto& message = *input.mutable_redelegate_message();
    message.set_delegator_address("cettest1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0dsez0j");
    message.set_validator_src_address("cettestvaloper1hpgvshhx4t3mm0t54v5d7xu0qmw675nnul5nhj");
    message.set_validator_dst_address("cettestvaloper1337ry5hvgzg75a2hj9v0m9ead2n894l5alf7se");
    auto& amountOfTx = *message.mutable_amount();
    amountOfTx.set_denom("cet");
    amountOfTx.set_amount(100000000000); // 1000 CET

    auto &fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("cet");
    amountOfFee->set_amount(4000000);

    auto privateKey = parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto signer = Coinex::Signer(std::move(input));
    auto signature = signer.sign();
    auto signatureInBase64 = Base64::encode(signature);

    auto output = signer.build();

    ASSERT_EQ("", output.json());

}

TEST(CoinexStaking, WithdrawStakeRewardAndRestake) {
    auto input = Proto::SigningInput();
    input.set_account_number(35);
    input.set_chain_id("coinexdex-test2001");
    input.set_memo("ifwallet");
    input.set_sequence(7);

    auto& message = *input.mutable_withdraw_stake_reward_and_restake_message();
    message.set_delegator_address("cettest1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0dsez0j");
    message.set_validator_address("cettestvaloper1337ry5hvgzg75a2hj9v0m9ead2n894l5alf7se");
    auto& amountOfTx = *message.mutable_amount();
    amountOfTx.set_denom("cet");
    amountOfTx.set_amount(100);

    auto &fee = *input.mutable_fee();
    fee.set_gas(200000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("cet");
    amountOfFee->set_amount(4000000);

    auto privateKey = parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto signer = Coinex::Signer(std::move(input));
    auto signature = signer.sign();
    auto signatureInBase64 = Base64::encode(signature);

    auto output = signer.build();

    ASSERT_EQ("", output.json());

}

}
