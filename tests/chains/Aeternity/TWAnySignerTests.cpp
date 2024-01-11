// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "uint256.h"
#include "proto/Aeternity.pb.h"
#include <TrustWalletCore/TWAnySigner.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Aeternity::tests {

TEST(TWAnySignerAeternity, Sign) {
    auto privateKey = parse_hex("4646464646464646464646464646464646464646464646464646464646464646");

    Proto::SigningInput input;
    input.set_from_address("ak_2p5878zbFhxnrm7meL7TmqwtvBaqcBddyp5eGzZbovZ5FeVfcw");
    input.set_to_address("ak_Egp9yVdpxmvAfQ7vsXGvpnyfNq71msbdUpkMNYGTeTe8kPL3v");
    auto amount = store(uint256_t(10));
    input.set_amount(amount.data(), amount.size());
    auto fee = store(uint256_t(20000000000000));
    input.set_fee(fee.data(), fee.size());
    input.set_payload("Hello World");
    input.set_ttl(82757);
    input.set_nonce(49);
    input.set_private_key(privateKey.data(), privateKey.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeAeternity);

    ASSERT_EQ(output.encoded(), "tx_+KkLAfhCuEDZ2XDV5OuHv1iuLn66sFLBUwnzp1K8JW1Zz+fEgmuEh6HEvNu0R112M3IYkVzvTSnT0pJ3TWhVOumgJ+IWwW8HuGH4XwwBoQHuk6T2b40WuBm7m+uf/M383BQS6H/uajJMKpmh4OZxSKEBHxOjsIvwAUAGYqaLadh194A87EwIZH9u1dhMeJe9UKMKhhIwnOVAAIMBQ0Uxi0hlbGxvIFdvcmxkDZqNSg==");
}

} // namespace TW::Aeternity::tests
