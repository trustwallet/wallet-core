// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Binance/Signer.h"
#include "Coin.h"
#include "Cosmos/Address.h"
#include "HDWallet.h"
#include "HexCoding.h"
#include "proto/Binance.pb.h"

#include <TrustWalletCore/TWHRP.h>
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

        auto &order = *input.mutable_trade_order();
        auto result = Cosmos::Address::decode("bnb1hgm0p7khfk85zpz5v0j8wnej3a90w709vhkdfu");
        ASSERT_TRUE(result.second);
        auto keyhash = result.first.keyHash;
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

        ASSERT_EQ(hex(signature.begin(), signature.end()),
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

        auto &order = *input.mutable_trade_order();
        auto address =
                Cosmos::Address(HRP_BINANCE, parse_hex("b6561dcc104130059a7c08f48c64610c1f6f9064"));
        auto keyhash = address.keyHash;
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

        ASSERT_EQ(
                hex(result.begin(), result.end()),
                "db01f0625dee0a65ce6dc0430a14b6561dcc104130059a7c08f48c64610c1f6f9064122b423635363144434331"
                "303431333030353941374330384634384336343631304331463646393036342d31311a0b4254432d3543345f42"
                "4e42200228013080c2d72f3880989abc044001126e0a26eb5ae98721029729a52e4e3c2b4a4e52aa74033eedaf"
                "8ba1df5ab6d1f518fd69e67bbd309b0e12402a78b6d9a108eb9440221802b626e24d80179395ac984f016db012"
                "ef1a0c16d71b4d7053e05366ae3ea2681fc8052398fda20551c965d74c5970bbc66b94b48e1801200a");
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

        auto &order = *signingInput.mutable_send_order();

        auto fromKeyhash = parse_hex("40c2979694bbc961023d1d27be6fc4d21a9febe6");
        auto fromAddress = Cosmos::Address(HRP_BINANCE, fromKeyhash);

        auto toKeyhash = parse_hex("88b37d5e05f3699e2a1406468e5d87cb9dcceb95");
        auto toAddress = Cosmos::Address(HRP_BINANCE, toKeyhash);

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
        ASSERT_EQ(hex(signature.begin(), signature.end()),
                  "c65a13440f18a155bd971ee40b9e0dd58586f5bf344e12ec4c76c439aebca8c7789bab7bfbfb4ce89aad"
                  "c4a02df225b6b6efc861c13bbeb5f7a3eea2d7ffc80f");

        auto result = signer.build();

        ASSERT_EQ(hex(result.begin(), result.end()),
                  "cc01f0625dee0a4e2a2c87fa0a230a1440c2979694bbc961023d1d27be6fc4d21a9febe6120b0a03424e"
                  "4210c098a8dd0312230a1488b37d5e05f3699e2a1406468e5d87cb9dcceb95120b0a03424e4210c098a8"
                  "dd03126e0a26eb5ae98721026a35920088d98c3888ca68c53dfc93f4564602606cbb87f0fe5ee533db38"
                  "e5021240c65a13440f18a155bd971ee40b9e0dd58586f5bf344e12ec4c76c439aebca8c7789bab7bfbfb"
                  "4ce89aadc4a02df225b6b6efc861c13bbeb5f7a3eea2d7ffc80f181320171a04746573742001");
    }

    TEST(BinanceSigner, BuildSend2) {
        const auto derivationPath = TW::derivationPath(TWCoinTypeBinance);

        const auto fromWallet =
                HDWallet("swift slam quote sail high remain mandate sample now stamp title among fiscal "
                         "captain joy puppy ghost arrow attract ozone situate install gain mean",
                         "");
        const auto fromPrivateKey = fromWallet.getKey(derivationPath);
        const auto fromPublicKey = PublicKey(fromPrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1));

        const auto toWallet =
                HDWallet("bottom quick strong ranch section decide pepper broken oven demand coin run "
                         "jacket curious business achieve mule bamboo remain vote kid rigid bench rubber",
                         "");
        const auto toPrivateKey = toWallet.getKey(derivationPath);
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
        auto fromKeyHash = Cosmos::Address(HRP_BINANCE, fromPublicKey).keyHash;
        input.set_address(fromKeyHash.data(), fromKeyHash.size());
        *input.add_coins() = token;

        auto output = Proto::SendOrder::Output();
        auto toKeyHash = Cosmos::Address(HRP_BINANCE, toPublicKey).keyHash;
        output.set_address(toKeyHash.data(), toKeyHash.size());
        *output.add_coins() = token;

        auto sendOrder = Proto::SendOrder();
        *sendOrder.add_inputs() = input;
        *sendOrder.add_outputs() = output;

        *signingInput.mutable_send_order() = sendOrder;

        const auto data = Signer(std::move(signingInput)).build();
        ASSERT_EQ(hex(data.begin(), data.end()),
                  "c601f0625dee0a522a2c87fa0a250a141d0e3086e8e4e0a53c38a90d55bd58b34d57d2fa120d0a03424e"
                  "42108080e983b1de1612250a146b571fc0a9961a7ddf45e49a88a4d83941fcabbe120d0a03424e421080"
                  "80e983b1de16126c0a26eb5ae98721027e69d96640300433654e016d218a8d7ffed751023d8efe81e55d"
                  "edbd6754c97112408b23eecfa8237a27676725173e58154e6c204bb291b31c3b7b507c8f04e2773909ba"
                  "70e01b54f4bd0bc76669f5712a5a66b9508acdf3aa5e4fde75fbe57622a12001");
    }

    TEST(BinanceSigner, BuildHTLT) {
        const auto fromPrivateKey = PrivateKey(parse_hex("eeba3f6f2db26ced519a3d4c43afff101db957a21d54d25dc7fd235c404d7a5d"));
        const auto fromPublicKey = PublicKey(fromPrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
        auto fromAddr = Cosmos::Address(HRP_BINANCE, fromPublicKey).keyHash;

        const auto toPrivateKey = PrivateKey(parse_hex("851fab89c14f4bbec0cc06f5e445ec065efc641068d78b308c67217d9bd5c88a"));
        const auto toPublicKey = PublicKey(toPrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
        auto toAddr = Cosmos::Address(HRP_BINANCE, toPublicKey).keyHash;

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

        auto &htltOrder = *signingInput.mutable_htlt_order();
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
        ASSERT_EQ(hex(data.begin(), data.end()),
                  "ee01f0625dee0a7ab33f9a240a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e112140153f11d6db7"
                  "e69c7d51e771c697378018fb6c242a20e8eae926261ab77d018202434791a335249b470246a7b02e28c3"
                  "b2fb6ffad8f330e1d1c7eb053a0a0a03424e421080c2d72f42113130303030303030303a4254432d3144"
                  "43489003126c0a26eb5ae9872103a9a55c040c8eb8120f3d1b32193250841c08af44ea561aac993dbe0f"
                  "6b6a8fc712406a5fec06eb0d59436a0f89934c0b70180cefd3fb2bd7018f8529ed41ec133d2e630c5a50"
                  "d3ad586f7cede6aeee11d8467617096e173d13ef804f20112ed3891a180f");
    }

    TEST(BinanceSigner, BuildDepositHTLT) {
        const auto fromPrivateKey = PrivateKey(parse_hex("851fab89c14f4bbec0cc06f5e445ec065efc641068d78b308c67217d9bd5c88a"));
        const auto fromPublicKey = PublicKey(fromPrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
        auto fromAddr = Cosmos::Address(HRP_BINANCE, fromPublicKey).keyHash;

        auto signingInput = Proto::SigningInput();
        signingInput.set_chain_id("test-chain");
        signingInput.set_account_number(16);
        signingInput.set_sequence(0);
        signingInput.set_private_key(fromPrivateKey.bytes.data(), fromPrivateKey.bytes.size());

        auto token = Proto::SendOrder::Token();
        token.set_denom("BTC-1DC");
        token.set_amount(100000000);

        auto swapID = parse_hex("dd8fd4719741844d35eb35ddbeca9531d5493a8e4667689c55e73c77503dd9e5");

        auto &depositHTLTOrder = *signingInput.mutable_deposithtlt_order();
        depositHTLTOrder.set_from(fromAddr.data(), fromAddr.size());
        depositHTLTOrder.set_swap_id(swapID.data(), swapID.size());
        *depositHTLTOrder.add_amount() = token;

        const auto data = Binance::Signer(std::move(signingInput)).build();
        ASSERT_EQ(hex(data.begin(), data.end()),
                  "c001f0625dee0a4c639864960a140153f11d6db7e69c7d51e771c697378018fb6c24120e0a074254432d"
                  "3144431080c2d72f1a20dd8fd4719741844d35eb35ddbeca9531d5493a8e4667689c55e73c77503dd9e5"
                  "126c0a26eb5ae98721038df6960084e20b2d07d50e1422f94105c6241d9f1482a4eb79ce8bfd460f19e4"
                  "1240ee4f7e4bec7715cbb16e935daf717e11e3c995ff4a65193b8e8dbf63c129f9403eeaa8879d7eb644"
                  "1bd2d554a3f697bcead191194d8a6a4f3f2348a9d56353111810");
    }

    TEST(BinanceSigner, BuildClaimHTLT) {
        const auto fromPrivateKey = PrivateKey(parse_hex("eeba3f6f2db26ced519a3d4c43afff101db957a21d54d25dc7fd235c404d7a5d"));
        const auto fromPublicKey = PublicKey(fromPrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
        auto fromAddr = Cosmos::Address(HRP_BINANCE, fromPublicKey).keyHash;

        auto signingInput = Proto::SigningInput();
        signingInput.set_chain_id("test-chain");
        signingInput.set_account_number(15);
        signingInput.set_sequence(1);
        signingInput.set_private_key(fromPrivateKey.bytes.data(), fromPrivateKey.bytes.size());

        auto randomNumber =
                parse_hex("bda6933c7757d0ca428aa01fb9d0935a231f87bf2deeb9b409cea3f2d580a2cc");
        auto swapID = parse_hex("dd8fd4719741844d35eb35ddbeca9531d5493a8e4667689c55e73c77503dd9e5");

        auto &claimHTLTOrder = *signingInput.mutable_claimhtlt_order();
        claimHTLTOrder.set_from(fromAddr.data(), fromAddr.size());
        claimHTLTOrder.set_swap_id(swapID.data(), swapID.size());
        claimHTLTOrder.set_random_number(randomNumber.data(), randomNumber.size());

        const auto data = Binance::Signer(std::move(signingInput)).build();
        ASSERT_EQ(
                hex(data.begin(), data.end()),
                "d401f0625dee0a5ec16653000a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e11220dd8fd4719741844d35"
                "eb35ddbeca9531d5493a8e4667689c55e73c77503dd9e51a20bda6933c7757d0ca428aa01fb9d0935a231f87bf"
                "2deeb9b409cea3f2d580a2cc126e0a26eb5ae9872103a9a55c040c8eb8120f3d1b32193250841c08af44ea561a"
                "ac993dbe0f6b6a8fc71240c2fd2b55e67afbbd97a4fe5064a297af27bc3339c2153d0f40478b6b0647b3e701f1"
                "df1ba0eb7eed4e21234261b662fd52fd9b4f406956b7f4be8de11a4de7b0180f2001");
    }

    TEST(BinanceSigner, BuildRefundHTLT) {
        const auto fromPrivateKey = PrivateKey(parse_hex("eeba3f6f2db26ced519a3d4c43afff101db957a21d54d25dc7fd235c404d7a5d"));
        const auto fromPublicKey = PublicKey(fromPrivateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
        auto fromAddr = Cosmos::Address(HRP_BINANCE, fromPublicKey).keyHash;

        auto signingInput = Proto::SigningInput();
        signingInput.set_chain_id("test-chain");
        signingInput.set_account_number(15);
        signingInput.set_sequence(1);
        signingInput.set_private_key(fromPrivateKey.bytes.data(), fromPrivateKey.bytes.size());

        auto swapID = parse_hex("dd8fd4719741844d35eb35ddbeca9531d5493a8e4667689c55e73c77503dd9e5");

        auto &refundHTLTOrder = *signingInput.mutable_refundhtlt_order();
        refundHTLTOrder.set_from(fromAddr.data(), fromAddr.size());
        refundHTLTOrder.set_swap_id(swapID.data(), swapID.size());

        const auto data = Binance::Signer(std::move(signingInput)).build();
        ASSERT_EQ(hex(data.begin(), data.end()),
                  "b201f0625dee0a3c3454a27c0a1408c7c918f6b72c3c0c21b7d08eb6fc66509998e11220dd8fd4719741"
                  "844d35eb35ddbeca9531d5493a8e4667689c55e73c77503dd9e5126e0a26eb5ae9872103a9a55c040c8e"
                  "b8120f3d1b32193250841c08af44ea561aac993dbe0f6b6a8fc712402a957b42bc99a7577a8928e5538a"
                  "97c0e35095efd0572ade0b4c42332818b0855d0e3b818f015cb21e6231bee8679b0cb0c1d621fc14c34a"
                  "b31eb05a57c482a6180f2001");
    }

} // namespace TW::Binance
