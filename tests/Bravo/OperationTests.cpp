#include <stdexcept>
#include <iostream>

#include "Bravo/Operation.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Bravo;

TEST(BravoOperation, Validation) {
    ASSERT_THROW(TransferOperation(std::string(MaxAccountNameSize + 1, 'a'), "bob", 90000, false, "Eva"), std::invalid_argument);

    ASSERT_THROW(TransferOperation("alice", std::string(MaxAccountNameSize + 1, 'a'), 80000, false, "Eva"), std::invalid_argument);

    ASSERT_THROW(TransferOperation("alice", "bob", 70000, false, std::string(MaxMemoSize + 1, 'a')), std::invalid_argument);

    ASSERT_NO_THROW(TransferOperation("alice", "bob", 60000, false, "Eva"));

    ASSERT_NO_THROW(TransferOperation("Einstein", "Eddington", "6.0 RANDOM", "Gravity bends light."));
}

TEST(BravoOperation, Serialization) {
    Data buf;
    TransferOperation("alice", "bob", 90000, false, "Eva").serialize(buf);
    ASSERT_EQ(
        hex(buf),
        "0205616c69636503626f62905f01000000000003425241564f000003457661"
    );

    buf.clear();
    TransferOperation("Ram", "Sita", 1000, false, "Lakshman").serialize(buf);
    ASSERT_EQ(
        hex(buf),
        "020352616d0453697461e80300000000000003425241564f0000084c616b73686d616e"
    );
}