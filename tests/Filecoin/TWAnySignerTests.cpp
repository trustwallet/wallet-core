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
    auto gasPrice = store(uint256_t(2));
    uint64_t gasLimit = 1000;

    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_to_address(toAddress);
    input.set_nonce(nonce);
    input.set_value(value.data(), value.size());
    input.set_gas_price(gasPrice.data(), gasPrice.size());
    input.set_gas_limit(gasLimit);

    auto inputString = input.SerializeAsString();
    auto inputData = TWDataCreateWithBytes((const byte*)inputString.data(), inputString.size());

    auto outputData = TWAnySignerSign(inputData, TWCoinTypeFilecoin);

    ASSERT_EQ(hex(TWDataBytes(outputData), TWDataBytes(outputData) + TWDataSize(outputData)),
              "0aa401"
              "828900583103a33d476e13eb8bde5d21becf2b86dd60642f0297cc6a5b914de8"
              "6bb1d096861ba99bb13c577fee003e72f51e89f837c45501cf01bf485f61435e"
              "6770b52615bf455e043a2361024a002086ac3510526000004200021903e80040"
              "584201d7320c388135b20dea596493156495519db9e3cfcb78ccb18462dde4aa"
              "1c507750a79ef014caf717acbe2ce562e5e55a4c990607c32a741f1c073fa922"
              "176cc900");

    Proto::SigningOutput output;
    output.ParseFromArray(TWDataBytes(outputData), static_cast<int>(TWDataSize(outputData)));

    ASSERT_EQ(hex(output.encoded()),
              "828900583103a33d476e13eb8bde5d21becf2b86dd60642f0297cc6a5b914de8"
              "6bb1d096861ba99bb13c577fee003e72f51e89f837c45501cf01bf485f61435e"
              "6770b52615bf455e043a2361024a002086ac3510526000004200021903e80040"
              "584201d7320c388135b20dea596493156495519db9e3cfcb78ccb18462dde4aa"
              "1c507750a79ef014caf717acbe2ce562e5e55a4c990607c32a741f1c073fa922"
              "176cc900");

    TWDataDelete(inputData);
    TWDataDelete(outputData);
}

TEST(TWAnySignerFilecoin, SignJSON) {
    auto json = STRING(
        R"({"toAddress":"f3um6uo3qt5of54xjbx3hsxbw5mbsc6auxzrvfxekn5bv3duewqyn2tg5rhrlx73qahzzpkhuj7a34iq7oifsq","nonce":"2","value":"IIasNRBSYAAA","gasPrice":"Ag==","gasLimit":"1000"})");
    auto key = DATA("1d969865e189957b9824bd34f26d5cbf357fda1a6d844cbf0c9ab1ed93fa7dbe");
    auto result = WRAPS(TWAnySignerSignJSON(json.get(), key.get(), TWCoinTypeFilecoin));

    ASSERT_TRUE(TWAnySignerSupportsJSON(TWCoinTypeFilecoin));
    assertStringsEqual(result, "828900583103a33d476e13eb8bde5d21becf2b86dd60642f0297cc6a5b914de8"
                               "6bb1d096861ba99bb13c577fee003e72f51e89f837c45501cf01bf485f61435e"
                               "6770b52615bf455e043a2361024a002086ac3510526000004200021903e80040"
                               "584201d7320c388135b20dea596493156495519db9e3cfcb78ccb18462dde4aa"
                               "1c507750a79ef014caf717acbe2ce562e5e55a4c990607c32a741f1c073fa922"
                               "176cc900");
}
