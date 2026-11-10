// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Base58.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "proto/Solana.pb.h"
#include "TestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>

#include <gtest/gtest.h>

using namespace TW;
namespace TW::Solana::tests {

const auto expectedString1 =
    "3p2kzZ1DvquqC6LApPuxpTg5CCDVPqJFokGSnGhnBHrta4uq7S2EyehV1XNUVXp51D69GxGzQZU"
    "jikfDzbWBG2aFtG3gHT1QfLzyFKHM4HQtMQMNXqay1NAeiiYZjNhx9UvMX4uAQZ4Q6rx6m2AYfQ"
    "7aoMUrejq298q1wBFdtS9XVB5QTiStnzC7zs97FUEK2T4XapjF1519EyFBViTfHpGpnf5bfizDz"
    "sW9kYUtRDW1UC2LgHr7npgq5W9TBmHf9hSmRgM9XXucjXLqubNWE7HUMhbKjuBqkirRM";

TEST(TWAnySignerSolana, SignTransfer) {
    auto privateKey = Base58::decode("A7psj2GW7ZMdY4E5hJq14KMeYg7HFjULSsWSrTXZLvYr");
    auto input = Proto::SigningInput();

    auto& message = *input.mutable_transfer_transaction();
    message.set_recipient("EN2sCsJ1WDV8UFqsiTXHcUPUxQ4juE71eCknHYYMifkd");
    message.set_value((uint64_t)42L);
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_recent_blockhash("11111111111111111111111111111111");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    ASSERT_EQ(output.encoded(), expectedString1);
    ASSERT_EQ(output.unsigned_tx(), "87PYsiS4MUU1UqXrsDoCBmD5FcKsXhwEBD8hc4zbq78yePu7bLENmbnmjmVbsj4VvaxnZhy4bERndPFzjSRH5WpwKwMLSCKvn9eSDmPESNcdkqne2UdMfWiFoq8ZeQBnF9h98dP8GM9kfzWPjvLmhjwuwA1E2k5WCtfii7LKQ34v6AtmFQGZqgdKiNqygP7ZKusHWGT8ZkTZ");
}

TEST(TWAnySignerSolana, SignTransferCoreAsset) {
    // Throwaway key, see rust/tw_tests/tests/chains/solana/solana_sign.rs.
    auto privateKey = parse_hex("cb90b1e7b2c559088c5ff6acbbb588ae392e221492046113a62eac0ab254cdf8");
    auto input = Proto::SigningInput();

    auto& message = *input.mutable_transfer_core_asset_transaction();
    message.set_asset_address("DdC5i5AFrHCvcaEUEcCktFBiytHoGbMgUQ7JBwfRGihL");
    message.set_recipient_address("ArgPLJSF7ujjPZgooHGpRcnFDmftpDSzvhLRNsH94zmo");
    message.set_collection_address("CCryptUfeFSZ3Fgc9FLeKrhLVAP67FSqi1GuVoj9CRac");
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_recent_blockhash("4G8PGaBMdz3Sqi43W9xL1rQHqhorF1ZbVZoabVejs7uv");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeSolana);

    ASSERT_EQ(output.error(), Common::Proto::OK);
    // Successfully broadcasted tx - https://solscan.io/tx/3DxmEtgGHpY8mm1QfzgRzBLL4TLQ1Q96mcYE2LdKG6gZuyWQQjnDCAXFqrhKvGvmEQriZuLpZWHsAkXcLJRSAiMC
    ASSERT_EQ(output.encoded(),
        "hq79tP91qTbzV6Qbe7FKZ8ru8wba2gVEN48LBaARqTLZGVZMja8xj9S8v7gKuUpQk6EgaWTk3t8z"
        "nG9Z7qKvWdNujkJjUbtVmffnkyMi8GdjhgTRc4i1wSytyZpgJaUD8vhCRAJEmcZBzrq4kTCi3iJT"
        "776jYuAgB1u3KPC3pPzEpk8ktpz8ocSkKo1A9oXiU6hkhgs8bj8FbCEoLbvAmg67Ut7GoyK8kb9g"
        "AYD8SZYNKPN39oerw5EaP3md3DBDGDJ1aLaHPsUwtbG66PyoLYnCWGbgbcRW2WPTBqDc8RdJWUR2"
        "8VdAj7wTEJ7jSeBaHLiFtYexNCPon8omyVV5KU5eqwHMGNCWMLkWjd6RrwB1p92M42E1M");
}

} // namespace TW::Solana::tests
