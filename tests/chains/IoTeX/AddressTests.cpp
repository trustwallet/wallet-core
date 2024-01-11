// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include <gtest/gtest.h>

#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"

#include "IoTeX/Address.h"

namespace TW::IoTeX {

TEST(IoTeXAddress, Invalid) {
    ASSERT_FALSE(Address::isValid("io187wzp08vnhjjpkydnr97qlh8kh0dpkkytfam8"));
    ASSERT_FALSE(Address::isValid("io187wzp08vnhjpkydnr97qlh8kh0dpkkytfam8j"));
    ASSERT_FALSE(Address::isValid("it187wzp08vnhjjpkydnr97qlh8kh0dpkkytfam8j"));
    ASSERT_FALSE(Address::isValid("ko187wzp08vnhjjpkydnr97qlh8kh0dpkkytfam8j"));
    ASSERT_FALSE(Address::isValid("io187wzp18vnhjjpkydnr97qlh8kh0dpkkytfam8j"));
    ASSERT_FALSE(Address::isValid("io187wzp08vnhjbpkydnr97qlh8kh0dpkkytfam8j"));
    ASSERT_FALSE(Address::isValid("io187wzp08vnhjjpkydnr97qlh8kh0dpkkytfam8k"));
}

TEST(IoTeXAddress, Valid) {
    ASSERT_TRUE(Address::isValid("io187wzp08vnhjjpkydnr97qlh8kh0dpkkytfam8j"));
}

TEST(IoTeXAddress, FromString) {
    Address address;
    ASSERT_TRUE(Address::decode("io187wzp08vnhjjpkydnr97qlh8kh0dpkkytfam8j", address));
    ASSERT_EQ("io187wzp08vnhjjpkydnr97qlh8kh0dpkkytfam8j", address.string());

    ASSERT_FALSE(Address::decode("io187wzp08vnhjbpkydnr97qlh8kh0dpkkytfam8j", address));
}

TEST(IoTeXAddress, FromPrivateKey) {
    const auto privateKey = PrivateKey(parse_hex("0806c458b262edd333a191e92f561aff338211ee3e18ab315a074a2d82aa343f"));
    const auto publicKey = PublicKey(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1Extended));
    const auto address = Address(publicKey);
    ASSERT_EQ(address.string(), "io187wzp08vnhjjpkydnr97qlh8kh0dpkkytfam8j");

    EXPECT_THROW({
        try
        {
            const auto _publicKey = PublicKey(privateKey.getPublicKey(TWPublicKeyTypeSECP256k1));
            const auto _address = Address(_publicKey);
        }
        catch( const std::invalid_argument& e )
        {
            EXPECT_STREQ("address may only be an extended SECP256k1 public key", e.what());
            throw;
        }
    }, std::invalid_argument);
}

TEST(IoTeXAddress, FromKeyHash) {
    const auto keyHash = parse_hex("3f9c20bcec9de520d88d98cbe07ee7b5ded0dac4");
    const auto address = Address(keyHash);
    ASSERT_EQ(address.string(), "io187wzp08vnhjjpkydnr97qlh8kh0dpkkytfam8j");

    EXPECT_THROW({
        try
        {
            const auto _keyHash = parse_hex("3f9c20bcec9de520d88d98cbe07ee7b5ded0da");
            const auto _address = Address(_keyHash);
        }
        catch( const std::invalid_argument& e )
        {
            EXPECT_STREQ("invalid address data", e.what());
            throw;
        }
    }, std::invalid_argument);
}

} // namespace TW::IoTeX
