// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "StoredKey.h"

#include <gtest/gtest.h>

namespace TW {
namespace Keystore {

TEST(StoredKey, LoadLegacyPrivateKey) {
    const auto key = StoredKey::load("tests/Keystore/private-key.json", "");
    ASSERT_EQ(key.id, "3051ca7d-3d36-4a4a-acc2-09e9083732b0");
}

}} // namespace
