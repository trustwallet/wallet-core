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
    input.set_to_address(toAddress);
    input.set_nonce(nonce);
    input.set_value(value.data(), value.size());
    input.set_gas_limit(gasLimit);
    input.set_gas_fee_cap(gasFeeCap.data(), gasFeeCap.size());
    input.set_gas_premium(gasPremium.data(), gasPremium.size());

    auto inputString = input.SerializeAsString();
    auto inputData = TWDataCreateWithBytes((const byte*)inputString.data(), inputString.size());

    auto outputData = TWAnySignerSign(inputData, TWCoinTypeFilecoin);

    ASSERT_EQ(hex(TWDataBytes(outputData), TWDataBytes(outputData) + TWDataSize(outputData)),
              "0ab701"
              "828a00583103a33d476e13eb8bde5d21becf2b86dd60642f0297cc6a5b914de86bb1d096861ba99bb13c"
              "577fee003e72f51e89f837c45501cf01bf485f61435e6770b52615bf455e043a2361024a002086ac3510"
              "526000001903e84a0025f273933db57000004a002b5e3af16b1880000000405842018cc46ef8e67f95fa"
              "69826a927c6b27b45ad12d6667520dd96262534d12ec56de15d77d74bd1dd5286269684e92e2c36f0ce1"
              "3ab0482637d6267d390f54adaa3201");

    Proto::SigningOutput output;
    output.ParseFromArray(TWDataBytes(outputData), static_cast<int>(TWDataSize(outputData)));

    ASSERT_EQ(hex(output.encoded()),
              "828a00583103a33d476e13eb8bde5d21becf2b86dd60642f0297cc6a5b914de86bb1d096861ba99bb13c"
              "577fee003e72f51e89f837c45501cf01bf485f61435e6770b52615bf455e043a2361024a002086ac3510"
              "526000001903e84a0025f273933db57000004a002b5e3af16b1880000000405842018cc46ef8e67f95fa"
              "69826a927c6b27b45ad12d6667520dd96262534d12ec56de15d77d74bd1dd5286269684e92e2c36f0ce1"
              "3ab0482637d6267d390f54adaa3201");

    TWDataDelete(inputData);
    TWDataDelete(outputData);
}

TEST(TWAnySignerFilecoin, SignJSON) {
    auto json = STRING(
        R"({
            "toAddress": "f3um6uo3qt5of54xjbx3hsxbw5mbsc6auxzrvfxekn5bv3duewqyn2tg5rhrlx73qahzzpkhuj7a34iq7oifsq",
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
