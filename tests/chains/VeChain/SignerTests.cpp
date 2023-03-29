// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "VeChain/Signer.h"

#include <gtest/gtest.h>

namespace TW::VeChain {

using boost::multiprecision::uint256_t;

TEST(Signer, Sign) {
    auto transaction = Transaction();
    transaction.chainTag = 1;
    transaction.blockRef = 1;
    transaction.expiration = 1;
    transaction.clauses.push_back(
        Clause(Ethereum::Address("0x3535353535353535353535353535353535353535"), 1000, {})
    );
    transaction.gasPriceCoef = 0;
    transaction.gas = 21000;
    transaction.nonce = 1;

    auto key = PrivateKey(parse_hex("0x4646464646464646464646464646464646464646464646464646464646464646"));
    auto signature = Signer::sign(key, transaction);

    ASSERT_EQ(hex(signature), "3181b1094150f8e4f51f370b805cc9c5b107504145b9e316e846d5e5dbeedb5c1c2b5d217f197a105983dfaad6a198414d5731c7447493cb6b5169907d73dbe101");
}

} // namespace TW::VeChain
