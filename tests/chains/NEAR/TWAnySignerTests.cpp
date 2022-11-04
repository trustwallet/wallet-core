// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "proto/NEAR.pb.h"
#include "TestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>
#include "Base58.h"
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

    // 3hoMW1HvnRLSFCLZnvPzWeoGwtdHzke34B2cTHM8rhcbG3TbuLKtShTv3DvyejnXKXKBiV7YPkLeqUHN1ghnqpFv
    // 3BPZ9Qu7CviWD4CeKy3DYbNc4suyuBJYnjhVT2oTRCrfb4CQPiTK5tFVdg8Z3ijozxWoxxt9Y1kwkwPntrcc3dom
    auto privateKey = parse_hex("35e0d9631bd538d5569266abf6be7a9a403ebfda92ddd49b3268e35360a6c2dd");// Base58::bitcoin.decode("3BPZ9Qu7CviWD4CeKy3DYbNc4suyuBJYnjhVT2oTRCrfb4CQPiTK5tFVdg8Z3ijozxWoxxt9Y1kwkwPntrcc3dom");
    ASSERT_EQ(privateKey, Base58::bitcoin.decode("3BPZ9Qu7CviWD4CeKy3DYbNc4suyuBJYnjhVT2oTRCrfb4CQPiTK5tFVdg8Z3ijozxWoxxt9Y1kwkwPntrcc3dom"));
    auto publicKey = parse_hex("b8d5df25047841365008f30fb6b30dd820e9a84d869f05623d114e96831f2fbf"); // Base58::bitcoin.decode("Es9VoyZmEW6pjhoUf6BrUwEKykBQN3D3F2woke5KmuKd");
    auto blockHash = Base58::bitcoin.decode("8f5gQd138LCkTstXGvW2dQ6TfYd41BPZXNqcB5zsmpLR");

    // 1
    auto amount = parse_hex("d3c21bcecceda1000000"); // little endian

    Proto::SigningInput input;
    input.set_signer_id("b8d5df25047841365008f30fb6b30dd820e9a84d869f05623d114e96831f2fbf");
    input.set_nonce(77701544000001);
     std::cout << hex(privateKey) << std::endl;
    input.set_receiver_id("cryptogarik.poolv1.near");
    input.set_private_key(privateKey.data(), privateKey.size());
    input.set_block_hash(blockHash.data(), blockHash.size());

    auto& action = *input.add_actions();
    auto& stake = *action.mutable_stake();
    stake.set_stake(amount.data(), amount.size());

    auto& pubkey = *stake.mutable_public_key();
    pubkey.set_data(publicKey.data(), publicKey.size());
    //pubkey.set_key_type(0);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeNEAR);

    //ASSERT_EQ(Base64::encode(parse_hex("4bb25117a55c29dc03c4090ae6ad363db2595edd32787cca1bd9656b0e50c621")), "");
    //ASSERT_EQ(hex(output.signed_transaction()), "400000006238643564663235303437383431333635303038663330666236623330646438323065396138346438363966303536323364313134653936383331663266626600b8d5df25047841365008f30fb6b30dd820e9a84d869f05623d114e96831f2fbf01baed4cab4600001700000063727970746f676172696b2e706f6f6c76312e6e656172f1fe6041dd77f0b0a44ecb571af5b6750e937d8778d413d95cda9c079cec9a92d10100000004000000004080d09f3c2e3b0300b8d5df25047841365008f30fb6b30dd820e9a84d869f05623d114e96831f2fbf00357e5b8d1910cac25ccb9a063443f6545d23f54e5f287d8e53be41fc7b43e8743a04c3167271fa14d232fe22770bfd2329ccf4be6cf26433767940676ee0af0d");
    // 5yek5WDcpxPpveEF114u8E3YMv14WpQ3ULS2AFdpzMdE
    // 95KRPyc9SQzxTExW5w2cmwBB1qHo6k1MqCaxAqLyvCDE
    std::cout << Base58::bitcoin.encode(data(output.hash())) << std::endl;
    std::cout << hex(output.hash()) << std::endl;
    //ASSERT_EQ(hex(output.hash()), "4bb25117a55c29dc03c4090ae6ad363db2595edd32787cca1bd9656b0e50c621");

    //ASSERT_EQ(Base64::encode(data(output.signed_transaction())), "QAAAAGI4ZDVkZjI1MDQ3ODQxMzY1MDA4ZjMwZmI2YjMwZGQ4MjBlOWE4NGQ4NjlmMDU2MjNkMTE0ZTk2ODMxZjJmYmYAuNXfJQR4QTZQCPMPtrMN2CDpqE2GnwViPRFOloMfL7/Fhj0os1QAABcAAABjcnlwdG9nYXJpay5wb29sdjEubmVhcuO2uRxh0WMllhnTygaKC3qLpnaSfGMPJBxVl63j22U38QEAAAAEFS0Cx+FK9gAAAAC41d8lBHhBNlAI8w+2sw3YIOmoTYafBWI9EU6Wgx8vvwCylmh0aTauy6kQ10AJXZ0FcBm8nIp740B4bbnMxSdMgbDJNZvSQKrQRXxevLYs6p+azMnq6w+8PKkuRxuWH+UE");
}

} // namespace TW::NEAR
