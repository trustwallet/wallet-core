// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Tezos/TWTezosForger.h"

#include <gtest/gtest.h>

TEST(TWTezosForger, ForgeBranch) {
  auto input = "BMNY6Jkas7BzKb7wDLCFoQ4YxfYoieU7Xmo1ED3Y9Lo3ZvVGdgW";
  auto expected = "da8eb4f57f98a647588b47d29483d1edfdbec1428c11609cee0da6e0f27cfc38";
  
  auto output = forgeBranch(input);

  ASSERT_EQ(ouput, expected)
}

TEST(TWTezosForger, ForgeBoolTrue) {
  auto expected = "00";
  
  auto output = forgeBool(true);
  
  ASSERT_EQ(output, expected);
}

TEST(TWTezosForger, ForgeBoolFalse) {
  auto expected = "FF";
  
  auto output = forgeBool(false);

  ASSERT_EQ(output, expected);
}

