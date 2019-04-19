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
    "1f05b3dddfcbca5748b02562645fe7c3f01044b78983ce673f84c230b2dc97beed19b2e8462f1e45f2ac7600c2900f9f90510efe0891141ad2c6b1ae33b21bcace",
    "2074ce46787639f1cb7009cfff3893bb91ed8cd03cbb8e7858e09b6d656c663499050a2b325c9884ff3049b459323593bbe8868ec3f3be659cf44f12bb083f0a0a",
    "1f1d562416a02d37f805aeb813925df13f4559baa56d26774b04dfeed39d8937bd499573df14d5a602cb7a672a18ffedd2b650edc89a4d3442de34c247828d9e0e",
    "206182810e412ae4ac9e89e3487d64fad048ec1747fb33b9ceae6b74b849671a826e45321e045f21e86de49b89de651a46c456dd08c6d8c6f4036bb831e47c4e56",
    "1f2fc97f4f1c24b09f08a17663ad97f5a0c95e0417a1f5aaf76e2743e68fd46eee08bef43ff863cfb65d1e183e42ecf31692663a289ea1205900e764a5c49bf0e7"
};

TEST(BravoTransaction, Serialization) {
    ASSERT_THROW(Transaction(Data(), 0), std::invalid_argument);

    auto referenceBlockId = parse_hex("0000086bf9e7704509aa41311a66fa0a1b479c6b");
    int32_t referenceBlockTime = 1552464180;
    auto chainId = parse_hex("0000000000000000000000000000000000000000000000000000000000000000");

    Signer signer {chainId};

    Transaction tx {referenceBlockId, referenceBlockTime};
    tx.addOperation(new TransferOperation("alice", "bob", 90000, false, "Eva"));

    for (int i = 0; i < 5; i++) {
        PrivateKey pk(Hash::sha256(std::string(i + 1, 'A')));

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

    ASSERT_NO_THROW(tx.serialize());
}

TEST(BravoTransaction, Signature) {
    ASSERT_THROW(Signature(parse_hex("aabbccdd")), std::invalid_argument);

    const Data validSig = parse_hex("1f05b3dddfcbca5748b02562645fe7c3f01044b78983ce673f84c230b2dc97beed19b2e8462f1e45f2ac7600c2900f9f90510efe0891141ad2c6b1ae33b21bcace");

    Signature *sig = nullptr;
    ASSERT_NO_THROW(sig = new Signature(validSig));

    ASSERT_EQ(
        sig->string(),
        hex(validSig)
    );

    Data buf;
    sig->serialize(buf);
    ASSERT_EQ(buf, validSig);

    delete sig;
}

// Tests for the new sign function in PrivateKey for graphene-based systems
TEST(BravoTransaction, GrapheneSigning) {
    const Data message = Hash::sha256(std::string("Hello, world!"));
    const PrivateKey privKey {Hash::sha256(std::string("A"))};
    Data secpSig = privKey.sign(message, TWCurveSECP256k1, Signer::is_canonical);
    Data nistSig = privKey.sign(message, TWCurveNIST256p1, Signer::is_canonical);

    // ensure both are canonical
    ASSERT_TRUE(Signer::is_canonical(*secpSig.data(), secpSig.data() + 1));
    ASSERT_TRUE(Signer::is_canonical(*nistSig.data(), nistSig.data() + 1));

    // check values
    ASSERT_EQ(
        hex(secpSig),
        "2060577922244d9916fe93231806e64718e5f970f6afa90ac09ac8b7e434dbf3c218034d9e98a6726ae17360408449362b038d885bb010f0241f78e963a38a7347"
    );
    ASSERT_EQ(
        hex(nistSig),
        "2007ea15cce415779f39107006ff95108b386d68bcb6e24d9a0b1fda5361cc87031ee8d37a668ca42a298b003045b32b6d753ab855c9740d462ed9b48ad75397d1"
    );
}