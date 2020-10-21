// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>

#include "Data.h"
#include "HexCoding.h"
#include "proto/Filecoin.pb.h"
#include "uint256.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace Filecoin;

TEST(TWAnySignerFilecoin, Sign) {
    Proto::SigningInput input;
    auto privateKey = parse_hex("1d969865e189957b9824bd34f26d5cbf357fda1a6d844cbf0c9ab1ed93fa7dbe");
    auto toAddress =
        "f3um6uo3qt5of54xjbx3hsxbw5mbsc6auxzrvfxekn5bv3duewqyn2tg5rhrlx73qahzzpkhuj7a34iq7oifsq";
    uint64_t nonce = 2;
    // 600 FIL
    // auto value = parse_hex("2086ac351052600000");
    auto value = store(uint256_t(600) * uint256_t(1'000'000'000) * uint256_t(1'000'000'000));
    uint64_t gasLimit = 1000;
    // auto gasFeeCap = parse_hex("25f273933db5700000");
    auto gasFeeCap = store(uint256_t(700) * uint256_t(1'000'000'000) * uint256_t(1'000'000'000));
    // auto gasPremium = parse_hex("2b5e3af16b18800000");
    auto gasPremium = store(uint256_t(800) * uint256_t(1'000'000'000) * uint256_t(1'000'000'000));

    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_to(toAddress);
    input.set_nonce(nonce);
    input.set_value(value.data(), value.size());
    input.set_gas_limit(gasLimit);
    input.set_gas_fee_cap(gasFeeCap.data(), gasFeeCap.size());
    input.set_gas_premium(gasPremium.data(), gasPremium.size());

    auto inputString = input.SerializeAsString();
    auto inputData = TWDataCreateWithBytes((const byte*)inputString.data(), inputString.size());

    auto outputData = TWAnySignerSign(inputData, TWCoinTypeFilecoin);

    ASSERT_EQ(
        hex(TWDataBytes(outputData), TWDataBytes(outputData) + TWDataSize(outputData)),
        "0ab701828a00583103a33d476e13eb8bde5d21becf2b86dd60642f0297cc6a5b914de86bb1d096861ba99bb13c"
        "577fee003e72f51e89f837c45501cf01bf485f61435e6770b52615bf455e043a2361024a002086ac3510526000"
        "001903e84a0025f273933db57000004a002b5e3af16b1880000000405842018cc46ef8e67f95fa69826a927c6b"
        "27b45ad12d6667520dd96262534d12ec56de15d77d74bd1dd5286269684e92e2c36f0ce13ab0482637d6267d39"
        "0f54adaa3201129a037b224d657373616765223a7b2246726f6d223a2266317a346133367363376d666276347a"
        "3371777574626c7032666c7963647569336261666679746271222c22476173466565436170223a223730303030"
        "30303030303030303030303030303030222c224761734c696d6974223a313030302c224761735072656d69756d"
        "223a22383030303030303030303030303030303030303030222c224e6f6e6365223a322c22546f223a22663375"
        "6d36756f337174356f663534786a6278336873786277356d627363366175787a72766678656b6e356276336475"
        "657771796e327467357268726c7837337161687a7a706b68756a376133346971376f69667371222c2256616c75"
        "65223a22363030303030303030303030303030303030303030227d2c225369676e6174757265223a7b22446174"
        "61223a226a4d52752b4f5a2f6c667070676d71536647736e744672524c575a6e5567335a596d4a5454524c7356"
        "74345631333130765233564b474a706145365334734e76444f45367345676d4e39596d66546b50564b32714d67"
        "453d222c2254797065223a317d7d");

    Proto::SigningOutput output;
    output.ParseFromArray(TWDataBytes(outputData), static_cast<int>(TWDataSize(outputData)));

    ASSERT_EQ(hex(output.encoded()),
              "828a00583103a33d476e13eb8bde5d21becf2b86dd60642f0297cc6a5b914de86bb1d096861ba99bb13c"
              "577fee003e72f51e89f837c45501cf01bf485f61435e6770b52615bf455e043a2361024a002086ac3510"
              "526000001903e84a0025f273933db57000004a002b5e3af16b1880000000405842018cc46ef8e67f95fa"
              "69826a927c6b27b45ad12d6667520dd96262534d12ec56de15d77d74bd1dd5286269684e92e2c36f0ce1"
              "3ab0482637d6267d390f54adaa3201");

    ASSERT_EQ(output.json(), R"({"Message":{"From":"f1z4a36sc7mfbv4z3qwutblp2flycdui3baffytbq","GasFeeCap":"700000000000000000000","GasLimit":1000,"GasPremium":"800000000000000000000","Nonce":2,"To":"f3um6uo3qt5of54xjbx3hsxbw5mbsc6auxzrvfxekn5bv3duewqyn2tg5rhrlx73qahzzpkhuj7a34iq7oifsq","Value":"600000000000000000000"},"Signature":{"Data":"jMRu+OZ/lfppgmqSfGsntFrRLWZnUg3ZYmJTTRLsVt4V1310vR3VKGJpaE6S4sNvDOE6sEgmN9YmfTkPVK2qMgE=","Type":1}})");
    TWDataDelete(inputData);
    TWDataDelete(outputData);
}

TEST(TWAnySignerFilecoin, SignJSON) {
    auto json = STRING(
        R"({
            "to": "f3um6uo3qt5of54xjbx3hsxbw5mbsc6auxzrvfxekn5bv3duewqyn2tg5rhrlx73qahzzpkhuj7a34iq7oifsq",
            "nonce": "2",
            "value": "IIasNRBSYAAA",
            "gasLimit": 1000,
            "gasFeeCap": "JfJzkz21cAAA",
            "gasPremium": "K1468WsYgAAA"
        })");
    auto key = DATA("1d969865e189957b9824bd34f26d5cbf357fda1a6d844cbf0c9ab1ed93fa7dbe");
    auto result = WRAPS(TWAnySignerSignJSON(json.get(), key.get(), TWCoinTypeFilecoin));

    ASSERT_TRUE(TWAnySignerSupportsJSON(TWCoinTypeFilecoin));
    assertStringsEqual(
        result,
        "828a00583103a33d476e13eb8bde5d21becf2b86dd60642f0297cc6a5b914de86bb1d096861ba99bb13c"
        "577fee003e72f51e89f837c45501cf01bf485f61435e6770b52615bf455e043a2361024a002086ac3510"
        "526000001903e84a0025f273933db57000004a002b5e3af16b1880000000405842018cc46ef8e67f95fa"
        "69826a927c6b27b45ad12d6667520dd96262534d12ec56de15d77d74bd1dd5286269684e92e2c36f0ce1"
        "3ab0482637d6267d390f54adaa3201");
}
