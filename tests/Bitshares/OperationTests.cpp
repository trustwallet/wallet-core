#include <stdexcept>
#include <iostream>

#include "Bitshares/Operation.h"
#include "Bravo/Signer.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Bitshares;

TEST(BitsharesOperation, Invalid) {
    ASSERT_THROW(TransferOperation(12, 16, Asset(511, 0), Asset(-1, 0)), std::invalid_argument);
    ASSERT_THROW(TransferOperation(12, 12, Asset(511, 0), Asset(2, 0)), std::invalid_argument);
    ASSERT_THROW(TransferOperation(12, 16, Asset(0, 0), Asset(2, 0)), std::invalid_argument);
}

TEST(BitsharesOperation, Serialization) {
    PrivateKey pk1{Hash::sha256(std::string("A"))};
    PrivateKey pk2{Hash::sha256(std::string("B"))};

    Memo *memo = new Memo(pk1, pk2.getPublicKey(TWPublicKeyTypeSECP256k1), "Hello, world!", 1);
    TransferOperation *op = nullptr;
    ASSERT_NO_THROW(op = new TransferOperation(12, 16, Asset(511, 0), Asset(2, 0), memo));

    Data buf;
    op->serialize(buf);
    ASSERT_EQ(
        hex(buf),
        "000200000000000000000c10ff0100000000000000010211c34c5d6472f63143085e0d0d78e1691b51d8244f759bc2ef2a50adb49404a902b08d858f075fb92b580a011d580ee2098a397177a46ab8bc67ed049b57c166380100000000000000203ccc8d3cfc1d68b4a2b18a4f51c3979c9ecad1cdc301f7b4a492c7a997e69fc800"
    );

    ASSERT_EQ(
        op->serialize().dump(),
        "[0,{\"amount\":{\"amount\":511,\"asset_id\":\"1.3.0\"},\"extensions\":[],\"fee\":{\"amount\":2,\"asset_id\":\"1.3.0\"},\"from\":\"1.2.12\",\"memo\":{\"from\":\"BTS52K5kMmwiRyNQYAf7ymCMz6hieE8siyrqNt1t57ac9hvBrRdaa\",\"message\":\"3ccc8d3cfc1d68b4a2b18a4f51c3979c9ecad1cdc301f7b4a492c7a997e69fc8\",\"nonce\":1,\"to\":\"BTS6EFA9Ge5KQaCS2jGZVf7xHZ6hzcH7uvikf5oR7YnYKemkucxB4\"},\"to\":\"1.2.16\"}]"
    );

    // Test if TransferOperation works properly with Bravo Transactions as well
    auto transaction = Bravo::Transaction(parse_hex("0000086bf9e7704509aa41311a66fa0a1b479c6b"), 1552464180);
    transaction.addOperation(op);
    op = nullptr;
    Bravo::Signer(Data(32)).sign(pk1, transaction);

    buf.clear();
    transaction.serialize(buf);
    ASSERT_EQ(
        hex(buf),
        "6b08f9e770458cbb885c01000200000000000000000c10ff0100000000000000010211c34c5d6472f63143085e0d0d78e1691b51d8244f759bc2ef2a50adb49404a902b08d858f075fb92b580a011d580ee2098a397177a46ab8bc67ed049b57c166380100000000000000203ccc8d3cfc1d68b4a2b18a4f51c3979c9ecad1cdc301f7b4a492c7a997e69fc80000"
    );

    ASSERT_EQ(
        transaction.getSignatures().back().string(),
        "1f7653bbcebb0be719e2fc29c77147570645eaa544eb82c9c36e48938b5feb5bea6e7e7758aa3e13ea1f7ff51692f30cbaa22e04fd77bbf01df9b92ce5faa688e6"
    );
}