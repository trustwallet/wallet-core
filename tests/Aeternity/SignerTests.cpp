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

using namespace TW;
using namespace TW::Aeternity;

TEST(AeternitySigner, Sign) {
    std::string sender_id = "ak_2p5878zbFhxnrm7meL7TmqwtvBaqcBddyp5eGzZbovZ5FeVfcw";
    std::string recipient_id = "ak_Egp9yVdpxmvAfQ7vsXGvpnyfNq71msbdUpkMNYGTeTe8kPL3v";
    uint64_t amount = 10;
    double fee = 2e13;
    std::string payload = "Hello World";
    uint64_t ttl = 82757;
    uint64_t nonce = 49;

    auto transaction = Transaction(sender_id, recipient_id, amount, fee, payload, ttl, nonce);
    auto privateKey = PrivateKey(parse_hex("4646464646464646464646464646464646464646464646464646464646464646"));

    auto result = Signer::sign(privateKey, transaction);
    EXPECT_EQ(result.signature(), "sg_2dlw1eTrh79Yri5+urBSwVMJ86dSvCVtWc/nxIJrhIehxLzbtEdddjNyGJFc700p09KSd01oVTrpoCfiFsFvB3kDla0=");
    EXPECT_EQ(result.encoded(), "tx_+KkLAfhCuEDZ2XDV5OuHv1iuLn66sFLBUwnzp1K8JW1Zz+fEgmuEh6HEvNu0R112M3IYkVzvTSnT0pJ3TWhVOumgJ+IWwW8HuGH4XwwBoQHuk6T2b40WuBm7m+uf/M383BQS6H/uajJMKpmh4OZxSKEBHxOjsIvwAUAGYqaLadh194A87EwIZH9u1dhMeJe9UKMKhhIwnOVAAIMBQ0Uxi0hlbGxvIFdvcmxkDZqNSg==");
}
