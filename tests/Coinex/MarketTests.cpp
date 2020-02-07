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

TEST(CoinexMarket, CreateSellOrder) {
    auto input = Proto::SigningInput();
    input.set_account_number(35);
    input.set_chain_id("coinexdex-test2001");
    input.set_memo("ifwallet");
    input.set_sequence(1);

    auto& message = *input.mutable_create_order_message();
    message.set_sender("cettest1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0dsez0j");
    message.set_identify(1);
    message.set_trading_pair("ift/cet");
    message.set_order_type(2);
    message.set_price_precision(2);
    message.set_price("100000");
    message.set_quantity("10000");
    message.set_side(2); // 1-buy, 2-sell
    message.set_time_in_force("3");
    message.set_exist_blocks("10000");

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

TEST(CoinexMarket, CreateBuyOrder) {
    auto input = Proto::SigningInput();
    input.set_account_number(35);
    input.set_chain_id("coinexdex-test2001");
    input.set_memo("ifwallet");
    input.set_sequence(2);

    auto& message = *input.mutable_create_order_message();
    message.set_sender("cettest1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0dsez0j");
    message.set_identify(1);
    message.set_trading_pair("ift/cet");
    message.set_order_type(2);
    message.set_price_precision(2);
    message.set_price("100000");
    message.set_quantity("10000");
    message.set_side(1); // 1-buy, 2-sell
    message.set_time_in_force("3");
    message.set_exist_blocks("10000");

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

TEST(CoinexMarket, CancelOrder) {
    auto input = Proto::SigningInput();
    input.set_account_number(30);
    input.set_chain_id("coinexdex-test2000");
    input.set_memo("ifwallet");
    input.set_sequence(6);

    auto& message = *input.mutable_cancel_order_message();
    message.set_order_id("cettest1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0dsez0j-5");
    message.set_sender("cettest1hkfq3zahaqkkzx5mjnamwjsfpq2jk7z0dsez0j");

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

TEST(CoinexMarket, ProposalVote) {
    auto input = Proto::SigningInput();
    input.set_account_number(24);
    input.set_chain_id("coinexdex-test2006");
    input.set_memo("ifwallet");
    input.set_sequence(183);

    auto& message = *input.mutable_proposal_vote_message();
    message.set_voter("cettest178w9m7yzkagpq090t593r6gqe5gkfkvjl76znl");
    message.set_proposal_id("14");
    message.set_option("Yes");

    auto &fee = *input.mutable_fee();
    fee.set_gas(100000);
    auto amountOfFee = fee.add_amounts();
    amountOfFee->set_denom("cet");
    amountOfFee->set_amount(2000000);

    auto privateKey = parse_hex("c4c4f50e9af57eb281f07fb5e4ebb76d7ca1132549ffaa3aba3862fe58653244");
    input.set_private_key(privateKey.data(), privateKey.size());

    auto signer = Coinex::Signer(std::move(input));
    auto signature = signer.sign();
    auto signatureInBase64 = Base64::encode(signature);

    auto output = signer.build();

    ASSERT_EQ("", output.json());

}


}
