// Copyright © 2019 Mart Roosmaa.
// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWAnySigner.h>
#include "proto/Nano.pb.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Nano;

TEST(TWNanoSigner, sign) {
    const auto privateKey = parse_hex("173c40e97fe2afcd24187e74f6b603cb949a5365e72fbdd065a6b165e2189e34");
    const auto linkBlock = parse_hex("491fca2c69a84607d374aaf1f6acd3ce70744c5be0721b5ed394653e85233507");

    auto input = Proto::SigningInput();
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_link_block(linkBlock.data(), linkBlock.size());
    input.set_representative("xrb_3arg3asgtigae3xckabaaewkx3bzsh7nwz7jkmjos79ihyaxwphhm6qgjps4");
    input.set_balance("96242336390000000000000000000");

    auto inputData = input.SerializeAsString();
    auto inputTWData = TWDataCreateWithBytes((const byte *)inputData.data(), inputData.size());
    auto outputTWData = TWAnySignerSign(inputTWData, TWCoinTypeNano);

    auto output = Proto::SigningOutput();
    output.ParseFromArray(TWDataBytes(outputTWData), TWDataSize(outputTWData));

    TWDataDelete(inputTWData);
    TWDataDelete(outputTWData);

    EXPECT_EQ(
        "{\"account\":\"nano_1bhbsc9yuh15anq3owu1izw1nk7bhhqefrkhfo954fyt8dk1q911buk1kk4c\","
        "\"balance\":\"96242336390000000000000000000\","
        "\"link\":\"491fca2c69a84607d374aaf1f6acd3ce70744c5be0721b5ed394653e85233507\","
        "\"link_as_account\":\"nano_1kazsap8mc481zbqbcqjytpf9mmigj87qr5k5fhf97579t4k8fa94octjx6d\","
        "\"previous\":\"0000000000000000000000000000000000000000000000000000000000000000\","
        "\"representative\":\"nano_3arg3asgtigae3xckabaaewkx3bzsh7nwz7jkmjos79ihyaxwphhm6qgjps4\","
        "\"signature\":\"d247f6b90383b24e612569c75a12f11242f6e03b4914eadc7d941577dcf54a3a7cb7f0a4aba4246a40d9ebb5ee1e00b4a0a834ad5a1e7bef24e11f62b95a9e09\","
        "\"type\":\"state\"}",
        output.json());    
}
