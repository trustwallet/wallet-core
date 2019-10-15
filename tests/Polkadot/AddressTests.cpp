// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Polkadot/Address.h"
#include <gtest/gtest.h>
#include <Base58.h>
#include <HexCoding.h>
#include "PrivateKey.h"
#include "PublicKey.h"

using namespace TW;
using namespace TW::Polkadot;

TEST(PolkaDot, ValidAddress) {
    ASSERT_TRUE(Address::isValid("136f5StL4ANS6oAdjCWZzEhdCXPcd5P3ZB4LtHnFBbWYv27r"));
    ASSERT_TRUE(Address::isValid("FsywiMe6BHC6jbdb826vLUCeGKzR1vdKcvCEzpLCSPnEGZH"));
    ASSERT_TRUE(Address::isValid("DtbB3Uy8zX9khHRf7SsSVk7esY5AngKvC9EJVhpjSzza2MU"));
    ASSERT_TRUE(Address::isValid("JHsk47Zje7rJVm5SCvY9PvHq9W2Rz1sJvGpCpFmwGG4rM8M"));
    ASSERT_TRUE(Address::isValid("DMZFfVn8yxNXroTXzuoRoVKnsitJJx4jnS3DoaficA4woPL"));
    ASSERT_TRUE(Address::isValid("FM4N1JNemHSJAnW8V99ijj7ncHSQ5X9fDs38pqkZZfW62Pr"));
    ASSERT_TRUE(Address::isValid("Dy8FsRWX8RMYDk4ndjUhBLHaeJikXBYFGaadjUDp8i2iKV4"));
    ASSERT_TRUE(Address::isValid("31nhDpVyLZJhQXdLfxGSceRKB2KhG99neZvBP82JtEjJwRMe"));
}

TEST(PolkaDot, InvalidAddress) {
    ASSERT_FALSE(Address::isValid("DtbB3Uy8zX9ihHRf7SsSVk7esY5AngKvC9EJVhpjSzza2MU"));
    ASSERT_FALSE(Address::isValid("DtbBza2MU"));
    ASSERT_FALSE(Address::isValid(""));
}

TEST(PolkaDot, AddressFromString1) {
    std::string addr = "DtbB3Uy8zX9khHRf7SsSVk7esY5AngKvC9EJVhpjSzza2MU";
    Address a(addr);
    ASSERT_EQ(addr, a.string());
}

TEST(PolkaDot, AddressFromString2) {
    std::string addr = "FQ4cZ1o9Bfq5FTViZM27NRPimh6JfJbYA4dsMS4Lhquj9XK";
    Address a(addr);
    ASSERT_EQ(addr, a.string());
}

TEST(PolkaDot, AddressFromString3) {
    std::string addr = "FsywiMe6BHC6jbdb826vLUCeGKzR1vdKcvCEzpLCSPnEGZH";
    Address a(addr);
    ASSERT_EQ(addr, a.string());
}

TEST(PolkaDot, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("5cc8df138a881319a75ab3a4f9621cf4fe614948f45a87118093edfab1679c64"), TWPublicKeyTypeED25519);
    Address a(publicKey, TWPolkaDotAddressTypePolkaDotLiveSS58, 2);
    ASSERT_EQ("136f5StL4ANS6oAdjCWZzEhdCXPcd5P3ZB4LtHnFBbWYv27r", a.string());
}

TEST(PolkaDot, FromPrivateKey) {
    auto privateKey = PrivateKey(parse_hex("ae92f15b17ad7a306f9e3e24d293d3988e625990c716ca099b623611695f02e0"));
    auto publicKey = PublicKey(privateKey.getPublicKey(TWPublicKeyTypeED25519));
    Address a(publicKey, TWPolkaDotAddressTypePolkaDotLiveSS58, 2);
    ASSERT_EQ("136f5StL4ANS6oAdjCWZzEhdCXPcd5P3ZB4LtHnFBbWYv27r", a.string());
}

TEST(PolkaDot, FromPublicKey2) {
    auto publicKey = PublicKey(parse_hex("3da0f8a016e4e0c7a7dd5bdaabfb8ce2df90f5a0434528546e1c0dc18a223d03"), TWPublicKeyTypeED25519);
    Address a(publicKey, TWPolkaDotAddressTypePolkaDotCanarySS58, 2);
    ASSERT_EQ("Dy8FsRWX8RMYDk4ndjUhBLHaeJikXBYFGaadjUDp8i2iKV4", a.string());
}

TEST(PolkaDot, FromPrivateKey2) {
    auto privateKey = PrivateKey(parse_hex("46476884d5e274c0119e07e74f6ac66b9d2eaf3fc30ed967c7bf035604bb23df"));
    auto publicKey = PublicKey(privateKey.getPublicKey(TWPublicKeyTypeED25519));
    Address a(publicKey, TWPolkaDotAddressTypePolkaDotCanarySS58, 2);
    ASSERT_EQ("Dy8FsRWX8RMYDk4ndjUhBLHaeJikXBYFGaadjUDp8i2iKV4", a.string());
}
