// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Coin.h"
#include "HDWallet.h"
#include "HexCoding.h"
#include "proto/Binance.pb.h"
#include "Cosmos/Address.h"
#include "Binance/Signer.h"
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

    auto& order = *input.mutable_trade_order();
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

    ASSERT_EQ(hex(signature.begin(), signature.end()), "9123cb6906bb20aeb753f4a121d4d88ff0e9750ba75b0c4e10d76caee1e7d2481290fa3b9887a6225d6997f5f939ef834ea61d596a314237c48e560da9e17b5a");
}

TEST(BinanceSigner, Build) {
    auto input = Proto::SigningInput();
    input.set_chain_id("chain-bnb");
    input.set_account_number(1);
    input.set_sequence(10);

    auto key = parse_hex("90335b9d2153ad1a9799a3ccc070bd64b4164e9642ee1dd48053c33f9a3a05e9");
    input.set_private_key(key.data(), key.size());

    auto& order = *input.mutable_trade_order();
    auto address = Cosmos::Address(HRP_BINANCE, parse_hex("b6561dcc104130059a7c08f48c64610c1f6f9064"));
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

    ASSERT_EQ(hex(result.begin(), result.end()), "db01"
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
    ASSERT_EQ(hex(signature.begin(), signature.end()), "c65a13440f18a155bd971ee40b9e0dd58586f5bf344e12ec4c76c439aebca8c7789bab7bfbfb4ce89aadc4a02df225b6b6efc861c13bbeb5f7a3eea2d7ffc80f");

    auto result = signer.build();

    ASSERT_EQ(hex(result.begin(), result.end()), "cc01"
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
    const auto fromPrivateKey = fromWallet.getKey(derivationPath);
    const auto fromPublicKey = PublicKey(fromPrivateKey.getPublicKey(PublicKeyType::secp256k1));

    const auto toWallet = HDWallet( "bottom quick strong ranch section decide pepper broken oven demand coin run jacket curious business achieve mule bamboo remain vote kid rigid bench rubber", "");
    const auto toPrivateKey = toWallet.getKey(derivationPath);
    const auto toPublicKey = PublicKey(toPrivateKey.getPublicKey(PublicKeyType::secp256k1));

    auto signingInput = Proto::SigningInput();
    signingInput.set_chain_id("bnbchain-1000");
    signingInput.set_account_number(0);
    signingInput.set_sequence(1);
    signingInput.set_private_key(fromPrivateKey.bytes.data(), fromPrivateKey.bytes.size());

    auto token =  Proto::SendOrder::Token();
    token.set_denom("BNB");
    token.set_amount(100000000000000);

    auto input =  Proto::SendOrder::Input();
    auto fromKeyHash = Cosmos::Address(HRP_BINANCE, fromPublicKey).keyHash;
    input.set_address(fromKeyHash.data(), fromKeyHash.size());
    *input.add_coins() = token;

    auto output =  Proto::SendOrder::Output();
    auto toKeyHash = Cosmos::Address(HRP_BINANCE, toPublicKey).keyHash;
    output.set_address(toKeyHash.data(), toKeyHash.size());
    *output.add_coins() = token;

    auto sendOrder =  Proto::SendOrder();
    *sendOrder.add_inputs() = input;
    *sendOrder.add_outputs() = output;

    *signingInput.mutable_send_order() = sendOrder;

    const auto data = Signer(std::move(signingInput)).build();
    ASSERT_EQ(hex(data.begin(), data.end()),
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

} // namespace TW::Binance
