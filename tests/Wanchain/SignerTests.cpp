// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Ethereum/Address.h"
#include "Ethereum/RLP.h"
#include "HexCoding.h"
#include "Wanchain/Signer.h"

#include <gtest/gtest.h>

namespace TW {
namespace Wanchain {

using boost::multiprecision::uint256_t;

class SignerExposed : public Signer {
public:
    SignerExposed(boost::multiprecision::uint256_t chainID) : Signer(chainID) {}
    using Signer::hash;
};

TEST(Signer, Sign) {
    auto address = Ethereum::Address("0x3535353535353535353535353535353535353535");
    auto transaction = Ethereum::Transaction(
        /* nonce: */ 9,
        /* gasPrice: */ 20000000000,
        /* gasLimit: */ 21000,
        /* to: */ address,
        /* amount: */ 1000000000000000000,
        /* payload: */ {}
    );

    auto key = PrivateKey(parse_hex("0x4646464646464646464646464646464646464646464646464646464646464646"));
    auto signer = SignerExposed(1);
    signer.sign(key, transaction);

    ASSERT_EQ(transaction.v, 37);
    ASSERT_EQ(transaction.r, uint256_t("81387416166961732074688826827458851755389597777750181844865627349674117325426"));
    ASSERT_EQ(transaction.s, uint256_t("57615878674481745412864351256912693004505281517367788339541976444424345429095"));
}

}} // namespace
