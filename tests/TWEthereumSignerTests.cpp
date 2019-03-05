
// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"
#include <TrustWalletCore/TWEthereumSigner.h>
#include "Ethereum/Transaction.h"
#include "Data.h"
#include "HexCoding.h"
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
    // Check uint256_t loading
    Data expectedData = {0x52, 0x08};
    auto value = uint256_t(21000);
    auto loaded = load(expectedData);
    ASSERT_EQ(loaded, value);

    // Check proto storing
    Proto::SigningInput input;
    auto storedData = store(value);
    input.set_gas_limit(storedData.data(), storedData.size());
    ASSERT_EQ(hex(input.gas_limit()), hex(expectedData));

    // Check proto loading
    auto protoLoaded = load(input.gas_limit());
    ASSERT_EQ(protoLoaded, value);
}
