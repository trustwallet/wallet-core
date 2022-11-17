//
// Created by Roman Sztergbaum on 15/11/2022.
//

#include "ImmutableX/StarkKey.h"
#include <gtest/gtest.h>

namespace TW::ImmutableX::tests {

TEST(ImmutableX, PathFromAddress) {
    // https://github.com/immutable/imx-core-sdk-swift/blob/main/Tests/ImmutableXCoreTests/Crypto/Stark/StarkKeyTests.swift#L30
    auto res = accountPathFromAddress("0xa76e3eeb2f7143165618ab8feaabcd395b6fac7f");
    ASSERT_EQ(res, "m/2645'/579218131'/211006541'/1534045311'/1431804530'/1");
}

TEST(ImmutableX, GrindKey) {
    // https://github.com/immutable/imx-core-sdk-swift/blob/main/Tests/ImmutableXCoreTests/Crypto/Stark/StarkKeyTests.swift#L30
    auto res = grindKey("86F3E7293141F20A8BAFF320E8EE4ACCB9D4A4BF2B4D295E8CEE784DB46E0519");
    ASSERT_EQ(res, "5c8c8683596c732541a59e03007b2d30dbbbb873556fe65b5fb63c16688f941");
}

}
