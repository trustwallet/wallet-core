// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Everscale/Address.h"
#include "Everscale/WorkchainType.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include <gtest/gtest.h>
#include <vector>

using namespace TW;

namespace TW::Everscale {

TEST(EverscaleAddress, Valid) {
    ASSERT_TRUE(Address::isValid("0:83a0352908060fa87839195d8a763a8d9ab28f8fa41468832b398a719cc6469a"));
}

TEST(EverscaleAddress, Invalid) {
    ASSERT_FALSE(Address::isValid("hello world"));
    ASSERT_FALSE(Address::isValid("83a0352908060fa87839195d8a763a8d9ab28f8fa41468832b398a719cc6469a"));
    ASSERT_FALSE(Address::isValid("1:83a0352908060fa87839195d8a763a8d9ab28f8fa41468832b398a719cc6469a"));
    ASSERT_FALSE(Address::isValid("-2:83a0352908060fa87839195d8a763a8d9ab28f8fa41468832b398a719cc6469a"));
    ASSERT_FALSE(Address::isValid("2147483648:83a0352908060fa87839195d8a763a8d9ab28f8fa41468832b398a719cc6469a"));
    ASSERT_FALSE(Address::isValid("0:83a0352908060fa87839195d8a763a8d9ab28f8fa41468832b398a719cc6469ab"));
}

TEST(EverscaleAddress, FromString) {
    auto address = Address("0:83a0352908060fa87839195d8a763a8d9ab28f8fa41468832b398a719cc6469a");
    ASSERT_EQ(address.string(), "0:83a0352908060fa87839195d8a763a8d9ab28f8fa41468832b398a719cc6469a");

    auto address_uppercase = Address("0:83A0352908060FA87839195D8A763A8D9AB28F8FA41468832B398A719CC6469A");
    ASSERT_EQ(address_uppercase.string(), "0:83a0352908060fa87839195d8a763a8d9ab28f8fa41468832b398a719cc6469a");
}

TEST(EverscaleAddress, FromPrivateKey) {
    auto privateKey = PrivateKey(parse_hex("5b59e0372d19b6355c73fa8cc708fa3301ae2ec21bb6277e8b79d386ccb7846f"));
    auto address = Address(privateKey.getPublicKey(TWPublicKeyTypeED25519), WorkchainType::Basechain);
    ASSERT_EQ(address.string(), "0:269fee242eb410786abe1777a14785c8bbeb1e34100c7570e17698b36ad66fb0");
}

TEST(EverscaleAddress, FromPublicKey) {
    auto publicKey = PublicKey(parse_hex("e4925f9932df8d7fd0042efff3e2178a972028b644ded3a3b66f6d0577f82e78"), TWPublicKeyTypeED25519);
    auto address = Address(publicKey, WorkchainType::Basechain);
    ASSERT_EQ(address.string(), "0:269fee242eb410786abe1777a14785c8bbeb1e34100c7570e17698b36ad66fb0");
}

} // namespace TW::Everscale
