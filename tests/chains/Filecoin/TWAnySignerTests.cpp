// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "TestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>

#include "Data.h"
#include "HexCoding.h"
#include "proto/Filecoin.pb.h"
#include "uint256.h"

#include <gtest/gtest.h>

using namespace TW;

namespace TW::Filecoin::tests {

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
    auto inputData = WRAPD(TWDataCreateWithBytes((const byte*)inputString.data(), inputString.size()));

    auto outputData = WRAPD(TWAnySignerSign(inputData.get(), TWCoinTypeFilecoin));

    Proto::SigningOutput output;
    output.ParseFromArray(TWDataBytes(outputData.get()), static_cast<int>(TWDataSize(outputData.get())));

    ASSERT_EQ(output.json(), R"({"Message":{"From":"f1z4a36sc7mfbv4z3qwutblp2flycdui3baffytbq","GasFeeCap":"700000000000000000000","GasLimit":1000,"GasPremium":"800000000000000000000","Method":0,"Nonce":2,"To":"f3um6uo3qt5of54xjbx3hsxbw5mbsc6auxzrvfxekn5bv3duewqyn2tg5rhrlx73qahzzpkhuj7a34iq7oifsq","Value":"600000000000000000000"},"Signature":{"Data":"jMRu+OZ/lfppgmqSfGsntFrRLWZnUg3ZYmJTTRLsVt4V1310vR3VKGJpaE6S4sNvDOE6sEgmN9YmfTkPVK2qMgE=","Type":1}})");
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
    assertStringsEqual(result, R"({"Message":{"From":"f1z4a36sc7mfbv4z3qwutblp2flycdui3baffytbq","GasFeeCap":"700000000000000000000","GasLimit":1000,"GasPremium":"800000000000000000000","Method":0,"Nonce":2,"To":"f3um6uo3qt5of54xjbx3hsxbw5mbsc6auxzrvfxekn5bv3duewqyn2tg5rhrlx73qahzzpkhuj7a34iq7oifsq","Value":"600000000000000000000"},"Signature":{"Data":"jMRu+OZ/lfppgmqSfGsntFrRLWZnUg3ZYmJTTRLsVt4V1310vR3VKGJpaE6S4sNvDOE6sEgmN9YmfTkPVK2qMgE=","Type":1}})");
}

} // namespace TW::Filecoin::tests
