#include <stdexcept>
#include <iostream>

#include "Bravo/Operation.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Bravo;

TEST(SteemOperation, Validation) {
	ASSERT_THROW(TransferOperation(std::string(MaxAccountNameSize + 1, 'a'), "bob", 90000, false, "Eva"), std::invalid_argument);

	ASSERT_THROW(TransferOperation("alice", std::string(MaxAccountNameSize + 1, 'a'), 90000, false, "Eva"), std::invalid_argument);

	ASSERT_THROW(TransferOperation("alice", "bob", 90000, false, std::string(MaxMemoSize + 1, 'a')), std::invalid_argument);

	ASSERT_THROW(TransferOperation("alice", "bob", 0, false, "Eva"), std::invalid_argument);

	ASSERT_NO_THROW(TransferOperation("alice", "bob", 90000, false, "Eva"));
}

TEST(SteemOperation, Serialization) {
	Data buf;
	TransferOperation("alice", "bob", "90.000 STEEM", "Eva").serialize(buf);
	ASSERT_EQ(
		hex(buf),
		"0205616c69636503626f62905f01000000000003535445454d000003457661"
	);

	buf.clear();
	TransferOperation("Ram", "Sita", "1.000 STEEM", "Lakshman").serialize(buf);
	ASSERT_EQ(
		hex(buf),
		"020352616d0453697461e80300000000000003535445454d0000084c616b73686d616e"
	);
}