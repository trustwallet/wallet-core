// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "rust/bindgen/WalletCoreRSBindgen.h"

#include "gtest/gtest.h"

TEST(RustBindgen, MoveParseFunctionArgument) {
    std::string arg = "10000000";
    auto* result = parse_function_argument_to_bcs(arg.c_str());
    ASSERT_EQ(std::string(result), "8096980000000000");
    free_string(result);
}
