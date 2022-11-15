//
// Created by Roman Sztergbaum on 15/11/2022.
//

#include "ImmutableX/StarkKey.h"
#include <gtest/gtest.h>

namespace TW::ImmutableX::tests {

TEST(ImmutableX, PathFromAddress) {
    // https://github.com/immutable/imx-core-sdk-swift/blob/main/Tests/ImmutableXCoreTests/Crypto/Stark/StarkKeyTests.swift#L30
    auto res = accountPathFromAddress("0xa76e3eeb2f7143165618ab8feaabcd395b6fac7f");
    ASSERT_EQ(res, "m/2645'/579218131'/211006541'/1534045311'/3937127737'/1");
}

}
