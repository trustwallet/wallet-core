// Copyright © 2019 Mart Roosmaa.
// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "proto/Nano.pb.h"
#include "TestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>
#include <gtest/gtest.h>

using namespace TW;

namespace TW::Nano::tests {

TEST(TWAnySignerNano, sign) {
    const auto privateKey = parse_hex("173c40e97fe2afcd24187e74f6b603cb949a5365e72fbdd065a6b165e2189e34");
    const auto linkBlock = parse_hex("491fca2c69a84607d374aaf1f6acd3ce70744c5be0721b5ed394653e85233507");

    auto input = Proto::SigningInput();
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_link_block(linkBlock.data(), linkBlock.size());
    input.set_representative("xrb_3arg3asgtigae3xckabaaewkx3bzsh7nwz7jkmjos79ihyaxwphhm6qgjps4");
    input.set_balance("96242336390000000000000000000");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeNano);

    EXPECT_EQ(
        "{\"account\":\"nano_1bhbsc9yuh15anq3owu1izw1nk7bhhqefrkhfo954fyt8dk1q911buk1kk4c\","
        "\"balance\":\"96242336390000000000000000000\","
        "\"link\":\"491fca2c69a84607d374aaf1f6acd3ce70744c5be0721b5ed394653e85233507\","
        "\"link_as_account\":\"nano_1kazsap8mc481zbqbcqjytpf9mmigj87qr5k5fhf97579t4k8fa94octjx6d\","
        "\"previous\":\"0000000000000000000000000000000000000000000000000000000000000000\","
        "\"representative\":\"nano_3arg3asgtigae3xckabaaewkx3bzsh7nwz7jkmjos79ihyaxwphhm6qgjps4\","
        "\"signature\":"
        "\"d247f6b90383b24e612569c75a12f11242f6e03b4914eadc7d941577dcf54a3a7cb7f0a4aba4246a40d9ebb5"
        "ee1e00b4a0a834ad5a1e7bef24e11f62b95a9e09\","
        "\"type\":\"state\"}",
        output.json());
}

TEST(TWAnySignerNano, SignJSON) {
    auto json = STRING(R"({"link_block":"SR/KLGmoRgfTdKrx9qzTznB0TFvgchte05RlPoUjNQc=","representative":"nano_3arg3asgtigae3xckabaaewkx3bzsh7nwz7jkmjos79ihyaxwphhm6qgjps4","balance":"96242336390000000000000000000"})");
    auto key = DATA("173c40e97fe2afcd24187e74f6b603cb949a5365e72fbdd065a6b165e2189e34");
    auto result = WRAPS(TWAnySignerSignJSON(json.get(), key.get(), TWCoinTypeNano));

    ASSERT_TRUE(TWAnySignerSupportsJSON(TWCoinTypeNano));
    assertStringsEqual(result, R"({"account":"nano_1bhbsc9yuh15anq3owu1izw1nk7bhhqefrkhfo954fyt8dk1q911buk1kk4c","balance":"96242336390000000000000000000","link":"491fca2c69a84607d374aaf1f6acd3ce70744c5be0721b5ed394653e85233507","link_as_account":"nano_1kazsap8mc481zbqbcqjytpf9mmigj87qr5k5fhf97579t4k8fa94octjx6d","previous":"0000000000000000000000000000000000000000000000000000000000000000","representative":"nano_3arg3asgtigae3xckabaaewkx3bzsh7nwz7jkmjos79ihyaxwphhm6qgjps4","signature":"d247f6b90383b24e612569c75a12f11242f6e03b4914eadc7d941577dcf54a3a7cb7f0a4aba4246a40d9ebb5ee1e00b4a0a834ad5a1e7bef24e11f62b95a9e09","type":"state"})");
}

} // namespace TW::Nano::tests
