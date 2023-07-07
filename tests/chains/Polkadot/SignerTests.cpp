// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "Polkadot/Address.h"
#include "Polkadot/Extrinsic.h"
#include "Polkadot/SS58Address.h"
#include "Polkadot/Signer.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "proto/Polkadot.pb.h"
#include "Coin.h"
#include "uint256.h"

#include <TrustWalletCore/TWSS58AddressType.h>
#include <gtest/gtest.h>


namespace TW::Polkadot::tests {
    auto privateKey = PrivateKey(parse_hex("0xabf8e5bdbe30c65656c0a3cbd181ff8a56294a69dfedd27982aace4a76909115"));
    auto privateKeyIOS = PrivateKey(parse_hex("37932b086586a6675e66e562fe68bd3eeea4177d066619c602fe3efc290ada62"));
    auto privateKeyThrow2 = PrivateKey(parse_hex("70a794d4f1019c3ce002f33062f45029c4f930a56b3d20ec477f7668c6bbc37f"));
    auto privateKeyPolkadot = PrivateKey(parse_hex("298fcced2b497ed48367261d8340f647b3fca2d9415d57c2e3c5ef90482a2266"));
    auto addressThrow2 = "14Ztd3KJDaB9xyJtRkREtSZDdhLSbm7UUKt8Z7AwSv7q85G2";
    auto toPublicKey = PublicKey(parse_hex("0x8eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48"), TWPublicKeyTypeED25519);
    auto genesisHash = parse_hex("91b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c3");
    auto controller1 = "14xKzzU1ZYDnzFj7FgdtDAYSMJNARjDc2gNw4XAFDgr4uXgp";

TEST(PolkadotSigner, SignTransfer_9fd062) {
    auto toAddress = Address("13ZLCqJNPsRZYEbwjtZZFpWt9GyFzg5WahXCVWKpWdUJqrQ5");

    auto input = Proto::SigningInput();
    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    auto blockHash = parse_hex("0x5d2143bb808626d63ad7e1cda70fa8697059d670a992e82cd440fbb95ea40351");
    input.set_block_hash(blockHash.data(), blockHash.size());
    input.set_nonce(3);
    input.set_spec_version(26);
    {
        PublicKey publicKey = privateKeyThrow2.getPublicKey(TWPublicKeyTypeED25519);
        Address address = Address(publicKey);
        EXPECT_EQ(address.string(), addressThrow2);
    }
    input.set_private_key(privateKeyThrow2.bytes.data(), privateKeyThrow2.bytes.size());
    input.set_network(ss58Prefix(TWCoinTypePolkadot));
    input.set_transaction_version(5);

    // era: for blockhash and block number, use curl -H "Content-Type: application/json" -H "Accept: text/plain" https://<polkadot-rpc-url>/transaction/material?noMeta=true
    auto era = input.mutable_era();
    era->set_block_number(3541050);
    era->set_period(64);

    auto balanceCall = input.mutable_balance_call();
    auto transfer = balanceCall->mutable_transfer();
    auto value = store(uint256_t(2000000000)); // 0.2
    transfer->set_to_address(toAddress.string());
    transfer->set_value(value.data(), value.size());

    auto extrinsic = Extrinsic(input);
    auto preimage = extrinsic.encodePayload();
    EXPECT_EQ(hex(preimage), "05007120f76076bcb0efdf94c7219e116899d0163ea61cb428183d71324eb33b2bce0300943577a5030c001a0000000500000091b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c35d2143bb808626d63ad7e1cda70fa8697059d670a992e82cd440fbb95ea40351");

    auto output = Signer::sign(input);
    // https://polkadot.subscan.io/extrinsic/0x9fd06208a6023e489147d8d93f0182b0cb7e45a40165247319b87278e08362d8
    EXPECT_EQ(hex(output.encoded()), "3502849dca538b7a925b8ea979cc546464a3c5f81d2398a3a272f6f93bdf4803f2f7830073e59cef381aedf56d7af076bafff9857ffc1e3bd7d1d7484176ff5b58b73f1211a518e1ed1fd2ea201bd31869c0798bba4ffe753998c409d098b65d25dff801a5030c0005007120f76076bcb0efdf94c7219e116899d0163ea61cb428183d71324eb33b2bce0300943577");
}

TEST(PolkadotSigner, SignTransferDOT) {

    auto blockHash = parse_hex("0x343a3f4258fd92f5ca6ca5abdf473d86a78b0bcd0dc09c568ca594245cc8c642");
    auto toAddress = SS58Address(toPublicKey, TWSS58AddressTypePolkadot);

    auto input = Proto::SigningInput();
    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    input.set_block_hash(blockHash.data(), blockHash.size());

    input.set_nonce(0);
    input.set_spec_version(17);
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    input.set_network(ss58Prefix(TWCoinTypePolkadot));
    input.set_transaction_version(3);

    auto& era = *input.mutable_era();
    era.set_block_number(927699);
    era.set_period(8);

    auto balanceCall = input.mutable_balance_call();
    auto& transfer = *balanceCall->mutable_transfer();
    auto value = store(uint256_t(12345));
    transfer.set_to_address(toAddress.string());
    transfer.set_value(value.data(), value.size());

    auto extrinsic = Extrinsic(input);
    auto preimage = extrinsic.encodePayload();
    auto output = Signer::sign(input);

    ASSERT_EQ(hex(preimage), "05008eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48e5c032000000110000000300000091b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c3343a3f4258fd92f5ca6ca5abdf473d86a78b0bcd0dc09c568ca594245cc8c642");
    ASSERT_EQ(hex(output.encoded()), "29028488dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee003d91a06263956d8ce3ce5c55455baefff299d9cb2bb3f76866b6828ee4083770b6c03b05d7b6eb510ac78d047002c1fe5c6ee4b37c9c5a8b09ea07677f12e50d3200000005008eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48e5c0");
}

TEST(PolkadotSigner, SignTransfer_72dd5b) {

    auto blockHash = parse_hex("7d5fa17b70251d0806f26156b1b698dfd09e040642fa092595ce0a78e9e84fcd");

    auto input = Proto::SigningInput();
    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    input.set_block_hash(blockHash.data(), blockHash.size());

    input.set_nonce(1);
    input.set_spec_version(28);
    input.set_private_key(privateKeyIOS.bytes.data(), privateKeyIOS.bytes.size());
    input.set_network(ss58Prefix(TWCoinTypePolkadot));
    input.set_transaction_version(6);

    auto& era = *input.mutable_era();
    era.set_block_number(3910736);
    era.set_period(64);

    auto balanceCall = input.mutable_balance_call();
    auto& transfer = *balanceCall->mutable_transfer();
    auto value = store(uint256_t(10000000000));
    transfer.set_to_address("13ZLCqJNPsRZYEbwjtZZFpWt9GyFzg5WahXCVWKpWdUJqrQ5");
    transfer.set_value(value.data(), value.size());

    auto extrinsic = Extrinsic(input);
    auto preimage = extrinsic.encodePayload();
    auto output = Signer::sign(input);

    ASSERT_EQ(hex(preimage), "0500007120f76076bcb0efdf94c7219e116899d0163ea61cb428183d71324eb33b2bce0700e40b5402050104001c0000000600000091b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c37d5fa17b70251d0806f26156b1b698dfd09e040642fa092595ce0a78e9e84fcd");
    ASSERT_EQ(hex(output.encoded()), "410284008d96660f14babe708b5e61853c9f5929bc90dd9874485bf4d6dc32d3e6f22eaa0038ec4973ab9773dfcbf170b8d27d36d89b85c3145e038d68914de83cf1f7aca24af64c55ec51ba9f45c5a4d74a9917dee380e9171108921c3e5546e05be15206050104000500007120f76076bcb0efdf94c7219e116899d0163ea61cb428183d71324eb33b2bce0700e40b5402");
}

TEST(PolkadotSigner, SignBond_8da66d) {
    auto input = Proto::SigningInput();
    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    auto blockHash = parse_hex("0xf1eee612825f29abd3299b486e401299df2faa55b7ce1e34bf2243bd591905fc");
    input.set_block_hash(blockHash.data(), blockHash.size());
    input.set_nonce(0);
    input.set_spec_version(26);
    {
        PublicKey publicKey = privateKeyThrow2.getPublicKey(TWPublicKeyTypeED25519);
        Address address = Address(publicKey);
        EXPECT_EQ(address.string(), addressThrow2);
    }
    input.set_private_key(privateKeyThrow2.bytes.data(), privateKeyThrow2.bytes.size());
    input.set_network(ss58Prefix(TWCoinTypePolkadot));
    input.set_transaction_version(5);

    // era: for blockhash and block number, use curl -H "Content-Type: application/json" -H "Accept: text/plain" https://<polkadot-rpc-url>/transaction/material?noMeta=true
    auto era = input.mutable_era();
    era->set_block_number(3540912);
    era->set_period(64);

    auto stakingCall = input.mutable_staking_call();
    auto bond = stakingCall->mutable_bond();
    auto value = store(uint256_t(11000000000)); // 1.1
    bond->set_controller(addressThrow2);        // myself
    bond->set_value(value.data(), value.size());
    bond->set_reward_destination(Proto::RewardDestination::STASH);

    auto output = Signer::sign(input);
    // https://polkadot.subscan.io/extrinsic/0x8da66d3fe0f592cff714ec107289370365117a1abdb72a19ac91181fdcf62bba
    ASSERT_EQ(hex(output.encoded()), "3d02849dca538b7a925b8ea979cc546464a3c5f81d2398a3a272f6f93bdf4803f2f783009025843bc49c1c4fbc99dbbd290c92f9879665d55b02f110abfb4800f0e7630877d2cffd853deae7466c22fbc8616a609e1b92615bb365ea8adccba5ef7624050503000007009dca538b7a925b8ea979cc546464a3c5f81d2398a3a272f6f93bdf4803f2f7830700aea68f0201");
}

TEST(PolkadotSigner, SignBondAndNominate_4955314_2) {

    auto key = parse_hex("7f44b19b391a8015ca4c7d94097b3695867a448d1391e7f3243f06987bdb6858");
    auto input = Proto::SigningInput();
    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    input.set_block_hash(genesisHash.data(), genesisHash.size());
    input.set_nonce(4);
    input.set_spec_version(30);
    input.set_private_key(key.data(), key.size());
    input.set_network(ss58Prefix(TWCoinTypePolkadot));
    input.set_transaction_version(7);

    auto stakingCall = input.mutable_staking_call();
    auto bondnom = stakingCall->mutable_bond_and_nominate();
    auto value = store(uint256_t(10000000000)); // 1 DOT
    bondnom->set_controller("13ZLCqJNPsRZYEbwjtZZFpWt9GyFzg5WahXCVWKpWdUJqrQ5");
    bondnom->set_value(value.data(), value.size());
    bondnom->set_reward_destination(Proto::RewardDestination::STASH);
    bondnom->add_nominators("1zugcavYA9yCuYwiEYeMHNJm9gXznYjNfXQjZsZukF1Mpow");
    bondnom->add_nominators("15oKi7HoBQbwwdQc47k71q4sJJWnu5opn1pqoGx4NAEYZSHs");

    auto output = Signer::sign(input);
    // https://polkadot.subscan.io/extrinsic/4955314-2
    ASSERT_EQ(hex(output.encoded()), "6103840036092fac541e0e5feda19e537c679b487566d7101141c203ac8322c27e5f076a00a8b1f859d788f11a958e98b731358f89cf3fdd41a667ea992522e8d4f46915f4c03a1896f2ac54bdc5f16e2ce8a2a3bf233d02aad8192332afd2113ed6688e0d0010001a02080700007120f76076bcb0efdf94c7219e116899d0163ea61cb428183d71324eb33b2bce0700e40b540201070508002c2a55b5ffdca266bd0207df97565b03255f70783ca1a349be5ed9f44589c36000d44533a4d21fd9d6f5d57c8cd05c61a6f23f9131cec8ae386b6b437db399ec3d");
}

TEST(PolkadotSigner, SignNominate_452522) {
    auto input = Proto::SigningInput();
    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    auto blockHash = parse_hex("0x211787d016e39007ac054547737a10542620013e73648b3134541d536cb44e2c");
    input.set_block_hash(blockHash.data(), blockHash.size());
    input.set_nonce(1);
    input.set_spec_version(26);
    input.set_private_key(privateKeyThrow2.bytes.data(), privateKeyThrow2.bytes.size());
    input.set_network(ss58Prefix(TWCoinTypePolkadot));
    input.set_transaction_version(5);

    // era: for blockhash and block number, use curl -H "Content-Type: application/json" -H "Accept: text/plain" https://<polkadot-rpc-url>/transaction/material?noMeta=true
    auto era = input.mutable_era();
    era->set_block_number(3540945);
    era->set_period(64);

    auto stakingCall = input.mutable_staking_call();
    auto nominate = stakingCall->mutable_nominate();

    nominate->add_nominators(controller1);
    nominate->add_nominators("1REAJ1k691g5Eqqg9gL7vvZCBG7FCCZ8zgQkZWd4va5ESih");

    auto output = Signer::sign(input);
    // https://polkadot.subscan.io/extrinsic/0x4525224b7d8f3e58de3a54a9fbfd071401c2b737f314c972a2bb087a0ff508a6
    ASSERT_EQ(hex(output.encoded()), "a502849dca538b7a925b8ea979cc546464a3c5f81d2398a3a272f6f93bdf4803f2f78300d73ff0dc456704743f70173a56e6c13e88a6e1dddb38a23552a066e44fb64e2c9d8a5e9a76afb9489b8540365f668bddd34b7d9c8dbdc4600e6316080e55a30315010400070508aee72821ca00e62304e4f0d858122a65b87c8df4f0eae224ae064b951d39f610127a30e486492921e58f2564b36ab1ca21ff630672f0e76920edd601f8f2b89a");
}

TEST(PolkadotSigner, SignNominate2) {
    auto blockHash = parse_hex("d22a6b2e3e61325050718bd04a14da9efca1f41c9f0a525c375d36106e25af68");
    auto input = Proto::SigningInput();

    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    input.set_block_hash(blockHash.data(), blockHash.size());
    input.set_nonce(0);
    input.set_spec_version(17);
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    input.set_network(ss58Prefix(TWCoinTypePolkadot));
    input.set_transaction_version(3);

    auto stakingCall = input.mutable_staking_call();
    auto& nominate = *stakingCall->mutable_nominate();
    // payload size larger than 256, will be hashed
    nominate.add_nominators("1zugcabYjgfQdMLC3cAzQ8tJZMo45tMnGpivpAzpxB4CZyK");
    nominate.add_nominators("1REAJ1k691g5Eqqg9gL7vvZCBG7FCCZ8zgQkZWd4va5ESih");
    nominate.add_nominators("1WG3jyNqniQMRZGQUc7QD2kVLT8hkRPGMSqAb5XYQM1UDxN");
    nominate.add_nominators("16QFrtU6kDdBjxY8qEKz5EEfuDkHxqG8pix3wSGKQzRcuWHo");
    nominate.add_nominators("14ShUZUYUR35RBZW6uVVt1zXDxmSQddkeDdXf1JkMA6P721N");
    nominate.add_nominators("15MUBwP6dyVw5CXF9PjSSv7SdXQuDSwjX86v1kBodCSWVR7c");

    auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.encoded()), "a1048488dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee00135bbc68b67fffadaf7e98b6402c4fc60382765f543225083a024b0e0ff8071d4ec4ddd67a65828113cc76f3208765608be010d2fcfdcd47e8fe342872704c000000000705182c2a55b5a116a4c88aff57e8f2b70ba72dda72dda4b78630e16ad0ca69006f18127a30e486492921e58f2564b36ab1ca21ff630672f0e76920edd601f8f2b89a1650c532ed1a8641e8922aa24ade0ff411d03edd9ed1c6b7fe42f1a801cee37ceee9d5d071a418b51c02b456d5f5cefd6231041ad59b0e8379c59c11ba4a2439984e16482c99cfad1436111e321a86d87d0fac203bf64538f888e45d793b5413c08d5de7a5d97bea2c7ddf516d0635bddc43f326ae2f80e2595b49d4a08c4619");
}

TEST(PolkadotSigner, SignChill) {
    auto blockHash = parse_hex("1d4a1ecc8b1c37bf0ba5d3e0bf14ec5402fbb035eeaf6d8042c07ca5f8c57429");
    auto input = Proto::SigningInput();

    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    input.set_block_hash(blockHash.data(), blockHash.size());
    input.set_nonce(0);
    input.set_spec_version(17);
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    input.set_network(ss58Prefix(TWCoinTypePolkadot));
    input.set_transaction_version(3);

    auto stakingCall = input.mutable_staking_call();
    auto __attribute__((unused))& chill = *stakingCall->mutable_chill();
    auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.encoded()), "9d018488dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee0088b5e1cd93ba74b82e329f95e1b22660385970182172b2ae280801fdd1ee5652cf7bf319e5e176ccc299dd8eb1e7fccb0ea7717efaf4aacd7640789dd09c1e070000000706");
}

TEST(PolkadotSigner, SignWithdraw) {
    auto blockHash = parse_hex("7b4d1d1e2573eabcc90a3e96058eb0d8d21d7a0b636e8030d152d9179a345dda");
    auto input = Proto::SigningInput();

    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    input.set_block_hash(blockHash.data(), blockHash.size());
    input.set_nonce(0);
    input.set_spec_version(17);
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    input.set_network(ss58Prefix(TWCoinTypePolkadot));
    input.set_transaction_version(3);

    auto stakingCall = input.mutable_staking_call();
    auto& withdraw = *stakingCall->mutable_withdraw_unbonded();
    withdraw.set_slashing_spans(10);

    auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.encoded()), "ad018488dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee002e49bf0dec9bef01dd3bd25419e2147dc983613d0860108f889f9ff2d062c5e3267e309e2dbc35dd2fc2b877b57d86a5f12cbeb8217485be32be3c34d2507d0e00000007030a000000");
}

TEST(PolkadotSigner, SignUnbond_070957) {
    auto input = Proto::SigningInput();

    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    auto blockHash = parse_hex("0x53040c71c6061bd256346b81fcb3545c13b5c34c7cd0c2c25f00aa6e564b16d5");
    input.set_block_hash(blockHash.data(), blockHash.size());
    input.set_nonce(2);
    input.set_spec_version(26);
    input.set_private_key(privateKeyThrow2.bytes.data(), privateKeyThrow2.bytes.size());
    input.set_network(ss58Prefix(TWCoinTypePolkadot));
    input.set_transaction_version(5);

    auto era = input.mutable_era();
    era->set_block_number(3540983);
    era->set_period(64);

    auto stakingCall = input.mutable_staking_call();
    auto unbond = stakingCall->mutable_unbond();
    auto value = store(uint256_t(4000000000));
    unbond->set_value(value.data(), value.size());

    auto output = Signer::sign(input);
    // https://polkadot.subscan.io/extrinsic/0x070957ab697adbe11f7d72a1314d0a81d272a747d2e6880818073317125f980a
    ASSERT_EQ(hex(output.encoded()), "b501849dca538b7a925b8ea979cc546464a3c5f81d2398a3a272f6f93bdf4803f2f783003a762d9dc3f2aba8922c4babf7e6622ca1d74da17ab3f152d8f29b0ffee53c7e5e150915912a9dfd98ef115d272e096543eef9f513207dd606eea97d023a64087503080007020300286bee");
}

TEST(PolkadotSigner, SignChillAndUnbond) {
    auto blockHash = parse_hex("0x35ba668bb19453e8da6334cadcef2a27c8d4141bfc8b49e78e853c3d73e1ecd0");
    auto input = Proto::SigningInput();

    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    input.set_block_hash(blockHash.data(), blockHash.size());
    input.set_nonce(6);
    input.set_spec_version(9200);
    input.set_private_key(privateKeyPolkadot.bytes.data(), privateKeyPolkadot.bytes.size());
    input.set_network(ss58Prefix(TWCoinTypePolkadot));
    input.set_transaction_version(12);

    auto era = input.mutable_era();
    era->set_block_number(10541373);
    era->set_period(64);

    auto stakingCall = input.mutable_staking_call();
    auto chillBond = stakingCall->mutable_chill_and_unbond();
    auto value = store(uint256_t(100500000000)); // 10.05 DOT
    chillBond->set_value(value.data(), value.size());

    auto output = Signer::sign(input);
    // https://polkadot.subscan.io/extrinsic/10541383-2
    ASSERT_EQ(hex(output.encoded()), "d10184008361bd08ddca5fda28b5e2aa84dc2621de566e23e089e555a42194c3eaf2da7900c891ba102db672e378945d74cf7f399226a76b43cab502436971599255451597fc2599902e4b62c7ce85ecc3f653c693fef3232be620984b5bb5bcecbbd7b209d50318001a02080706070207004d446617");
}

TEST(PolkadotSigner, PolymeshEncodeAndSign) {
    // tx on mainnet
    // https://polymesh.subscan.io/extrinsic/0x9a4283cc38f7e769c53ad2d1c5cf292fc85a740ec1c1aa80c180847e51928650

    Polkadot::Proto::SigningInput input;
    input.set_network(12);
    input.set_multi_address(true);
    auto blockHash = parse_hex("898bba6413c38f79a284aec8749f297f6c8734c501f67517b5a6aadc338d1102");
    auto vGenesisHash = parse_hex("6fbd74e5e1d0a61d52ccfe9d4adaed16dd3a7caa37c6bc4d0c2fa12e8b2f4063");
    input.set_block_hash(std::string(blockHash.begin(), blockHash.end()));
    input.set_genesis_hash(std::string(vGenesisHash.begin(), vGenesisHash.end()));
    input.set_nonce(1UL);
    input.set_spec_version(3010u);
    input.set_transaction_version(2u);

    auto* era = input.mutable_era();
    era->set_block_number(4298130UL);
    era->set_period(64UL);

    auto* transfer = input.mutable_balance_call()->mutable_transfer();
    transfer->set_to_address("2FSoQykVV3uWe5ChZuazMDHBoaZmCPPuoYx5KHL5VqXooDQW");
    auto value = store(1000000);
    transfer->set_value(std::string(value.begin(), value.end()));
    transfer->set_memo("MEMO PADDED WITH SPACES");

    auto* callIndices = transfer->mutable_call_indices()->mutable_custom();
    callIndices->set_module_index(0x05);
    callIndices->set_method_index(0x01);

    auto preImage = Signer::signaturePreImage(input);
    ASSERT_EQ(hex(preImage), "050100849e2f6b165d4b28b39ef3d98f86c0520d82bc349536324365c10af08f323f8302093d00014d454d4f2050414444454420574954482053504143455300000000000000000025010400c20b0000020000006fbd74e5e1d0a61d52ccfe9d4adaed16dd3a7caa37c6bc4d0c2fa12e8b2f4063898bba6413c38f79a284aec8749f297f6c8734c501f67517b5a6aadc338d1102");

    auto publicKey = parse_hex("4322cf71da08f9d56181a707af7c0c437dfcb93e6caac9825a5aba57548142ee");
    auto signature = parse_hex("0791ee378775eaff34ef7e529ab742f0d81d281fdf20ace0aa765ca484f5909c4eea0a59c8dbbc534c832704924b424ba3230c38acd0ad5360cef023ca2a420f");
    auto encoded = Signer::encodeTransaction(input, publicKey, signature);
    ASSERT_EQ(hex(encoded), "bd0284004322cf71da08f9d56181a707af7c0c437dfcb93e6caac9825a5aba57548142ee000791ee378775eaff34ef7e529ab742f0d81d281fdf20ace0aa765ca484f5909c4eea0a59c8dbbc534c832704924b424ba3230c38acd0ad5360cef023ca2a420f25010400050100849e2f6b165d4b28b39ef3d98f86c0520d82bc349536324365c10af08f323f8302093d00014d454d4f20504144444544205749544820535041434553000000000000000000");
}

TEST(PolkadotSigner, Statemint_encodeTransaction_transfer) {
    // tx on mainnet
    // https://statemint.subscan.io/extrinsic/2686030-2

    Polkadot::Proto::SigningInput input;
    input.set_network(0);
    input.set_multi_address(true);
    auto blockHash = parse_hex("68d56f15f85d3136970ec16946040bc1752654e906147f7e43e9d539d7c3de2f");
    auto vGenesisHash = parse_hex("68d56f15f85d3136970ec16946040bc1752654e906147f7e43e9d539d7c3de2f");
    input.set_block_hash(std::string(blockHash.begin(), blockHash.end()));
    input.set_genesis_hash(std::string(vGenesisHash.begin(), vGenesisHash.end()));
    input.set_nonce(0UL);
    input.set_spec_version(9320U);
    input.set_transaction_version(9U);

    auto* transfer = input.mutable_balance_call()->mutable_asset_transfer();
    transfer->set_to_address("12q4hq1dgqHZVGzHbwZmqq1cFwatN15Visfd7YmUiMB5ZWkH");

    auto value = store(100000);
    transfer->set_value(std::string(value.begin(), value.end()));
    transfer->set_asset_id(1984);
    transfer->set_fee_asset_id(0x00); // native token

    auto* callIndices = transfer->mutable_call_indices()->mutable_custom();
    callIndices->set_module_index(0x32);
    callIndices->set_method_index(0x05);

    auto publicKey = parse_hex("81f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c91");
    auto signature = parse_hex("e0ae36a5ceaaa7ff53fadfecc8a285a436b15e39c43ea09e8897f34fa3fe55133028eb7d8a9ea2cd42ff1c786e945cd47a02243454ecb39c81acc3409d96f903");
    auto encoded = Signer::encodeTransaction(input, publicKey, signature);
    ASSERT_EQ(hex(encoded), "4102840081f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c9100e0ae36a5ceaaa7ff53fadfecc8a285a436b15e39c43ea09e8897f34fa3fe55133028eb7d8a9ea2cd42ff1c786e945cd47a02243454ecb39c81acc3409d96f903000000003205011f0050e47b3c8aef60bc4fc744d8d979cb0eb2d45fa25c2e9da74e1e5ebd9e117518821a0600");

    // 3d  02840081f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c9100e0ae36a5ceaaa7ff53fadfecc8a285a436b15e39c43ea09e8897f34fa3fe55133028eb7d8a9ea2cd42ff1c786e945cd47a02243454ecb39c81acc3409d96f903000000      3205011f0050e47b3c8aef60bc4fc744d8d979cb0eb2d45fa25c2e9da74e1e5ebd9e117518821a0600
    // 41  02840081f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c9100e0ae36a5ceaaa7ff53fadfecc8a285a436b15e39c43ea09e8897f34fa3fe55133028eb7d8a9ea2cd42ff1c786e945cd47a02243454ecb39c81acc3409d96f903000000  00  3205011f0050e47b3c8aef60bc4fc744d8d979cb0eb2d45fa25c2e9da74e1e5ebd9e117518821a0600
}

TEST(PolkadotSigner, Statemint_encodeTransaction_transfer_keep_alive) {
    // tx on mainnet
    // https://statemint.subscan.io/extrinsic/2686081-2

    Polkadot::Proto::SigningInput input;
    input.set_network(0);
    input.set_multi_address(true);
    auto blockHash = parse_hex("e8f10f9a841dc73578148c763afa17638670c8655542172a80af2e03bf3cbe62");
    auto vGenesisHash = parse_hex("68d56f15f85d3136970ec16946040bc1752654e906147f7e43e9d539d7c3de2f");
    input.set_block_hash(std::string(blockHash.begin(), blockHash.end()));
    input.set_genesis_hash(std::string(vGenesisHash.begin(), vGenesisHash.end()));
    input.set_nonce(2UL);
    input.set_spec_version(9320U);
    input.set_transaction_version(9U);

    auto* era = input.mutable_era();
    era->set_block_number(2686056UL);
    era->set_period(64UL);

    auto* transfer = input.mutable_balance_call()->mutable_asset_transfer();
    transfer->set_to_address("12q4hq1dgqHZVGzHbwZmqq1cFwatN15Visfd7YmUiMB5ZWkH");

    auto value = store(100000);
    transfer->set_value(std::string(value.begin(), value.end()));
    transfer->set_asset_id(1984);
    transfer->set_fee_asset_id(0x00); // native token

    auto* callIndices = transfer->mutable_call_indices()->mutable_custom();
    callIndices->set_module_index(0x32);
    callIndices->set_method_index(0x06);

    auto publicKey = parse_hex("81f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c91");
    auto signature = parse_hex("68c40526bd9e56e340bfc9385ea463afce34e5c49be75b5946974d9ef6a357f90842036cd1b811b60882ae7183aa23545ef5825aafc8aaa6274d71a03414dc0a");
    auto encoded = Signer::encodeTransaction(input, publicKey, signature);
    ASSERT_EQ(hex(encoded), "4502840081f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c910068c40526bd9e56e340bfc9385ea463afce34e5c49be75b5946974d9ef6a357f90842036cd1b811b60882ae7183aa23545ef5825aafc8aaa6274d71a03414dc0a85020800003206011f0050e47b3c8aef60bc4fc744d8d979cb0eb2d45fa25c2e9da74e1e5ebd9e117518821a0600");
}

TEST(PolkadotSigner, Statemint_encodeTransaction_batch_transfer_keep_alive) {
    // tx on mainnet
    // https://statemint.subscan.io/extrinsic/2711054-2

    Polkadot::Proto::SigningInput input;
    input.set_network(0);
    input.set_multi_address(true);
    auto blockHash = parse_hex("c8a2e9492f822f8c07f3717a00e36f68a3090a878b07998724ec1f178f4cf514");
    auto vGenesisHash = parse_hex("68d56f15f85d3136970ec16946040bc1752654e906147f7e43e9d539d7c3de2f");
    input.set_block_hash(std::string(blockHash.begin(), blockHash.end()));
    input.set_genesis_hash(std::string(vGenesisHash.begin(), vGenesisHash.end()));
    input.set_nonce(3UL);
    input.set_spec_version(9320U);
    input.set_transaction_version(9U);

    auto* era = input.mutable_era();
    era->set_block_number(2711016UL);
    era->set_period(64UL);

    auto* transfer = input.mutable_balance_call()->mutable_batch_asset_transfer();
    transfer->set_fee_asset_id(0x00);
    auto* batchCallIndices = transfer->mutable_call_indices()->mutable_custom();
    batchCallIndices->set_module_index(0x28);
    batchCallIndices->set_method_index(0x00);

    auto* t1 = transfer->add_transfers();
    t1->set_to_address("12q4hq1dgqHZVGzHbwZmqq1cFwatN15Visfd7YmUiMB5ZWkH");
    auto value = store(100000);
    t1->set_value(std::string(value.begin(), value.end()));
    t1->set_asset_id(1984);
    t1->set_fee_asset_id(0x00); // native token
    auto* t1CallIndices = t1->mutable_call_indices()->mutable_custom();
    t1CallIndices->set_module_index(0x32);
    t1CallIndices->set_method_index(0x06);

    auto* t2 = transfer->add_transfers();
    t2->set_to_address("12q4hq1dgqHZVGzHbwZmqq1cFwatN15Visfd7YmUiMB5ZWkH");
    t2->set_value(std::string(value.begin(), value.end()));
    t2->set_asset_id(1984);
    t2->set_fee_asset_id(0x00); // native token
    auto* t2CallIndices = t2->mutable_call_indices()->mutable_custom();
    t2CallIndices->set_module_index(0x32);
    t2CallIndices->set_method_index(0x06);

    auto publicKey = parse_hex("81f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c91");
    auto signature = parse_hex("e1d541271965858ff2ba1a1296f0b4d28c8cbcaddf0ea06a9866869caeca3d16eff1265591d11b46d66882493079fde9e425cd941f166260135e9d81f7daf60c");
    auto encoded = Signer::encodeTransaction(input, publicKey, signature);
    ASSERT_EQ(hex(encoded), "f502840081f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c9100e1d541271965858ff2ba1a1296f0b4d28c8cbcaddf0ea06a9866869caeca3d16eff1265591d11b46d66882493079fde9e425cd941f166260135e9d81f7daf60c85020c00002800083206011f0050e47b3c8aef60bc4fc744d8d979cb0eb2d45fa25c2e9da74e1e5ebd9e117518821a06003206011f0050e47b3c8aef60bc4fc744d8d979cb0eb2d45fa25c2e9da74e1e5ebd9e117518821a0600");
}

TEST(PolkadotSigner, Statemint_encodeTransaction_dot_transfer_keep_alive) {
    // tx on mainnet
    // https://statemint.subscan.io/extrinsic/2789245-2

    Polkadot::Proto::SigningInput input;
    input.set_network(0);
    input.set_multi_address(true);
    auto blockHash = parse_hex("68d56f15f85d3136970ec16946040bc1752654e906147f7e43e9d539d7c3de2f");
    auto vGenesisHash = parse_hex("68d56f15f85d3136970ec16946040bc1752654e906147f7e43e9d539d7c3de2f");
    input.set_block_hash(std::string(blockHash.begin(), blockHash.end()));
    input.set_genesis_hash(std::string(vGenesisHash.begin(), vGenesisHash.end()));
    input.set_nonce(7UL);
    input.set_spec_version(9320U);
    input.set_transaction_version(9U);

    auto* transfer = input.mutable_balance_call()->mutable_asset_transfer();
    transfer->set_to_address("12q4hq1dgqHZVGzHbwZmqq1cFwatN15Visfd7YmUiMB5ZWkH");

    auto value = store(100000);
    transfer->set_value(std::string(value.begin(), value.end()));
    transfer->set_asset_id(0x00);
    transfer->set_fee_asset_id(0x00); // native token

    auto* callIndices = transfer->mutable_call_indices()->mutable_custom();
    callIndices->set_module_index(0x0a);
    callIndices->set_method_index(0x03);

    auto publicKey = parse_hex("81f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c91");
    auto signature = parse_hex("c4f7cb46605986ff6dd1a192736feddd8ae468a10b1b458eadfa855ed6b59ad442a96c18e7109ad594d11ba2fd52920545f8a450234e9b03ee3e8f59a8f06f00");
    auto encoded = Signer::encodeTransaction(input, publicKey, signature);
    ASSERT_EQ(hex(encoded), "3902840081f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c9100c4f7cb46605986ff6dd1a192736feddd8ae468a10b1b458eadfa855ed6b59ad442a96c18e7109ad594d11ba2fd52920545f8a450234e9b03ee3e8f59a8f06f00001c00000a030050e47b3c8aef60bc4fc744d8d979cb0eb2d45fa25c2e9da74e1e5ebd9e117518821a0600");
}

TEST(PolkadotSigner, Statemint_encodeTransaction_usdt_transfer_keep_alive) {
    // tx on mainnet
    // https://statemint.subscan.io/extrinsic/2789377-2

    Polkadot::Proto::SigningInput input;
    input.set_network(0);
    input.set_multi_address(true);
    auto blockHash = parse_hex("68d56f15f85d3136970ec16946040bc1752654e906147f7e43e9d539d7c3de2f");
    auto vGenesisHash = parse_hex("68d56f15f85d3136970ec16946040bc1752654e906147f7e43e9d539d7c3de2f");
    input.set_block_hash(std::string(blockHash.begin(), blockHash.end()));
    input.set_genesis_hash(std::string(vGenesisHash.begin(), vGenesisHash.end()));
    input.set_nonce(8UL);
    input.set_spec_version(9320U);
    input.set_transaction_version(9U);

    auto* transfer = input.mutable_balance_call()->mutable_asset_transfer();
    transfer->set_to_address("12q4hq1dgqHZVGzHbwZmqq1cFwatN15Visfd7YmUiMB5ZWkH");

    auto value = store(100000);
    transfer->set_value(std::string(value.begin(), value.end()));
    transfer->set_asset_id(1984);
    transfer->set_fee_asset_id(1984);

    auto* callIndices = transfer->mutable_call_indices()->mutable_custom();
    callIndices->set_module_index(0x32);
    callIndices->set_method_index(0x06);

    auto publicKey = parse_hex("81f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c91");
    auto signature = parse_hex("d22583408806c005a24caf16f2084691f4c6dcb6015e6645adc86fc1474369b0e0b7dbcc0ef25b17eae43844aff6fb42a0b279a19e822c76043cac015be5e40a");
    auto encoded = Signer::encodeTransaction(input, publicKey, signature);
    ASSERT_EQ(hex(encoded), "5102840081f5dd1432e5dd60aa71819e1141ad5e54d6f4277d7d128030154114444b8c9100d22583408806c005a24caf16f2084691f4c6dcb6015e6645adc86fc1474369b0e0b7dbcc0ef25b17eae43844aff6fb42a0b279a19e822c76043cac015be5e40a00200001c00700003206011f0050e47b3c8aef60bc4fc744d8d979cb0eb2d45fa25c2e9da74e1e5ebd9e117518821a0600");
}

TEST(PolkadotSigner, encodeTransaction_Add_authorization) {
    // tx on mainnet
    // https://polymesh.subscan.io/extrinsic/0x7d9b9109027b36b72d37ba0648cb70e5254524d3d6752cc6b41601f4bdfb1af0

    Polkadot::Proto::SigningInput input;
    input.set_network(12);
    input.set_multi_address(true);
    auto blockHash = parse_hex("ce0c2109db498e45abf8fd447580dcfa7b7a07ffc2bfb1a0fbdd1af3e8816d2b");
    auto vGenesisHash = parse_hex("6fbd74e5e1d0a61d52ccfe9d4adaed16dd3a7caa37c6bc4d0c2fa12e8b2f4063");
    input.set_block_hash(std::string(blockHash.begin(), blockHash.end()));
    input.set_genesis_hash(std::string(vGenesisHash.begin(), vGenesisHash.end()));
    input.set_nonce(5UL);
    input.set_spec_version(3010U);
    input.set_transaction_version(2U);

    auto* era = input.mutable_era();
    era->set_block_number(4395451UL);
    era->set_period(64UL);

    auto* addAuthorization = input.mutable_polymesh_call()->mutable_identity_call()->mutable_add_authorization();
    addAuthorization->set_target("2HEVN4PHYKj7B1krQ9bctAQXZxHQQkANVNCcfbdYk2gZ4cBR");
    auto* callIndices = addAuthorization->mutable_call_indices()->mutable_custom();
    callIndices->set_module_index(0x07);
    callIndices->set_method_index(0x0d);

    auto publicKey = parse_hex("4322cf71da08f9d56181a707af7c0c437dfcb93e6caac9825a5aba57548142ee");
    auto signature = parse_hex("81e6561e4391862b5da961d7033baced1c4b25f0e27f938b02321af1118e0b859e1c2bd5607576a258f2c2befbc5f397ea4adb62938f30eb73c8060ab0eabf01");
    auto encoded = Signer::encodeTransaction(input, publicKey, signature);
    ASSERT_EQ(hex(encoded), "490284004322cf71da08f9d56181a707af7c0c437dfcb93e6caac9825a5aba57548142ee0081e6561e4391862b5da961d7033baced1c4b25f0e27f938b02321af1118e0b859e1c2bd5607576a258f2c2befbc5f397ea4adb62938f30eb73c8060ab0eabf01b5031400070d01d3b2f1c41b9b4522eb3e23329b81aca6cc0231167ecfa3580c5a71ff6d0610540501000100010000");
}

TEST(PolkadotSigner, encodeTransaction_JoinIdentityAsKey) {
    // tx on mainnet
    // https://polymesh.subscan.io/extrinsic/0x9d7297d8b38af5668861996cb115f321ed681989e87024fda64eae748c2dc542

    Polkadot::Proto::SigningInput input;
    input.set_network(12);
    input.set_multi_address(true);
    auto blockHash = parse_hex("45c80153c47f5d16acc7a66d473870e8d4574437a7d8c813f47da74cae3812c2");
    auto vGenesisHash = parse_hex("6fbd74e5e1d0a61d52ccfe9d4adaed16dd3a7caa37c6bc4d0c2fa12e8b2f4063");
    input.set_block_hash(std::string(blockHash.begin(), blockHash.end()));
    input.set_genesis_hash(std::string(vGenesisHash.begin(), vGenesisHash.end()));
    input.set_nonce(0UL);
    input.set_spec_version(3010U);
    input.set_transaction_version(2U);

    auto* era = input.mutable_era();
    era->set_block_number(4395527UL);
    era->set_period(64UL);

    auto* key = input.mutable_polymesh_call()->mutable_identity_call()->mutable_join_identity_as_key();
    key->set_auth_id(21435);
    auto* callIndices = key->mutable_call_indices()->mutable_custom();
    callIndices->set_module_index(0x07);
    callIndices->set_method_index(0x05);

    auto publicKey = parse_hex("d3b2f1c41b9b4522eb3e23329b81aca6cc0231167ecfa3580c5a71ff6d061054");
    auto signature = parse_hex("7f5adbb2749e2f0ace29b409c41dd717681495b1f22dc5358311646a9fb8af8a173fc47f1b19748fb56831c2128773e2976986685adee83c741ab49934d80006");
    auto encoded = Signer::encodeTransaction(input, publicKey, signature);
    ASSERT_EQ(hex(encoded), "c5018400d3b2f1c41b9b4522eb3e23329b81aca6cc0231167ecfa3580c5a71ff6d061054007f5adbb2749e2f0ace29b409c41dd717681495b1f22dc5358311646a9fb8af8a173fc47f1b19748fb56831c2128773e2976986685adee83c741ab49934d80006750000000705bb53000000000000");
}

TEST(PolkadotSigner, Kusama_SignBond_NoController) {
    // tx on mainnet
    // https://kusama.subscan.io/extrinsic/0x4e52e59b63910cbdb8c5430c2d100908934f473363c8994cddfd6d1501b017f5

    Polkadot::Proto::SigningInput input;
    input.set_network(ss58Prefix(TWCoinTypeKusama));
    auto blockHash = parse_hex("beb02a3ee782f4bd60ffcfc3de473e3c5a00b2cf124dd302c559b0e77b4331eb");
    auto vGenesisHash = parse_hex("b0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe");
    input.set_block_hash(std::string(blockHash.begin(), blockHash.end()));
    input.set_genesis_hash(std::string(vGenesisHash.begin(), vGenesisHash.end()));
    input.set_nonce(3UL);
    input.set_spec_version(9430U);
    input.set_transaction_version(23U);
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());

    auto* era = input.mutable_era();
    era->set_block_number(18672490UL);
    era->set_period(64UL);

    // Ignore `controller` as it was removed from the `Staking::bond` function at `spec_version = 9430`
    // https://kusama.subscan.io/runtime/Staking?version=9430
    auto* bond = input.mutable_staking_call()->mutable_bond();
    auto value = store(uint256_t(120'000'000'000)); // 0.12
    bond->set_value(value.data(), value.size());
    bond->set_reward_destination(Proto::RewardDestination::CONTROLLER);

    auto output = Signer::sign(input);
    ASSERT_EQ(hex(output.encoded()), "c101840088dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee00bc4d7a166bd1e7e2bfe9b53e81239c9e340d5a326f17c0a3d2768fcc127f20f4f85d888ecb90aa3ed9a0943f8ae8116b9a19747e563c8d8151dfe3b1b5deb40ca5020c0006000700b08ef01b02");
}

} // namespace TW::Polkadot::tests
