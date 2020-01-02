// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Cardano/Signer.h"
#include "Cardano/Address.h"
#include "proto/Cardano.pb.h"

#include "HexCoding.h"
#include "PublicKey.h"
#include "PrivateKey.h"

#include <map>

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Cardano;
using namespace std;


const Data privateKey_b8c3 = parse_hex("b8c31abcc41d931ae881be11da9e4d9242b1f01cae4e69fa29d5ba1f89f9c1549ec844c6b39c70fa6d3a254fe57c1efee1a75eb9755e0b751e96dd288deabc881ae60957699bf72b212ca823520cf7d86af5d1304cd90248fe60bd1fe442870f");

// Helper to fill an utxo
void setUtxo(Proto::UnspentTransaction* utxo, const string& txid, int32_t index, uint64_t amount, const string& address) {
    Data txidData = parse_hex(txid);
    utxo->mutable_out_point()->set_txid(txidData.data(), txidData.size());
    utxo->mutable_out_point()->set_index(index);
    utxo->set_amount(amount);
    utxo->set_address(address);
}

// Helper to copy an utxo
void copyUtxo(Proto::UnspentTransaction* copy, const Proto::UnspentTransaction& orig) {
    copy->mutable_out_point()->set_txid(orig.out_point().txid());
    copy->mutable_out_point()->set_index(orig.out_point().index());
    copy->set_amount(orig.amount());
    copy->set_address(orig.address());
}

TEST(CardanoSigner, SignMessage) {
    // from cardano-crypto.js test
    auto privateKey = PrivateKey(
        parse_hex("d809b1b4b4c74734037f76aace501730a3fe2fca30b5102df99ad3f7c0103e48"),
        parse_hex("d54cde47e9041b31f3e6873d700d83f7a937bea746dadfa2c5b0a6a92502356c"),
        parse_hex("69272d81c376382b8a87c21370a7ae9618df8da708d1a9490939ec54ebe43000")
    );
    Data message = TW::data("Hello world");
    Data signature = privateKey.sign(message, TWCurveED25519Extended);
    EXPECT_EQ(
        "1096ddcfb2ad21a4c0d861ef3fabe18841e8de88105b0d8e36430d7992c588634ead4100c32b2800b31b65e014d54a8238bdda63118d829bf0bcf1b631e86f0e",
        hex(signature)
    );
}

TEST(CardanoSigner, PlanAndSign) {
    // Plan and sign a transaction
    Proto::SigningInput input;
    input.set_amount((uint64_t)(1.0 * 1000000));
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    setUtxo(input.add_utxo(), "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14", 6, (uint64_t)(15.0 * 1000000), "Ae2tdPwUPEZ6SqAETdiJgPYHpAey2MWakEVRDESWYzBePi7u5uAL5ah26qx");
    input.add_private_key(privateKey_b8c3.data(), privateKey_b8c3.size());
    
    Proto::TransactionPlan plan = Signer::planTransaction(input);
    EXPECT_EQ("", plan.error());
    EXPECT_EQ(15000000, plan.available_amount());
    EXPECT_EQ(1000000, plan.amount());
    EXPECT_EQ(167994, plan.fee());
    EXPECT_EQ(13832006, plan.change());
    Proto::SigningOutput output = Signer::sign(input, plan);

    EXPECT_EQ(
        "82839f8200d818582482582059991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac1406ff9f8282d818582183581c4cf4eba3500c1127ab6ce9e011167bc817d6bc5634f2a1e5f6752c3ea0001a6d87265f1a000f42408282d818582183581ceba562709cf7b5e88fe9d9bdcec4f01382aded9b03d31f16da0efdb0a0001acb24046d1a00d30f46ffa0818200d8185885825840835610e371e632d829eb63bf44ec39d0487c19d4d98b8dce9dd88d88414ce5bb1ae60957699bf72b212ca823520cf7d86af5d1304cd90248fe60bd1fe442870f58403646fec783197bdf3d83858d8303f9d72e09a5df882202716c7737e55148c1c2106c550890c0ab001f7a27cb75fabe827f81132486bec41e17861aba1a069508",
        hex(output.encoded())
    );
    EXPECT_EQ("4374315285f5af17c5fd37d78bcaa94534968e925c06e293e10d2348d9420fac", output.transaction_id());
    EXPECT_EQ("", output.error());
}

TEST(CardanoSigner, PlanAndSign_524a) {
    // real-world transaction with default fee
    // see this tx: https://cardanoexplorer.com/tx/524adf0cb4273108ddc1ff24508bb195ba3e03280577438a8a651b8c9a8e0c7f
    Proto::SigningInput input;
    input.set_amount((uint64_t)(1.000000 * 1000000));
    input.set_to_address("Ae2tdPwUPEZ47c19RqdjJKkydcKNkGCzpNTNnMkWxr1nJQm3iF3a2vf9BZQ");
    input.set_change_address("Ae2tdPwUPEZJzHcf5jZRQ4jz2kVT2aM8rATQXqo5gbMfWQpjK6SvTzLTAxn");
    setUtxo(input.add_utxo(), "ebf58670ee1512d597876fe632e92790bd70568374bdbe5a69c5d8ed107607ee", 0, (uint64_t)(8.0 * 1000000), "Ae2tdPwUPEYxKHuuYNsYkpR64XNZz7Xm8vMep7mZ2rrP9HgqbbeX1uMxmGd");
    setUtxo(input.add_utxo(), "ebf58670ee1512d597876fe632e92790bd70568374bdbe5a69c5d8ed107607ee", 1, (uint64_t)(1.830116 * 1000000), "Ae2tdPwUPEYz548sTWdiTxBx13kxECHH4cmYtxQgPgEaQwmkymYFZzGkPrH");
    setUtxo(input.add_utxo(), "a49bad3f69bbab0e4d3e51991ce7a1116c0fd322a7731246b92df455e67e6861", 1, (uint64_t)(3.660232 * 1000000), "Ae2tdPwUPEZJzHcf5jZRQ4jz2kVT2aM8rATQXqo5gbMfWQpjK6SvTzLTAxn");
    setUtxo(input.add_utxo(), "d498c692e3101a39d19da9c7a7beccd65c7d1ea6d23008806ac8d46e81e4918f", 0, (uint64_t)(1.0 * 1000000), "Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    // Ae2tdPwUPEYxKHuuYNsYkpR64XNZz7Xm8vMep7mZ2rrP9HgqbbeX1uMxmGd
    Data prikey_7_f89a = parse_hex("f89a0283d06d64b5deb5216b3be1c4029796294b42dfd81faf2ac0118ff9c154d157299b76b8fd328f96c60ca51f0641e23e4085cf3f21529ae7353b2bb96c89acc6e00ab3b78d299ccaf4ee25e0ef0fd2cedbaef731ad46fd25c9cd31abe205");
    input.add_private_key(prikey_7_f89a.data(), prikey_7_f89a.size());

    Proto::TransactionPlan plan = Signer::planTransaction(input);
    EXPECT_EQ("", plan.error());
    EXPECT_EQ(14490348, plan.available_amount());
    EXPECT_EQ(1000000, plan.amount());
    EXPECT_EQ(167994, plan.fee());
    EXPECT_EQ(6832006, plan.change());
    ASSERT_EQ(1, plan.utxo_size());
    EXPECT_EQ("ebf58670ee1512d597876fe632e92790bd70568374bdbe5a69c5d8ed107607ee", hex(plan.utxo(0).out_point().txid()));
    EXPECT_EQ(0, plan.utxo(0).out_point().index());
    Proto::SigningOutput output = Signer::sign(input, plan);

    EXPECT_EQ(
        "82839f8200d8185824825820ebf58670ee1512d597876fe632e92790bd70568374bdbe5a69c5d8ed107607ee00ff9f8282d818582183581c493cb686130c371b99f9708ffd36f25f7dc314d183cefcd36acd8757a0001a0bd8a93f1a000f42408282d818582183581cde4dbddf5ecf59c3b6945c4f7f59b693be9b8d0afa7984f44814392ca0001a83f8a5c71a00683f86ffa0818200d8185885825840d53677f5a6d08ed0e0ccf4fd4e43d0720da594b573d2f3020728bf2688c3958facc6e00ab3b78d299ccaf4ee25e0ef0fd2cedbaef731ad46fd25c9cd31abe20558408e7e9bb65a26333fdcb831d5d00eabbcfe4849fa7bfae816fe8f217cfcd04e4162da7096735b554e76ccd688442462d8e4bb2701f11cc896b14f8b575093fb04",
        hex(output.encoded())
    );
    EXPECT_EQ("524adf0cb4273108ddc1ff24508bb195ba3e03280577438a8a651b8c9a8e0c7f", output.transaction_id());
    EXPECT_EQ("", output.error());
}

TEST(CardanoSigner, PlanAndSign2Input_51a2) {
    // real-world transaction with 2 inputs, default fee
    // see this tx: https://cardanoexplorer.com/tx/51a28ad4353f18dc00e08dc3c26d39ab044f7ada9f37d0d8a4a56cf5c7ee82ed
    Proto::SigningInput input;
    input.set_amount((uint64_t)(4.0 * 1000000));
    input.set_to_address("Ae2tdPwUPEZCwHtv1qHWHfbM3MNv9GnFQnvmu8HyiFckTuZhxsBuo5gBEc8");
    input.set_change_address("Ae2tdPwUPEZJzHcf5jZRQ4jz2kVT2aM8rATQXqo5gbMfWQpjK6SvTzLTAxn");
    setUtxo(input.add_utxo(), "d498c692e3101a39d19da9c7a7beccd65c7d1ea6d23008806ac8d46e81e4918f", 0, (uint64_t)(1.0 * 1000000), "Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    setUtxo(input.add_utxo(), "a49bad3f69bbab0e4d3e51991ce7a1116c0fd322a7731246b92df455e67e6861", 1, (uint64_t)(3.660232 * 1000000), "Ae2tdPwUPEZJzHcf5jZRQ4jz2kVT2aM8rATQXqo5gbMfWQpjK6SvTzLTAxn");
    // Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp
    Data prikey_1_50bb = parse_hex("50bbfe7e5c246ad70936729b9fd93dba8842a72dda129e9ef629ac298df9c1544c43dc269b069abacdeaa422d1735047cd769d0d312f304136eaee7c6bf1b13cc19fcabee149608328cc0d8e72c45ded708bd1166f04a27c5cc327d4b3c27552");
    // Ae2tdPwUPEZJzHcf5jZRQ4jz2kVT2aM8rATQXqo5gbMfWQpjK6SvTzLTAxn
    Data prikey_8_0803 = parse_hex("0803690ed2f9058a218cb3965c1005417bbf09696a55b91953b3d4f78cf9c1547e5c062d5ca85f02318e8bd8dfbdcb7ff983598fa30394495cb01d586e0112bfec2719f8458fb56cff4ae538faaf049c1cbef916db5f49b912ebc478fea0c3de");
    input.add_private_key(prikey_1_50bb.data(), prikey_1_50bb.size());
    input.add_private_key(prikey_8_0803.data(), prikey_8_0803.size());

    Proto::TransactionPlan plan = Signer::planTransaction(input);
    EXPECT_EQ("", plan.error());
    EXPECT_EQ(4660232, plan.available_amount());
    EXPECT_EQ(4000000, plan.amount());
    EXPECT_EQ(175948, plan.fee());
    EXPECT_EQ(484284, plan.change());
    EXPECT_EQ(2, plan.utxo_size());
    Proto::SigningOutput output = Signer::sign(input, plan);

    EXPECT_EQ(
        "82839f8200d8185824825820d498c692e3101a39d19da9c7a7beccd65c7d1ea6d23008806ac8d46e81e4918f008200d8185824825820a49bad3f69bbab0e4d3e51991ce7a1116c0fd322a7731246b92df455e67e686101ff9f8282d818582183581ca1a75ea8751857bcf34a6f2e7794287d69d224c2339edb8ff68c8aaba0001a31c747c91a003d09008282d818582183581cde4dbddf5ecf59c3b6945c4f7f59b693be9b8d0afa7984f44814392ca0001a83f8a5c71a000763bcffa0828200d818588582584002256a7240b541a4f5eb3d8a6c65f85f04cf98412140f44ca7a885b8157cc181c19fcabee149608328cc0d8e72c45ded708bd1166f04a27c5cc327d4b3c27552584050c5e0e484903f99696da4fb9f5cc2b3bb461bc0075a70a074bc9cf2c367d0277519223f66f6afb549f56a32ed34dadc3c0e8fae1b429e7d42049072e836ca028200d8185885825840c9e3db490cc52207bc9e12bb2d65308ebd6b45a0e36a03c8020c38e740ffbb15ec2719f8458fb56cff4ae538faaf049c1cbef916db5f49b912ebc478fea0c3de584091164f698d7d21cd1903e9358348cbbf7188c86eb7f4f0f76e5130ab50678e9f54af7b442867c0f84e76021100c8efa1978600fbaab64402b85bcea1371dfa0c",
        hex(output.encoded())
    );
    EXPECT_EQ("51a28ad4353f18dc00e08dc3c26d39ab044f7ada9f37d0d8a4a56cf5c7ee82ed", output.transaction_id());
    EXPECT_EQ("", output.error());
}

TEST(CardanoSigner, PlanAndSign5InputMax_13af) {
    // real-world transaction with 5 inputs, max amount, and default fee
    // see this tx: https://cardanoexplorer.com/tx/13af7330116b40edc401a94b9c7563b359c9578473424fbb337c6042b727dc88
    Proto::SigningInput input;
    input.set_amount((uint64_t)(14.14 * 1000000)); // not relevant, use_max
    input.set_to_address("Ae2tdPwUPEZLmY8zETphpoDsrE7pDv3PpufADh5bS41Xqajrgzm3MhJdgmw");
    input.set_change_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    setUtxo(input.add_utxo(), "ebf58670ee1512d597876fe632e92790bd70568374bdbe5a69c5d8ed107607ee", 1, 1830116, "Ae2tdPwUPEYz548sTWdiTxBx13kxECHH4cmYtxQgPgEaQwmkymYFZzGkPrH");
    setUtxo(input.add_utxo(), "524adf0cb4273108ddc1ff24508bb195ba3e03280577438a8a651b8c9a8e0c7f", 0, 1000000, "Ae2tdPwUPEZ47c19RqdjJKkydcKNkGCzpNTNnMkWxr1nJQm3iF3a2vf9BZQ");
    setUtxo(input.add_utxo(), "524adf0cb4273108ddc1ff24508bb195ba3e03280577438a8a651b8c9a8e0c7f", 1, 6832006, "Ae2tdPwUPEZJzHcf5jZRQ4jz2kVT2aM8rATQXqo5gbMfWQpjK6SvTzLTAxn");
    setUtxo(input.add_utxo(), "51a28ad4353f18dc00e08dc3c26d39ab044f7ada9f37d0d8a4a56cf5c7ee82ed", 0, 4000000, "Ae2tdPwUPEZCwHtv1qHWHfbM3MNv9GnFQnvmu8HyiFckTuZhxsBuo5gBEc8");
    setUtxo(input.add_utxo(), "51a28ad4353f18dc00e08dc3c26d39ab044f7ada9f37d0d8a4a56cf5c7ee82ed", 1, 484284, "Ae2tdPwUPEZJzHcf5jZRQ4jz2kVT2aM8rATQXqo5gbMfWQpjK6SvTzLTAxn");
    // Private keys: there are 5 inputs, but from 4 addressess, 4 private keys needed
    // Ae2tdPwUPEZJzHcf5jZRQ4jz2kVT2aM8rATQXqo5gbMfWQpjK6SvTzLTAxn
    Data prikey_8_0803 = parse_hex("0803690ed2f9058a218cb3965c1005417bbf09696a55b91953b3d4f78cf9c1547e5c062d5ca85f02318e8bd8dfbdcb7ff983598fa30394495cb01d586e0112bfec2719f8458fb56cff4ae538faaf049c1cbef916db5f49b912ebc478fea0c3de");
    // Ae2tdPwUPEYz548sTWdiTxBx13kxECHH4cmYtxQgPgEaQwmkymYFZzGkPrH
    Data prikey_9_6bcf = parse_hex("68cfcff79c9caa82cd301c2ef65f48b2e9bfcd54f264f372bf9154ea8ff9c1543e3da9acca7f2cead0e6fbcb8e411652c65735262c36ccf21882664851db34a888ae0d5ad0d49785e48243adc054bcf0b088a6d9bd77269ae478dcbd00330365");
    // Ae2tdPwUPEZ47c19RqdjJKkydcKNkGCzpNTNnMkWxr1nJQm3iF3a2vf9BZQ
    Data prikey_10_a867 = parse_hex("a8674ff71f742708efb718eed3e1ed28cba3e2b1ea902d4d4ff439988bf9c1549d7cee0f8461111d6b62f37cb472b1ae714180843e067c4c2776500b7afdf30767b9f678be9941eeddc2c26abc2103d2fb767caa5e1fee93d3587e6ad013e259");
    // Ae2tdPwUPEZCwHtv1qHWHfbM3MNv9GnFQnvmu8HyiFckTuZhxsBuo5gBEc8
    Data prikey_11_684e = parse_hex("684e95395a752df68d0570b05dee2e9f3fd3b8f2f76df4c78fecf1ff89f9c1544675ba65e82a17e3277516946ede486b5ef07f549353eef23447a001b8f92cb9d42e3d411b458637639a96d08d603e72a46e0f2ad31b92d22cd9a1d29e609079");
    input.add_private_key(prikey_8_0803.data(), prikey_8_0803.size());
    input.add_private_key(prikey_9_6bcf.data(), prikey_9_6bcf.size());
    input.add_private_key(prikey_10_a867.data(), prikey_10_a867.size());
    input.add_private_key(prikey_11_684e.data(), prikey_11_684e.size());
    input.set_use_max_amount(true);

    Proto::TransactionPlan plan = Signer::planTransaction(input);
    EXPECT_EQ("", plan.error());
    EXPECT_EQ(14146406, plan.available_amount());
    EXPECT_EQ(13948748, plan.amount());
    EXPECT_EQ(197658, plan.fee());
    EXPECT_EQ(0, plan.change());
    EXPECT_EQ(5, plan.utxo_size());
    // shuffled UTXO order matters, rearrange them (in a copy plan)
    // actual order used, defined here by utxo amounts (unique in this case):
    vector<uint64_t> orderByAmount = {1830116, 4000000, 6832006, 484284, 1000000};
    map<uint64_t, int> mapAmountToIdx;
    for (int i = 0; i < plan.utxo_size(); ++i) { mapAmountToIdx[plan.utxo(i).amount()] = i; }
    Proto::TransactionPlan planCopy(plan);
    planCopy.clear_utxo();
    for (int i = 0; i < orderByAmount.size(); ++i) {
        copyUtxo(planCopy.add_utxo(), plan.utxo(mapAmountToIdx[orderByAmount[i]]));
    }

    Proto::SigningOutput output = Signer::sign(input, planCopy);

    EXPECT_EQ(
        "82839f8200d8185824825820ebf58670ee1512d597876fe632e92790bd70568374bdbe5a69c5d8ed107607ee018200d818582482582051a28ad4353f18dc00e08dc3c26d39ab044f7ada9f37d0d8a4a56cf5c7ee82ed008200d8185824825820524adf0cb4273108ddc1ff24508bb195ba3e03280577438a8a651b8c9a8e0c7f018200d818582482582051a28ad4353f18dc00e08dc3c26d39ab044f7ada9f37d0d8a4a56cf5c7ee82ed018200d8185824825820524adf0cb4273108ddc1ff24508bb195ba3e03280577438a8a651b8c9a8e0c7f00ff9f8282d818582183581cf0250d7450336449f8eebc0a252a179d50f3ebd32b3ecc8bff82008ba0001ac068235a1a00d4d74cffa0858200d81858858258404a33c2ee5041115dbbe82561fcab1c3b44de40b7d9ed41e46d26b354edc442fb88ae0d5ad0d49785e48243adc054bcf0b088a6d9bd77269ae478dcbd00330365584081c6a6f7cb7fb83f65dd8abd36e5fbb26c9e6adcd89af20532b89162e8cdfe40d37b83884802ee54fc2334fb33ba402e0df08bcbd5db1a02602ecfca7bc8d4008200d8185885825840eb3c2a5ec32964cf9a3cac1b0d96c1df27c6f38dc3ba506550a0db03fa8fa9a1d42e3d411b458637639a96d08d603e72a46e0f2ad31b92d22cd9a1d29e60907958400b4e6b2ecf9ffbc6802f369f808af50146309a50ec618ee72f4816196fc25e2349a437338fdced36ef3a269fa754d5ad9268249ad15eb081d02b95a01af9c70a8200d8185885825840c9e3db490cc52207bc9e12bb2d65308ebd6b45a0e36a03c8020c38e740ffbb15ec2719f8458fb56cff4ae538faaf049c1cbef916db5f49b912ebc478fea0c3de58406ec6e4db32d321b0c80aca46b91cb1b2728156d13e5b60e3f8c2b846a904ab1e9921f7217aa9af2e48b93558bfee4eb001274f676328d3d951bf42c9e0b6cd008200d8185885825840c9e3db490cc52207bc9e12bb2d65308ebd6b45a0e36a03c8020c38e740ffbb15ec2719f8458fb56cff4ae538faaf049c1cbef916db5f49b912ebc478fea0c3de58406ec6e4db32d321b0c80aca46b91cb1b2728156d13e5b60e3f8c2b846a904ab1e9921f7217aa9af2e48b93558bfee4eb001274f676328d3d951bf42c9e0b6cd008200d8185885825840bd9e52f4a2d00a142a1862db5696c5f298a50889e278e756afe43d0af75eb7a067b9f678be9941eeddc2c26abc2103d2fb767caa5e1fee93d3587e6ad013e2595840a2ba13244430eb54fa83ab0be1733882a5b202336c9db48a40459b3c1c2fedb9f3955b265350e27073abd71d0083e60749825db385ea346c9d97a75db539d00d",
        hex(output.encoded())
    );
    EXPECT_EQ("13af7330116b40edc401a94b9c7563b359c9578473424fbb337c6042b727dc88", output.transaction_id());
    EXPECT_EQ("", output.error());
}

TEST(CardanoSigner, PlanTransaction) {
    // plan transaction, estimate fee
    Proto::SigningInput input;
    input.set_amount((uint64_t)(1.0 * 1000000));
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    string txid = "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14";
    setUtxo(input.add_utxo(), "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14", 6, (uint64_t)(15.0 * 1000000), "Ae2tdPwUPEZ6SqAETdiJgPYHpAey2MWakEVRDESWYzBePi7u5uAL5ah26qx");
    input.add_private_key(privateKey_b8c3.data(), privateKey_b8c3.size());
    
    Proto::TransactionPlan plan = Signer::planTransaction(input);

    EXPECT_EQ(15000000, plan.available_amount());
    EXPECT_EQ(1000000, plan.amount());
    EXPECT_EQ(167994, plan.fee());
    EXPECT_EQ(13832006, plan.change());
    ASSERT_EQ(1, plan.utxo_size());
    EXPECT_EQ(txid, hex(plan.utxo(0).out_point().txid()));
    EXPECT_EQ(6, plan.utxo(0).out_point().index());
    EXPECT_EQ(15000000, plan.utxo(0).amount());
    EXPECT_EQ("", plan.error());
}

TEST(CardanoSigner, PlanTransactionUseMax) {
    // plan transaction, with max available
    Proto::SigningInput input;
    input.set_amount(1000000); // not relevsant, use_max
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    string txid = "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14";
    setUtxo(input.add_utxo(), "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14", 6, (uint64_t)(15.0 * 1000000), "Ae2tdPwUPEZ6SqAETdiJgPYHpAey2MWakEVRDESWYzBePi7u5uAL5ah26qx");
    input.add_private_key(privateKey_b8c3.data(), privateKey_b8c3.size());
    input.set_use_max_amount(true);
    
    Proto::TransactionPlan plan = Signer::planTransaction(input);

    EXPECT_EQ(15000000, plan.available_amount());
    EXPECT_EQ(14834159, plan.amount());
    EXPECT_EQ(165841, plan.fee());
    EXPECT_EQ(0, plan.change());
    ASSERT_EQ(1, plan.utxo_size());
    EXPECT_EQ(txid, hex(plan.utxo(0).out_point().txid()));
    EXPECT_EQ(6, plan.utxo(0).out_point().index());
    EXPECT_EQ(15000000, plan.utxo(0).amount());
    EXPECT_EQ("", plan.error());
}

TEST(CardanoSigner, PlanInput3) {
    // plan a transaction, with 3 available inputs, 3 chosen
    Proto::SigningInput input;
    input.set_amount((uint64_t)(25.0 * 1000000));
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    setUtxo(input.add_utxo(), "ab00000000000000000000000000000000000000000000000000000000000001", 1, (uint64_t)(10.1 * 1000000), "Ae2tdPwUPEZ6SqAETdiJgPYHpAey2MWakEVRDESWYzBePi7u5uAL5ah26qx");
    setUtxo(input.add_utxo(), "ab00000000000000000000000000000000000000000000000000000000000002", 2, (uint64_t)(10.2 * 1000000), "Ae2tdPwUPEZ6SqAETdiJgPYHpAey2MWakEVRDESWYzBePi7u5uAL5ah26qx");
    setUtxo(input.add_utxo(), "ab00000000000000000000000000000000000000000000000000000000000003", 3, (uint64_t)(10.3 * 1000000), "Ae2tdPwUPEZ6SqAETdiJgPYHpAey2MWakEVRDESWYzBePi7u5uAL5ah26qx");
    input.add_private_key(privateKey_b8c3.data(), privateKey_b8c3.size());

    Proto::TransactionPlan plan = Signer::planTransaction(input);

    EXPECT_EQ(30600000, plan.available_amount());
    EXPECT_EQ(25000000, plan.amount());
    EXPECT_EQ(183902, plan.fee());
    EXPECT_EQ(5416098, plan.change());
    EXPECT_EQ("", plan.error());
    // 3 utxos selected
    ASSERT_EQ(3, plan.utxo_size());
    // check amounts, sum(utxo)
    uint64_t sumUtxo = 0;
    for(int i = 0; i < 3; ++i) { sumUtxo += plan.utxo(i).amount(); }
    EXPECT_EQ(30600000, sumUtxo);
    EXPECT_EQ(sumUtxo, plan.amount() + plan.fee() + plan.change());
}

TEST(CardanoSigner, PlanInput3of5) {
    // plan a transaction, with 5 available inputs, and 3 chosen.  Which 3 is a matter of shuffling.
    Proto::SigningInput input;
    input.set_amount((uint64_t)(25.0 * 1000000));
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    setUtxo(input.add_utxo(), "ab00000000000000000000000000000000000000000000000000000000000001", 1, (uint64_t)(10.1 * 1000000), "Ae2tdPwUPEZ6SqAETdiJgPYHpAey2MWakEVRDESWYzBePi7u5uAL5ah26qx");
    setUtxo(input.add_utxo(), "ab00000000000000000000000000000000000000000000000000000000000002", 2, (uint64_t)(10.2 * 1000000), "Ae2tdPwUPEZ6SqAETdiJgPYHpAey2MWakEVRDESWYzBePi7u5uAL5ah26qx");
    setUtxo(input.add_utxo(), "ab00000000000000000000000000000000000000000000000000000000000003", 3, (uint64_t)(10.3 * 1000000), "Ae2tdPwUPEZ6SqAETdiJgPYHpAey2MWakEVRDESWYzBePi7u5uAL5ah26qx");
    setUtxo(input.add_utxo(), "ab00000000000000000000000000000000000000000000000000000000000004", 4, (uint64_t)(10.4 * 1000000), "Ae2tdPwUPEZ6SqAETdiJgPYHpAey2MWakEVRDESWYzBePi7u5uAL5ah26qx");
    setUtxo(input.add_utxo(), "ab00000000000000000000000000000000000000000000000000000000000004", 5, (uint64_t)(10.5 * 1000000), "Ae2tdPwUPEZ6SqAETdiJgPYHpAey2MWakEVRDESWYzBePi7u5uAL5ah26qx");
    input.add_private_key(privateKey_b8c3.data(), privateKey_b8c3.size());
    
    Proto::TransactionPlan plan = Signer::planTransaction(input);

    EXPECT_EQ(51500000, plan.available_amount());
    EXPECT_EQ(25000000, plan.amount());
    EXPECT_EQ(183902, plan.fee());
    EXPECT_TRUE(plan.change() >= 5416098 && plan.change() <= 6016098);
    EXPECT_EQ("", plan.error());
    // 3 utxos selected
    ASSERT_EQ(3, plan.utxo_size());
    // check amounts, sum(utxo)
    uint64_t sumUtxo = 0;
    for(int i = 0; i < 3; ++i) { sumUtxo += plan.utxo(i).amount(); }
    EXPECT_TRUE(sumUtxo >= 30600000 && sumUtxo <= 31200000);
    EXPECT_EQ(sumUtxo, plan.amount() + plan.fee() + plan.change());
}

TEST(CardanoSigner, SignTx_d498) {
    // real-world transaction with non-default fee
    // see this tx: https://cardanoexplorer.com/tx/d498c692e3101a39d19da9c7a7beccd65c7d1ea6d23008806ac8d46e81e4918f
    Proto::SigningInput input;
    input.set_amount((uint64_t)(1.0 * 1000000));
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    setUtxo(input.add_utxo(), "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14", 6, (uint64_t)(15.0 * 1000000), "Ae2tdPwUPEZ6SqAETdiJgPYHpAey2MWakEVRDESWYzBePi7u5uAL5ah26qx");
    input.add_private_key(privateKey_b8c3.data(), privateKey_b8c3.size());

    Proto::TransactionPlan plan = Signer::planTransaction(input);
    EXPECT_EQ("", plan.error());
    // set non-default fee, 1000000 + 169884 + 13830116 = 15000000
    EXPECT_EQ(167994, plan.fee());
    EXPECT_EQ(13832006, plan.change());
    EXPECT_EQ(15000000, plan.utxo(0).amount());
    plan.set_fee((uint64_t)(0.169884 * 1000000));
    plan.set_change(13830116);

    Proto::SigningOutput output = Signer::sign(input, plan);

    EXPECT_EQ(
        "82839f8200d818582482582059991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac1406ff9f8282d818582183581c4cf4eba3500c1127ab6ce9e011167bc817d6bc5634f2a1e5f6752c3ea0001a6d87265f1a000f42408282d818582183581ceba562709cf7b5e88fe9d9bdcec4f01382aded9b03d31f16da0efdb0a0001acb24046d1a00d307e4ffa0818200d8185885825840835610e371e632d829eb63bf44ec39d0487c19d4d98b8dce9dd88d88414ce5bb1ae60957699bf72b212ca823520cf7d86af5d1304cd90248fe60bd1fe442870f5840a60e06de3986d77c6f126d98c3107749ec13f8593a1d33e00e3c0c2ac5d2d41c3bcbd52afe2439b8afb816da2e6701fdd01092fa8d305f08b8fd41f249282709",
        hex(output.encoded())
    );
    EXPECT_EQ("d498c692e3101a39d19da9c7a7beccd65c7d1ea6d23008806ac8d46e81e4918f", output.transaction_id());
    EXPECT_EQ("", output.error());
}

TEST(CardanoSigner, PrepareUnsignedTx_d498) {
    // see this tx: https://cardanoexplorer.com/tx/d498c692e3101a39d19da9c7a7beccd65c7d1ea6d23008806ac8d46e81e4918f
    Proto::SigningInput input;
    input.set_amount((uint64_t)(1.0 * 1000000));
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    setUtxo(input.add_utxo(), "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14", 6, (uint64_t)(15.0 * 1000000), "Ae2tdPwUPEZ6SqAETdiJgPYHpAey2MWakEVRDESWYzBePi7u5uAL5ah26qx");

    Proto::TransactionPlan plan = Signer::planTransactionWithFee(input, 169884);
    EXPECT_EQ("", plan.error());
    Data unisgnedEncodedCborData = Signer::prepareUnsignedTx(input, plan);
    EXPECT_EQ(
        "839f8200d818582482582059991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac1406ff9f8282d818582183581c4cf4eba3500c1127ab6ce9e011167bc817d6bc5634f2a1e5f6752c3ea0001a6d87265f1a000f42408282d818582183581ceba562709cf7b5e88fe9d9bdcec4f01382aded9b03d31f16da0efdb0a0001acb24046d1a00d307e4ffa0",
        hex(unisgnedEncodedCborData)
    );

    Data txId = Hash::blake2b(unisgnedEncodedCborData, 32);
    EXPECT_EQ("d498c692e3101a39d19da9c7a7beccd65c7d1ea6d23008806ac8d46e81e4918f", hex(txId));

    PrivateKey fromPriKey = PrivateKey(privateKey_b8c3);
    PublicKey fromPubKey = fromPriKey.getPublicKey(TWPublicKeyTypeED25519Extended);
    EXPECT_EQ(
        "835610e371e632d829eb63bf44ec39d0487c19d4d98b8dce9dd88d88414ce5bb1ae60957699bf72b212ca823520cf7d86af5d1304cd90248fe60bd1fe442870f",
        hex(fromPubKey.bytes)
    );
    Address fromAddr = Address(fromPubKey);
    EXPECT_EQ("Ae2tdPwUPEZ6SqAETdiJgPYHpAey2MWakEVRDESWYzBePi7u5uAL5ah26qx", fromAddr.string());
}

TEST(CardanoSigner, SignNoChange) {
    // if change is 0, do not include change output
    Proto::SigningInput input;
    input.set_amount((uint64_t)(14.832006 * 1000000));
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    setUtxo(input.add_utxo(), "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14", 6, (uint64_t)(15.0 * 1000000), "Ae2tdPwUPEZ6SqAETdiJgPYHpAey2MWakEVRDESWYzBePi7u5uAL5ah26qx");
    input.add_private_key(privateKey_b8c3.data(), privateKey_b8c3.size());
    
    Proto::TransactionPlan plan = Signer::planTransaction(input);
    EXPECT_EQ("", plan.error());
    EXPECT_EQ(167994, plan.fee());
    Proto::SigningOutput output = Signer::sign(input, plan);

    EXPECT_EQ(
        "82839f8200d818582482582059991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac1406ff9f8282d818582183581c4cf4eba3500c1127ab6ce9e011167bc817d6bc5634f2a1e5f6752c3ea0001a6d87265f1a00e25186ffa0818200d8185885825840835610e371e632d829eb63bf44ec39d0487c19d4d98b8dce9dd88d88414ce5bb1ae60957699bf72b212ca823520cf7d86af5d1304cd90248fe60bd1fe442870f5840dfbfef76b1f790bf6f2d5ab4f3aab3086b962d26777c8637224dc039df9a95adc18e6a20d226fb072d1842dca1efe80fbb64907c16e239095f68b85d185f7b02",
        hex(output.encoded())
    );
    EXPECT_EQ("5c6b8768bb5af3c7a055bb79576d44d2ab8c1f483e77c62f0a9a054e189b6877", output.transaction_id());
    EXPECT_EQ("", output.error());
}

TEST(CardanoSigner, PlanNegativeInvalidAddr) {
    Proto::SigningInput input;
    input.set_amount((uint64_t)(1.0 * 1000000));
    input.set_to_address("__INVALID_ADDRESS__");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    setUtxo(input.add_utxo(), "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14", 6, (uint64_t)(15.0 * 1000000), "Ae2tdPwUPEZ6SqAETdiJgPYHpAey2MWakEVRDESWYzBePi7u5uAL5ah26qx");
    input.add_private_key(privateKey_b8c3.data(), privateKey_b8c3.size());
    
    Proto::TransactionPlan plan = Signer::planTransaction(input);
    EXPECT_EQ(0, plan.amount());
    EXPECT_EQ(0, plan.fee());
    EXPECT_NE("", plan.error());
    EXPECT_TRUE(plan.error().find("nvalid address") != string::npos);
}

TEST(CardanoSigner, PlanNegativeInsufficientBalance) {
    Proto::SigningInput input;
    input.set_amount((uint64_t)(1000.0 * 1000000));
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    setUtxo(input.add_utxo(), "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14", 6, (uint64_t)(15.0 * 1000000), "Ae2tdPwUPEZ6SqAETdiJgPYHpAey2MWakEVRDESWYzBePi7u5uAL5ah26qx");
    input.add_private_key(privateKey_b8c3.data(), privateKey_b8c3.size());
    
    Proto::TransactionPlan plan = Signer::planTransaction(input);
    EXPECT_EQ(0, plan.amount());
    EXPECT_EQ(0, plan.fee());
    EXPECT_NE("", plan.error());
    EXPECT_TRUE(plan.error().find("nsufficient balance") != string::npos);
}

TEST(CardanoSigner, PlanNegativeNoPrivKey) {
    Proto::SigningInput input;
    input.set_amount((uint64_t)(1.0 * 1000000));
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    setUtxo(input.add_utxo(), "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14", 6, (uint64_t)(15.0 * 1000000), "Ae2tdPwUPEZ6SqAETdiJgPYHpAey2MWakEVRDESWYzBePi7u5uAL5ah26qx");
    
    Proto::TransactionPlan plan = Signer::planTransaction(input);
    EXPECT_NE("", plan.error());
    EXPECT_TRUE(plan.error().find("rivate key missing") != string::npos);
}

TEST(CardanoSigner, PlanNegativeNoUtxo) {
    Proto::SigningInput input;
    input.set_amount((uint64_t)(1.0 * 1000000));
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    input.add_private_key(privateKey_b8c3.data(), privateKey_b8c3.size());
    
    Proto::TransactionPlan plan = Signer::planTransaction(input);
    EXPECT_NE("", plan.error());
    EXPECT_TRUE(plan.error().find("nsufficient balance") != string::npos);
}

TEST(CardanoSigner, PlanNegativeLowBalance) {
    // balance lower than feee
    Proto::SigningInput input;
    input.set_amount((uint64_t)(0.000001 * 1000000));
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    setUtxo(input.add_utxo(), "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14", 6, (uint64_t)(0.000009 * 1000000), "Ae2tdPwUPEZ6SqAETdiJgPYHpAey2MWakEVRDESWYzBePi7u5uAL5ah26qx");
    input.add_private_key(privateKey_b8c3.data(), privateKey_b8c3.size());
    
    Proto::TransactionPlan plan = Signer::planTransaction(input);
    EXPECT_NE("", plan.error());
    EXPECT_TRUE(plan.error().find("nsufficient balance") != string::npos);
}

TEST(CardanoSigner, SignNegativeEmptyPlan) {
    Proto::SigningInput input;
    input.set_amount((uint64_t)(1.0 * 1000000));
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    setUtxo(input.add_utxo(), "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14", 6, (uint64_t)(15.0 * 1000000), "Ae2tdPwUPEZ6SqAETdiJgPYHpAey2MWakEVRDESWYzBePi7u5uAL5ah26qx");
    input.add_private_key(privateKey_b8c3.data(), privateKey_b8c3.size());
    
    Proto::TransactionPlan emptyPlan;
    Proto::SigningOutput output = Signer::sign(input, emptyPlan);
    EXPECT_NE("", output.error());
    EXPECT_TRUE(output.error().find("ero fee") != string::npos);
}

TEST(CardanoSigner, SignNegativeAmountMismatch) {
    Proto::SigningInput input;
    input.set_amount((uint64_t)(1.0 * 1000000));
    input.set_to_address("Ae2tdPwUPEZ4V8WWZsGRnaszaeFnTs3NKvFP2xRse56EPMDabmJAJgrWibp");
    input.set_change_address("Ae2tdPwUPEZLKW7531GsfhQC1bNSTKTZr4NcAymSgkaDJHZAwoBk75ATZyW");
    setUtxo(input.add_utxo(), "59991b7aa2d09961f979afddcd9571ff1c637a1bc0dab09a7233f078d17dac14", 6, (uint64_t)(15.0 * 1000000), "Ae2tdPwUPEZ6SqAETdiJgPYHpAey2MWakEVRDESWYzBePi7u5uAL5ah26qx");
    input.add_private_key(privateKey_b8c3.data(), privateKey_b8c3.size());
    
    Proto::TransactionPlan plan = Signer::planTransaction(input);
    EXPECT_EQ("", plan.error());
    EXPECT_EQ(167994, plan.fee());
    plan.set_fee(2000000);

    Proto::SigningOutput output = Signer::sign(input, plan);
    EXPECT_NE("", output.error());
    EXPECT_TRUE(output.error().find("mount mismatch") != string::npos);
}
