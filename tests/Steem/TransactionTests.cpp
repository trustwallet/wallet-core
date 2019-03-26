#include "Steem/Signer.h"
#include "Bravo/Transaction.h"
#include "Bravo/Serialization.h"
#include "Bravo/Signer.h"
#include "PrivateKey.h"
#include "HexCoding.h"
#include "Hash.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Bravo;

TEST(SteemTransaction, Serialization) {

	std::string referenceBlockId = "000000c4819e7d5c54c23cca3f438a1f62aa2464";
	uint32_t referenceBlockTime = 1553596224;
	auto chainId = parse_hex("0000000000000000000000000000000000000000000000000000000000000000");

	Signer signer{ chainId };


	Transaction tx{ referenceBlockId, referenceBlockTime };
	tx.addOperation(new TransferOperation("alice", "bob", "90.000 STEEM", "Eva"));

	for (int i = 0; i < 5; i++) {
		PrivateKey pk(Hash::sha256(hex(std::string(i, 'A'))));

		ASSERT_NO_THROW(signer.sign(pk, tx, TW::Steem::is_canonical));
		const auto& sigs = tx.getSignatures();
		ASSERT_TRUE(sigs.size() == i + 1);

		Data buf;
		sigs.back().serialize(buf);

		// create our own digest
		Data hashInput(chainId);
		tx.serialize(hashInput);
		Data digest = Hash::sha256(hashInput);

		Data pubKey;
		pubKey.resize(65);
		ASSERT_TRUE(signer.verify(pk, sigs.back(), tx));
	}

	Data buf;
	tx.serialize(buf);
	ASSERT_EQ(
		hex(buf),
		"c400819e7d5c98019a5c010205616c69636503626f62905f01000000000003535445454d00000345766100"
	);
}