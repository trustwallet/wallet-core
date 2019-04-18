#include <stdexcept>
#include <iostream>

#include "Bravo/Operation.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Bravo;

TEST(BravoAsset, Serialization) {
    Data buf;
    Asset(5000, false).serialize(buf);
    ASSERT_EQ(
        hex(buf),
        "881300000000000003425241564f0000"
    );

    buf.clear();
    Asset(90000, false).serialize(buf);
    ASSERT_EQ(
        hex(buf),
        "905f01000000000003425241564f0000"
    );

    buf.clear();
    Asset(1000, false).serialize(buf);
    ASSERT_EQ(
        hex(buf),
        "e80300000000000003425241564f0000"
    );

    std::string assetStr = "3.141 PI";
    ASSERT_EQ(
        Asset::fromString(assetStr).string(),
        assetStr
    );

    // add tests for negative amounts, fractional amounts
}