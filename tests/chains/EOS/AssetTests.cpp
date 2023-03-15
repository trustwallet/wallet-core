// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "EOS/Asset.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

namespace TW::EOS::tests {

TEST(EOSAsset, Serialization) {
    Data buf;
    Asset(5000, 3, "BRAVO").serialize(buf);
    ASSERT_EQ(hex(buf), "881300000000000003425241564f0000");

    buf.clear();
    Asset(90000, 3, "BRAVO").serialize(buf);
    ASSERT_EQ(hex(buf), "905f01000000000003425241564f0000");

    buf.clear();
    Asset(1000, 3, "BRAVO").serialize(buf);
    ASSERT_EQ(hex(buf), "e80300000000000003425241564f0000");

    std::string assetStr = "3.141 PI";
    ASSERT_EQ(Asset::fromString(assetStr).string(), assetStr);

    // add tests for negative amounts, fractional amounts
}

} // namespace TW::EOS
