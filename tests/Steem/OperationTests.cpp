#include <iostream>
#include <stdexcept>

#include "Bravo/Operation.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Bravo;

TEST(SteemOperation, Serialization) {
    Data buf;
    TransferOperation("alice", "bob", Asset(90000, Asset::decimals, "STEEM"), "Eva").serialize(buf);
    ASSERT_EQ(hex(buf), "0205616c69636503626f62905f01000000000003535445454d000003457661");

    buf.clear();
    TransferOperation("Ram", "Sita", Asset(1000, Asset::decimals, "STEEM"), "Lakshman")
        .serialize(buf);
    ASSERT_EQ(hex(buf), "020352616d0453697461e80300000000000003535445454d0000084c616b73686d616e");
}