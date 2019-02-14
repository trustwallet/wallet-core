// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include <TrustWalletCore/TWHRP.h>

#include "gtest/gtest.h"
#include "TWTestUtilities.h"

TEST(TWHRP, StringForHRP) {
    ASSERT_STREQ(stringForHRP(TWHRPUnknown), nullptr);
    ASSERT_STREQ(stringForHRP(TWHRPBitcoin), "bc");
    ASSERT_STREQ(stringForHRP(TWHRPLitecoin), "ltc");
    ASSERT_STREQ(stringForHRP(TWHRPBitcoinCash), "bitcoincash");
    ASSERT_STREQ(stringForHRP(TWHRPBinance), "bnb");
}

TEST(TWHRP, HRPForString) {
    ASSERT_EQ(hrpForString(""), TWHRPUnknown);
    ASSERT_EQ(hrpForString("bc"), TWHRPBitcoin);
    ASSERT_EQ(hrpForString("ltc"), TWHRPLitecoin);
    ASSERT_EQ(hrpForString("bitcoincash"), TWHRPBitcoinCash);
    ASSERT_EQ(hrpForString("bnb"), TWHRPBinance);
}
