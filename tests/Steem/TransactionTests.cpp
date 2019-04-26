#include "Bravo/Serialization.h"
#include "Bravo/Signer.h"
#include "Bravo/Transaction.h"
#include "Hash.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Bravo;

const std::string signs[] = {"20e8d87c007becf88802ab0e074c001f070cd6522e8d9966feb344b3cd761d4328612"
                             "7787d25fa0aa4cb76969f1facb34e76fc460f7feb029c650bef697080e55e",
                             "2092b04bff8bd7ebfe0dd9ce938038f98d5c4bd4fd40d99bf94318b9c675f5b93440c"
                             "650fe74e355a9e62a86acd800872666f3841524ede3bd0db117e322c9a558",
                             "1f6bdaa662ef4da525e9c6d1013c64e92801a826eb00e82ced1c161758aefe7284318"
                             "c6996e78cd8a2699d696a315ef56b1263411a34f7589993db19aaa1643fc2",
                             "204cb00a8561afed98ed2d7ddae89ad143f3029585114007725a0e287d2d7c7496036"
                             "1cab8676ec10b2c14ef43a87b6aeb1a569e573b96aad1825ad134f8efc8ad",
                             "20f80ab9f58ed70a646a71857e0a2739128a0efbe158d751249467a9a4b3d834a8637"
                             "f6391dedc9620f0380dc71cd545bf7dcefd36b67071d366a2c99de5688c7a"};

TEST(SteemTransaction, Serialization) {

    auto referenceBlockId = parse_hex("000000c4819e7d5c54c23cca3f438a1f62aa2464");
    int32_t referenceBlockTime = 1553596224;
    auto chainId = parse_hex("0000000000000000000000000000000000000000000000000000000000000000");

    Signer signer{chainId};

    Transaction tx{referenceBlockId, referenceBlockTime};
    tx.addOperation(
        new TransferOperation("alice", "bob", Asset(90000, Asset::decimals, "STEEM"), "Eva"));

    for (int i = 0; i < 5; i++) {
        PrivateKey pk(Hash::sha256(std::string(i + 1, 'A')));

        ASSERT_NO_THROW(signer.sign(pk, tx, nullptr));
        const auto &sigs = tx.getSignatures();
        ASSERT_TRUE(sigs.size() == i + 1);

        ASSERT_EQ(hex(sigs.back().data), signs[i]);
    }

    Data buf;
    tx.serialize(buf);
    ASSERT_EQ(
        hex(buf),
        "c400819e7d5c98019a5c010205616c69636503626f62905f01000000000003535445454d00000345766100");
}