// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bech32Address.h"
#include "Binance/Address.h"
#include "Binance/Signer.h"
#include "Coin.h"
#include "Ethereum/Address.h"
#include "HDWallet.h"
#include "HexCoding.h"
#include "proto/Binance.pb.h"

#include <gtest/gtest.h>

namespace TW::Binance {

TEST(BinanceSigner, Sign) {
    auto input = Proto::SigningInput();
    input.set_chain_id("chain-bnb");
    input.set_account_number(12);
    input.set_sequence(35);
    input.set_memo("");
    input.set_source(1);

    auto key = parse_hex("90335b9d2153ad1a9799a3ccc070bd64b4164e9642ee1dd48053c33f9a3a05e9");
    input.set_private_key(key.data(), key.size());

    auto& order = *input.mutable_trade_order();
    Binance::Address address;
    ASSERT_TRUE(Binance::Address::decode("bnb1hgm0p7khfk85zpz5v0j8wnej3a90w709vhkdfu", address));
    auto keyhash = address.getKeyHash();
    order.set_sender(keyhash.data(), keyhash.size());
    order.set_id("BA36F0FAD74D8F41045463E4774F328F4AF779E5-36");
    order.set_symbol("NNB-338_BNB");
    order.set_ordertype(2);
    order.set_side(1);
    order.set_price(136350000);
    order.set_quantity(100000000);
    order.set_timeinforce(1);

    auto signer = Binance::Signer(std::move(input));
    auto signature = signer.sign();

    ASSERT_EQ(hex(signature),
              "9123cb6906bb20aeb753f4a121d4d88ff0e9750ba75b0c4e10d76caee1e7d2481290fa3b9887a6225d69"
              "97f5f939ef834ea61d596a314237c48e560da9e17b5a");
}

TEST(BinanceSigner, Build) {
    auto input = Proto::SigningInput();
    input.set_chain_id("chain-bnb");
    input.set_account_number(1);
    input.set_sequence(10);

    auto key = parse_hex("90335b9d2153ad1a9799a3ccc070bd64b4164e9642ee1dd48053c33f9a3a05e9");
    input.set_private_key(key.data(), key.size());

    auto& order = *input.mutable_trade_order();
    auto address = Binance::Address(parse_hex("b6561dcc104130059a7c08f48c64610c1f6f9064"));
    auto keyhash = address.getKeyHash();
    order.set_sender(keyhash.data(), keyhash.size());
    order.set_id("B6561DCC104130059A7C08F48C64610C1F6F9064-11");
    order.set_symbol("BTC-5C4_BNB");
    order.set_ordertype(2);
    order.set_side(1);
    order.set_price(100000000);
    order.set_quantity(1200000000);
    order.set_timeinforce(1);

    auto signer = Binance::Signer(std::move(input));
    auto result = signer.build();

    ASSERT_EQ(hex(result), "db01"
        "f0625dee"
        "0a65"
            "ce6dc043"
            "0a14""b6561dcc104130059a7c08f48c64610c1f6f9064"
            "122b""423635363144434331303431333030353941374330384634384336343631304331463646393036342d3131"
            "1a0b""4254432d3543345f424e42"
            "2002"
            "2801"
            "3080c2d72f"
            "3880989abc04"
            "4001"
        "126e"
            "0a26"
            "eb5ae987"
            "21029729a52e4e3c2b4a4e52aa74033eedaf8ba1df5ab6d1f518fd69e67bbd309b0e"
            "1240""2a78b6d9a108eb9440221802b626e24d80179395ac984f016db012ef1a0c16d71b4d7053e05366ae3ea2681fc8052398fda20551c965d74c5970bbc66b94b48e"
            "1801"
            "200a"
    );
}

TEST(BinanceSigner, BuildSend) {
    auto signingInput = Proto::SigningInput();
    signingInput.set_chain_id("chain-bnb");
    signingInput.set_account_number(19);
    signingInput.set_sequence(23);
    signingInput.set_memo("test");
    signingInput.set_source(1);

    auto key = parse_hex("95949f757db1f57ca94a5dff23314accbe7abee89597bf6a3c7382c84d7eb832");
    signingInput.set_private_key(key.data(), key.size());

    auto& order = *signingInput.mutable_send_order();

    auto fromKeyhash = parse_hex("40c2979694bbc961023d1d27be6fc4d21a9febe6");
    auto fromAddress = Binance::Address(fromKeyhash);

    auto toKeyhash = parse_hex("88b37d5e05f3699e2a1406468e5d87cb9dcceb95");
    auto toAddress = Binance::Address(toKeyhash);

    auto input = order.add_inputs();
    input->set_address(fromKeyhash.data(), fromKeyhash.size());
    auto inputCoin = input->add_coins();
    inputCoin->set_denom("BNB");
    inputCoin->set_amount(1'001'000'000);

    auto output = order.add_outputs();
    output->set_address(toKeyhash.data(), toKeyhash.size());
    auto outputCoin = output->add_coins();
    outputCoin->set_denom("BNB");
    outputCoin->set_amount(1'001'000'000);

    auto signer = Binance::Signer(std::move(signingInput));
    auto signature = signer.sign();
    ASSERT_EQ(hex(signature),
              "c65a13440f18a155bd971ee40b9e0dd58586f5bf344e12ec4c76c439aebca8c7789bab7bfbfb4ce89aad"
              "c4a02df225b6b6efc861c13bbeb5f7a3eea2d7ffc80f");

    auto result = signer.build();

    ASSERT_EQ(hex(result), "cc01"
        "f0625dee"
        "0a4e"
            "2a2c87fa"
            "0a23""0a1440c2979694bbc961023d1d27be6fc4d21a9febe6120b0a03424e4210c098a8dd03"
            "1223""0a1488b37d5e05f3699e2a1406468e5d87cb9dcceb95120b0a03424e4210c098a8dd03"
        "126e"
            "0a26"
            "eb5ae987"
            "21026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38e502"
            "1240""c65a13440f18a155bd971ee40b9e0dd58586f5bf344e12ec4c76c439aebca8c7789bab7bfbfb4ce89aadc4a02df225b6b6efc861c13bbeb5f7a3eea2d7ffc80f"
            "1813"
            "2017"
        "1a04""74657374"
        "2001"
    );
}

TEST(BinanceSigner, BuildSend2) {
    const auto derivationPath = TW::derivationPath(TWCoinTypeBinance);

    const auto fromWallet = HDWallet("swift slam quote sail high remain mandate sample now stamp title among fiscal captain joy puppy ghost arrow attract ozone situate install gain mean", "");
    const auto fromPrivateKey = fromWallet.getKey(TWCoinTypeBinance, derivationPath);
    const auto fromPublicKey = PublicKey(fromPrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1));

    const auto toWallet = HDWallet( "bottom quick strong ranch section decide pepper broken oven demand coin run jacket curious business achieve mule bamboo remain vote kid rigid bench rubber", "");
    const auto toPrivateKey = toWallet.getKey(TWCoinTypeBinance, derivationPath);
    const auto toPublicKey = PublicKey(toPrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1));

    auto signingInput = Proto::SigningInput();
    signingInput.set_chain_id("bnbchain-1000");
    signingInput.set_account_number(0);
    signingInput.set_sequence(1);
    signingInput.set_private_key(fromPrivateKey.bytes.data(), fromPrivateKey.bytes.size());

    auto token = Proto::SendOrder::Token();
    token.set_denom("BNB");
    token.set_amount(100000000000000);

    auto input = Proto::SendOrder::Input();
    auto fromKeyHash = Binance::Address(fromPublicKey).getKeyHash();
    input.set_address(fromKeyHash.data(), fromKeyHash.size());
    *input.add_coins() = token;

    auto output = Proto::SendOrder::Output();
    auto toKeyHash = Binance::Address(toPublicKey).getKeyHash();
    output.set_address(toKeyHash.data(), toKeyHash.size());
    *output.add_coins() = token;

    auto sendOrder = Proto::SendOrder();
    *sendOrder.add_inputs() = input;
    *sendOrder.add_outputs() = output;

    *signingInput.mutable_send_order() = sendOrder;

    const auto data = Signer(std::move(signingInput)).build();
    ASSERT_EQ(hex(data),
        "c601"
        "f0625dee"
        "0a52"
            "2a2c87fa"
            "0a25""0a141d0e3086e8e4e0a53c38a90d55bd58b34d57d2fa120d0a03424e42108080e983b1de16"
            "1225""0a146b571fc0a9961a7ddf45e49a88a4d83941fcabbe120d0a03424e42108080e983b1de16"
        "126c"
            "0a26"
            "eb5ae987"
            "21027e69d96640300433654e016d218a8d7ffed751023d8efe81e55dedbd6754c971"
            "1240""8b23eecfa8237a27676725173e58154e6c204bb291b31c3b7b507c8f04e2773909ba70e01b54f4bd0bc76669f5712a5a66b9508acdf3aa5e4fde75fbe57622a1"
            "2001"
    );
}

TEST(BinanceSigner, BuildHTLT) {
    const auto fromPrivateKey =
        PrivateKey(parse_hex("eeba3f6f2db26ced519a3d4c43afff101db957a21d54d25dc7fd235c404d7a5d"));
    const auto fromPublicKey = PublicKey(fromPrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    auto fromAddr = Binance::Address(fromPublicKey).getKeyHash();

    const auto toPrivateKey =
        PrivateKey(parse_hex("851fab89c14f4bbec0cc06f5e445ec065efc641068d78b308c67217d9bd5c88a"));
    const auto toPublicKey = PublicKey(toPrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    auto toAddr = Binance::Address(toPublicKey).getKeyHash();

    auto signingInput = Proto::SigningInput();
    signingInput.set_chain_id("test-chain");
    signingInput.set_account_number(15);
    signingInput.set_sequence(0);
    signingInput.set_private_key(fromPrivateKey.bytes.data(), fromPrivateKey.bytes.size());

    auto token = Proto::SendOrder::Token();
    token.set_denom("BNB");
    token.set_amount(100000000);

    auto randomNumberHash =
        parse_hex("e8eae926261ab77d018202434791a335249b470246a7b02e28c3b2fb6ffad8f3");

    auto& htltOrder = *signingInput.mutable_htlt_order();
    htltOrder.set_from(fromAddr.data(), fromAddr.size());
    htltOrder.set_to(toAddr.data(), toAddr.size());
    htltOrder.set_recipient_other_chain("");
    htltOrder.set_sender_other_chain("");
    *htltOrder.add_amount() = token;
    htltOrder.set_height_span(400);
    htltOrder.set_expected_income("100000000:BTC-1DC");
    htltOrder.set_timestamp(1567746273);
    htltOrder.set_random_number_hash(randomNumberHash.data(), randomNumberHash.size());
    htltOrder.set_cross_chain(false);

    const auto data = Binance::Signer(std::move(signingInput)).build();
    ASSERT_EQ(hex(data),
              "ee01f0625dee0a7ab33f9a240a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e112140153f11d6db7"
              "e69c7d51e771c697378018fb6c242a20e8eae926261ab77d018202434791a335249b470246a7b02e28c3"
              "b2fb6ffad8f330e1d1c7eb053a0a0a03424e421080c2d72f42113130303030303030303a4254432d3144"
              "43489003126c0a26eb5ae9872103a9a55c040c8eb8120f3d1b32193250841c08af44ea561aac993dbe0f"
              "6b6a8fc7124051439de2da19fe9fd22137c903cfc5dc87553bf05dca0bb202c0e07c47f9b51269efa272"
              "43eb7b55888f5384a84ac1eac6d325c830d1be0ed042838e2dc0f6a9180f");
}

TEST(BinanceSigner, BuildDepositHTLT) {
    const auto fromPrivateKey =
        PrivateKey(parse_hex("851fab89c14f4bbec0cc06f5e445ec065efc641068d78b308c67217d9bd5c88a"));
    const auto fromPublicKey = PublicKey(fromPrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    auto fromAddr = Binance::Address(fromPublicKey).getKeyHash();

    auto signingInput = Proto::SigningInput();
    signingInput.set_chain_id("test-chain");
    signingInput.set_account_number(16);
    signingInput.set_sequence(0);
    signingInput.set_private_key(fromPrivateKey.bytes.data(), fromPrivateKey.bytes.size());

    auto token = Proto::SendOrder::Token();
    token.set_denom("BTC-1DC");
    token.set_amount(100000000);

    auto swapID = parse_hex("dd8fd4719741844d35eb35ddbeca9531d5493a8e4667689c55e73c77503dd9e5");

    auto& depositHTLTOrder = *signingInput.mutable_deposithtlt_order();
    depositHTLTOrder.set_from(fromAddr.data(), fromAddr.size());
    depositHTLTOrder.set_swap_id(swapID.data(), swapID.size());
    *depositHTLTOrder.add_amount() = token;

    const auto data = Binance::Signer(std::move(signingInput)).build();
    ASSERT_EQ(hex(data),
              "c001f0625dee0a4c639864960a140153f11d6db7e69c7d51e771c697378018fb6c24120e0a074254432d"
              "3144431080c2d72f1a20dd8fd4719741844d35eb35ddbeca9531d5493a8e4667689c55e73c77503dd9e5"
              "126c0a26eb5ae98721038df6960084e20b2d07d50e1422f94105c6241d9f1482a4eb79ce8bfd460f19e4"
              "12400ca4144c6818e2836d09b4faf3161781d85f9adfc00badb2eaa0953174610a233b81413dafcf8471"
              "6abad48a4ed3aeb9884d90eb8416eec5d5c0c6930ab60bd01810");
}

TEST(BinanceSigner, BuildClaimHTLT) {
    const auto fromPrivateKey =
        PrivateKey(parse_hex("eeba3f6f2db26ced519a3d4c43afff101db957a21d54d25dc7fd235c404d7a5d"));
    const auto fromPublicKey = PublicKey(fromPrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    auto fromAddr = Binance::Address(fromPublicKey).getKeyHash();

    auto signingInput = Proto::SigningInput();
    signingInput.set_chain_id("test-chain");
    signingInput.set_account_number(15);
    signingInput.set_sequence(1);
    signingInput.set_private_key(fromPrivateKey.bytes.data(), fromPrivateKey.bytes.size());

    auto randomNumber =
        parse_hex("bda6933c7757d0ca428aa01fb9d0935a231f87bf2deeb9b409cea3f2d580a2cc");
    auto swapID = parse_hex("dd8fd4719741844d35eb35ddbeca9531d5493a8e4667689c55e73c77503dd9e5");

    auto& claimHTLTOrder = *signingInput.mutable_claimhtlt_order();
    claimHTLTOrder.set_from(fromAddr.data(), fromAddr.size());
    claimHTLTOrder.set_swap_id(swapID.data(), swapID.size());
    claimHTLTOrder.set_random_number(randomNumber.data(), randomNumber.size());

    const auto data = Binance::Signer(std::move(signingInput)).build();
    ASSERT_EQ(
        hex(data),
        "d401f0625dee0a5ec16653000a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e11220dd8fd4719741844d35"
        "eb35ddbeca9531d5493a8e4667689c55e73c77503dd9e51a20bda6933c7757d0ca428aa01fb9d0935a231f87bf"
        "2deeb9b409cea3f2d580a2cc126e0a26eb5ae9872103a9a55c040c8eb8120f3d1b32193250841c08af44ea561a"
        "ac993dbe0f6b6a8fc71240fa30ba50111aa31d8329dacb6d044c1c7d54f1cb782bc9aa2a50c3fabce02a4579d7"
        "5b76ca69a9fab11b676d9da66b5af7aa4c9ad3d18e24fffeb16433be39fb180f2001");
}

TEST(BinanceSigner, BuildRefundHTLT) {
    const auto fromPrivateKey =
        PrivateKey(parse_hex("eeba3f6f2db26ced519a3d4c43afff101db957a21d54d25dc7fd235c404d7a5d"));
    const auto fromPublicKey = PublicKey(fromPrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    auto fromAddr = Binance::Address(fromPublicKey).getKeyHash();

    auto signingInput = Proto::SigningInput();
    signingInput.set_chain_id("test-chain");
    signingInput.set_account_number(15);
    signingInput.set_sequence(1);
    signingInput.set_private_key(fromPrivateKey.bytes.data(), fromPrivateKey.bytes.size());

    auto swapID = parse_hex("dd8fd4719741844d35eb35ddbeca9531d5493a8e4667689c55e73c77503dd9e5");

    auto& refundHTLTOrder = *signingInput.mutable_refundhtlt_order();
    refundHTLTOrder.set_from(fromAddr.data(), fromAddr.size());
    refundHTLTOrder.set_swap_id(swapID.data(), swapID.size());

    const auto data = Binance::Signer(std::move(signingInput)).build();
    ASSERT_EQ(hex(data),
              "b201f0625dee0a3c3454a27c0a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e11220dd8fd4719741"
              "844d35eb35ddbeca9531d5493a8e4667689c55e73c77503dd9e5126e0a26eb5ae9872103a9a55c040c8e"
              "b8120f3d1b32193250841c08af44ea561aac993dbe0f6b6a8fc71240c9f36142534d16ec8ce656f8eb73"
              "70b32206a2d15198b7165acf1e2a18952c9e4570b0f862e1ab7bb868c30781a42c9e3ec0ae08982e8d6c"
              "91c55b83c71b7b1e180f2001");
}

TEST(BinanceSigner, BuildIssueOrder) {
    const auto fromPrivateKey =
        PrivateKey(parse_hex("eeba3f6f2db26ced519a3d4c43afff101db957a21d54d25dc7fd235c404d7a5d"));
    const auto fromPublicKey = PublicKey(fromPrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    const Data fromAddr = Binance::Address(fromPublicKey).getKeyHash();

    auto signingInput = Proto::SigningInput();
    signingInput.set_chain_id("test-chain");
    signingInput.set_account_number(15);
    signingInput.set_sequence(1);
    signingInput.set_private_key(fromPrivateKey.bytes.data(), fromPrivateKey.bytes.size());

    auto& issueOrder = *signingInput.mutable_issue_order();
    issueOrder.set_from(fromAddr.data(), fromAddr.size());
    issueOrder.set_name("NewBinanceToken");
    issueOrder.set_symbol("NNB-338_BNB");
    issueOrder.set_total_supply(1000000000);
    issueOrder.set_mintable(true);

    const auto data = Binance::Signer(std::move(signingInput)).build();
    ASSERT_EQ(hex(data),
              "b601f0625dee0a40"
              "17efab80"
              "0a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e1120f"
              "4e657742696e616e6365546f6b656e"
              "1a0b"
              "4e4e422d3333385f424e42"
        "208094ebdc032801126e0a26eb5ae9872103a9a55c040c8eb8120f3d1b32193250841c08af44ea561aac993dbe0f6b6a8fc712401fbb993d643f03b3e8e757a502035f58c4c45aaaa6e107a3059ab7c6164283c10f1254e87feee21477c64c87b1a27d8481048533ae2f685b3ac0dc66e4edbc0b180f2001"
    );
}

TEST(BinanceSigner, BuildMintOrder) {
    const auto fromPrivateKey =
        PrivateKey(parse_hex("eeba3f6f2db26ced519a3d4c43afff101db957a21d54d25dc7fd235c404d7a5d"));
    const auto fromPublicKey = PublicKey(fromPrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    const Data fromAddr = Binance::Address(fromPublicKey).getKeyHash();

    auto signingInput = Proto::SigningInput();
    signingInput.set_chain_id("test-chain");
    signingInput.set_account_number(15);
    signingInput.set_sequence(1);
    signingInput.set_private_key(fromPrivateKey.bytes.data(), fromPrivateKey.bytes.size());

    auto& mintOrder = *signingInput.mutable_mint_order();
    mintOrder.set_from(fromAddr.data(), fromAddr.size());
    mintOrder.set_symbol("NNB-338_BNB");
    mintOrder.set_amount(1000000);

    const auto data = Binance::Signer(std::move(signingInput)).build();
    ASSERT_EQ(hex(data),
              "a101f0625dee0a2b"
              "467e0829"
              "0a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e1120b"
              "4e4e422d3333385f424e42"
        "18c0843d126e0a26eb5ae9872103a9a55c040c8eb8120f3d1b32193250841c08af44ea561aac993dbe0f6b6a8fc71240e3022069d897bf5bf4846d354fcd2c0e85807053be643c8b8c8596306003f7340d43a162722673eb848258b0435b1f49993d0e75d4ae43d03453a3ae57fe6991180f2001"
    );
}

TEST(BinanceSigner, BuildBurnOrder) {
    const auto fromPrivateKey =
        PrivateKey(parse_hex("eeba3f6f2db26ced519a3d4c43afff101db957a21d54d25dc7fd235c404d7a5d"));
    const auto fromPublicKey = PublicKey(fromPrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    const Data fromAddr = Binance::Address(fromPublicKey).getKeyHash();

    auto signingInput = Proto::SigningInput();
    signingInput.set_chain_id("test-chain");
    signingInput.set_account_number(15);
    signingInput.set_sequence(1);
    signingInput.set_private_key(fromPrivateKey.bytes.data(), fromPrivateKey.bytes.size());

    auto& burnOrder = *signingInput.mutable_burn_order();
    burnOrder.set_from(fromAddr.data(), fromAddr.size());
    burnOrder.set_symbol("NNB-338_BNB");
    burnOrder.set_amount(1000000);

    const auto data = Binance::Signer(std::move(signingInput)).build();
    ASSERT_EQ(hex(data),
              "a101f0625dee0a2b"
              "7ed2d2a0"
              "0a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e1120b"
              "4e4e422d3333385f424e42"
        "18c0843d126e0a26eb5ae9872103a9a55c040c8eb8120f3d1b32193250841c08af44ea561aac993dbe0f6b6a8fc71240e3022069d897bf5bf4846d354fcd2c0e85807053be643c8b8c8596306003f7340d43a162722673eb848258b0435b1f49993d0e75d4ae43d03453a3ae57fe6991180f2001"
    );
}

TEST(BinanceSigner, BuildFreezeOrder) {
    const auto fromPrivateKey =
        PrivateKey(parse_hex("eeba3f6f2db26ced519a3d4c43afff101db957a21d54d25dc7fd235c404d7a5d"));
    const auto fromPublicKey = PublicKey(fromPrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    const Data fromAddr = Binance::Address(fromPublicKey).getKeyHash();

    auto signingInput = Proto::SigningInput();
    signingInput.set_chain_id("test-chain");
    signingInput.set_account_number(15);
    signingInput.set_sequence(1);
    signingInput.set_private_key(fromPrivateKey.bytes.data(), fromPrivateKey.bytes.size());

    auto& freezeOrder = *signingInput.mutable_freeze_order();
    freezeOrder.set_from(fromAddr.data(), fromAddr.size());
    freezeOrder.set_symbol("NNB-338_BNB");
    freezeOrder.set_amount(1000000);

    const auto data = Binance::Signer(std::move(signingInput)).build();
    ASSERT_EQ(hex(data),
              "a101f0625dee0a2b"
              "e774b32d"
              "0a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e1120b"
              "4e4e422d3333385f424e42"
              "18c0843d126e0a26eb5ae9872103a9a55c040c8eb8120f3d1b32193250841c08af44ea561aac993dbe0f"
              "6b6a8fc71240e3022069d897bf5bf4846d354fcd2c0e85807053be643c8b8c8596306003f7340d43a162"
              "722673eb848258b0435b1f49993d0e75d4ae43d03453a3ae57fe6991180f2001");
}

TEST(BinanceSigner, BuildUnfreezeOrder) {
    const auto fromPrivateKey =
        PrivateKey(parse_hex("eeba3f6f2db26ced519a3d4c43afff101db957a21d54d25dc7fd235c404d7a5d"));
    const auto fromPublicKey = PublicKey(fromPrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    const Data fromAddr = Binance::Address(fromPublicKey).getKeyHash();

    auto signingInput = Proto::SigningInput();
    signingInput.set_chain_id("test-chain");
    signingInput.set_account_number(15);
    signingInput.set_sequence(1);
    signingInput.set_private_key(fromPrivateKey.bytes.data(), fromPrivateKey.bytes.size());

    auto& unfreezeOrder = *signingInput.mutable_unfreeze_order();
    unfreezeOrder.set_from(fromAddr.data(), fromAddr.size());
    unfreezeOrder.set_symbol("NNB-338_BNB");
    unfreezeOrder.set_amount(1000000);

    const auto data = Binance::Signer(std::move(signingInput)).build();
    ASSERT_EQ(hex(data),
              "a101f0625dee0a2b"
              "6515ff0d"
              "0a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e1120b"
              "4e4e422d3333385f424e42"
              "18c0843d126e0a26eb5ae9872103a9a55c040c8eb8120f3d1b32193250841c08af44ea561aac993dbe0f"
              "6b6a8fc71240e3022069d897bf5bf4846d354fcd2c0e85807053be643c8b8c8596306003f7340d43a162"
              "722673eb848258b0435b1f49993d0e75d4ae43d03453a3ae57fe6991180f2001");
}

TEST(BinanceSigner, BuildTransferOutOrder) {
    const auto fromPrivateKey =
        PrivateKey(parse_hex("eeba3f6f2db26ced519a3d4c43afff101db957a21d54d25dc7fd235c404d7a5d"));
    const auto fromPublicKey = PublicKey(fromPrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    const Data fromAddr = Binance::Address(fromPublicKey).getKeyHash();

    const auto toAddr = Ethereum::Address("0x35552c16704d214347f29Fa77f77DA6d75d7C752");
    const auto toBytes = Data(toAddr.bytes.begin(), toAddr.bytes.end());

    auto input = Proto::SigningInput();
    input.set_chain_id("test-chain");
    input.set_account_number(15);
    input.set_sequence(1);
    input.set_private_key(fromPrivateKey.bytes.data(), fromPrivateKey.bytes.size());

    auto& order = *input.mutable_transfer_out_order();
    order.set_from(fromAddr.data(), fromAddr.size());
    order.set_to(toBytes.data(), toBytes.size());
    order.set_expire_time(12345678);

    auto& token = *order.mutable_amount();
    token.set_denom("BNB");
    token.set_amount(100000000);

    const auto data = Binance::Signer(std::move(input)).build();
    ASSERT_EQ(hex(data),
              "b701f0625dee0a41800819c00a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e1121435552c16704d"
              "214347f29fa77f77da6d75d7c7521a0a0a03424e421080c2d72f20cec2f105126e0a26eb5ae9872103a9"
              "a55c040c8eb8120f3d1b32193250841c08af44ea561aac993dbe0f6b6a8fc712407eda148e1167b1be12"
              "71a788ccf4e3eade1c7e1773e9d2093982d7f802f8f85f35ef550049011728206e4eda1a272f9e96fd95"
              "ef3983cad85a29cd14262c22e0180f2001");
}

TEST(BinanceSigner, BuildSideChainDelegate) {
    const auto fromPrivateKey =
        PrivateKey(parse_hex("eeba3f6f2db26ced519a3d4c43afff101db957a21d54d25dc7fd235c404d7a5d"));
    const auto fromPublicKey = PublicKey(fromPrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    const Data fromAddr = Binance::Address(fromPublicKey).getKeyHash();
    auto validator = Bech32Address("");
    Bech32Address::decode("bva10npy5809y303f227g4leqw7vs3s6ep5ul26sq2", validator, "bva");

    auto input = Proto::SigningInput();
    input.set_chain_id("test-chain");
    input.set_account_number(15);
    input.set_sequence(1);
    input.set_private_key(fromPrivateKey.bytes.data(), fromPrivateKey.bytes.size());

    auto& order = *input.mutable_side_delegate_order();
    order.set_delegator_addr(fromAddr.data(), fromAddr.size());
    order.set_validator_addr(validator.getKeyHash().data(), validator.getKeyHash().size());
    order.set_chain_id("chapel");

    auto& token = *order.mutable_delegation();
    token.set_denom("BNB");
    token.set_amount(200000000);

    const auto data = Binance::Signer(std::move(input)).build();
    ASSERT_EQ(hex(data),
              "ba01f0625dee0a44e3a07fd20a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e112147cc24a1de524"
              "5f14a95e457f903bcc8461ac869c1a0a0a03424e42108084af5f220663686170656c126e0a26eb5ae987"
              "2103a9a55c040c8eb8120f3d1b32193250841c08af44ea561aac993dbe0f6b6a8fc7124039302c9975fb"
              "2a09ac2b6b6fb1d3b9fb5b4c03630d3d7a7da42b1c6736d6127142a3fcdca0b70a3d065da8d4f4df8b5d"
              "9d8f46aeb3627a7d7aa901fe186af34c180f2001");
}

TEST(BinanceSigner, BuildSideChainRedelegate) {
    const auto fromPrivateKey =
        PrivateKey(parse_hex("eeba3f6f2db26ced519a3d4c43afff101db957a21d54d25dc7fd235c404d7a5d"));
    const auto fromPublicKey = PublicKey(fromPrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    const Data fromAddr = Binance::Address(fromPublicKey).getKeyHash();
    auto validator1 = Bech32Address("");
    auto validator2 = Bech32Address("");
    Bech32Address::decode("bva1echrty7p8r23cwx8g3ezwcza9azy4zq7ca0pzw", validator1, "bva");
    Bech32Address::decode("bva1p7s26ervsmv3w83k5696glautc9sm5rchz5f5e", validator2, "bva");

    auto input = Proto::SigningInput();
    input.set_chain_id("test-chain");
    input.set_account_number(15);
    input.set_sequence(1);
    input.set_private_key(fromPrivateKey.bytes.data(), fromPrivateKey.bytes.size());

    auto& order = *input.mutable_side_redelegate_order();
    order.set_delegator_addr(fromAddr.data(), fromAddr.size());
    order.set_validator_src_addr(validator1.getKeyHash().data(), validator1.getKeyHash().size());
    order.set_validator_dst_addr(validator2.getKeyHash().data(), validator2.getKeyHash().size());
    order.set_chain_id("chapel");

    auto& token = *order.mutable_amount();
    token.set_denom("BNB");
    token.set_amount(100000000);

    const auto data = Binance::Signer(std::move(input)).build();
    ASSERT_EQ(hex(data),
        "d001f0625dee0a5ae3ced3640a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e11214ce2e3593c138"
        "d51c38c7447227605d2f444a881e1a140fa0ad646c86d9171e36a68ba47fbc5e0b0dd078220a0a03424e"
        "421080c2d72f2a0663686170656c126e0a26eb5ae9872103a9a55c040c8eb8120f3d1b32193250841c08"
        "af44ea561aac993dbe0f6b6a8fc71240114c6927423e95ecc831ec763b629b3a40db8feeb330528a941f"
        "d74843c0d63b4271b23916770d4901988c1f56b20086e5768a12290ebec265e30a80f8f3d88e180f2001"
    );
}

TEST(BinanceSigner, BuildSideChainUndelegate) {
    const auto fromPrivateKey =
        PrivateKey(parse_hex("eeba3f6f2db26ced519a3d4c43afff101db957a21d54d25dc7fd235c404d7a5d"));
    const auto fromPublicKey = PublicKey(fromPrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    const Data fromAddr = Binance::Address(fromPublicKey).getKeyHash();
    auto validator = Bech32Address("");
    Bech32Address::decode("bva1echrty7p8r23cwx8g3ezwcza9azy4zq7ca0pzw", validator, "bva");

    auto input = Proto::SigningInput();
    input.set_chain_id("test-chain");
    input.set_account_number(15);
    input.set_sequence(1);
    input.set_private_key(fromPrivateKey.bytes.data(), fromPrivateKey.bytes.size());

    auto& order = *input.mutable_side_undelegate_order();
    order.set_delegator_addr(fromAddr.data(), fromAddr.size());
    order.set_validator_addr(validator.getKeyHash().data(), validator.getKeyHash().size());
    order.set_chain_id("chapel");

    auto& token = *order.mutable_amount();
    token.set_denom("BNB");
    token.set_amount(100000000);

    const auto data = Binance::Signer(std::move(input)).build();
    ASSERT_EQ(hex(data),
        "ba01f0625dee0a44514f7e0e0a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e11214ce2e3593c138"
        "d51c38c7447227605d2f444a881e1a0a0a03424e421080c2d72f220663686170656c126e0a26eb5ae987"
        "2103a9a55c040c8eb8120f3d1b32193250841c08af44ea561aac993dbe0f6b6a8fc71240a622b7ca7a28"
        "75e5eaa675a5ed976b2ec3b8ca055a2b05e7fb471d328bd04df854789437dd06407e41ebb1e5a345604c"
        "93663dfb660e223800636c0b94c2e798180f2001"
    );
}

TEST(BinanceSigner, BuildTimeLockOrder) {
    const auto fromPrivateKey =
        PrivateKey(parse_hex("eeba3f6f2db26ced519a3d4c43afff101db957a21d54d25dc7fd235c404d7a5d"));
    const auto fromPublicKey = PublicKey(fromPrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    const Data fromAddr = Binance::Address(fromPublicKey).getKeyHash();

    auto signingInput = Proto::SigningInput();
    signingInput.set_chain_id("test-chain");
    signingInput.set_account_number(15);
    signingInput.set_sequence(1);
    signingInput.set_private_key(fromPrivateKey.bytes.data(), fromPrivateKey.bytes.size());

    auto& lockOrder = *signingInput.mutable_time_lock_order();
    lockOrder.set_from_address(fromAddr.data(), fromAddr.size());
    lockOrder.set_description("Description locked for offer");
    auto amount = lockOrder.add_amount();
    amount->set_denom("BNB");
    amount->set_amount(1000000);
    lockOrder.set_lock_time(1600001371);

    const auto data = Binance::Signer(std::move(signingInput)).build();
    EXPECT_EQ(hex(data),
        "bf01f0625dee0a49"
        "07921531"
        "0a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e1121c4465736372697074696f6e206c6f636b656420666f72206f666665721a090a03424e4210c0843d20dbaaf8fa05126e0a26eb5ae9872103a9a55c040c8eb8120f3d1b32193250841c08af44ea561aac993dbe0f6b6a8fc71240c270822b9515ba486c6a6b3472d388a5aea872ed960c0b53de0fafdc8682ef473a126f01e7dd2c00f04a0138a601b9540f54b14026846de362f7ab7f9fed948b180f2001"
    );
}

TEST(BinanceSigner, BuildTimeRelockOrder) {
    const auto fromPrivateKey =
        PrivateKey(parse_hex("eeba3f6f2db26ced519a3d4c43afff101db957a21d54d25dc7fd235c404d7a5d"));
    const auto fromPublicKey = PublicKey(fromPrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    const Data fromAddr = Binance::Address(fromPublicKey).getKeyHash();

    auto signingInput = Proto::SigningInput();
    signingInput.set_chain_id("test-chain");
    signingInput.set_account_number(15);
    signingInput.set_sequence(1);
    signingInput.set_private_key(fromPrivateKey.bytes.data(), fromPrivateKey.bytes.size());

    auto& relockOrder = *signingInput.mutable_time_relock_order();
    relockOrder.set_from_address(fromAddr.data(), fromAddr.size());
    relockOrder.set_id(333);
    relockOrder.set_description("Description locked for offer");
    auto amount = relockOrder.add_amount();
    amount->set_denom("BNB");
    amount->set_amount(1000000);
    relockOrder.set_lock_time(1600001371);

    const auto data = Binance::Signer(std::move(signingInput)).build();
    EXPECT_EQ(hex(data),
              "c201f0625dee0a4c504711da0a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e110cd021a1c446573"
              "6372697074696f6e206c6f636b656420666f72206f6666657222090a03424e4210c0843d28dbaaf8fa05"
              "126e0a26eb5ae9872103a9a55c040c8eb8120f3d1b32193250841c08af44ea561aac993dbe0f6b6a8fc7"
              "124086ddaa077c8ae551d402fa409cf7e91663982b0542200967c03c0b5876b181353250f689d342f221"
              "7624a077b671ce7d09649187e29879f40abbbee9de7ab27c180f2001");
}

TEST(BinanceSigner, BuildTimeUnlockOrder) {
    const auto fromPrivateKey =
        PrivateKey(parse_hex("eeba3f6f2db26ced519a3d4c43afff101db957a21d54d25dc7fd235c404d7a5d"));
    const auto fromPublicKey = PublicKey(fromPrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
    const Data fromAddr = Binance::Address(fromPublicKey).getKeyHash();

    auto signingInput = Proto::SigningInput();
    signingInput.set_chain_id("test-chain");
    signingInput.set_account_number(15);
    signingInput.set_sequence(1);
    signingInput.set_private_key(fromPrivateKey.bytes.data(), fromPrivateKey.bytes.size());

    auto& unlockOrder = *signingInput.mutable_time_unlock_order();
    unlockOrder.set_from_address(fromAddr.data(), fromAddr.size());
    unlockOrder.set_id(333);

    const auto data = Binance::Signer(std::move(signingInput)).build();
    EXPECT_EQ(hex(data),
              "9301f0625dee0a1dc4050c6c0a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e110cd02126e0a26eb"
              "5ae9872103a9a55c040c8eb8120f3d1b32193250841c08af44ea561aac993dbe0f6b6a8fc71240da777b"
              "fd2032834f59ec9fe69fd6eaa4aca24242dfbc5ec4ef8c435cb9da7eb05ab78e1b8ca9f109657cb77996"
              "898f1b59137b3d8f1e00f842e409e18033b347180f2001");
}

} // namespace TW::Binance
