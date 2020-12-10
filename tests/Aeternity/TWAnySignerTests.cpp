// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "uint256.h"
#include "proto/Aeternity.pb.h"
#include <TrustWalletCore/TWAnySigner.h>

#include "../interface/TWTestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Aeternity;

TEST(TWAnySignerAeternity, SignTransfer) {
    auto privateKey = parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
    
    Proto::SigningInput input;
    input.set_from_address("ak_2p5878zbFhxnrm7meL7TmqwtvBaqcBddyp5eGzZbovZ5FeVfcw");
    input.set_to_address("ak_Egp9yVdpxmvAfQ7vsXGvpnyfNq71msbdUpkMNYGTeTe8kPL3v");
    auto amount = store(uint256_t(10));
    input.mutable_payload()->mutable_payload_transfer()->set_amount(amount.data(), amount.size());
    auto fee = store(uint256_t(20000000000000));
    input.set_fee(fee.data(), fee.size());
    input.set_ttl(82757);
    input.set_nonce(49);
    input.set_private_key(privateKey.data(), privateKey.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeAeternity);

    ASSERT_EQ(output.encoded(), "tx_+J4LAfhCuEC1SpnmpRoEbMF9BABAePYkH7yNuar+WN8CTrmw0xwhqhsj9a/DxLcEM+gvWsgkr/SEJkbV0aoxR/ecLCsiV1UPuFb4VAwBoQHuk6T2b40WuBm7m+uf/M383BQS6H/uajJMKpmh4OZxSKEBHxOjsIvwAUAGYqaLadh194A87EwIZH9u1dhMeJe9UKMKhhIwnOVAAIMBQ0UxgFv6iPQ=");
}

TEST(TWAnySignerAeternity, SignContractCall) {
    auto privateKey = parse_hex("4646464646464646464646464646464646464646464646464646464646464646");
    
    Proto::SigningInput input;
    input.set_from_address("ak_2p5878zbFhxnrm7meL7TmqwtvBaqcBddyp5eGzZbovZ5FeVfcw");
    input.set_to_address("ak_Egp9yVdpxmvAfQ7vsXGvpnyfNq71msbdUpkMNYGTeTe8kPL3v");
    auto amount = store(uint256_t(10));
    input.mutable_payload()->mutable_payload_contract_generic()->set_payload("Hello World");
    auto fee = store(uint256_t(20000000000000));
    input.set_fee(fee.data(), fee.size());
    input.set_ttl(82757);
    input.set_nonce(49);
    input.set_private_key(privateKey.data(), privateKey.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeAeternity);

    ASSERT_EQ(output.encoded(), "tx_+KkLAfhCuEBAxVYM3tXXtZNrpF5oTQ7JCjviPN9k0sLuE24QlLXW+xp5BUhYTBNcVE5AQPBHxEEqtzQh+K+7i1vzhTylwtQBuGH4XwwBoQHuk6T2b40WuBm7m+uf/M383BQS6H/uajJMKpmh4OZxSKEBHxOjsIvwAUAGYqaLadh194A87EwIZH9u1dhMeJe9UKMAhhIwnOVAAIMBQ0Uxi0hlbGxvIFdvcmxkooC1hg==");
}
