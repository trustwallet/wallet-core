
// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TWTestUtilities.h"

//YZ: Not create TWHarmonySigner.h
//#include <TrustWalletCore/TWHarmonySigner.h>
// directly use the Ethereum one
#include <TrustWalletCore/TWEthereumSigner.h>

#include "Harmony/Transaction.h"
#include "Data.h"
#include "HexCoding.h"
#include "uint256.h"

// YZ: using Ethereum.pb
#include "proto/Ethereum.pb.h"

#include <gtest/gtest.h>

// YZ

using namespace TW;
using namespace Harmony;

TEST(TWHarmonySigner, EmptyValue) {
    auto str = std::string("");
    uint256_t zero = load(str);

    ASSERT_EQ(zero, uint256_t(0));
}

TEST(TWHarmonySigner, BigInt) {
    // Check uint256_t loading
    Data expectedData = {0x52, 0x08};
    auto value = uint256_t(21000);
    auto loaded = load(expectedData);
    ASSERT_EQ(loaded, value);

    // YZ added Ethereum::
    // Check proto storing
    Ethereum::Proto::SigningInput input;
    auto storedData = store(value);
    input.set_gas_limit(storedData.data(), storedData.size());
    ASSERT_EQ(hex(input.gas_limit()), hex(expectedData));

    // Check proto loading
    auto protoLoaded = load(input.gas_limit());
    ASSERT_EQ(protoLoaded, value);
}
