// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Aeternity/Signer.h"
#include "Aeternity/Transaction.h"
#include "HexCoding.h"

#include "Aeternity/Address.h"
#include <gtest/gtest.h>
#include "uint256.h"

namespace TW::Aeternity::tests {

TEST(AeternitySigner, Sign) {
    std::string sender_id = "ak_2p5878zbFhxnrm7meL7TmqwtvBaqcBddyp5eGzZbovZ5FeVfcw";
    std::string recipient_id = "ak_Egp9yVdpxmvAfQ7vsXGvpnyfNq71msbdUpkMNYGTeTe8kPL3v";
    uint256_t amount = 10;
    uint256_t fee = 20000000000000;
    std::string payload = "Hello World";
    uint64_t ttl = 82757;
    uint64_t nonce = 49;

    auto transaction = Transaction(sender_id, recipient_id, amount, fee, payload, ttl, nonce);
    auto privateKey = PrivateKey(parse_hex("4646464646464646464646464646464646464646464646464646464646464646"));

    auto result = Signer::sign(privateKey, transaction);
    EXPECT_EQ(result.signature(), "sg_VW42qDPP3MMNFAStYaumjZz7mC7BZYpbNa15E57ejqUe7JdQFWCiX65eLNUpGMpt8tSpfgCfkYzcaFppqx7W75CrcWdC8");
    EXPECT_EQ(result.encoded(), "tx_+KkLAfhCuEDZ2XDV5OuHv1iuLn66sFLBUwnzp1K8JW1Zz+fEgmuEh6HEvNu0R112M3IYkVzvTSnT0pJ3TWhVOumgJ+IWwW8HuGH4XwwBoQHuk6T2b40WuBm7m+uf/M383BQS6H/uajJMKpmh4OZxSKEBHxOjsIvwAUAGYqaLadh194A87EwIZH9u1dhMeJe9UKMKhhIwnOVAAIMBQ0Uxi0hlbGxvIFdvcmxkDZqNSg==");
}

TEST(AeternitySigner, SignTxWithZeroTtl) {
    std::string sender_id = "ak_2p5878zbFhxnrm7meL7TmqwtvBaqcBddyp5eGzZbovZ5FeVfcw";
    std::string recipient_id = "ak_Egp9yVdpxmvAfQ7vsXGvpnyfNq71msbdUpkMNYGTeTe8kPL3v";
    uint256_t amount = 10;
    uint256_t fee = 20000000000000;
    std::string payload = "Hello World";
    uint64_t ttl = 0;
    uint64_t nonce = 49;

    auto transaction = Transaction(sender_id, recipient_id, amount, fee, payload, ttl, nonce);
    auto privateKey = PrivateKey(parse_hex("4646464646464646464646464646464646464646464646464646464646464646"));

    auto result = Signer::sign(privateKey, transaction);
    EXPECT_EQ(result.signature(), "sg_7qJK868bqEZ5ciC2P3WCKYfhayvKTHvPsz3bdPgpfF3Ky7yNg9f8k22A3gxjjSm9afa6JmP8TJpF4GJkFh2k7gGaog9KS");
    EXPECT_EQ(result.encoded(), "tx_+KYLAfhCuEA0OgWhpq/VfS6ksMS+Df4ewZxIITEhjaaMOiyT0aRuAEe6b5+d2cQtzoyz58NNr+N4MFowctrGXrCrrkhNIywLuF74XAwBoQHuk6T2b40WuBm7m+uf/M383BQS6H/uajJMKpmh4OZxSKEBHxOjsIvwAUAGYqaLadh194A87EwIZH9u1dhMeJe9UKMKhhIwnOVAAAAxi0hlbGxvIFdvcmxkjoDNvQ==");
}

TEST(AeternitySigner, SignTxWithZeroAmount) {
    std::string sender_id = "ak_2p5878zbFhxnrm7meL7TmqwtvBaqcBddyp5eGzZbovZ5FeVfcw";
    std::string recipient_id = "ak_Egp9yVdpxmvAfQ7vsXGvpnyfNq71msbdUpkMNYGTeTe8kPL3v";
    uint256_t amount = 0;
    uint256_t fee = 20000000000000;
    std::string payload = "Zero amount test";
    uint64_t ttl = 113579;
    uint64_t nonce = 7;

    auto transaction = Transaction(sender_id, recipient_id, amount, fee, payload, ttl, nonce);
    auto privateKey = PrivateKey(parse_hex("4646464646464646464646464646464646464646464646464646464646464646"));

    auto result = Signer::sign(privateKey, transaction);
    EXPECT_EQ(result.signature(), "sg_ShWvujPnyKBT1Ng2X5k6XSchVK8Bq7LYEisPMH11DUoPkXZcooBzqw81j9j5JewoFFpT9xEhUptj1azcLA21ogURYh4Lz");
    EXPECT_EQ(result.encoded(), "tx_+K4LAfhCuEDEbeoiVYmJCXm91KNfZXOvZMoT9x/sZja09EXZmErFBxm52b1IVoM4806Zr+TsliAYzUyKfUUFo3jGfXEPdZ8PuGb4ZAwBoQHuk6T2b40WuBm7m+uf/M383BQS6H/uajJMKpmh4OZxSKEBHxOjsIvwAUAGYqaLadh194A87EwIZH9u1dhMeJe9UKMAhhIwnOVAAIMBu6sHkFplcm8gYW1vdW50IHRlc3S5L3Vn");
}

TEST(AeternitySigner, SignTxWithZeroNonce) {
    std::string sender_id = "ak_2p5878zbFhxnrm7meL7TmqwtvBaqcBddyp5eGzZbovZ5FeVfcw";
    std::string recipient_id = "ak_Egp9yVdpxmvAfQ7vsXGvpnyfNq71msbdUpkMNYGTeTe8kPL3v";
    uint256_t amount = 3369980000000000000;
    uint256_t fee = 20000000000000;
    std::string payload = "Zero nonce test";
    uint64_t ttl = 113579;
    uint64_t nonce = 0;

    auto transaction = Transaction(sender_id, recipient_id, amount, fee, payload, ttl, nonce);
    auto privateKey = PrivateKey(parse_hex("4646464646464646464646464646464646464646464646464646464646464646"));

    auto result = Signer::sign(privateKey, transaction);
    EXPECT_EQ(result.signature(), "sg_MaJc4ptSUhq5kH6mArszDAvu4f7PejyuhmgM6U8GEr8bRUTaSFbdFPx4C6FEYA5v5Lgwu9EToaWnHgR2xkqZ9JjHnaBpA");
    EXPECT_EQ(result.encoded(), "tx_+LULAfhCuECdQsgcE8bp+9CANdasxkt5gxfjBSI1ztyPl1aNJbm+MwUvE7Lu/qvAkHijfe+Eui2zrqhZRYc5mblRa+oLOIIEuG34awwBoQHuk6T2b40WuBm7m+uf/M383BQS6H/uajJMKpmh4OZxSKEBHxOjsIvwAUAGYqaLadh194A87EwIZH9u1dhMeJe9UKOILsSS9IArwACGEjCc5UAAgwG7qwCPWmVybyBub25jZSB0ZXN0piWfFA==");
}

} // namespace TW::Aeternity::tests
