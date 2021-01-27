// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Polkadot/Signer.h"
#include "Polkadot/Extrinsic.h"
#include "Polkadot/Address.h"
#include "SS58Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "proto/Polkadot.pb.h"
#include "uint256.h"

#include <TrustWalletCore/TWSS58AddressType.h>
#include <gtest/gtest.h>


namespace TW::Polkadot {
    auto privateKey = PrivateKey(parse_hex("0xabf8e5bdbe30c65656c0a3cbd181ff8a56294a69dfedd27982aace4a76909115"));
    auto privateKeyThrow1 = PrivateKey(parse_hex("8da3da12e488d5fab0c28aab8b881e2cb515eba113a5f4017a9192ad491a1b44"));
    auto addressThrow1 = "13M79SsQnBvAGEJPdDrUVStwMYChH2YatmGG2EZ2i6628N6Q";
    auto toPublicKey = PublicKey(parse_hex("0x8eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48"), TWPublicKeyTypeED25519);
    auto genesisHash = parse_hex("91b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c3");
    auto controller1 = "14xKzzU1ZYDnzFj7FgdtDAYSMJNARjDc2gNw4XAFDgr4uXgp";

TEST(PolkadotSigner, SignTransfer_1621) {
    auto toAddress = Address("13ZLCqJNPsRZYEbwjtZZFpWt9GyFzg5WahXCVWKpWdUJqrQ5");

    auto input = Proto::SigningInput();
    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    input.set_block_hash(genesisHash.data(), genesisHash.size()); // no era

    input.set_nonce(0);
    input.set_spec_version(26);
    {
        PublicKey publicKey = privateKeyThrow1.getPublicKey(TWPublicKeyTypeED25519);
        Address address = Address(publicKey);
        EXPECT_EQ(address.string(), addressThrow1);
    }
    input.set_private_key(privateKeyThrow1.bytes.data(), privateKeyThrow1.bytes.size());
    input.set_network(Proto::Network::POLKADOT);
    input.set_transaction_version(5);

    auto balanceCall = input.mutable_balance_call();
    auto transfer = balanceCall->mutable_transfer();
    auto value = store(uint256_t(10000000000));
    transfer->set_to_address(toAddress.string());
    transfer->set_value(value.data(), value.size());

    auto extrinsic = Extrinsic(input);
    auto preimage = extrinsic.encodePayload();
    EXPECT_EQ(hex(preimage), "05007120f76076bcb0efdf94c7219e116899d0163ea61cb428183d71324eb33b2bce0700e40b54020000001a0000000500000091b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c391b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c3");

    auto output = Signer::sign(input);
    // https://polkascan.io/polkadot/transaction/0x1621362639a2af1dfbf70f622df9e5e8079fd4f8fcf3ba01b757096f1338d41c
    EXPECT_EQ(hex(output.encoded()), "35028467ce10209e619e32639dd74a71d9865807839f3a4f09b4fd1cd52fab9496dd4800d25b7278fc621758e8d8588b032ef60127d628e1fbd9a07a9f194b0f33fe62212352fc47237ec54ae4a06458632df1f2ded6c31c22fb37750dd6674f3aecc20300000005007120f76076bcb0efdf94c7219e116899d0163ea61cb428183d71324eb33b2bce0700e40b5402");
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
    input.set_network(Proto::Network::POLKADOT);
    input.set_transaction_version(3);

    auto &era = *input.mutable_era();
    era.set_block_number(927699);
    era.set_period(8);

    auto balanceCall = input.mutable_balance_call();
    auto &transfer = *balanceCall->mutable_transfer();
    auto value = store(uint256_t(12345));
    transfer.set_to_address(toAddress.string());
    transfer.set_value(value.data(), value.size());

    auto extrinsic = Extrinsic(input);
    auto preimage = extrinsic.encodePayload();
    auto output = Signer::sign(input);

    ASSERT_EQ(hex(preimage), "05008eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48e5c032000000110000000300000091b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c3343a3f4258fd92f5ca6ca5abdf473d86a78b0bcd0dc09c568ca594245cc8c642");
    ASSERT_EQ(hex(output.encoded()), "29028488dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee003d91a06263956d8ce3ce5c55455baefff299d9cb2bb3f76866b6828ee4083770b6c03b05d7b6eb510ac78d047002c1fe5c6ee4b37c9c5a8b09ea07677f12e50d3200000005008eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48e5c0");
}

TEST(PolkadotSigner, SignBond_cd61) {
    auto input = Proto::SigningInput();
    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    input.set_block_hash(genesisHash.data(), genesisHash.size());
    input.set_nonce(5);
    input.set_spec_version(26);
    {
        PublicKey publicKey = privateKeyThrow1.getPublicKey(TWPublicKeyTypeED25519);
        Address address = Address(publicKey);
        EXPECT_EQ(address.string(), addressThrow1);
    }
    input.set_private_key(privateKeyThrow1.bytes.data(), privateKeyThrow1.bytes.size());
    input.set_network(Proto::Network::POLKADOT);
    input.set_transaction_version(5);

    auto stakingCall = input.mutable_staking_call();
    auto bond = stakingCall->mutable_bond();
    auto value = store(uint256_t(20000000000));
    bond->set_controller(controller1);
    bond->set_value(value.data(), value.size());
    bond->set_reward_destination(Proto::RewardDestination::STAKED);

    auto output = Signer::sign(input);
    // https://polkadot.subscan.io/extrinsic/0xcd616e4dd52ce13e9864cfe9d2dd044119ace37e8858a0ff922f19f7fb61f9fc
    ASSERT_EQ(hex(output.encoded()), "39028467ce10209e619e32639dd74a71d9865807839f3a4f09b4fd1cd52fab9496dd48008999e4976d5881781442c74514a3246f0ade6f06af625711a8de49d3aaf41fe8edd09ade79b592e774f28c30cfbeba9c1d84595375264ab7764a9e84777d58040014000700aee72821ca00e62304e4f0d858122a65b87c8df4f0eae224ae064b951d39f6100700c817a80400");
}

TEST(PolkadotSigner, SignNominate_7f93) {
    auto input = Proto::SigningInput();
    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    auto blockHash = parse_hex("0x42cf54409c348ca2a27cafefa208823a473ab2c3c41651281698fe039ffd5d22");
    input.set_block_hash(blockHash.data(), blockHash.size());
    input.set_nonce(14);
    input.set_spec_version(26);
    input.set_private_key(privateKeyThrow1.bytes.data(), privateKeyThrow1.bytes.size());
    input.set_network(Proto::Network::POLKADOT);
    input.set_transaction_version(5);

    // era: for blockhash and block number, use curl -H "Content-Type: application/json" -H "Accept: text/plain" https://<polkadot-rpc-url>/transaction/material?noMeta=true
    auto era = input.mutable_era();
    era->set_block_number(3526285);
    era->set_period(64);

    auto stakingCall = input.mutable_staking_call();
    auto nominate = stakingCall->mutable_nominate();

    nominate->add_nominators(controller1);
    nominate->add_nominators("1REAJ1k691g5Eqqg9gL7vvZCBG7FCCZ8zgQkZWd4va5ESih");

    auto output = Signer::sign(input);
    // https://polkadot.subscan.io/extrinsic/0x7f9eb3c48f65710d71bb5f2efdd171346bcfb90c060591e67828468eb3d87c80
    ASSERT_EQ(hex(output.encoded()), "a5028467ce10209e619e32639dd74a71d9865807839f3a4f09b4fd1cd52fab9496dd4800cf2416af2e86aaf61f9c7f58bdd6de109c3aec6c544853c4a49fc03bd05a58ea7d63fd138aa632b058daacf34c11f629d396b5f3e01bb54753e495b63dca1a06d5003800070508aee72821ca00e62304e4f0d858122a65b87c8df4f0eae224ae064b951d39f610127a30e486492921e58f2564b36ab1ca21ff630672f0e76920edd601f8f2b89a");
}

TEST(PolkadotSigner, SignNominate2) {
    auto blockHash = parse_hex("d22a6b2e3e61325050718bd04a14da9efca1f41c9f0a525c375d36106e25af68");
    auto input = Proto::SigningInput();

    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    input.set_block_hash(blockHash.data(), blockHash.size());
    input.set_nonce(0);
    input.set_spec_version(17);
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    input.set_network(Proto::Network::POLKADOT);
    input.set_transaction_version(3);

    auto stakingCall = input.mutable_staking_call();
    auto &nominate = *stakingCall->mutable_nominate();
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
    input.set_network(Proto::Network::POLKADOT);
    input.set_transaction_version(3);

    auto stakingCall = input.mutable_staking_call();
    auto __attribute__((unused)) &chill = *stakingCall->mutable_chill();
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
    input.set_network(Proto::Network::POLKADOT);
    input.set_transaction_version(3);

    auto stakingCall = input.mutable_staking_call();
    auto &withdraw = *stakingCall->mutable_withdraw_unbonded();
    withdraw.set_slashing_spans(10);

    auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.encoded()), "ad018488dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee002e49bf0dec9bef01dd3bd25419e2147dc983613d0860108f889f9ff2d062c5e3267e309e2dbc35dd2fc2b877b57d86a5f12cbeb8217485be32be3c34d2507d0e00000007030a000000");
}

TEST(PolkadotSigner, SignUnbond_dde4) {
    auto input = Proto::SigningInput();

    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    auto blockHash = parse_hex("0x83fb746423fb018f7b40cbc146d0c34d2d882822eb121ea857da8ea4bb36680d");
    input.set_block_hash(blockHash.data(), blockHash.size());
    input.set_nonce(15);
    input.set_spec_version(26);
    input.set_private_key(privateKeyThrow1.bytes.data(), privateKeyThrow1.bytes.size());
    input.set_network(Proto::Network::POLKADOT);
    input.set_transaction_version(5);

    auto era = input.mutable_era();
    era->set_block_number(3526326);
    era->set_period(64);

    auto stakingCall = input.mutable_staking_call();
    auto unbond = stakingCall->mutable_unbond();
    auto value = store(uint256_t(20000000000));
    unbond->set_value(value.data(), value.size());

    auto output = Signer::sign(input);
    // https://polkadot.subscan.io/extrinsic/0xdde496045c304cc18a610c1b677571547c4e4355a4a927cd2fb1343e4e36ea37
    ASSERT_EQ(hex(output.encoded()), "b9018467ce10209e619e32639dd74a71d9865807839f3a4f09b4fd1cd52fab9496dd4800fbe412c54b85f51b29c37a0c063ceeef484412cd22901714c048597d6658065d37092fbf5977c16202b0bd0f0ad5ce19f6123208adbc3a47ac491f773234990f65033c0007020700c817a804");
}

} // namespace
