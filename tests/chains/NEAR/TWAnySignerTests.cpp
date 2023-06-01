// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "proto/NEAR.pb.h"
#include "TestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>
#include "Base58.h"
#include "Base64.h"
#include <gtest/gtest.h>

namespace TW::NEAR {

TEST(TWAnySignerNEAR, SignTransfer) {

    auto privateKey = parse_hex("8737b99bf16fba78e1e753e23ba00c4b5423ac9c45d9b9caae9a519434786568");
    auto blockHash = parse_hex("0fa473fd26901df296be6adc4cc4df34d040efa2435224b6986910e630c2fef6");
    // uint128_t / little endian byte order
    auto deposit = parse_hex("01000000000000000000000000000000");

    Proto::SigningInput input;
    input.set_signer_id("test.near");
    input.set_nonce(1);
    input.set_receiver_id("whatever.near");
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_block_hash(blockHash.data(), blockHash.size());
    
    auto& action = *input.add_actions();
    auto& transfer = *action.mutable_transfer();
    transfer.set_deposit(deposit.data(), deposit.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeNEAR);

    ASSERT_EQ(hex(output.signed_transaction()), "09000000746573742e6e65617200917b3d268d4b58f7fec1b150bd68d69be3ee5d4cc39855e341538465bb77860d01000000000000000d00000077686174657665722e6e6561720fa473fd26901df296be6adc4cc4df34d040efa2435224b6986910e630c2fef601000000030100000000000000000000000000000000969a83332186ee9755e4839325525806e189a3d2d2bb4b4760e94443e97e1c4f22deeef0059a8e9713100eda6e19144da7e8a0ef7e539b20708ba1d8d021bd01");
    ASSERT_EQ(hex(output.hash()), "eea6e680f3ea51a7f667e9a801d0bfadf66e03d41ed54975b3c6006351461b32");
}

TEST(TWAnySignerNEAR, SignStake) {

    auto privateKey = parse_hex("d22149327ceb8e86f70962be0c7293f8308d85d0cbea2cc24e47c3033da7440f");
    auto publicKey = parse_hex("a3cb23dbb9810abd4a6804328eec47a17236383b5c234cae903b064e9dc426da");
    auto blockHash = parse_hex("a2fbdae8a769c636d109952e4fe760b03688e629933cbf693aedfd97a470c7a5");

    // 2490000000000000000000000000
    auto amount = parse_hex("000000fa4f3f757902ae0b0800000000"); // little endian

    Proto::SigningInput input;
    input.set_signer_id("vdx.testnet");
    input.set_nonce(93128451000005);
    input.set_receiver_id("vdx.testnet");
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_block_hash(blockHash.data(), blockHash.size());
    
    auto& action = *input.add_actions();
    auto& stake = *action.mutable_stake();
    stake.set_stake(amount.data(), amount.size());

    auto& pubkey = *stake.mutable_public_key();
    pubkey.set_data(publicKey.data(), publicKey.size());
    pubkey.set_key_type(0);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeNEAR);

    ASSERT_EQ(hex(output.signed_transaction()), "0b0000007664782e746573746e657400a3cb23dbb9810abd4a6804328eec47a17236383b5c234cae903b064e9dc426dac5863d28b35400000b0000007664782e746573746e6574a2fbdae8a769c636d109952e4fe760b03688e629933cbf693aedfd97a470c7a50100000004000000fa4f3f757902ae0b080000000000a3cb23dbb9810abd4a6804328eec47a17236383b5c234cae903b064e9dc426da0011fdbc234d4ce470ec7f2ac5e4d3d8f8fe1525f83e9a2425e7000aea52f7260ff4f5191beaa1a5ac29256e68c6acd368ada0d06ed033e9a204ee119f5ef1b104");
    ASSERT_EQ(hex(output.hash()), "c8aedbf75fcaa9b663a3959d27f1deae809e1923460791471e5219eafecc4ba8");
}

TEST(TWAnySignerNEAR, SignStakeMainnetReplication) {
    auto privateKey = Base58::decode("3BPZ9Qu7CviWD4CeKy3DYbNc4suyuBJYnjhVT2oTRCrfb4CQPiTK5tFVdg8Z3ijozxWoxxt9Y1kwkwPntrcc3dom");
    auto blockHash = parse_hex("e78680996127b7a0f3f2343502e442f24366cba5f79cb72f8bc6d0debb26ce24");
    
    // 0.1 with 24 decimal precision in big endian
    auto amount = parse_hex("000080f64ae1c7022d15000000000000");

    Proto::SigningInput input;
    input.set_signer_id("b8d5df25047841365008f30fb6b30dd820e9a84d869f05623d114e96831f2fbf");
    input.set_nonce(77701544000004);
    input.set_receiver_id("avado.poolv1.near");
    input.set_private_key(privateKey.data(), 32);
    input.set_block_hash(blockHash.data(), blockHash.size());

    auto& action = *input.add_actions();
    auto& call = *action.mutable_function_call();
    call.set_method_name("deposit_and_stake");
    call.set_args("{}");
    call.set_gas(125000000000000);
    call.set_deposit(amount.data(), amount.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeNEAR);

    // https://explorer.near.org/transactions/kd7ajFw1CfXB8LiJXvhz5NDS7QpQXkuQraAbhb5MMMq
    ASSERT_EQ(Base58::encode(data(output.hash())), "kd7ajFw1CfXB8LiJXvhz5NDS7QpQXkuQraAbhb5MMMq");
    ASSERT_EQ(Base64::encode(data(output.signed_transaction())), "QAAAAGI4ZDVkZjI1MDQ3ODQxMzY1MDA4ZjMwZmI2YjMwZGQ4MjBlOWE4NGQ4NjlmMDU2MjNkMTE0ZTk2ODMxZjJmYmYAzgCT6NK76nb1mB7pToefgkGUHfUe5BKvvr3gW/nq+MgEuu1Mq0YAABEAAABhdmFkby5wb29sdjEubmVhcueGgJlhJ7eg8/I0NQLkQvJDZsul95y3L4vG0N67Js4kAQAAAAIRAAAAZGVwb3NpdF9hbmRfc3Rha2UCAAAAe30A0JjUr3EAAAAAgPZK4ccCLRUAAAAAAAAALNrorr8qTL6u1nlxLpuPa45nFdYmjU96i7CmJP08mVHVzHUaw/bGN30Z3u3o1F2o2yefCBNqO9Ogn9fM25NGCg==");
}

TEST(TWAnySignerNEAR, SignUnstakeMainnetReplication) {
    auto privateKey = Base58::decode("3BPZ9Qu7CviWD4CeKy3DYbNc4suyuBJYnjhVT2oTRCrfb4CQPiTK5tFVdg8Z3ijozxWoxxt9Y1kwkwPntrcc3dom");
    auto blockHash = Base58::decode("CehJc9uZhqE2m17ZrkqcAog4mxSz6JSvYv1JEK1iBsX9");

    auto amount = parse_hex("00000000000000000000000000000000");

    Proto::SigningInput input;
    input.set_signer_id("b8d5df25047841365008f30fb6b30dd820e9a84d869f05623d114e96831f2fbf");
    input.set_nonce(77701544000006);
    input.set_receiver_id("avado.poolv1.near");
    input.set_private_key(privateKey.data(), 32);
    input.set_block_hash(blockHash.data(), blockHash.size());

    auto& action = *input.add_actions();
    auto& call = *action.mutable_function_call();
    call.set_method_name("unstake_all");
    call.set_args("{}");
    call.set_gas(125000000000000);
    call.set_deposit(amount.data(), amount.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeNEAR);

    // https://explorer.near.org/transactions/DH6QAX3TkY6XtkteorvKBoGT5hA5ADkURZdzrbbKRs8P
    ASSERT_EQ(Base58::encode(data(output.hash())), "DH6QAX3TkY6XtkteorvKBoGT5hA5ADkURZdzrbbKRs8P");
    ASSERT_EQ(Base64::encode(data(output.signed_transaction())), "QAAAAGI4ZDVkZjI1MDQ3ODQxMzY1MDA4ZjMwZmI2YjMwZGQ4MjBlOWE4NGQ4NjlmMDU2MjNkMTE0ZTk2ODMxZjJmYmYAzgCT6NK76nb1mB7pToefgkGUHfUe5BKvvr3gW/nq+MgGuu1Mq0YAABEAAABhdmFkby5wb29sdjEubmVhcq0YnhRlt+TTtagkoy0qKn56zAfGhE+jkTJW6PR5k5r8AQAAAAILAAAAdW5zdGFrZV9hbGwCAAAAe30A0JjUr3EAAAAAAAAAAAAAAAAAAAAAAAAABaFP0EkfJU3VQZ4QAiTwq9ebWDJ7jx7TxbA+VGH4hwKX3gWnmDHVve+LK7/UbbffjF/y8vn0KrPxdh3ONAG0Ag==");
}

/// Implements NEP-141:
/// https://nomicon.io/Standards/Tokens/FungibleToken/Core
///
/// Successfully broadcasted tx:
/// https://explorer.near.org/transactions/ABQY6nfLdNrRVynHYNjYkfUM6Up5pDHHpuhRJe6FCMRu
TEST(TWAnySignerNEAR, SignTokenTransfer) {
    auto privateKey = parse_hex("77006e227658c18da47546413926a26b839204b1b19e807c4a13d994d661c72e");

    auto blockHash = Base58::decode("2dQBYs8XjprLLgtH7eVsJ3e58A5QuRcbuqFisSk9fFWQ");

    // Deposit should be 1 yocto NEAR for security purposes.
    auto deposit = parse_hex("01000000000000000000000000000000");

    Proto::SigningInput input;
    input.set_signer_id("105396228ac2e0ef144b93bcc5322fca1167d524422bb73d17440d35c714a58f");
    input.set_nonce(93062928000003);
    input.set_receiver_id("token.paras.near");
    input.set_private_key(privateKey.data(), 32);
    input.set_block_hash(blockHash.data(), blockHash.size());

    auto& action = *input.add_actions();
    auto& tokenTransfer = *action.mutable_token_transfer();
    tokenTransfer.set_token_amount("100000000000000000");
    tokenTransfer.set_receiver_id("c6d5e3e8f328436f595856a598239b691d3d136b24c05a4614f9e9716edc14fe");
    tokenTransfer.set_gas(15000000000000);
    tokenTransfer.set_deposit(deposit.data(), deposit.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeNEAR);

    ASSERT_EQ(Base58::encode(data(output.hash())), "ABQY6nfLdNrRVynHYNjYkfUM6Up5pDHHpuhRJe6FCMRu");
    ASSERT_EQ(Base64::encode(data(output.signed_transaction())), "QAAAADEwNTM5NjIyOGFjMmUwZWYxNDRiOTNiY2M1MzIyZmNhMTE2N2Q1MjQ0MjJiYjczZDE3NDQwZDM1YzcxNGE1OGYAEFOWIorC4O8US5O8xTIvyhFn1SRCK7c9F0QNNccUpY8D5MPmo1QAABAAAAB0b2tlbi5wYXJhcy5uZWFyGC7O0jXN2b4SH1XfMtNISEnU8XATKOhZwxx0pLLZqTEBAAAAAgsAAABmdF90cmFuc2ZlcnAAAAB7ImFtb3VudCI6IjEwMDAwMDAwMDAwMDAwMDAwMCIsInJlY2VpdmVyX2lkIjoiYzZkNWUzZThmMzI4NDM2ZjU5NTg1NmE1OTgyMzliNjkxZDNkMTM2YjI0YzA1YTQ2MTRmOWU5NzE2ZWRjMTRmZSJ9APCrdaQNAAABAAAAAAAAAAAAAAAAAAAAANUjO7fmnTebSNW9EcHHwYwPNlQJcReGWJfJUuxWzPDAGEeo4JTcLB8pLCkqxKKsI0NE1Szv2+GAt5mCBum5mQY=");
}

} // namespace TW::NEAR
