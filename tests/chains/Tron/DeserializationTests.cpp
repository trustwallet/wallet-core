// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Tron/Deserialization.h"
#include "Tron/Serialization.h"
#include "HexCoding.h"

#include <gtest/gtest.h>

namespace TW::Tron {

// Each test takes a JSON string produced by transactionJSON() and verifies that
// transactionFromJSON() reconstructs a protocol::Transaction whose re-serialized
// raw_data_hex is bit-for-bit identical to the one embedded in the original JSON.
//
// Note: blockHeader fields (tx_trie_root, parent_hash, number, witness_address, version)
// are signer inputs consumed to derive ref_block_bytes and ref_block_hash. They are not
// stored in protocol::Transaction, so they are verified indirectly via those two fields.

TEST(TronDeserialization, TransferAsset) {
    const std::string txJson = R"({"raw_data":{"contract":[{"parameter":{"type_url":"type.googleapis.com/protocol.TransferAssetContract","value":{"amount":4,"asset_name":"31303030393539","owner_address":"415cd0fb0ab3ce40f3051414c604b27756e69e43db","to_address":"41521ea197907927725ef36d70f25f850d1659c7c7"}},"type":"TransferAssetContract"}],"expiration":1541926116000,"ref_block_bytes":"b801","ref_block_hash":"0e2bc08d550f5f58","timestamp":1539295479000},"raw_data_hex":"0a02b80122080e2bc08d550f5f5840a0b5d58ff02c5a730802126f0a32747970652e676f6f676c65617069732e636f6d2f70726f746f636f6c2e5472616e736665724173736574436f6e747261637412390a07313030303935391215415cd0fb0ab3ce40f3051414c604b27756e69e43db1a1541521ea197907927725ef36d70f25f850d1659c7c7200470d889a4a9e62c","signature":["77f5eabde31e739d34a66914540f1756981dc7d782c9656f5e14e53b59a15371603a183aa12124adeee7991bf55acc8e488a6ca04fb393b1a8ac16610eeafdfc00"],"txID":"546a3d07164c624809cf4e564a083a7a7974bb3c4eff6bb3e278b0ca21083fcb"})";

    const auto result = transactionFromJSON(txJson);
    ASSERT_TRUE(result.isSuccess()) << "Deserialization failed with error: " << result.error();
    const auto tx = result.payload();

    // raw_data_hex round-trip
    EXPECT_EQ(hex(serializeTxRawData(tx)), "0a02b80122080e2bc08d550f5f5840a0b5d58ff02c5a730802126f0a32747970652e676f6f676c65617069732e636f6d2f70726f746f636f6c2e5472616e736665724173736574436f6e747261637412390a07313030303935391215415cd0fb0ab3ce40f3051414c604b27756e69e43db1a1541521ea197907927725ef36d70f25f850d1659c7c7200470d889a4a9e62c");

    // raw_data header fields
    // ref_block_bytes/ref_block_hash are derived from blockHeader (number=3979265, parent_hash=00000000003cb800...)
    EXPECT_EQ(hex(tx.raw_data().ref_block_bytes()), "b801");
    EXPECT_EQ(hex(tx.raw_data().ref_block_hash()),  "0e2bc08d550f5f58");
    EXPECT_EQ(tx.raw_data().timestamp(),  1539295479000);
    EXPECT_EQ(tx.raw_data().expiration(), 1541926116000);

    // contract
    ASSERT_EQ(tx.raw_data().contract_size(), 1);
    EXPECT_EQ(tx.raw_data().contract(0).type(), protocol::Transaction::Contract::TransferAssetContract);

    protocol::TransferAssetContract contract;
    ASSERT_TRUE(tx.raw_data().contract(0).parameter().UnpackTo(&contract));
    EXPECT_EQ(hex(contract.asset_name()),    "31303030393539"); // "1000959"
    EXPECT_EQ(hex(contract.owner_address()), "415cd0fb0ab3ce40f3051414c604b27756e69e43db");
    EXPECT_EQ(hex(contract.to_address()),    "41521ea197907927725ef36d70f25f850d1659c7c7");
    EXPECT_EQ(contract.amount(), 4);
}

TEST(TronDeserialization, TransferContract) {
    const std::string txJson = R"({"raw_data":{"contract":[{"parameter":{"type_url":"type.googleapis.com/protocol.TransferContract","value":{"amount":2000000,"owner_address":"415cd0fb0ab3ce40f3051414c604b27756e69e43db","to_address":"41521ea197907927725ef36d70f25f850d1659c7c7"}},"type":"TransferContract"}],"expiration":1539331479000,"ref_block_bytes":"7b3b","ref_block_hash":"b21ace8d6ac20e7e","timestamp":1539295479000},"raw_data_hex":"0a027b3b2208b21ace8d6ac20e7e40d8abb9bae62c5a67080112630a2d747970652e676f6f676c65617069732e636f6d2f70726f746f636f6c2e5472616e73666572436f6e747261637412320a15415cd0fb0ab3ce40f3051414c604b27756e69e43db121541521ea197907927725ef36d70f25f850d1659c7c71880897a70d889a4a9e62c","txID":"dc6f6d9325ee44ab3c00528472be16e1572ab076aa161ccd12515029869d0451"})";

    const auto result = transactionFromJSON(txJson);
    ASSERT_TRUE(result.isSuccess()) << "Deserialization failed with error: " << result.error();
    const auto tx = result.payload();

    // raw_data_hex round-trip
    EXPECT_EQ(hex(serializeTxRawData(tx)), "0a027b3b2208b21ace8d6ac20e7e40d8abb9bae62c5a67080112630a2d747970652e676f6f676c65617069732e636f6d2f70726f746f636f6c2e5472616e73666572436f6e747261637412320a15415cd0fb0ab3ce40f3051414c604b27756e69e43db121541521ea197907927725ef36d70f25f850d1659c7c71880897a70d889a4a9e62c");

    // raw_data header fields
    // ref_block_bytes/ref_block_hash are derived from blockHeader (number=3111739, parent_hash=00000000002f7b3a...)
    EXPECT_EQ(hex(tx.raw_data().ref_block_bytes()), "7b3b");
    EXPECT_EQ(hex(tx.raw_data().ref_block_hash()),  "b21ace8d6ac20e7e");
    EXPECT_EQ(tx.raw_data().timestamp(),  1539295479000);
    EXPECT_EQ(tx.raw_data().expiration(), 1539331479000);

    // contract
    ASSERT_EQ(tx.raw_data().contract_size(), 1);
    EXPECT_EQ(tx.raw_data().contract(0).type(), protocol::Transaction::Contract::TransferContract);

    protocol::TransferContract contract;
    ASSERT_TRUE(tx.raw_data().contract(0).parameter().UnpackTo(&contract));
    EXPECT_EQ(hex(contract.owner_address()), "415cd0fb0ab3ce40f3051414c604b27756e69e43db");
    EXPECT_EQ(hex(contract.to_address()),    "41521ea197907927725ef36d70f25f850d1659c7c7");
    EXPECT_EQ(contract.amount(), 2000000);
}

TEST(TronDeserialization, VoteAsset) {
    const std::string txJson = R"({"raw_data":{"contract":[{"parameter":{"type_url":"type.googleapis.com/protocol.VoteAssetContract","value":{"count":1,"owner_address":"415cd0fb0ab3ce40f3051414c604b27756e69e43db","support":true,"vote_address":["41521ea197907927725ef36d70f25f850d1659c7c7"]}},"type":"VoteAssetContract"}],"expiration":1539331479000,"ref_block_bytes":"7b3b","ref_block_hash":"b21ace8d6ac20e7e","timestamp":1539295479000},"raw_data_hex":"0a027b3b2208b21ace8d6ac20e7e40d8abb9bae62c5a68080312640a2e747970652e676f6f676c65617069732e636f6d2f70726f746f636f6c2e566f74654173736574436f6e747261637412320a15415cd0fb0ab3ce40f3051414c604b27756e69e43db121541521ea197907927725ef36d70f25f850d1659c7c71801280170d889a4a9e62c","signature":["501e04b08f359116a26d9ec784abc50830f92a9dc05d2c1aceefe0eba79466d2730b63b6739edf0f1f1972181618b201ce0b4167d14a66abf40eba4097c39ec400"],"txID":"59b5736fb9756124f9470e4fadbcdafdc8c970da7157fa0ad34a41559418bf0a"})";

    const auto result = transactionFromJSON(txJson);
    ASSERT_TRUE(result.isSuccess()) << "Deserialization failed with error: " << result.error();
    const auto tx = result.payload();

    // raw_data_hex round-trip
    EXPECT_EQ(hex(serializeTxRawData(tx)), "0a027b3b2208b21ace8d6ac20e7e40d8abb9bae62c5a68080312640a2e747970652e676f6f676c65617069732e636f6d2f70726f746f636f6c2e566f74654173736574436f6e747261637412320a15415cd0fb0ab3ce40f3051414c604b27756e69e43db121541521ea197907927725ef36d70f25f850d1659c7c71801280170d889a4a9e62c");

    // raw_data header fields
    EXPECT_EQ(hex(tx.raw_data().ref_block_bytes()), "7b3b");
    EXPECT_EQ(hex(tx.raw_data().ref_block_hash()),  "b21ace8d6ac20e7e");
    EXPECT_EQ(tx.raw_data().timestamp(),  1539295479000);
    EXPECT_EQ(tx.raw_data().expiration(), 1539331479000);

    // contract
    ASSERT_EQ(tx.raw_data().contract_size(), 1);
    EXPECT_EQ(tx.raw_data().contract(0).type(), protocol::Transaction::Contract::VoteAssetContract);

    protocol::VoteAssetContract contract;
    ASSERT_TRUE(tx.raw_data().contract(0).parameter().UnpackTo(&contract));
    EXPECT_EQ(hex(contract.owner_address()), "415cd0fb0ab3ce40f3051414c604b27756e69e43db");
    EXPECT_TRUE(contract.support());
    EXPECT_EQ(contract.count(), 1);
    ASSERT_EQ(contract.vote_address_size(), 1);
    EXPECT_EQ(hex(contract.vote_address(0)), "41521ea197907927725ef36d70f25f850d1659c7c7");
}

TEST(TronDeserialization, VoteWitness) {
    const std::string txJson = R"({"raw_data":{"contract":[{"parameter":{"type_url":"type.googleapis.com/protocol.VoteWitnessContract","value":{"owner_address":"415cd0fb0ab3ce40f3051414c604b27756e69e43db","support":true,"votes":[{"vote_address":"41521ea197907927725ef36d70f25f850d1659c7c7","vote_count":3}]}},"type":"VoteWitnessContract"}],"expiration":1539331479000,"ref_block_bytes":"7b3b","ref_block_hash":"b21ace8d6ac20e7e","timestamp":1539295479000},"raw_data_hex":"0a027b3b2208b21ace8d6ac20e7e40d8abb9bae62c5a6c080412680a30747970652e676f6f676c65617069732e636f6d2f70726f746f636f6c2e566f74655769746e657373436f6e747261637412340a15415cd0fb0ab3ce40f3051414c604b27756e69e43db12190a1541521ea197907927725ef36d70f25f850d1659c7c71003180170d889a4a9e62c","signature":["79ec1073ae1319ef9303a2f5a515876cfd67f8f0e155bdbde1115d391c05358a3c32f148bfafacf07e1619aaed728d9ffbc2c7e4a5046003c7b74feb86fc68e400"],"txID":"3f923e9dd9571a66624fafeda27baa3e00aba1709d3fdc5c97c77b81fda18c1f"})";

    const auto result = transactionFromJSON(txJson);
    ASSERT_TRUE(result.isSuccess()) << "Deserialization failed with error: " << result.error();
    const auto tx = result.payload();

    // raw_data_hex round-trip
    EXPECT_EQ(hex(serializeTxRawData(tx)), "0a027b3b2208b21ace8d6ac20e7e40d8abb9bae62c5a6c080412680a30747970652e676f6f676c65617069732e636f6d2f70726f746f636f6c2e566f74655769746e657373436f6e747261637412340a15415cd0fb0ab3ce40f3051414c604b27756e69e43db12190a1541521ea197907927725ef36d70f25f850d1659c7c71003180170d889a4a9e62c");

    // raw_data header fields
    EXPECT_EQ(hex(tx.raw_data().ref_block_bytes()), "7b3b");
    EXPECT_EQ(hex(tx.raw_data().ref_block_hash()),  "b21ace8d6ac20e7e");
    EXPECT_EQ(tx.raw_data().timestamp(),  1539295479000);
    EXPECT_EQ(tx.raw_data().expiration(), 1539331479000);

    // contract
    ASSERT_EQ(tx.raw_data().contract_size(), 1);
    EXPECT_EQ(tx.raw_data().contract(0).type(), protocol::Transaction::Contract::VoteWitnessContract);

    protocol::VoteWitnessContract contract;
    ASSERT_TRUE(tx.raw_data().contract(0).parameter().UnpackTo(&contract));
    EXPECT_EQ(hex(contract.owner_address()), "415cd0fb0ab3ce40f3051414c604b27756e69e43db");
    EXPECT_TRUE(contract.support());
    ASSERT_EQ(contract.votes_size(), 1);
    EXPECT_EQ(hex(contract.votes(0).vote_address()), "41521ea197907927725ef36d70f25f850d1659c7c7");
    EXPECT_EQ(contract.votes(0).vote_count(), 3);
}

TEST(TronDeserialization, TriggerSmartContract) {
    const std::string txJson = R"({"raw_data":{"contract":[{"parameter":{"type_url":"type.googleapis.com/protocol.TriggerSmartContract","value":{"call_token_value":10000,"contract_address":"41521ea197907927725ef36d70f25f850d1659c7c7","data":"736f6d652064617461","owner_address":"415cd0fb0ab3ce40f3051414c604b27756e69e43db","token_id":1}},"type":"TriggerSmartContract"}],"expiration":1539331479000,"ref_block_bytes":"7b3b","ref_block_hash":"b21ace8d6ac20e7e","timestamp":1539295479000},"raw_data_hex":"0a027b3b2208b21ace8d6ac20e7e40d8abb9bae62c5a77081f12730a31747970652e676f6f676c65617069732e636f6d2f70726f746f636f6c2e54726967676572536d617274436f6e7472616374123e0a15415cd0fb0ab3ce40f3051414c604b27756e69e43db121541521ea197907927725ef36d70f25f850d1659c7c72209736f6d65206461746128904e300170d889a4a9e62c","signature":["21a99aafeabdddfdfae86538df048d120a83eb36bbcf5656595919ba6afddacd0a07d0ba051ae80337613174b109f36cb583b6e46ee5aecf6ffe3392fdbb8a2a01"],"txID":"9927d3daae10ad001b25ef3c1bb03073c928cc0e0823f6f3ce404c2b03ce3570"})";

    const auto result = transactionFromJSON(txJson);
    ASSERT_TRUE(result.isSuccess()) << "Deserialization failed with error: " << result.error();
    const auto tx = result.payload();

    // raw_data_hex round-trip
    EXPECT_EQ(hex(serializeTxRawData(tx)), "0a027b3b2208b21ace8d6ac20e7e40d8abb9bae62c5a77081f12730a31747970652e676f6f676c65617069732e636f6d2f70726f746f636f6c2e54726967676572536d617274436f6e7472616374123e0a15415cd0fb0ab3ce40f3051414c604b27756e69e43db121541521ea197907927725ef36d70f25f850d1659c7c72209736f6d65206461746128904e300170d889a4a9e62c");

    // raw_data header fields
    EXPECT_EQ(hex(tx.raw_data().ref_block_bytes()), "7b3b");
    EXPECT_EQ(hex(tx.raw_data().ref_block_hash()),  "b21ace8d6ac20e7e");
    EXPECT_EQ(tx.raw_data().timestamp(),  1539295479000);
    EXPECT_EQ(tx.raw_data().expiration(), 1539331479000);

    // contract
    ASSERT_EQ(tx.raw_data().contract_size(), 1);
    EXPECT_EQ(tx.raw_data().contract(0).type(), protocol::Transaction::Contract::TriggerSmartContract);

    protocol::TriggerSmartContract contract;
    ASSERT_TRUE(tx.raw_data().contract(0).parameter().UnpackTo(&contract));
    EXPECT_EQ(hex(contract.owner_address()),    "415cd0fb0ab3ce40f3051414c604b27756e69e43db");
    EXPECT_EQ(hex(contract.contract_address()), "41521ea197907927725ef36d70f25f850d1659c7c7");
    EXPECT_EQ(hex(contract.data()),             "736f6d652064617461"); // "some data"
    EXPECT_EQ(contract.call_value(),       0);
    EXPECT_EQ(contract.call_token_value(), 10000);
    EXPECT_EQ(contract.token_id(),         1);
}

TEST(TronDeserialization, TransferTrc20Contract) {
    // to_address (41dbd7c5...) and amount (1000 = 0x3e8) are ABI-encoded in `data`:
    //   selector:    a9059cbb
    //   to_address:  000...0041dbd7c53729b3310e1843083000fa84abad996961
    //   amount:      000...000000000000000000000000000000000000000003e8
    const std::string txJson = R"({"raw_data":{"contract":[{"parameter":{"type_url":"type.googleapis.com/protocol.TriggerSmartContract","value":{"contract_address":"41521ea197907927725ef36d70f25f850d1659c7c7","data":"a9059cbb000000000000000000000041dbd7c53729b3310e1843083000fa84abad99696100000000000000000000000000000000000000000000000000000000000003e8","owner_address":"415cd0fb0ab3ce40f3051414c604b27756e69e43db"}},"type":"TriggerSmartContract"}],"expiration":1539331479000,"ref_block_bytes":"7b3b","ref_block_hash":"b21ace8d6ac20e7e","timestamp":1539295479000},"raw_data_hex":"0a027b3b2208b21ace8d6ac20e7e40d8abb9bae62c5aae01081f12a9010a31747970652e676f6f676c65617069732e636f6d2f70726f746f636f6c2e54726967676572536d617274436f6e747261637412740a15415cd0fb0ab3ce40f3051414c604b27756e69e43db121541521ea197907927725ef36d70f25f850d1659c7c72244a9059cbb000000000000000000000041dbd7c53729b3310e1843083000fa84abad99696100000000000000000000000000000000000000000000000000000000000003e870d889a4a9e62c","signature":["bec790877b3a008640781e3948b070740b1f6023c29ecb3f7b5835433c13fc5835e5cad3bd44360ff2ddad5ed7dc9d7dee6878f90e86a40355b7697f5954b88c01"],"txID":"0d644290e3cf554f6219c7747f5287589b6e7e30e1b02793b48ba362da6a5058"})";

    const auto result = transactionFromJSON(txJson);
    ASSERT_TRUE(result.isSuccess()) << "Deserialization failed with error: " << result.error();
    const auto tx = result.payload();

    // raw_data_hex round-trip
    EXPECT_EQ(hex(serializeTxRawData(tx)), "0a027b3b2208b21ace8d6ac20e7e40d8abb9bae62c5aae01081f12a9010a31747970652e676f6f676c65617069732e636f6d2f70726f746f636f6c2e54726967676572536d617274436f6e747261637412740a15415cd0fb0ab3ce40f3051414c604b27756e69e43db121541521ea197907927725ef36d70f25f850d1659c7c72244a9059cbb000000000000000000000041dbd7c53729b3310e1843083000fa84abad99696100000000000000000000000000000000000000000000000000000000000003e870d889a4a9e62c");

    // raw_data header fields
    EXPECT_EQ(hex(tx.raw_data().ref_block_bytes()), "7b3b");
    EXPECT_EQ(hex(tx.raw_data().ref_block_hash()),  "b21ace8d6ac20e7e");
    EXPECT_EQ(tx.raw_data().timestamp(),  1539295479000);
    EXPECT_EQ(tx.raw_data().expiration(), 1539331479000);

    // contract
    ASSERT_EQ(tx.raw_data().contract_size(), 1);
    EXPECT_EQ(tx.raw_data().contract(0).type(), protocol::Transaction::Contract::TriggerSmartContract);

    protocol::TriggerSmartContract contract;
    ASSERT_TRUE(tx.raw_data().contract(0).parameter().UnpackTo(&contract));
    EXPECT_EQ(hex(contract.owner_address()),    "415cd0fb0ab3ce40f3051414c604b27756e69e43db");
    EXPECT_EQ(hex(contract.contract_address()), "41521ea197907927725ef36d70f25f850d1659c7c7");
    EXPECT_EQ(contract.call_value(),       0);
    EXPECT_EQ(contract.call_token_value(), 0);
    EXPECT_EQ(contract.token_id(),         0);
    // data: ERC-20 transfer(to=41dbd7c5..., amount=1000)
    EXPECT_EQ(hex(contract.data()), "a9059cbb"
                                    "000000000000000000000041dbd7c53729b3310e1843083000fa84abad996961"
                                    "00000000000000000000000000000000000000000000000000000000000003e8");
}

TEST(TronDeserialization, TransferTrc20Contract_LargeAmount) {
    // to_address (41dbd7c5...) and amount (10000000000000000000000 = 0x21e19e0c9bab2400000)
    // are ABI-encoded in `data`.
    const std::string txJson = R"({"raw_data":{"contract":[{"parameter":{"type_url":"type.googleapis.com/protocol.TriggerSmartContract","value":{"contract_address":"41521ea197907927725ef36d70f25f850d1659c7c7","data":"a9059cbb000000000000000000000041dbd7c53729b3310e1843083000fa84abad99696100000000000000000000000000000000000000000000021e19e0c9bab2400000","owner_address":"415cd0fb0ab3ce40f3051414c604b27756e69e43db"}},"type":"TriggerSmartContract"}],"expiration":1539331479000,"ref_block_bytes":"7b3b","ref_block_hash":"b21ace8d6ac20e7e","timestamp":1539295479000},"raw_data_hex":"0a027b3b2208b21ace8d6ac20e7e40d8abb9bae62c5aae01081f12a9010a31747970652e676f6f676c65617069732e636f6d2f70726f746f636f6c2e54726967676572536d617274436f6e747261637412740a15415cd0fb0ab3ce40f3051414c604b27756e69e43db121541521ea197907927725ef36d70f25f850d1659c7c72244a9059cbb000000000000000000000041dbd7c53729b3310e1843083000fa84abad99696100000000000000000000000000000000000000000000021e19e0c9bab240000070d889a4a9e62c","signature":["8207cbae6aff799cfefa1ab4d8a0c52b6a59be43491bd25b4f03754f0e8115b006b5f1393a3934ec3489f5d3c272a7af42658bdc165dc632b36114bd3180da2e00"],"txID":"774422d8d205760876496f22b7d4395cfceda03f139b8362a3693f1f405f0c36"})";

    const auto result = transactionFromJSON(txJson);
    ASSERT_TRUE(result.isSuccess()) << "Deserialization failed with error: " << result.error();
    const auto tx = result.payload();

    // raw_data_hex round-trip
    EXPECT_EQ(hex(serializeTxRawData(tx)), "0a027b3b2208b21ace8d6ac20e7e40d8abb9bae62c5aae01081f12a9010a31747970652e676f6f676c65617069732e636f6d2f70726f746f636f6c2e54726967676572536d617274436f6e747261637412740a15415cd0fb0ab3ce40f3051414c604b27756e69e43db121541521ea197907927725ef36d70f25f850d1659c7c72244a9059cbb000000000000000000000041dbd7c53729b3310e1843083000fa84abad99696100000000000000000000000000000000000000000000021e19e0c9bab240000070d889a4a9e62c");

    // raw_data header fields
    EXPECT_EQ(hex(tx.raw_data().ref_block_bytes()), "7b3b");
    EXPECT_EQ(hex(tx.raw_data().ref_block_hash()),  "b21ace8d6ac20e7e");
    EXPECT_EQ(tx.raw_data().timestamp(),  1539295479000);
    EXPECT_EQ(tx.raw_data().expiration(), 1539331479000);

    // contract
    ASSERT_EQ(tx.raw_data().contract_size(), 1);
    EXPECT_EQ(tx.raw_data().contract(0).type(), protocol::Transaction::Contract::TriggerSmartContract);

    protocol::TriggerSmartContract contract;
    ASSERT_TRUE(tx.raw_data().contract(0).parameter().UnpackTo(&contract));
    EXPECT_EQ(hex(contract.owner_address()),    "415cd0fb0ab3ce40f3051414c604b27756e69e43db");
    EXPECT_EQ(hex(contract.contract_address()), "41521ea197907927725ef36d70f25f850d1659c7c7");
    EXPECT_EQ(contract.call_value(),       0);
    EXPECT_EQ(contract.call_token_value(), 0);
    EXPECT_EQ(contract.token_id(),         0);
    // data: ERC-20 transfer(to=41dbd7c5..., amount=10000000000000000000000)
    EXPECT_EQ(hex(contract.data()), "a9059cbb"
                                    "000000000000000000000041dbd7c53729b3310e1843083000fa84abad996961"
                                    "00000000000000000000000000000000000000000000021e19e0c9bab2400000");
}

TEST(TronDeserialization, TriggerSmartContract_ContractNamePlainText) {
    // ContractName is set as plain UTF-8 text ("TetherToken") rather than hex.
    // It sits on the contract wrapper alongside "type" and "parameter", not inside "value".
    // Note: Serialization.cpp does not emit ContractName back to JSON, so no raw_data_hex
    // round-trip is asserted here — this test focuses on correct deserialization of the field.
    const std::string txJson = R"({
        "raw_data": {
            "ref_block_bytes": "7b3b",
            "ref_block_hash": "b21ace8d6ac20e7e",
            "expiration": 1539331479000,
            "timestamp": 1539295479000,
            "contract": [{
                "type": "TriggerSmartContract",
                "ContractName": "TetherToken",
                "parameter": {
                    "type_url": "type.googleapis.com/protocol.TriggerSmartContract",
                    "value": {
                        "owner_address": "415cd0fb0ab3ce40f3051414c604b27756e69e43db",
                        "contract_address": "41521ea197907927725ef36d70f25f850d1659c7c7",
                        "data": "a9059cbb000000000000000000000041dbd7c53729b3310e1843083000fa84abad99696100000000000000000000000000000000000000000000000000000000000003e8"
                    }
                }
            }]
        }
    })";

    const auto result = transactionFromJSON(txJson);
    ASSERT_TRUE(result.isSuccess()) << "Deserialization failed with error: " << result.error();
    const auto tx = result.payload();

    // raw_data header fields
    EXPECT_EQ(hex(tx.raw_data().ref_block_bytes()), "7b3b");
    EXPECT_EQ(hex(tx.raw_data().ref_block_hash()),  "b21ace8d6ac20e7e");
    EXPECT_EQ(tx.raw_data().timestamp(),  1539295479000);
    EXPECT_EQ(tx.raw_data().expiration(), 1539331479000);

    // contract type
    ASSERT_EQ(tx.raw_data().contract_size(), 1);
    const auto& c = tx.raw_data().contract(0);
    EXPECT_EQ(c.type(), protocol::Transaction::Contract::TriggerSmartContract);

    // ContractName: plain text "TetherToken" must be stored as-is in the bytes field
    EXPECT_EQ(c.contractname(), "TetherToken");

    // contract value
    protocol::TriggerSmartContract contract;
    ASSERT_TRUE(c.parameter().UnpackTo(&contract));
    EXPECT_EQ(hex(contract.owner_address()),    "415cd0fb0ab3ce40f3051414c604b27756e69e43db");
    EXPECT_EQ(hex(contract.contract_address()), "41521ea197907927725ef36d70f25f850d1659c7c7");
    EXPECT_EQ(contract.call_value(),       0);
    EXPECT_EQ(contract.call_token_value(), 0);
    EXPECT_EQ(contract.token_id(),         0);
    EXPECT_EQ(hex(contract.data()), "a9059cbb"
                                    "000000000000000000000041dbd7c53729b3310e1843083000fa84abad996961"
                                    "00000000000000000000000000000000000000000000000000000000000003e8");
}

TEST(TronDeserialization, InvalidJson_ReturnsFailure) {
    // Completely malformed input
    EXPECT_TRUE(transactionFromJSON("not json at all").isFailure());
    EXPECT_TRUE(transactionFromJSON("").isFailure());

    // Missing or wrong type for top-level raw_data
    EXPECT_TRUE(transactionFromJSON("{}").isFailure());
    EXPECT_TRUE(transactionFromJSON(R"({"raw_data":null})").isFailure());
    EXPECT_TRUE(transactionFromJSON(R"({"raw_data":"string"})").isFailure());
    EXPECT_TRUE(transactionFromJSON(R"({"raw_data":42})").isFailure());

    // raw_data present but no contract array
    EXPECT_TRUE(transactionFromJSON(R"({"raw_data":{}})").isFailure());

    // contract array is empty
    EXPECT_TRUE(transactionFromJSON(R"({"raw_data":{"contract":[]}})").isFailure());

    // contract array is not an array
    EXPECT_TRUE(transactionFromJSON(R"({"raw_data":{"contract":"bad"}})").isFailure());

    // contract entry missing "type" field
    EXPECT_TRUE(transactionFromJSON(R"({"raw_data":{"contract":[{"parameter":{}}]}})").isFailure());

    // contract entry "type" is not a string
    EXPECT_TRUE(transactionFromJSON(R"({"raw_data":{"contract":[{"type":42}]}})").isFailure());

    // contract entry "type" is an unknown/unsupported type name
    EXPECT_TRUE(transactionFromJSON(R"({"raw_data":{"contract":[{"type":"UnknownContract","parameter":{}}]}})").isFailure());

    // contract entry "type" is AccountCreateContract (explicitly unsupported in the switch)
    EXPECT_TRUE(transactionFromJSON(R"({"raw_data":{"contract":[{"type":"AccountCreateContract","parameter":{"value":{}}}]}})").isFailure());

    // contract entry missing "parameter" field
    EXPECT_TRUE(transactionFromJSON(R"({"raw_data":{"contract":[{"type":"TransferContract"}]}})").isFailure());

    // contract entry "parameter" is not an object
    EXPECT_TRUE(transactionFromJSON(R"({"raw_data":{"contract":[{"type":"TransferContract","parameter":"bad"}]}})").isFailure());
}

} // namespace TW::Tron
