
// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"
#include <TrustWalletCore/TWEthereumSigner.h>
#include "Ethereum/Transaction.h"
#include "Data.h"
#include "uint256.h"
#include "proto/Ethereum.pb.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace Ethereum;

TEST(TWEthereumSigner, EmptyValue) {
    auto str = std::string("");
    uint256_t zero = load(str);

    ASSERT_EQ(zero, uint256_t(0));
}

TEST(TWEthereumSigner, BigInt) {
    /// From Swift BigInt(21000).magnitude.serialize()
    Data bytes = {0x52, 0x08};
    auto limit = uint256_t(21000);
    auto loaded = load(bytes);
    ASSERT_EQ(loaded, limit);

    /// But it becomes "R\b" after assigning to SigningInput
    Proto::SigningInput input;
    input.set_gas_limit("R\b");

    auto limit2 = uint256_t(10504);
    auto loaded2 = load(input.gas_limit());
    ASSERT_EQ(limit2, loaded2);

    ASSERT_EQ(loaded2, loaded);
}
