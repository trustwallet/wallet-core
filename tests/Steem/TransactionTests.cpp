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

const std::string signs[] = {
	"20be8c63fa6229c1f5f924977cd95c65ca751cf4dbce4d0e995ac1be45d4576166031eb486c6909119eaa31a2faf8237bc2a9d9de4921361ac8abe6e6ffff96f92",
	"209aa272c77eab11f3d46dd8684828fa722dfaa093021257a78b9ab016bd20cfb67aa7847c3e612485945f1d5cc06fabf2f20f99085e5517ffe9c2bd79184ccada",
	"1fefa7457dd7a5d95e42dc920bff9a513e2b9fc92d0ecf508e4ca503e2f8f947687f15ef64c162d807fba3840b69adc677f56957ff9c421b66ac3100fbef2d3cae",
	"1f956bd1a9f1e7acaea655fffe571639f6260a40ea182a128c1b31b91f526933fe66b41fd9e3bc191c8cd78fdc70f67448879c40ea6a98d403b12d715924de36fb",
	"1f519e8bed1c8a463034ba7b718355bca1f29ec6e1d3f5e9d7c4917726766fc5bd3a226d699ab0db2d25e8e92e40ba40b0f21b0be420bb3a1db17c00caff3de2f4"
};

TEST(SteemTransaction, Serialization) {

	std::string referenceBlockId = "000000c4819e7d5c54c23cca3f438a1f62aa2464";
	uint32_t referenceBlockTime = 1553596224;
	auto chainId = parse_hex("0000000000000000000000000000000000000000000000000000000000000000");

	Signer signer{ chainId };


	Transaction tx{ referenceBlockId, referenceBlockTime };
	tx.addOperation(new TransferOperation("alice", "bob", Asset(90000, Asset::decimals, "STEEM"), "Eva"));

	for (int i = 0; i < 5; i++) {
		PrivateKey pk(Hash::sha256(hex(std::string(i, 'A'))));

		ASSERT_NO_THROW(signer.sign(pk, tx, nullptr));
		const auto& sigs = tx.getSignatures();
		ASSERT_TRUE(sigs.size() == i + 1);

        ASSERT_EQ(
        	hex(sigs.back().data),
            signs[i]
        );
	}

	Data buf;
	tx.serialize(buf);
	ASSERT_EQ(
		hex(buf),
		"c400819e7d5c98019a5c010205616c69636503626f62905f01000000000003535445454d00000345766100"
	);
}