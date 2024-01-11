// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Base58.h"
#include "HexCoding.h"
#include "proto/Waves.pb.h"
#include "TestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>
#include <gtest/gtest.h>

namespace TW::Waves::tests {

TEST(TWAnySignerWaves, Sign) {
    auto input = Proto::SigningInput();
    const auto privateKey = Base58::decode("83mqJpmgB5Mko1567sVAdqZxVKsT6jccXt3eFSi4G1zE");

    input.set_timestamp(int64_t(1559146613));
    input.set_private_key(privateKey.data(), privateKey.size());
    auto& message = *input.mutable_transfer_message();
    message.set_amount(int64_t(100000000));
    message.set_asset("DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD81zq");
    message.set_fee(int64_t(100000));
    message.set_fee_asset("DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD81zq");
    message.set_to("3PPCZQkvdMJpmx7Zrz1cnYsPe9Bt1XT2Ckx");
    message.set_attachment("hello");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeWaves);

    ASSERT_EQ(hex(output.signature()), "5d6a77b1fd9b53d9735cd2543ba94215664f2b07d6c7befb081221fcd49f5b6ad6b9ac108582e8d3e74943bdf35fd80d985edf4b4de1fb1c5c427e84d0879f8f");
}

} // namespace TW::Waves::tests
