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

    ASSERT_EQ(hex(output.id()), "c55020234343ddd055bded92e5656382a03370f33579518786fb637dc55c004b");
    ASSERT_EQ(hex(output.signature()), "d75839158e854d98631a010f0aaebf7285a733b38afb2dcb59fcf523aa869b561d9db159826f06ecb5b1b7824c2405fb37b178d70aa3d4188b9c53af7f684e4a00");
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

    ASSERT_EQ(hex(output.id()), "9d6c70491f7f20857b3a92ad59683485941e70bab2f09debf42eceabe31619f4");
    ASSERT_EQ(hex(output.signature()), "1063f470a8ab4948000993c8b067d1c7ce542a0651360f0d4dc41f95e6591b512b244d4dcffc36670dfe70674cac40b9c32a88d4e9a4bd639c029e0e4b5f5a8301");
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

    ASSERT_EQ(hex(output.id()), "432bd5cf77ff134787712724709a672fc6e51763de00292438db02d23931e13d");
    ASSERT_EQ(hex(output.signature()), "f493d8f275538a50bb8a832d759df9cad535bb2c5cc73296b04983f551d8398b6d7a30fc0fdfd73e8a9cac77a1a6a9435dc6309bb98fbb219035e88809a0b65901");
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

    ASSERT_EQ(hex(output.id()), "69aaa954dcd61f28a6a73e979addece6e36541522e5b3374b18b4ef9bc3de4cb");
    ASSERT_EQ(hex(output.signature()), "cb7d23a5eb23284a25ba6deaa231de0f18d8d103592e3312bff101a4219a3e02167eca24b3f4ce78b34f0c1842b6f7fb8d813f530c4c54342cdedef9f8e1f85100");
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

    ASSERT_EQ(hex(output.id()), "59b5736fb9756124f9470e4fadbcdafdc8c970da7157fa0ad34a41559418bf0a");
    ASSERT_EQ(hex(output.signature()), "501e04b08f359116a26d9ec784abc50830f92a9dc05d2c1aceefe0eba79466d2730b63b6739edf0f1f1972181618b201ce0b4167d14a66abf40eba4097c39ec400");
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

    ASSERT_EQ(hex(output.id()), "3f923e9dd9571a66624fafeda27baa3e00aba1709d3fdc5c97c77b81fda18c1f");
    ASSERT_EQ(hex(output.signature()), "79ec1073ae1319ef9303a2f5a515876cfd67f8f0e155bdbde1115d391c05358a3c32f148bfafacf07e1619aaed728d9ffbc2c7e4a5046003c7b74feb86fc68e400");
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

    ASSERT_EQ(hex(output.id()), "9927d3daae10ad001b25ef3c1bb03073c928cc0e0823f6f3ce404c2b03ce3570");
    ASSERT_EQ(hex(output.signature()), "21a99aafeabdddfdfae86538df048d120a83eb36bbcf5656595919ba6afddacd0a07d0ba051ae80337613174b109f36cb583b6e46ee5aecf6ffe3392fdbb8a2a01");
}
} // namespace TW::Tron
