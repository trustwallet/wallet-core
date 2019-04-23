#include "Bravo/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Bravo;

TEST(BravoAddress, Invalid) {
    ASSERT_FALSE(Address::isValid("abc"));
    ASSERT_FALSE(Address::isValid("5LZBF18uucMKVyKMQ7r6qnXBWn9StM5AJYHs1kidPeyNexuBUm"));
    ASSERT_FALSE(Address::isValid("BRV5LZBF18uucMKVyKMQ7r6qnXBWn9StM5AJYHs1kidPeyNexuBU"));
    ASSERT_FALSE(Address::isValid("BRV5LZBF18uucMKVyKMQ7r6qnXBWn9StM5AJYHs1kidPeyNexuBUmm"));
    ASSERT_FALSE(Address::isValid("BRV115LZBF18uucMKVyKMQ7r6qnXBWn9StM5AJYHs1kidPeyNexuBUm"));
}

TEST(BravoAddress, Base58) {
    ASSERT_EQ(
        Address("BRV5LZBF18uucMKVyKMQ7r6qnXBWn9StM5AJYHs1kidPeyNexuBUm").string(),
        "BRV5LZBF18uucMKVyKMQ7r6qnXBWn9StM5AJYHs1kidPeyNexuBUm"
    );
    ASSERT_EQ(
        Address("BRV8Z6A8JeF4JMRaVAVcBxCE5NhtXz1WVHr6u1ckUnqqyc7HiTF4n").string(),
        "BRV8Z6A8JeF4JMRaVAVcBxCE5NhtXz1WVHr6u1ckUnqqyc7HiTF4n"
    );
    ASSERT_EQ(
        Address("BRV4yPnD1zXSqyK4LFHkt1d5m5LZq2oaMFGYjosVXURUfqaUAXTKQ").string(),
        "BRV4yPnD1zXSqyK4LFHkt1d5m5LZq2oaMFGYjosVXURUfqaUAXTKQ"
    );
    ASSERT_EQ(
        Address("BRV67eD8Rx4us2bjjxyc1ciWzP1MQJd8CvQDmiiqRnu5659LXMJiv").string(),
        "BRV67eD8Rx4us2bjjxyc1ciWzP1MQJd8CvQDmiiqRnu5659LXMJiv"
    );
}

TEST(BravoAddress, FromPrivateKey) {
    std::string privArray[3] {  "e3d8e3800d179fae45807f1b983694cec1ae66ef05fea0646220fc6452fe31c1",
                                "c41d416a87a7058940ab86e38d10fc0b3accffda976600706aa7585eb749ba03",
                                "de0dc4ec3cbc3ed94c992f195e0d992e4b3d092817360784227a8beb47292472" };

    std::string pubArray[3] {   "BRV86GA7pTawoLFbQdb8Rhgfbf3PhRthkpfgzsE479Wg63jFV56Ej",
                                "BRV7qtSUjB9vtm5NvSKNDjxiHCgxEKyFzFgKmwpnNYKTgCnkN2hT9",
                                "BRV7BFpuiYQRbi6JeZdBpgKovvqNo5WZfNhzGRyWGNBErBuGkM5LV" };

    for (int i = 0; i < 3; i++) { 
        const auto privateKey = PrivateKey(parse_hex(privArray[i]));
        const auto publicKey = PublicKey(privateKey.getPublicKey(PublicKeyType::secp256k1));
        const auto address = Address(publicKey);
   
        ASSERT_EQ(address.string(), pubArray[i]);
    }
}

TEST(BravoAddress, IsValid) {
    ASSERT_TRUE(Address::isValid("BRV5LZBF18uucMKVyKMQ7r6qnXBWn9StM5AJYHs1kidPeyNexuBUm"));
    ASSERT_TRUE(Address::isValid("BRV8Z6A8JeF4JMRaVAVcBxCE5NhtXz1WVHr6u1ckUnqqyc7HiTF4n"));
    ASSERT_TRUE(Address::isValid("BRV4yPnD1zXSqyK4LFHkt1d5m5LZq2oaMFGYjosVXURUfqaUAXTKQ"));
    ASSERT_TRUE(Address::isValid("BRV67eD8Rx4us2bjjxyc1ciWzP1MQJd8CvQDmiiqRnu5659LXMJiv"));
}