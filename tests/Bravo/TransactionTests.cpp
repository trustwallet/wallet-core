#include "Bravo/Transaction.h"
#include "Bravo/Serialization.h"
#include "Bravo/Signer.h"
#include "PrivateKey.h"
#include "HexCoding.h"
#include "Hash.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Bravo;

const std::string signs[] {
    "20217d319053597bfafc7d341d3abb3a29b87e4ba2242a0704e53e9108eabf37463d498d826508999561779d509c1d1d72212dae57b292b17af76bc7c958cae17e",
    "2061018907ff2c6fb58c33f2a6ba4a6a1a6f87d06a0e4f8b5c67eb58507c0ad5782f59a17ae1e7cf5c1b93caa13cdc0bc12ce0dc5150bb96afa7aebb87b972fedb",
    "1f588dd65fff1ad6378cc9120dff8262688c969d606f35601e1b9a4e7dac845f233c1e4dab15517bf4ee7103f2cdd1c1727da2792b51a1604e458e2ab9a81655c2",
    "2001420f58ebb0d28bfd448a2570ef90e9ec5087fc2e6d42636403c54153bea2a329ad3e611d391c10c67754949c8aa44ebb99579f121963f4a523bac47e597455",
    "2028e5b873f9d3b039c8dfa5af054e569cd4643bbe0751eef4d14a4cb3f3cfc7053c8f0e59d60dd82ebf8f016e8705fde42b55360f4a18c41fb1456c57017e5c15"
};

TEST(BravoTransaction, Serialization) {

    std::string referenceBlockId = "0000086bf9e7704509aa41311a66fa0a1b479c6b";
    uint32_t referenceBlockTime = 1552464180;
    auto chainId = parse_hex("0000000000000000000000000000000000000000000000000000000000000000");

    Signer signer {chainId};


    Transaction tx {referenceBlockId, referenceBlockTime};
    tx.addOperation(new TransferOperation("alice", "bob", 90000, false, "Eva"));

    for (int i = 0; i < 5; i++) {
        PrivateKey pk(Hash::sha256(hex(std::string(i, 'A'))));

        ASSERT_NO_THROW(signer.sign(pk, tx));
        const auto& sigs = tx.getSignatures();
        ASSERT_TRUE(sigs.size() == i + 1);

        ASSERT_EQ(hex(sigs.back().data),
            signs[i]
        );
    }

    Data buf;
    tx.serialize(buf);
    ASSERT_EQ(
        hex(buf),
        "6b08f9e770458cbb885c010205616c69636503626f62905f01000000000003425241564f00000345766100"
    );
}