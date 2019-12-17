// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Base58.h"
#include "PrivateKey.h"
#include "Solana/Address.h"

#include <gtest/gtest.h>

using namespace std;
using namespace TW;
using namespace TW::Solana;

TEST(SolanaAddress, addressFromValidatorSeed) {
    auto user = Address("zVSpQnbBZ7dyUWzXhrUQRsTYYNzoAdJWHsHSqhPj3Xu");
    auto validator = Address("4jpwTqt1qZoR7u6u639z2AngYFGN3nakvKhowcnRZDEC");
    auto programId = Address("Stake11111111111111111111111111111111111111");
    auto expected = Address("6u9vJH9pRj66N5oJFCBADEbpMTrLxQATcL6q5p5MXwYv");
    ASSERT_EQ(addressFromValidatorSeed(user, validator, programId), expected);
}

TEST(SolanaAddress, FromPublicKey) {
    const auto addressString = "2gVkYWexTHR5Hb2aLeQN3tnngvWzisFKXDUPrgMHpdST";
    const auto publicKey = PublicKey(Base58::bitcoin.decode(addressString), TWPublicKeyTypeED25519);
    const auto address = Address(publicKey);
    ASSERT_EQ(addressString, address.string());
}

TEST(SolanaAddress, FromString) {
    string addressString = "2gVkYWexTHR5Hb2aLeQN3tnngvWzisFKXDUPrgMHpdST";
    const auto address = Address(addressString);
    ASSERT_EQ(address.string(), addressString);
}

TEST(SolanaAddress, isValid) {
    ASSERT_TRUE(Address::isValid("2gVkYWexTHR5Hb2aLeQN3tnngvWzisFKXDUPrgMHpdST"));
    ASSERT_FALSE(Address::isValid(
        "2gVkYWexTHR5Hb2aLeQN3tnngvWzisFKXDUPrgMHpdSl")); // Contains invalid base-58 character
    ASSERT_FALSE(
        Address::isValid("2gVkYWexTHR5Hb2aLeQN3tnngvWzisFKXDUPrgMHpd")); // Is invalid length
}
