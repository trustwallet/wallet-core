// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Cardano/Transaction.h"
#include "Cardano/AddressV3.h"
#include "Cardano/Signer.h"
#include "proto/Cardano.pb.h"
#include <TrustWalletCore/TWCardano.h>

#include "HexCoding.h"
#include "PrivateKey.h"
#include "Cbor.h"
#include "TestUtilities.h"

#include <gtest/gtest.h>

namespace TW::Cardano::VoteDelegationTests {

const auto privateKeyTest1 = "40b03ce671f57c56b3483a9f946d4650ed03b1583357018b40aeef3bec81e74df8472ff07b6dfe76e83ea50102f7063595fb24aedaaf01dd9b2cd526a5c7f2db2275254e04a05729852388f1be93a47f859f42087c22e4c78f9d4f6f09d9557820716973437afb26051f2493813d22fc356a1fbdd91ab44f3294ca26ec81e74d7729d9e2323846c7545e6d780bbeb2df204c1a9a1b58abfac647200aefa3cbc1e0746aaad10f546ceb656c75e4d6dadffa5197cf9dc58579f1348a6b3d397d58";
const auto ownAddress1 = "addr1qx2v7hcfyctmv9vv42yntu40um8t67lxkdzq24gvkfjmy8mlajpf8n6kj3v2sprfkaqfqhtayzjy7nx6apm0vn3j0dqs8x4237";
const auto stakingAddress1 = "stake1u9l7eq5neatfgk9gq35mwsyst47jpfz0fndwsahkfce8ksgsx8ef2";
const auto dRepAddressCIP129 = "drep1y29h2q6cst2pvkl2sqqvf5ljcy36uv7pmy482xnczddzgqshus24w"; // Eternl DRep Committee
const auto dRepAddressCIP105 = "drep13d6sxkyz6st9h65qqrzd8ukpywhr8swe9f6357qntgjqye0gttd"; // Eternl DRep Committee

TEST(CardanoVoteDelegation, DelegateToSpecificDRepCIP129) {
    const auto privateKeyData = parse_hex(privateKeyTest1);
    const auto publicKey = PrivateKey(privateKeyData, TWCurveED25519ExtendedCardano).getPublicKey(TWPublicKeyTypeED25519Cardano);
    const auto ownAddress = AddressV3(publicKey).string();
    EXPECT_EQ(ownAddress, ownAddress1);
    const auto stakingAddress = AddressV3(publicKey).getStakingAddress();
    EXPECT_EQ(stakingAddress, stakingAddress1);

    Proto::SigningInput input;
    auto* utxo1 = input.add_utxos();
    const auto txHash1 = parse_hex("b02dfc255f1048260d1915c9ebcfcdb69bab7677e75d45ec1245c468f0283d7e");
    utxo1->mutable_out_point()->set_tx_hash(txHash1.data(), txHash1.size());
    utxo1->mutable_out_point()->set_output_index(0);
    utxo1->set_address(ownAddress);
    utxo1->set_amount(4790541ul);

    input.add_private_key(privateKeyData.data(), privateKeyData.size());

    input.mutable_transfer_message()->set_to_address(ownAddress);
    input.mutable_transfer_message()->set_change_address(ownAddress);
    input.mutable_transfer_message()->set_use_max_amount(true);
    input.set_ttl(148398202);

    // Vote delegate to always abstain
    input.mutable_vote_delegation()->set_staking_address(stakingAddress);
    input.mutable_vote_delegation()->set_drep_type(Proto::VoteDelegation::DREP_ID);
    input.mutable_vote_delegation()->set_drep_id(dRepAddressCIP129);

    const auto amount = 4617164ul;
    input.mutable_plan()->set_amount(amount);
    input.mutable_plan()->set_available_amount(4790541ul);
    input.mutable_plan()->set_fee(173377);
    input.mutable_plan()->set_change(0);
    *(input.mutable_plan()->add_utxos()) = input.utxos(0);
    input.mutable_plan()->set_error(Common::Proto::OK);

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::OK);

    const auto txid = data(output.tx_id());
    // Broadcasted here: https://cardanoscan.io/transaction/290f07d69f75f396e78b4ee65088dbf7dc63b86d5b3a5bb7d6aeec59bd487d25
    EXPECT_EQ(hex(txid), "290f07d69f75f396e78b4ee65088dbf7dc63b86d5b3a5bb7d6aeec59bd487d25");

    const auto encoded = data(output.encoded());
    const auto txHex = hex(encoded);
    EXPECT_EQ(txHex, "84a50081825820b02dfc255f1048260d1915c9ebcfcdb69bab7677e75d45ec1245c468f0283d7e0001818258390194cf5f092617b6158caa8935f2afe6cebd7be6b34405550cb265b21f7fec8293cf569458a80469b740905d7d20a44f4cdae876f64e327b411a004673cc021a0002a541031a08d8607a048183098200581c7fec8293cf569458a80469b740905d7d20a44f4cdae876f64e327b418200581c8b75035882d4165bea8000c4d3f2c123ae33c1d92a751a78135a2402a10082825820b4ccb8cb788b37b038d327f0da87b32c6abedf4b131c87103637ef2ed04710365840c3f26171f723e4876cccbbac40edb373c64ccee8b5eec1b39bfa2a478f0b963540a7e91f10423b2a42b43732c5fbbe536c85dbc228e9b395889c05bf5cb1190982582064982cd6a2a0f49aa8225ab48b3ae3bffab2e19e486f932b303052dc227bc4035840518b68bcca2b0d732614b114f384abfffe79b92877d599bf7e9810642f4eef2df696ea113b698eff6a6e25a791d342faef0ef565aaeed3226c7b88f357ba1900f5f6");

    {
        const auto decode = Cbor::Decode(encoded);
        ASSERT_TRUE(decode.isValid());
        EXPECT_EQ(decode.dumpToString(), "[{0: [[h\"b02dfc255f1048260d1915c9ebcfcdb69bab7677e75d45ec1245c468f0283d7e\", 0]], 1: [[h\"0194cf5f092617b6158caa8935f2afe6cebd7be6b34405550cb265b21f7fec8293cf569458a80469b740905d7d20a44f4cdae876f64e327b41\", 4617164]], 2: 173377, 3: 148398202, 4: [[9, [0, h\"7fec8293cf569458a80469b740905d7d20a44f4cdae876f64e327b41\"], [0, h\"8b75035882d4165bea8000c4d3f2c123ae33c1d92a751a78135a2402\"]]]}, {0: [[h\"b4ccb8cb788b37b038d327f0da87b32c6abedf4b131c87103637ef2ed0471036\", h\"c3f26171f723e4876cccbbac40edb373c64ccee8b5eec1b39bfa2a478f0b963540a7e91f10423b2a42b43732c5fbbe536c85dbc228e9b395889c05bf5cb11909\"], [h\"64982cd6a2a0f49aa8225ab48b3ae3bffab2e19e486f932b303052dc227bc403\", h\"518b68bcca2b0d732614b114f384abfffe79b92877d599bf7e9810642f4eef2df696ea113b698eff6a6e25a791d342faef0ef565aaeed3226c7b88f357ba1900\"]]}, spec 21, null]");
        EXPECT_EQ(decode.getArrayElements().size(), 4ul);
    }
}

TEST(CardanoVoteDelegation, DelegateToSpecificDRepCIP105) {
    const auto privateKeyData = parse_hex(privateKeyTest1);
    const auto publicKey = PrivateKey(privateKeyData, TWCurveED25519ExtendedCardano).getPublicKey(TWPublicKeyTypeED25519Cardano);
    const auto ownAddress = AddressV3(publicKey).string();
    EXPECT_EQ(ownAddress, ownAddress1);
    const auto stakingAddress = AddressV3(publicKey).getStakingAddress();
    EXPECT_EQ(stakingAddress, stakingAddress1);

    Proto::SigningInput input;
    auto* utxo1 = input.add_utxos();
    const auto txHash1 = parse_hex("b02dfc255f1048260d1915c9ebcfcdb69bab7677e75d45ec1245c468f0283d7e");
    utxo1->mutable_out_point()->set_tx_hash(txHash1.data(), txHash1.size());
    utxo1->mutable_out_point()->set_output_index(0);
    utxo1->set_address(ownAddress);
    utxo1->set_amount(4790541ul);

    input.add_private_key(privateKeyData.data(), privateKeyData.size());

    input.mutable_transfer_message()->set_to_address(ownAddress);
    input.mutable_transfer_message()->set_change_address(ownAddress);
    input.mutable_transfer_message()->set_use_max_amount(true);
    input.set_ttl(148398202);

    // Vote delegate to always abstain
    input.mutable_vote_delegation()->set_staking_address(stakingAddress);
    input.mutable_vote_delegation()->set_drep_type(Proto::VoteDelegation::DREP_ID);
    input.mutable_vote_delegation()->set_drep_id(dRepAddressCIP105);

    const auto amount = 4617164ul;
    input.mutable_plan()->set_amount(amount);
    input.mutable_plan()->set_available_amount(4790541ul);
    input.mutable_plan()->set_fee(173377);
    input.mutable_plan()->set_change(0);
    *(input.mutable_plan()->add_utxos()) = input.utxos(0);
    input.mutable_plan()->set_error(Common::Proto::OK);

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::OK);

    const auto txid = data(output.tx_id());
    // Broadcasted here: https://cardanoscan.io/transaction/290f07d69f75f396e78b4ee65088dbf7dc63b86d5b3a5bb7d6aeec59bd487d25
    EXPECT_EQ(hex(txid), "290f07d69f75f396e78b4ee65088dbf7dc63b86d5b3a5bb7d6aeec59bd487d25");

    const auto encoded = data(output.encoded());
    const auto txHex = hex(encoded);
    EXPECT_EQ(txHex, "84a50081825820b02dfc255f1048260d1915c9ebcfcdb69bab7677e75d45ec1245c468f0283d7e0001818258390194cf5f092617b6158caa8935f2afe6cebd7be6b34405550cb265b21f7fec8293cf569458a80469b740905d7d20a44f4cdae876f64e327b411a004673cc021a0002a541031a08d8607a048183098200581c7fec8293cf569458a80469b740905d7d20a44f4cdae876f64e327b418200581c8b75035882d4165bea8000c4d3f2c123ae33c1d92a751a78135a2402a10082825820b4ccb8cb788b37b038d327f0da87b32c6abedf4b131c87103637ef2ed04710365840c3f26171f723e4876cccbbac40edb373c64ccee8b5eec1b39bfa2a478f0b963540a7e91f10423b2a42b43732c5fbbe536c85dbc228e9b395889c05bf5cb1190982582064982cd6a2a0f49aa8225ab48b3ae3bffab2e19e486f932b303052dc227bc4035840518b68bcca2b0d732614b114f384abfffe79b92877d599bf7e9810642f4eef2df696ea113b698eff6a6e25a791d342faef0ef565aaeed3226c7b88f357ba1900f5f6");

    {
        const auto decode = Cbor::Decode(encoded);
        ASSERT_TRUE(decode.isValid());
        EXPECT_EQ(decode.dumpToString(), "[{0: [[h\"b02dfc255f1048260d1915c9ebcfcdb69bab7677e75d45ec1245c468f0283d7e\", 0]], 1: [[h\"0194cf5f092617b6158caa8935f2afe6cebd7be6b34405550cb265b21f7fec8293cf569458a80469b740905d7d20a44f4cdae876f64e327b41\", 4617164]], 2: 173377, 3: 148398202, 4: [[9, [0, h\"7fec8293cf569458a80469b740905d7d20a44f4cdae876f64e327b41\"], [0, h\"8b75035882d4165bea8000c4d3f2c123ae33c1d92a751a78135a2402\"]]]}, {0: [[h\"b4ccb8cb788b37b038d327f0da87b32c6abedf4b131c87103637ef2ed0471036\", h\"c3f26171f723e4876cccbbac40edb373c64ccee8b5eec1b39bfa2a478f0b963540a7e91f10423b2a42b43732c5fbbe536c85dbc228e9b395889c05bf5cb11909\"], [h\"64982cd6a2a0f49aa8225ab48b3ae3bffab2e19e486f932b303052dc227bc403\", h\"518b68bcca2b0d732614b114f384abfffe79b92877d599bf7e9810642f4eef2df696ea113b698eff6a6e25a791d342faef0ef565aaeed3226c7b88f357ba1900\"]]}, spec 21, null]");
        EXPECT_EQ(decode.getArrayElements().size(), 4ul);
    }
}

TEST(CardanoVoteDelegation, DelegateToAlwaysAbstain) {
    const auto privateKeyData = parse_hex(privateKeyTest1);
    const auto publicKey = PrivateKey(privateKeyData, TWCurveED25519ExtendedCardano).getPublicKey(TWPublicKeyTypeED25519Cardano);
    const auto ownAddress = AddressV3(publicKey).string();
    EXPECT_EQ(ownAddress, ownAddress1);
    const auto stakingAddress = AddressV3(publicKey).getStakingAddress();
    EXPECT_EQ(stakingAddress, stakingAddress1);

    Proto::SigningInput input;
    auto* utxo1 = input.add_utxos();
    const auto txHash1 = parse_hex("6a96dc0aaa9a7ec6cd7dfcea679a71ff55e7661d609772829f3a4a33a0b7c942");
    utxo1->mutable_out_point()->set_tx_hash(txHash1.data(), txHash1.size());
    utxo1->mutable_out_point()->set_output_index(0);
    utxo1->set_address(ownAddress);
    utxo1->set_amount(5307944ul);

    input.add_private_key(privateKeyData.data(), privateKeyData.size());

    input.mutable_transfer_message()->set_to_address(ownAddress);
    input.mutable_transfer_message()->set_change_address(ownAddress);
    input.mutable_transfer_message()->set_use_max_amount(true);
    input.set_ttl(148390086);

    // Vote delegate to always abstain
    input.mutable_vote_delegation()->set_staking_address(stakingAddress);
    input.mutable_vote_delegation()->set_drep_type(Proto::VoteDelegation::DREP_ALWAYS_ABSTAIN);

    const auto amount = 5135887ul;
    input.mutable_plan()->set_amount(amount);
    input.mutable_plan()->set_available_amount(5307944ul);
    input.mutable_plan()->set_fee(172057);
    input.mutable_plan()->set_change(0);
    *(input.mutable_plan()->add_utxos()) = input.utxos(0);
    input.mutable_plan()->set_error(Common::Proto::OK);

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::OK);

    const auto txid = data(output.tx_id());
    // Broadcasted here: https://cardanoscan.io/transaction/99b8a9d39e6c0c7df8ff3004097b68f7e6acd0e382182126afa00859c37b1bda
    EXPECT_EQ(hex(txid), "99b8a9d39e6c0c7df8ff3004097b68f7e6acd0e382182126afa00859c37b1bda");

    const auto encoded = data(output.encoded());
    const auto txHex = hex(encoded);
    EXPECT_EQ(txHex, "84a500818258206a96dc0aaa9a7ec6cd7dfcea679a71ff55e7661d609772829f3a4a33a0b7c9420001818258390194cf5f092617b6158caa8935f2afe6cebd7be6b34405550cb265b21f7fec8293cf569458a80469b740905d7d20a44f4cdae876f64e327b411a004e5e0f021a0002a019031a08d840c6048183098200581c7fec8293cf569458a80469b740905d7d20a44f4cdae876f64e327b418102a10082825820b4ccb8cb788b37b038d327f0da87b32c6abedf4b131c87103637ef2ed04710365840c5ff299707cab2067d4c7d62cc1e72fd73c510d5d509376fea25ec8e7545fdb686db986a88b9a50c7100b61d564dd40fb796c1aeb1a2a1d3555df2d0cc086e0282582064982cd6a2a0f49aa8225ab48b3ae3bffab2e19e486f932b303052dc227bc4035840555ee0ed2bc5ed50e3a22981b08e61867353c6a591265e5b66aec736ba0ee12eb5b181a38e678175df84b7bc8686b74fbf37e1bc3773748d5af002f1501a5d00f5f6");

    {
        const auto decode = Cbor::Decode(encoded);
        ASSERT_TRUE(decode.isValid());
        EXPECT_EQ(decode.dumpToString(), "[{0: [[h\"6a96dc0aaa9a7ec6cd7dfcea679a71ff55e7661d609772829f3a4a33a0b7c942\", 0]], 1: [[h\"0194cf5f092617b6158caa8935f2afe6cebd7be6b34405550cb265b21f7fec8293cf569458a80469b740905d7d20a44f4cdae876f64e327b41\", 5135887]], 2: 172057, 3: 148390086, 4: [[9, [0, h\"7fec8293cf569458a80469b740905d7d20a44f4cdae876f64e327b41\"], [2]]]}, {0: [[h\"b4ccb8cb788b37b038d327f0da87b32c6abedf4b131c87103637ef2ed0471036\", h\"c5ff299707cab2067d4c7d62cc1e72fd73c510d5d509376fea25ec8e7545fdb686db986a88b9a50c7100b61d564dd40fb796c1aeb1a2a1d3555df2d0cc086e02\"], [h\"64982cd6a2a0f49aa8225ab48b3ae3bffab2e19e486f932b303052dc227bc403\", h\"555ee0ed2bc5ed50e3a22981b08e61867353c6a591265e5b66aec736ba0ee12eb5b181a38e678175df84b7bc8686b74fbf37e1bc3773748d5af002f1501a5d00\"]]}, spec 21, null]");
        EXPECT_EQ(decode.getArrayElements().size(), 4ul);
    }
}

TEST(CardanoVoteDelegation, DelegateToNoConfidence) {
    const auto privateKeyData = parse_hex(privateKeyTest1);
    const auto publicKey = PrivateKey(privateKeyData, TWCurveED25519ExtendedCardano).getPublicKey(TWPublicKeyTypeED25519Cardano);
    const auto ownAddress = AddressV3(publicKey).string();
    EXPECT_EQ(ownAddress, ownAddress1);
    const auto stakingAddress = AddressV3(publicKey).getStakingAddress();
    EXPECT_EQ(stakingAddress, stakingAddress1);

    Proto::SigningInput input;
    auto* utxo1 = input.add_utxos();
    const auto txHash1 = parse_hex("8de1ec78274095fbc02ad579b471c9394665d402f29fc8329f8de61039328bbf");
    utxo1->mutable_out_point()->set_tx_hash(txHash1.data(), txHash1.size());
    utxo1->mutable_out_point()->set_output_index(1);
    utxo1->set_address(ownAddress);
    utxo1->set_amount(5000000ul);

    input.add_private_key(privateKeyData.data(), privateKeyData.size());

    input.mutable_transfer_message()->set_to_address(ownAddress);
    input.mutable_transfer_message()->set_change_address(ownAddress);
    input.mutable_transfer_message()->set_use_max_amount(true);
    input.set_ttl(148393182ul);

    // Vote delegate to always abstain
    input.mutable_vote_delegation()->set_staking_address(stakingAddress);
    input.mutable_vote_delegation()->set_drep_type(Proto::VoteDelegation::DREP_NO_CONFIDENCE);

    const auto amount = 4827943ul;
    input.mutable_plan()->set_amount(amount);
    input.mutable_plan()->set_available_amount(5000000ul);
    input.mutable_plan()->set_fee(172057);
    input.mutable_plan()->set_change(0);
    *(input.mutable_plan()->add_utxos()) = input.utxos(0);
    input.mutable_plan()->set_error(Common::Proto::OK);

    auto signer = Signer(input);
    const auto output = signer.sign();

    EXPECT_EQ(output.error(), Common::Proto::OK);

    const auto txid = data(output.tx_id());
    // Broadcasted here: https://cardanoscan.io/transaction/b2fa7c73261af656bd7dd3869d3f3e57e527d7c0777f82766c851a627defb4e2
    EXPECT_EQ(hex(txid), "b2fa7c73261af656bd7dd3869d3f3e57e527d7c0777f82766c851a627defb4e2");

    const auto encoded = data(output.encoded());
    const auto txHex = hex(encoded);
    EXPECT_EQ(txHex, "84a500818258208de1ec78274095fbc02ad579b471c9394665d402f29fc8329f8de61039328bbf0101818258390194cf5f092617b6158caa8935f2afe6cebd7be6b34405550cb265b21f7fec8293cf569458a80469b740905d7d20a44f4cdae876f64e327b411a0049ab27021a0002a019031a08d84cde048183098200581c7fec8293cf569458a80469b740905d7d20a44f4cdae876f64e327b418103a10082825820b4ccb8cb788b37b038d327f0da87b32c6abedf4b131c87103637ef2ed04710365840ee72cde1827222eb587ada0f4dc4a6f4a35a3059d91c8cebdca0d8424126f322db540f985e0e8968e526c5bdd15a63b04afcd7864bd45a038de38ce057bbf00982582064982cd6a2a0f49aa8225ab48b3ae3bffab2e19e486f932b303052dc227bc4035840895174677319cfc209030d64b6d50bce1604f6b30b873d5905bb3d33551e01bb7d340a5f3f36e60e6fa42befcb2bec4f0f890e1ec6977b064b21511837e8cc0ff5f6");

    {
        const auto decode = Cbor::Decode(encoded);
        ASSERT_TRUE(decode.isValid());
        EXPECT_EQ(decode.dumpToString(), "[{0: [[h\"8de1ec78274095fbc02ad579b471c9394665d402f29fc8329f8de61039328bbf\", 1]], 1: [[h\"0194cf5f092617b6158caa8935f2afe6cebd7be6b34405550cb265b21f7fec8293cf569458a80469b740905d7d20a44f4cdae876f64e327b41\", 4827943]], 2: 172057, 3: 148393182, 4: [[9, [0, h\"7fec8293cf569458a80469b740905d7d20a44f4cdae876f64e327b41\"], [3]]]}, {0: [[h\"b4ccb8cb788b37b038d327f0da87b32c6abedf4b131c87103637ef2ed0471036\", h\"ee72cde1827222eb587ada0f4dc4a6f4a35a3059d91c8cebdca0d8424126f322db540f985e0e8968e526c5bdd15a63b04afcd7864bd45a038de38ce057bbf009\"], [h\"64982cd6a2a0f49aa8225ab48b3ae3bffab2e19e486f932b303052dc227bc403\", h\"895174677319cfc209030d64b6d50bce1604f6b30b873d5905bb3d33551e01bb7d340a5f3f36e60e6fa42befcb2bec4f0f890e1ec6977b064b21511837e8cc0f\"]]}, spec 21, null]");
        EXPECT_EQ(decode.getArrayElements().size(), 4ul);
    }
}

} // namespace TW::Cardano::VoteDelegationTests
