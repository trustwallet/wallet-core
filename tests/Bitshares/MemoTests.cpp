#include <stdexcept>
#include <iostream>

#include "Bitshares/Memo.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Bitshares;

TEST(BitsharesMemo, Invalid) {
    PrivateKey pk1{Hash::sha256(std::string("A"))};
    PrivateKey pk2{Hash::sha256(std::string("B"))};

    ASSERT_THROW(Memo(pk1, pk2.getPublicKey(TWPublicKeyTypeSECP256k1Extended), "Hello, world!"), std::invalid_argument);
    ASSERT_THROW(Memo(pk1, pk2.getPublicKey(TWPublicKeyTypeSECP256k1), ""), std::invalid_argument);
}

TEST(BitsharesMemo, Serialization) {
    PrivateKey pk1{Hash::sha256(std::string("A"))};
    PrivateKey pk2{Hash::sha256(std::string("B"))};

    Memo *memo = nullptr;

    ASSERT_NO_THROW(memo = new Memo(pk1, pk2.getPublicKey(TWPublicKeyTypeSECP256k1), "Hello, world! How are you doing?", 1));

    Data buf;
    memo->serialize(buf);
    ASSERT_EQ(
        hex(buf),
        "0211c34c5d6472f63143085e0d0d78e1691b51d8244f759bc2ef2a50adb49404a902b08d858f075fb92b580a011d580ee2098a397177a46ab8bc67ed049b57c166380100000000000000305578960ffe4d94864bdbb6ab330814860924919b2b843077d0b1dba13f682c7474434c00262fccc5b079f963f6bc818f"
    );

    ASSERT_EQ(
        memo->serialize().dump(),
        "{\"from\":\"BTS52K5kMmwiRyNQYAf7ymCMz6hieE8siyrqNt1t57ac9hvBrRdaa\",\"message\":\"5578960ffe4d94864bdbb6ab330814860924919b2b843077d0b1dba13f682c7474434c00262fccc5b079f963f6bc818f\",\"nonce\":1,\"to\":\"BTS6EFA9Ge5KQaCS2jGZVf7xHZ6hzcH7uvikf5oR7YnYKemkucxB4\"}"
    );

    delete memo;
}