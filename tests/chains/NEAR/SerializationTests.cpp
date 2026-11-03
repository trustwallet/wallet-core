// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "Base58.h"
#include "proto/NEAR.pb.h"
#include "NEAR/Serialization.h"

#include <TrustWalletCore/TWHRP.h>
#include <gtest/gtest.h>

namespace TW::NEAR {

TEST(NEARSerialization, SerializeTransferTransaction) {
    auto publicKey = Base58::decode("Anu7LYDfpLtkP7E16LT9imXF694BdQaa9ufVkQiwTQxC");

    auto input = Proto::SigningInput();
    input.set_signer_id("test.near");
    input.set_nonce(1);
    input.set_receiver_id("whatever.near");

    input.add_actions();
    auto& transfer = *input.mutable_actions(0)->mutable_transfer();
    Data deposit(16, 0);
    deposit[0] = 1;
    transfer.set_deposit(deposit.data(), deposit.size());

    auto blockHash = Base58::decode("244ZQ9cgj3CQ6bWBdytfrJMuMQ1jdXLFGnr4HhvtCTnM");
    input.set_block_hash(blockHash.data(), blockHash.size());

    auto privateKey = Base58::decode("3hoMW1HvnRLSFCLZnvPzWeoGwtdHzke34B2cTHM8rhcbG3TbuLKtShTv3DvyejnXKXKBiV7YPkLeqUHN1ghnqpFv");
    input.set_private_key(privateKey.data(), 32);

    auto serialized = transactionData(input).payload();
    auto serializedHex = hex(serialized);

    ASSERT_EQ(serializedHex, "09000000746573742e6e65617200917b3d268d4b58f7fec1b150bd68d69be3ee5d4cc39855e341538465bb77860d01000000000000000d00000077686174657665722e6e6561720fa473fd26901df296be6adc4cc4df34d040efa2435224b6986910e630c2fef6010000000301000000000000000000000000000000");
}

TEST(NEARSerialization, SerializeFunctionCallTransaction) {
    auto publicKey = Base58::decode("Anu7LYDfpLtkP7E16LT9imXF694BdQaa9ufVkQiwTQxC");

    auto input = Proto::SigningInput();
    input.set_signer_id("test.near");
    input.set_nonce(1);
    input.set_receiver_id("whatever.near");

    input.add_actions();
    auto& functionCall = *input.mutable_actions(0)->mutable_function_call();

    functionCall.set_method_name("qqq");
    functionCall.set_gas(1000);

    Data deposit(16, 0);
    deposit[0] = 1;
    functionCall.set_deposit(deposit.data(), deposit.size());

    Data args(3, 0);
    args[0] = 1;
    args[1] = 2;
    args[2] = 3;
    functionCall.set_args(args.data(), args.size());

    auto blockHash = Base58::decode("244ZQ9cgj3CQ6bWBdytfrJMuMQ1jdXLFGnr4HhvtCTnM");
    input.set_block_hash(blockHash.data(), blockHash.size());

    auto privateKey = Base58::decode("3hoMW1HvnRLSFCLZnvPzWeoGwtdHzke34B2cTHM8rhcbG3TbuLKtShTv3DvyejnXKXKBiV7YPkLeqUHN1ghnqpFv");
    input.set_private_key(privateKey.data(), 32);

    auto serialized = transactionData(input).payload();
    auto serializedHex = hex(serialized);

    ASSERT_EQ(serializedHex, "09000000746573742e6e65617200917b3d268d4b58f7fec1b150bd68d69be3ee5d4cc39855e341538465bb77860d01000000000000000d00000077686174657665722e6e6561720fa473fd26901df296be6adc4cc4df34d040efa2435224b6986910e630c2fef601000000020300000071717103000000010203e80300000000000001000000000000000000000000000000");
}

TEST(NEARSerialization, SerializeStakeTransaction) {
    auto publicKey = Base58::decode("Anu7LYDfpLtkP7E16LT9imXF694BdQaa9ufVkQiwTQxC");

    auto input = Proto::SigningInput();
    input.set_signer_id("test.near");
    input.set_nonce(1);
    input.set_receiver_id("whatever.near");

    input.add_actions();
    auto& stake = *input.mutable_actions(0)->mutable_stake();
    Data amount(16, 0);
    amount[0] = 1;
    stake.set_stake(amount.data(), amount.size());

    auto& pKey = *stake.mutable_public_key();
    pKey.set_data(publicKey.data(), publicKey.size());
    pKey.set_key_type(0);

    auto blockHash = Base58::decode("244ZQ9cgj3CQ6bWBdytfrJMuMQ1jdXLFGnr4HhvtCTnM");
    input.set_block_hash(blockHash.data(), blockHash.size());

    auto privateKey = Base58::decode("3hoMW1HvnRLSFCLZnvPzWeoGwtdHzke34B2cTHM8rhcbG3TbuLKtShTv3DvyejnXKXKBiV7YPkLeqUHN1ghnqpFv");
    input.set_private_key(privateKey.data(), 32);

    auto serialized = transactionData(input).payload();
    auto serializedHex = hex(serialized);

    ASSERT_EQ(serializedHex, "09000000746573742e6e65617200917b3d268d4b58f7fec1b150bd68d69be3ee5d4cc39855e341538465bb77860d01000000000000000d00000077686174657665722e6e6561720fa473fd26901df296be6adc4cc4df34d040efa2435224b6986910e630c2fef601000000040100000000000000000000000000000000917b3d268d4b58f7fec1b150bd68d69be3ee5d4cc39855e341538465bb77860d");
}

TEST(NEARSerialization, SerializeStakeTransaction2) {
    auto publicKey = Base58::decode("C2P7YcEmBv31vtCHLBcESteN4Yi4vSCkXEXMTANyB649");

    auto input = Proto::SigningInput();
    input.set_signer_id("vdx.testnet");
    input.set_nonce(93128451000005);
    input.set_receiver_id("vdx.testnet");

    input.add_actions();
    auto& stake = *input.mutable_actions(0)->mutable_stake();
    // 2490000000000000000000000000
    auto amount = parse_hex("000000fa4f3f757902ae0b0800000000"); // little endian
    stake.set_stake(amount.data(), amount.size());

    auto& pKey = *stake.mutable_public_key();
    pKey.set_data(publicKey.data(), publicKey.size());
    pKey.set_key_type(0);

    auto blockHash = Base58::decode("ByDnm7c25npQXwNUX5yivbYbpjFcNuNumF6BJjaK3vhJ");
    input.set_block_hash(blockHash.data(), blockHash.size());

    auto privateKey = Base58::decode("5Cfk7QBnmDxxFxQk75FFq4ADrQS9gxHKe6vtuGH6JCCm8WV8aRPEGVqp579JHNmmHMUt49gkCVcH2t7NRnh2v7Qu");
    input.set_private_key(privateKey.data(), 32);

    auto serialized = transactionData(input).payload();
    auto serializedHex = hex(serialized);

    ASSERT_EQ(serializedHex, "0b0000007664782e746573746e657400a3cb23dbb9810abd4a6804328eec47a17236383b5c234cae903b064e9dc426dac5863d28b35400000b0000007664782e746573746e6574a2fbdae8a769c636d109952e4fe760b03688e629933cbf693aedfd97a470c7a50100000004000000fa4f3f757902ae0b080000000000a3cb23dbb9810abd4a6804328eec47a17236383b5c234cae903b064e9dc426da");
}

TEST(NEARSerialization, SerializeAddKeyFunctionCallTransaction) {
    auto publicKey = Base58::decode("Anu7LYDfpLtkP7E16LT9imXF694BdQaa9ufVkQiwTQxC");

    auto input = Proto::SigningInput();
    input.set_signer_id("test.near");
    input.set_nonce(1);
    input.set_receiver_id("whatever.near");

    input.add_actions();
    auto& addKey = *input.mutable_actions(0)->mutable_add_key();

    auto& pKey = *addKey.mutable_public_key();
    pKey.set_data(publicKey.data(), publicKey.size());
    pKey.set_key_type(0);

    auto& accessKey = *addKey.mutable_access_key();
    accessKey.set_nonce(0);
    auto& functionCallPermission = *accessKey.mutable_function_call();
    functionCallPermission.set_receiver_id("zzz");
    functionCallPermission.add_method_names("www");

    auto blockHash = Base58::decode("244ZQ9cgj3CQ6bWBdytfrJMuMQ1jdXLFGnr4HhvtCTnM");
    input.set_block_hash(blockHash.data(), blockHash.size());

    auto privateKey = Base58::decode("3hoMW1HvnRLSFCLZnvPzWeoGwtdHzke34B2cTHM8rhcbG3TbuLKtShTv3DvyejnXKXKBiV7YPkLeqUHN1ghnqpFv");
    input.set_private_key(privateKey.data(), 32);

    auto serialized = transactionData(input).payload();
    auto serializedHex = hex(serialized);

    ASSERT_EQ(serializedHex, "09000000746573742e6e65617200917b3d268d4b58f7fec1b150bd68d69be3ee5d4cc39855e341538465bb77860d01000000000000000d00000077686174657665722e6e6561720fa473fd26901df296be6adc4cc4df34d040efa2435224b6986910e630c2fef6010000000500917b3d268d4b58f7fec1b150bd68d69be3ee5d4cc39855e341538465bb77860d00000000000000000000030000007a7a7a0100000003000000777777");
}

TEST(NEARSerialization, SerializeAddKeyFullAccessTransaction) {
    auto publicKey = Base58::decode("Anu7LYDfpLtkP7E16LT9imXF694BdQaa9ufVkQiwTQxC");

    auto input = Proto::SigningInput();
    input.set_signer_id("test.near");
    input.set_nonce(1);
    input.set_receiver_id("whatever.near");

    input.add_actions();
    auto& addKey = *input.mutable_actions(0)->mutable_add_key();

    auto& pKey = *addKey.mutable_public_key();
    pKey.set_data(publicKey.data(), publicKey.size());
    pKey.set_key_type(0);

    auto& accessKey = *addKey.mutable_access_key();
    accessKey.set_nonce(0);

    accessKey.mutable_full_access();

    auto blockHash = Base58::decode("244ZQ9cgj3CQ6bWBdytfrJMuMQ1jdXLFGnr4HhvtCTnM");
    input.set_block_hash(blockHash.data(), blockHash.size());

    auto privateKey = Base58::decode("3hoMW1HvnRLSFCLZnvPzWeoGwtdHzke34B2cTHM8rhcbG3TbuLKtShTv3DvyejnXKXKBiV7YPkLeqUHN1ghnqpFv");
    input.set_private_key(privateKey.data(), 32);

    auto serialized = transactionData(input).payload();
    auto serializedHex = hex(serialized);

    ASSERT_EQ(serializedHex, "09000000746573742e6e65617200917b3d268d4b58f7fec1b150bd68d69be3ee5d4cc39855e341538465bb77860d01000000000000000d00000077686174657665722e6e6561720fa473fd26901df296be6adc4cc4df34d040efa2435224b6986910e630c2fef6010000000500917b3d268d4b58f7fec1b150bd68d69be3ee5d4cc39855e341538465bb77860d000000000000000001");
}

TEST(NEARSerialization, SerializeDeleteKeyTransaction) {
    auto publicKey = Base58::decode("Anu7LYDfpLtkP7E16LT9imXF694BdQaa9ufVkQiwTQxC");

    auto input = Proto::SigningInput();
    input.set_signer_id("test.near");
    input.set_nonce(1);
    input.set_receiver_id("whatever.near");

    input.add_actions();
    auto& deleteKey = *input.mutable_actions(0)->mutable_delete_key();

    auto& pKey = *deleteKey.mutable_public_key();
    pKey.set_data(publicKey.data(), publicKey.size());
    pKey.set_key_type(0);

    auto blockHash = Base58::decode("244ZQ9cgj3CQ6bWBdytfrJMuMQ1jdXLFGnr4HhvtCTnM");
    input.set_block_hash(blockHash.data(), blockHash.size());

    auto privateKey = Base58::decode("3hoMW1HvnRLSFCLZnvPzWeoGwtdHzke34B2cTHM8rhcbG3TbuLKtShTv3DvyejnXKXKBiV7YPkLeqUHN1ghnqpFv");
    input.set_private_key(privateKey.data(), 32);

    auto serialized = transactionData(input).payload();
    auto serializedHex = hex(serialized);

    ASSERT_EQ(serializedHex, "09000000746573742e6e65617200917b3d268d4b58f7fec1b150bd68d69be3ee5d4cc39855e341538465bb77860d01000000000000000d00000077686174657665722e6e6561720fa473fd26901df296be6adc4cc4df34d040efa2435224b6986910e630c2fef6010000000600917b3d268d4b58f7fec1b150bd68d69be3ee5d4cc39855e341538465bb77860d");
}

TEST(NEARSerialization, SerializeCreateAccountTransaction) {
    auto publicKey = Base58::decode("Anu7LYDfpLtkP7E16LT9imXF694BdQaa9ufVkQiwTQxC");

    auto input = Proto::SigningInput();
    input.set_signer_id("test.near");
    input.set_nonce(1);
    input.set_receiver_id("whatever.near");

    input.add_actions();
    input.mutable_actions(0)->mutable_create_account();

    auto blockHash = Base58::decode("244ZQ9cgj3CQ6bWBdytfrJMuMQ1jdXLFGnr4HhvtCTnM");
    input.set_block_hash(blockHash.data(), blockHash.size());

    auto privateKey = Base58::decode("3hoMW1HvnRLSFCLZnvPzWeoGwtdHzke34B2cTHM8rhcbG3TbuLKtShTv3DvyejnXKXKBiV7YPkLeqUHN1ghnqpFv");
    input.set_private_key(privateKey.data(), 32);

    auto serialized = transactionData(input).payload();
    auto serializedHex = hex(serialized);

    ASSERT_EQ(serializedHex, "09000000746573742e6e65617200917b3d268d4b58f7fec1b150bd68d69be3ee5d4cc39855e341538465bb77860d01000000000000000d00000077686174657665722e6e6561720fa473fd26901df296be6adc4cc4df34d040efa2435224b6986910e630c2fef60100000000");
}

TEST(NEARSerialization, SerializeDeployContractTransaction) {
    auto input = Proto::SigningInput();
    input.set_signer_id("test.near");
    input.set_nonce(1);
    input.set_receiver_id("whatever.near");

    auto code = parse_hex("010203");
    input.add_actions()->mutable_deploy_contract()->set_code(code.data(), code.size());

    auto blockHash = Base58::decode("244ZQ9cgj3CQ6bWBdytfrJMuMQ1jdXLFGnr4HhvtCTnM");
    input.set_block_hash(blockHash.data(), blockHash.size());

    auto privateKey = Base58::decode("3hoMW1HvnRLSFCLZnvPzWeoGwtdHzke34B2cTHM8rhcbG3TbuLKtShTv3DvyejnXKXKBiV7YPkLeqUHN1ghnqpFv");
    input.set_private_key(privateKey.data(), 32);

    auto serialized = transactionData(input).payload();
    auto serializedHex = hex(serialized);

    // Action tag 1, then `code` as a Borsh `Vec<u8>`: u32 length (3) followed by the bytes.
    ASSERT_EQ(serializedHex, "09000000746573742e6e65617200917b3d268d4b58f7fec1b150bd68d69be3ee5d4cc39855e341538465bb77860d01000000000000000d00000077686174657665722e6e6561720fa473fd26901df296be6adc4cc4df34d040efa2435224b6986910e630c2fef6010000000103000000010203");
}

TEST(NEARSerialization, EmptyActionRejected) {
    auto input = Proto::SigningInput();
    input.set_signer_id("test.near");
    input.set_nonce(1);
    input.set_receiver_id("whatever.near");
    input.add_actions();

    auto blockHash = Base58::decode("244ZQ9cgj3CQ6bWBdytfrJMuMQ1jdXLFGnr4HhvtCTnM");
    input.set_block_hash(blockHash.data(), blockHash.size());

    auto privateKey = Base58::decode("3hoMW1HvnRLSFCLZnvPzWeoGwtdHzke34B2cTHM8rhcbG3TbuLKtShTv3DvyejnXKXKBiV7YPkLeqUHN1ghnqpFv");
    input.set_private_key(privateKey.data(), 32);
    input.set_public_key(Base58::decode("Anu7LYDfpLtkP7E16LT9imXF694BdQaa9ufVkQiwTQxC").data(), 32);

    for (const auto& result : {transactionData(input), transactionDataWithPublicKey(input)}) {
        ASSERT_FALSE(result);
        EXPECT_EQ(result.error(), Common::Proto::Error_invalid_params);
    }
}

TEST(NEARSerialization, SerializeDeleteAccountTransaction) {
    auto publicKey = Base58::decode("Anu7LYDfpLtkP7E16LT9imXF694BdQaa9ufVkQiwTQxC");

    auto input = Proto::SigningInput();
    input.set_signer_id("test.near");
    input.set_nonce(1);
    input.set_receiver_id("whatever.near");

    input.add_actions();
    auto& deleteAccount = *input.mutable_actions(0)->mutable_delete_account();
    deleteAccount.set_beneficiary_id("123");

    auto blockHash = Base58::decode("244ZQ9cgj3CQ6bWBdytfrJMuMQ1jdXLFGnr4HhvtCTnM");
    input.set_block_hash(blockHash.data(), blockHash.size());

    auto privateKey = Base58::decode("3hoMW1HvnRLSFCLZnvPzWeoGwtdHzke34B2cTHM8rhcbG3TbuLKtShTv3DvyejnXKXKBiV7YPkLeqUHN1ghnqpFv");
    input.set_private_key(privateKey.data(), 32);

    auto serialized = transactionData(input).payload();
    auto serializedHex = hex(serialized);

    ASSERT_EQ(serializedHex, "09000000746573742e6e65617200917b3d268d4b58f7fec1b150bd68d69be3ee5d4cc39855e341538465bb77860d01000000000000000d00000077686174657665722e6e6561720fa473fd26901df296be6adc4cc4df34d040efa2435224b6986910e630c2fef6010000000703000000313233");
}

namespace {

Proto::SigningInput inputWithActionKey(uint32_t keyType, const Data& keyData, Proto::Action::PayloadCase payload) {
    auto input = Proto::SigningInput();
    input.set_signer_id("test.near");
    input.set_nonce(1);
    input.set_receiver_id("whatever.near");
    auto blockHash = Base58::decode("244ZQ9cgj3CQ6bWBdytfrJMuMQ1jdXLFGnr4HhvtCTnM");
    input.set_block_hash(blockHash.data(), blockHash.size());
    auto privateKey = Base58::decode("3hoMW1HvnRLSFCLZnvPzWeoGwtdHzke34B2cTHM8rhcbG3TbuLKtShTv3DvyejnXKXKBiV7YPkLeqUHN1ghnqpFv");
    input.set_private_key(privateKey.data(), 32);
    input.set_public_key(Base58::decode("Anu7LYDfpLtkP7E16LT9imXF694BdQaa9ufVkQiwTQxC").data(), 32);

    auto& action = *input.add_actions();
    Proto::PublicKey* pKey = nullptr;
    switch (payload) {
    case Proto::Action::kStake: {
        auto& stake = *action.mutable_stake();
        auto amount = Data(16, 0);
        stake.set_stake(amount.data(), amount.size());
        pKey = stake.mutable_public_key();
        break;
    }
    case Proto::Action::kAddKey: {
        auto& addKey = *action.mutable_add_key();
        addKey.mutable_access_key()->mutable_full_access();
        pKey = addKey.mutable_public_key();
        break;
    }
    default:
        pKey = action.mutable_delete_key()->mutable_public_key();
        break;
    }
    pKey->set_key_type(keyType);
    pKey->set_data(keyData.data(), keyData.size());
    return input;
}

} // namespace

TEST(NEARSerialization, ActionPublicKeyLengthValidation) {
    const auto ed25519Key = Base58::decode("Anu7LYDfpLtkP7E16LT9imXF694BdQaa9ufVkQiwTQxC");
    auto overlong = ed25519Key;
    append(overlong, Data(84, 0x41));
    const auto shortKey = Data(ed25519Key.begin(), ed25519Key.end() - 1);
    const auto secp256k1Key = Data(64, 0x02);

    for (const auto payload : {Proto::Action::kStake, Proto::Action::kAddKey, Proto::Action::kDeleteKey}) {
        // Well-formed keys of both types are accepted.
        EXPECT_TRUE(transactionData(inputWithActionKey(0, ed25519Key, payload)));
        EXPECT_TRUE(transactionDataWithPublicKey(inputWithActionKey(0, ed25519Key, payload)));
        EXPECT_TRUE(transactionData(inputWithActionKey(1, secp256k1Key, payload)));

        const std::vector<std::pair<uint32_t, Data>> invalid = {
            {0, overlong},
            {0, shortKey},
            {0, Data()},
            {0, secp256k1Key},
            {1, ed25519Key},
            // Would be truncated to ED25519 (0) by the u8 cast.
            {256, ed25519Key},
            {2, ed25519Key},
        };
        for (const auto& [keyType, keyData] : invalid) {
            const auto input = inputWithActionKey(keyType, keyData, payload);
            for (const auto& result : {transactionData(input), transactionDataWithPublicKey(input)}) {
                ASSERT_FALSE(result);
                EXPECT_EQ(result.error(), Common::Proto::Error_invalid_params);
            }
        }
    }
}

}
