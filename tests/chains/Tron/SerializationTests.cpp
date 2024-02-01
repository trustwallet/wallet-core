// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "proto/Tron.pb.h"
#include "Tron/Signer.h"
#include "PrivateKey.h"
#include "HexCoding.h"
#include "uint256.h"

#include <gtest/gtest.h>

namespace TW::Tron {
    TEST(TronSerialization, TransferAsset) {
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

        ASSERT_EQ(output.json(), R"({"raw_data":{"contract":[{"parameter":{"type_url":"type.googleapis.com/protocol.TransferAssetContract","value":{"amount":4,"asset_name":"31303030393539","owner_address":"415cd0fb0ab3ce40f3051414c604b27756e69e43db","to_address":"41521ea197907927725ef36d70f25f850d1659c7c7"}},"type":"TransferAssetContract"}],"expiration":1541926116000,"ref_block_bytes":"b801","ref_block_hash":"0e2bc08d550f5f58","timestamp":1539295479000},"signature":["77f5eabde31e739d34a66914540f1756981dc7d782c9656f5e14e53b59a15371603a183aa12124adeee7991bf55acc8e488a6ca04fb393b1a8ac16610eeafdfc00"],"txID":"546a3d07164c624809cf4e564a083a7a7974bb3c4eff6bb3e278b0ca21083fcb"})");
    }

    TEST(TronSerialization, SignVoteAsset) {
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

        ASSERT_EQ(output.json(), R"({"raw_data":{"contract":[{"parameter":{"type_url":"type.googleapis.com/protocol.VoteAssetContract","value":{"count":1,"owner_address":"415cd0fb0ab3ce40f3051414c604b27756e69e43db","support":true,"vote_address":["41521ea197907927725ef36d70f25f850d1659c7c7"]}},"type":"VoteAssetContract"}],"expiration":1539331479000,"ref_block_bytes":"7b3b","ref_block_hash":"b21ace8d6ac20e7e","timestamp":1539295479000},"signature":["501e04b08f359116a26d9ec784abc50830f92a9dc05d2c1aceefe0eba79466d2730b63b6739edf0f1f1972181618b201ce0b4167d14a66abf40eba4097c39ec400"],"txID":"59b5736fb9756124f9470e4fadbcdafdc8c970da7157fa0ad34a41559418bf0a"})");
    }

    TEST(TronSerialization, SignVoteWitness) {
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

        ASSERT_EQ(output.json(), R"({"raw_data":{"contract":[{"parameter":{"type_url":"type.googleapis.com/protocol.VoteWitnessContract","value":{"owner_address":"415cd0fb0ab3ce40f3051414c604b27756e69e43db","support":true,"votes":[{"vote_address":"41521ea197907927725ef36d70f25f850d1659c7c7","vote_count":3}]}},"type":"VoteWitnessContract"}],"expiration":1539331479000,"ref_block_bytes":"7b3b","ref_block_hash":"b21ace8d6ac20e7e","timestamp":1539295479000},"signature":["79ec1073ae1319ef9303a2f5a515876cfd67f8f0e155bdbde1115d391c05358a3c32f148bfafacf07e1619aaed728d9ffbc2c7e4a5046003c7b74feb86fc68e400"],"txID":"3f923e9dd9571a66624fafeda27baa3e00aba1709d3fdc5c97c77b81fda18c1f"})");
    }

    TEST(TronSerialization, SignTriggerSmartContract) {
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

        ASSERT_EQ(output.json(), R"({"raw_data":{"contract":[{"parameter":{"type_url":"type.googleapis.com/protocol.TriggerSmartContract","value":{"call_token_value":10000,"contract_address":"41521ea197907927725ef36d70f25f850d1659c7c7","data":"736f6d652064617461","owner_address":"415cd0fb0ab3ce40f3051414c604b27756e69e43db","token_id":1}},"type":"TriggerSmartContract"}],"expiration":1539331479000,"ref_block_bytes":"7b3b","ref_block_hash":"b21ace8d6ac20e7e","timestamp":1539295479000},"signature":["21a99aafeabdddfdfae86538df048d120a83eb36bbcf5656595919ba6afddacd0a07d0ba051ae80337613174b109f36cb583b6e46ee5aecf6ffe3392fdbb8a2a01"],"txID":"9927d3daae10ad001b25ef3c1bb03073c928cc0e0823f6f3ce404c2b03ce3570"})");
    }

    TEST(TronSerialization, SignTransferTrc20Contract) {
        auto input = Proto::SigningInput();
        auto& transaction = *input.mutable_transaction();
        auto& transfer_contract = *transaction.mutable_transfer_trc20_contract();
        transfer_contract.set_owner_address("TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC");
        transfer_contract.set_contract_address("THTR75o8xXAgCTQqpiot2AFRAjvW1tSbVV");
        transfer_contract.set_to_address("TW1dU4L3eNm7Lw8WvieLKEHpXWAussRG9Z");
        Data amount = store(uint256_t(1000));
        transfer_contract.set_amount(std::string(amount.begin(), amount.end()));

        transaction.set_timestamp(1539295479000);

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

        ASSERT_EQ(output.json(), R"({"raw_data":{"contract":[{"parameter":{"type_url":"type.googleapis.com/protocol.TriggerSmartContract","value":{"contract_address":"41521ea197907927725ef36d70f25f850d1659c7c7","data":"a9059cbb000000000000000000000041dbd7c53729b3310e1843083000fa84abad99696100000000000000000000000000000000000000000000000000000000000003e8","owner_address":"415cd0fb0ab3ce40f3051414c604b27756e69e43db"}},"type":"TriggerSmartContract"}],"expiration":1539331479000,"ref_block_bytes":"7b3b","ref_block_hash":"b21ace8d6ac20e7e","timestamp":1539295479000},"signature":["bec790877b3a008640781e3948b070740b1f6023c29ecb3f7b5835433c13fc5835e5cad3bd44360ff2ddad5ed7dc9d7dee6878f90e86a40355b7697f5954b88c01"],"txID":"0d644290e3cf554f6219c7747f5287589b6e7e30e1b02793b48ba362da6a5058"})");
    }

    TEST(TronSerialization, SignTransferTrc20Contract_LargeAmount) {
        auto input = Proto::SigningInput();
        auto& transaction = *input.mutable_transaction();
        auto& transfer_contract = *transaction.mutable_transfer_trc20_contract();
        transfer_contract.set_owner_address("TJRyWwFs9wTFGZg3JbrVriFbNfCug5tDeC");
        transfer_contract.set_contract_address("THTR75o8xXAgCTQqpiot2AFRAjvW1tSbVV");
        transfer_contract.set_to_address("TW1dU4L3eNm7Lw8WvieLKEHpXWAussRG9Z");
        Data amount = store(uint256_t("10000000000000000000000")); // over 64 bits, corresponds to 10000 in case of 18 decimals
        transfer_contract.set_amount(std::string(amount.begin(), amount.end()));

        transaction.set_timestamp(1539295479000);

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

        ASSERT_EQ(output.json(), R"({"raw_data":{"contract":[{"parameter":{"type_url":"type.googleapis.com/protocol.TriggerSmartContract","value":{"contract_address":"41521ea197907927725ef36d70f25f850d1659c7c7","data":"a9059cbb000000000000000000000041dbd7c53729b3310e1843083000fa84abad99696100000000000000000000000000000000000000000000021e19e0c9bab2400000","owner_address":"415cd0fb0ab3ce40f3051414c604b27756e69e43db"}},"type":"TriggerSmartContract"}],"expiration":1539331479000,"ref_block_bytes":"7b3b","ref_block_hash":"b21ace8d6ac20e7e","timestamp":1539295479000},"signature":["8207cbae6aff799cfefa1ab4d8a0c52b6a59be43491bd25b4f03754f0e8115b006b5f1393a3934ec3489f5d3c272a7af42658bdc165dc632b36114bd3180da2e00"],"txID":"774422d8d205760876496f22b7d4395cfceda03f139b8362a3693f1f405f0c36"})");
    }
}
