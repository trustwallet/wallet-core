// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Bitcoin/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "proto/Tron.pb.h"
#include "Tron/Signer.h"

#include <gtest/gtest.h>

namespace TW::Tron {

TEST(TronSigner, SignTransferAsset) {
    auto input = Proto::SigningInput();
    auto& transaction = *input.mutable_transaction();

    auto& transfer = *transaction.mutable_transfer_asset();
    transfer.set_owner_address("TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC");
    transfer.set_to_address("THTR75o8xXAgCTQqpiot2AFRAjvW1tSbVV");
    transfer.set_amount(4);
    transfer.set_asset_name("1000959");

    transaction.set_timestamp(1539295479000);
    transaction.set_expiration(1541890116000 + 10 * 60 * 60 * 1000);

    auto& blockHeader = *transaction.mutable_block_header();
    blockHeader.set_timestamp(1541890116000);
    const auto txTrieRoot = parse_hex("845ab51bf63c2c21ee71a4dc0ac3781619f07a7cd05e1e0bd8ba828979332ffa");
    blockHeader.set_tx_trie_root(txTrieRoot.data(), txTrieRoot.size());
    const auto parentHash = parse_hex("00000000003cb800a7e69e9144e3d16f0cf33f33a95c7ce274097822c67243c1");
    blockHeader.set_parent_hash(parentHash.data(), parentHash.size());
    blockHeader.set_number(3979265);
    const auto witnessAddress = parse_hex("41b487cdc02de90f15ac89a68c82f44cbfe3d915ea");
    blockHeader.set_witness_address(witnessAddress.data(), witnessAddress.size());
    blockHeader.set_version(3);

    const auto privateKey = PrivateKey(parse_hex("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.id()), "546a3d07164c624809cf4e564a083a7a7974bb3c4eff6bb3e278b0ca21083fcb");
    ASSERT_EQ(hex(output.signature()), "77f5eabde31e739d34a66914540f1756981dc7d782c9656f5e14e53b59a15371603a183aa12124adeee7991bf55acc8e488a6ca04fb393b1a8ac16610eeafdfc00");
}

TEST(TronSigner, SignTransfer) {
    auto input = Proto::SigningInput();
    auto& transaction = *input.mutable_transaction();

    auto& transfer = *transaction.mutable_transfer();
    transfer.set_owner_address("TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC");
    transfer.set_to_address("THTR75o8xXAgCTQqpiot2AFRAjvW1tSbVV");
    transfer.set_amount(2000000);

    transaction.set_timestamp(1539295479000);
    transaction.set_expiration(1539295479000 + 10 * 60 * 60 * 1000);

    auto& blockHeader = *transaction.mutable_block_header();
    blockHeader.set_timestamp(1539295479000);
    const auto txTrieRoot = parse_hex("64288c2db0641316762a99dbb02ef7c90f968b60f9f2e410835980614332f86d");
    blockHeader.set_tx_trie_root(txTrieRoot.data(), txTrieRoot.size());
    const auto parentHash = parse_hex("00000000002f7b3af4f5f8b9e23a30c530f719f165b742e7358536b280eead2d");
    blockHeader.set_parent_hash(parentHash.data(), parentHash.size());
    blockHeader.set_number(3111739);
    const auto witnessAddress = parse_hex("415863f6091b8e71766da808b1dd3159790f61de7d");
    blockHeader.set_witness_address(witnessAddress.data(), witnessAddress.size());
    blockHeader.set_version(3);

    const auto privateKey = PrivateKey(parse_hex("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.id()), "dc6f6d9325ee44ab3c00528472be16e1572ab076aa161ccd12515029869d0451");
    ASSERT_EQ(hex(output.signature()), "ede769f6df28aefe6a846be169958c155e23e7e5c9621d2e8dce1719b4d952b63e8a8bf9f00e41204ac1bf69b1a663dacdf764367e48e4a5afcd6b055a747fb200");
}

TEST(TronSigner, SignFreezeBalance) {
    auto input = Proto::SigningInput();
    auto& transaction = *input.mutable_transaction();

    auto& freeze = *transaction.mutable_freeze_balance();
    freeze.set_owner_address("TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC");
    freeze.set_receiver_address("THTR75o8xXAgCTQqpiot2AFRAjvW1tSbVV");
    freeze.set_frozen_duration(1000000);
    freeze.set_frozen_duration(100);
    freeze.set_resource("ENERGY");

    transaction.set_timestamp(1539295479000);
    transaction.set_expiration(1539295479000 + 10 * 60 * 60 * 1000);

    auto& blockHeader = *transaction.mutable_block_header();
    blockHeader.set_timestamp(1539295479000);
    const auto txTrieRoot = parse_hex("64288c2db0641316762a99dbb02ef7c90f968b60f9f2e410835980614332f86d");
    blockHeader.set_tx_trie_root(txTrieRoot.data(), txTrieRoot.size());
    const auto parentHash = parse_hex("00000000002f7b3af4f5f8b9e23a30c530f719f165b742e7358536b280eead2d");
    blockHeader.set_parent_hash(parentHash.data(), parentHash.size());
    blockHeader.set_number(3111739);
    const auto witnessAddress = parse_hex("415863f6091b8e71766da808b1dd3159790f61de7d");
    blockHeader.set_witness_address(witnessAddress.data(), witnessAddress.size());
    blockHeader.set_version(3);

    const auto privateKey = PrivateKey(parse_hex("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.id()), "a66845c0f55ac36081b22f5391b97df65ab7e1c724e766d88017121a3e37022f");
    ASSERT_EQ(hex(output.signature()), "c9f16c7d0d486515d7aaad8424a374a3e6f9b19d0b84ae810088db6ade164e717f1e52f08c76118a975eadcacce4c068b2adc2255e6bde12746e75f70b75abae00");
}

TEST(TronSigner, SignUnFreezeBalance) {
    auto input = Proto::SigningInput();
    auto& transaction = *input.mutable_transaction();

    auto& unfreeze = *transaction.mutable_unfreeze_balance();
    unfreeze.set_owner_address("TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC");
    unfreeze.set_receiver_address("THTR75o8xXAgCTQqpiot2AFRAjvW1tSbVV");
    unfreeze.set_resource("ENERGY");

    transaction.set_timestamp(1539295479000);
    transaction.set_expiration(1539295479000 + 10 * 60 * 60 * 1000);

    auto& blockHeader = *transaction.mutable_block_header();
    blockHeader.set_timestamp(1539295479000);
    const auto txTrieRoot = parse_hex("64288c2db0641316762a99dbb02ef7c90f968b60f9f2e410835980614332f86d");
    blockHeader.set_tx_trie_root(txTrieRoot.data(), txTrieRoot.size());
    const auto parentHash = parse_hex("00000000002f7b3af4f5f8b9e23a30c530f719f165b742e7358536b280eead2d");
    blockHeader.set_parent_hash(parentHash.data(), parentHash.size());
    blockHeader.set_number(3111739);
    const auto witnessAddress = parse_hex("415863f6091b8e71766da808b1dd3159790f61de7d");
    blockHeader.set_witness_address(witnessAddress.data(), witnessAddress.size());
    blockHeader.set_version(3);

    const auto privateKey = PrivateKey(parse_hex("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.id()), "0c17d6eac276d68f6c63945252268620aa49aa881b28d766f1206c8627f782a7");
    ASSERT_EQ(hex(output.signature()), "19e8fdc4450515479a179e74e023768830e1ec51a34787cb09a56798f51b50f2471abc925fcb52208d48ec523bc3ae391e60c68297fb25ea39be691a8c8f08ef00");
}

TEST(TronSigner, SignUnFreezeAsset) {
    auto input = Proto::SigningInput();
    auto& transaction = *input.mutable_transaction();

    auto& unfreeze = *transaction.mutable_unfreeze_asset();
    unfreeze.set_owner_address("TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC");

    transaction.set_timestamp(1539295479000);
    transaction.set_expiration(1539295479000 + 10 * 60 * 60 * 1000);

    auto& blockHeader = *transaction.mutable_block_header();
    blockHeader.set_timestamp(1539295479000);
    const auto txTrieRoot = parse_hex("64288c2db0641316762a99dbb02ef7c90f968b60f9f2e410835980614332f86d");
    blockHeader.set_tx_trie_root(txTrieRoot.data(), txTrieRoot.size());
    const auto parentHash = parse_hex("00000000002f7b3af4f5f8b9e23a30c530f719f165b742e7358536b280eead2d");
    blockHeader.set_parent_hash(parentHash.data(), parentHash.size());
    blockHeader.set_number(3111739);
    const auto witnessAddress = parse_hex("415863f6091b8e71766da808b1dd3159790f61de7d");
    blockHeader.set_witness_address(witnessAddress.data(), witnessAddress.size());
    blockHeader.set_version(3);

    const auto privateKey = PrivateKey(parse_hex("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.id()), "01536f7ec1482651dc6ffbe6958ba57727e5446b68ffd923404fe55500b0b118");
    ASSERT_EQ(hex(output.signature()), "d6c1971678f0e6e1f766fb350a2c31c05cc4873505b99146d526be83ea400d5573a49a72b22147d84dc6d1ca5f93c36b9afa1720de1ccce71577baef1e0af0d601");
}

TEST(TronSigner, SignWithdrawBalance) {
    auto input = Proto::SigningInput();
    auto& transaction = *input.mutable_transaction();

    auto& unfreeze = *transaction.mutable_withdraw_balance();
    unfreeze.set_owner_address("TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC");

    transaction.set_timestamp(1539295479000);
    transaction.set_expiration(1539295479000 + 10 * 60 * 60 * 1000);

    auto& blockHeader = *transaction.mutable_block_header();
    blockHeader.set_timestamp(1539295479000);
    const auto txTrieRoot = parse_hex("64288c2db0641316762a99dbb02ef7c90f968b60f9f2e410835980614332f86d");
    blockHeader.set_tx_trie_root(txTrieRoot.data(), txTrieRoot.size());
    const auto parentHash = parse_hex("00000000002f7b3af4f5f8b9e23a30c530f719f165b742e7358536b280eead2d");
    blockHeader.set_parent_hash(parentHash.data(), parentHash.size());
    blockHeader.set_number(3111739);
    const auto witnessAddress = parse_hex("415863f6091b8e71766da808b1dd3159790f61de7d");
    blockHeader.set_witness_address(witnessAddress.data(), witnessAddress.size());
    blockHeader.set_version(3);

    const auto privateKey = PrivateKey(parse_hex("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.id()), "1cf610ef619136831fbe07ff3f901280a5653ff731b92ec40b664e2dbbd7b326");
    ASSERT_EQ(hex(output.signature()), "325e965c159ab47e4817ca0b3c3be0ff0fe2730007113bcb76a27cef8ddfb449434235f935d846578ee5347ed8ce1199e5509d9a1e0991aa6a92808b579f7de900");
}

TEST(TronSigner, SignVoteAsset) {
    auto input = Proto::SigningInput();
    auto& transaction = *input.mutable_transaction();

    auto& vote = *transaction.mutable_vote_asset();
    vote.set_owner_address("TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC");
    vote.add_vote_address("THTR75o8xXAgCTQqpiot2AFRAjvW1tSbVV");
    vote.set_support(true);
    vote.set_count(1);

    transaction.set_timestamp(1539295479000);
    transaction.set_expiration(1539295479000 + 10 * 60 * 60 * 1000);

    auto& blockHeader = *transaction.mutable_block_header();
    blockHeader.set_timestamp(1539295479000);
    const auto txTrieRoot = parse_hex("64288c2db0641316762a99dbb02ef7c90f968b60f9f2e410835980614332f86d");
    blockHeader.set_tx_trie_root(txTrieRoot.data(), txTrieRoot.size());
    const auto parentHash = parse_hex("00000000002f7b3af4f5f8b9e23a30c530f719f165b742e7358536b280eead2d");
    blockHeader.set_parent_hash(parentHash.data(), parentHash.size());
    blockHeader.set_number(3111739);
    const auto witnessAddress = parse_hex("415863f6091b8e71766da808b1dd3159790f61de7d");
    blockHeader.set_witness_address(witnessAddress.data(), witnessAddress.size());
    blockHeader.set_version(3);

    const auto privateKey = PrivateKey(parse_hex("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.id()), "6e94c42063a6ed8b6af573578c91d457ae7e87c139c91099ba10600988689d18");
    ASSERT_EQ(hex(output.signature()), "e223e5dc0e9d47e1d7288780d9fca933454d88cef98fe2d14be81c9beb591d2b0eb1d615d6a41de4accc738096a3c55dc7f3afb276f39ac463f5f9e919b4f13300");
}

TEST(TronSigner, SignVoteWitness) {
    auto input = Proto::SigningInput();
    auto& transaction = *input.mutable_transaction();

    auto& vote_witness = *transaction.mutable_vote_witness();
    vote_witness.set_owner_address("TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC");
    vote_witness.set_support(true);

    auto& vote = *vote_witness.add_votes();
    vote.set_vote_address("THTR75o8xXAgCTQqpiot2AFRAjvW1tSbVV");
    vote.set_vote_count(3);

    transaction.set_timestamp(1539295479000);
    transaction.set_expiration(1539295479000 + 10 * 60 * 60 * 1000);

    auto& blockHeader = *transaction.mutable_block_header();
    blockHeader.set_timestamp(1539295479000);
    const auto txTrieRoot = parse_hex("64288c2db0641316762a99dbb02ef7c90f968b60f9f2e410835980614332f86d");
    blockHeader.set_tx_trie_root(txTrieRoot.data(), txTrieRoot.size());
    const auto parentHash = parse_hex("00000000002f7b3af4f5f8b9e23a30c530f719f165b742e7358536b280eead2d");
    blockHeader.set_parent_hash(parentHash.data(), parentHash.size());
    blockHeader.set_number(3111739);
    const auto witnessAddress = parse_hex("415863f6091b8e71766da808b1dd3159790f61de7d");
    blockHeader.set_witness_address(witnessAddress.data(), witnessAddress.size());
    blockHeader.set_version(3);

    const auto privateKey = PrivateKey(parse_hex("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.id()), "4f647f0a0a825a8ea8c3a1a480df56748f855708e60c2f6efd6e0f4ec0c13b2c");
    ASSERT_EQ(hex(output.signature()), "baaacdc352c5a0fd64eafb94d08e52e6a6c914504a292435502fdb381b19943a0e1eabbef7425362f39f5368b7da80034778a282bdd1ca26474f035d433d746601");
}

TEST(TronSigner, SignTriggerSmartContract) {
    auto input = Proto::SigningInput();
    auto data = parse_hex("736f6d652064617461");
    auto& transaction = *input.mutable_transaction();
    auto& trigger_contract = *transaction.mutable_trigger_smart_contract();
    trigger_contract.set_owner_address("TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC");
    trigger_contract.set_contract_address("THTR75o8xXAgCTQqpiot2AFRAjvW1tSbVV");
    trigger_contract.set_call_value(0);
    trigger_contract.set_call_token_value(10000);
    trigger_contract.set_token_id(1);
    trigger_contract.set_data(data.data(), data.size());

    transaction.set_timestamp(1539295479000);
    transaction.set_expiration(1539295479000 + 10 * 60 * 60 * 1000);

    auto& blockHeader = *transaction.mutable_block_header();
    blockHeader.set_timestamp(1539295479000);
    const auto txTrieRoot = parse_hex("64288c2db0641316762a99dbb02ef7c90f968b60f9f2e410835980614332f86d");
    blockHeader.set_tx_trie_root(txTrieRoot.data(), txTrieRoot.size());
    const auto parentHash = parse_hex("00000000002f7b3af4f5f8b9e23a30c530f719f165b742e7358536b280eead2d");
    blockHeader.set_parent_hash(parentHash.data(), parentHash.size());
    blockHeader.set_number(3111739);
    const auto witnessAddress = parse_hex("415863f6091b8e71766da808b1dd3159790f61de7d");
    blockHeader.set_witness_address(witnessAddress.data(), witnessAddress.size());
    blockHeader.set_version(3);

    const auto privateKey = PrivateKey(parse_hex("2d8f68944bdbfbc0769542fba8fc2d2a3de67393334471624364c7006da2aa54"));
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    const auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.id()), "f9ab370a5100d125614b6e6ebbe1f13e03c4d82d6d61a6539905e39757ea1495");
    ASSERT_EQ(hex(output.signature()), "48c29695a5cc7736671257f37db40ddbdec0df9c01fadd6f202014e1bafb217c089481e948b33331cc254fdf21211765516cf16cf0dd2ae35245f764e6249f6400");
}
} // namespace TW::Tron
