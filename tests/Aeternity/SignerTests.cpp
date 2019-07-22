// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Aeternity/Signer.h"
#include "Aeternity/Transaction.h"
#include "HexCoding.h"

#include <Aeternity/Address.h>
#include <gtest/gtest.h>
#include <uint256.h>

using namespace TW;
using namespace TW::Aeternity;

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
    EXPECT_EQ(result.signature(), "sg_2dlw1eTrh79Yri5+urBSwVMJ86dSvCVtWc/nxIJrhIehxLzbtEdddjNyGJFc700p09KSd01oVTrpoCfiFsFvB3kDla0=");
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
    EXPECT_EQ(result.signature(), "sg_NDoFoaav1X0upLDEvg3+HsGcSCExIY2mjDosk9GkbgBHum+fndnELc6Ms+fDTa/jeDBaMHLaxl6wq65ITSMsC74JayU=");
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
    EXPECT_EQ(result.signature(), "sg_xG3qIlWJiQl5vdSjX2Vzr2TKE/cf7GY2tPRF2ZhKxQcZudm9SFaDOPNOma/k7JYgGM1Min1FBaN4xn1xD3WfD3ZUCmM=");
    EXPECT_EQ(result.encoded(), "tx_+KYLAfhCuEA0OgWhpq/VfS6ksMS+Df4ewZxIITEhjaaMOiyT0aRuAEe6b5+d2cQtzoyz58NNr+N4MFowctrGXrCrrkhNIywLuF74XAwBoQHuk6T2b40WuBm7m+uf/M383BQS6H/uajJMKpmh4OZxSKEBHxOjsIvwAUAGYqaLadh194A87EwIZH9u1dhMeJe9UKMKhhIwnOVAAAAxi0hlbGxvIFdvcmxkjoDNvQ==");
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
    EXPECT_EQ(result.signature(), "sg_nULIHBPG6fvQgDXWrMZLeYMX4wUiNc7cj5dWjSW5vjMFLxOy7v6rwJB4o33vhLots66oWUWHOZm5UWvqCziCBG+4fA8=");
    EXPECT_EQ(result.encoded(), "tx_+KYLAfhCuEA0OgWhpq/VfS6ksMS+Df4ewZxIITEhjaaMOiyT0aRuAEe6b5+d2cQtzoyz58NNr+N4MFowctrGXrCrrkhNIywLuF74XAwBoQHuk6T2b40WuBm7m+uf/M383BQS6H/uajJMKpmh4OZxSKEBHxOjsIvwAUAGYqaLadh194A87EwIZH9u1dhMeJe9UKMKhhIwnOVAAAAxi0hlbGxvIFdvcmxkjoDNvQ==");
}


