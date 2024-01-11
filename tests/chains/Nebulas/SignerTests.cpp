// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "Nebulas/Address.h"
#include "Nebulas/Signer.h"
#include <time.h>

#include <gtest/gtest.h>

namespace TW::Nebulas {

class SignerExposed : public Signer {
  public:
    SignerExposed(boost::multiprecision::uint256_t chainID) : Signer(chainID) {}
    using Signer::hash;
};

TEST(NebulasSigner, Hash) {
    auto from = Address("n1V5bB2tbaM3FUiL4eRwpBLgEredS5C2wLY");
    auto to = Address("n1SAeQRVn33bamxN4ehWUT7JGdxipwn8b17");
    auto transaction = Transaction(
        /* to: */ from,
        /* nonce: */ 7,
        /* gasPrice: */ 1000000,
        /* gasLimit: */ 200000,
        /* to: */ to,
        /* amount: */ 11000000000000000000ULL,
        /* timestamp: */ 1560052938,
        /* payload: */ std::string());
    auto signer = SignerExposed(1);
    auto hash = signer.hash(transaction);

    ASSERT_EQ(hex(hash), "505dd4769de32a9c4bb6d6afd4f8e1ea6474815fd43484d8917cbd9e0993b885");
}

} // namespace TW::Nebulas
