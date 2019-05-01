#include "Bravo/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "Steem/Address.h"

#include <gtest/gtest.h>
#include <string>

using namespace TW;
using namespace TW::Bravo;

TEST(SteemAddress, Base58) {

    ASSERT_EQ(Address("STM6LLegbAgLAy28EHrffBVuANFWcFgmqRMW13wBmTExqFE9SCkg4",
                      {TW::Steem::MainnetPrefix, TW::Steem::TestnetPrefix})
                  .string(),
              "STM6LLegbAgLAy28EHrffBVuANFWcFgmqRMW13wBmTExqFE9SCkg4");
    ASSERT_EQ(Address("TST52K5kMmwiRyNQYAf7ymCMz6hieE8siyrqNt1t57ac9hvBrRdaa",
                      {TW::Steem::MainnetPrefix, TW::Steem::TestnetPrefix})
                  .string(),
              "TST52K5kMmwiRyNQYAf7ymCMz6hieE8siyrqNt1t57ac9hvBrRdaa");
}

TEST(SteemAddress, IsValid) {

    ASSERT_TRUE(Address::isValid("STM52K5kMmwiRyNQYAf7ymCMz6hieE8siyrqNt1t57ac9hvBrRdaa",
                                 {TW::Steem::MainnetPrefix, TW::Steem::TestnetPrefix}));
    ASSERT_TRUE(Address::isValid("TST6EFA9Ge5KQaCS2jGZVf7xHZ6hzcH7uvikf5oR7YnYKemkucxB4",
                                 {TW::Steem::MainnetPrefix, TW::Steem::TestnetPrefix}));
}