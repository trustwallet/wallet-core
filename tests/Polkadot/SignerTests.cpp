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
    auto toPublicKey = PublicKey(parse_hex("0x8eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48"), TWPublicKeyTypeED25519);
    auto genesisHash = parse_hex("91b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c3");

TEST(PolkadotSigner, SignTransfer_1621) {
    auto toAddress = Address("13ZLCqJNPsRZYEbwjtZZFpWt9GyFzg5WahXCVWKpWdUJqrQ5");

    auto input = Proto::SigningInput();
    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    input.set_block_hash(genesisHash.data(), genesisHash.size());

    input.set_nonce(0);
    input.set_spec_version(26);
    {
        PublicKey publicKey = privateKeyThrow1.getPublicKey(TWPublicKeyTypeED25519);
        Address address = Address(publicKey);
        EXPECT_EQ(address.string(), "13M79SsQnBvAGEJPdDrUVStwMYChH2YatmGG2EZ2i6628N6Q");
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
    era.set_phase(927699);
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

TEST(PolkadotSigner, SignNominate) {
    auto blockHash = parse_hex("52bc855411f95698cb987dc9af93f719f2e06e87bfa7b75a83d58baafd08bca7");

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

    nominate.add_nominators("1zugcabYjgfQdMLC3cAzQ8tJZMo45tMnGpivpAzpxB4CZyK");
    nominate.add_nominators("1REAJ1k691g5Eqqg9gL7vvZCBG7FCCZ8zgQkZWd4va5ESih");

    auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.encoded()), "a1028488dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee0038c999e3878beb7a927ff63483390f6e237120612678d5cc9c40c27028250d94c9a894cf8befe8444ff4b69a0b64c2220aa7c30f886f54e0589f73257f59ed0f0000000705082c2a55b5a116a4c88aff57e8f2b70ba72dda72dda4b78630e16ad0ca69006f18127a30e486492921e58f2564b36ab1ca21ff630672f0e76920edd601f8f2b89a");
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

TEST(PolkadotSigner, SignUnbond) {
    auto blockHash = parse_hex("84c9509904b5b2b3ff0a35fc7f033db9306e6bf0700c32d44e049578bfd33ba1");
    auto input = Proto::SigningInput();

    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    input.set_block_hash(blockHash.data(), blockHash.size());
    input.set_nonce(0);
    input.set_spec_version(17);
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    input.set_network(Proto::Network::POLKADOT);
    input.set_transaction_version(3);

    auto stakingCall = input.mutable_staking_call();
    auto &unbond = *stakingCall->mutable_unbond();

    auto value = store(uint256_t(123456));
    unbond.set_value(value.data(), value.size());
    auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.encoded()), "ad018488dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee00cababd023dd35e6639b5a55fb35e5863142e15637d2942c7de58ab966a6d81c5794e9a2fa53b1da7c5f9ed272e52f865fb9ccf1ccf5881bbee7af357101bf90c000000070202890700");
}

TEST(PolkadotSigner, SignBond) {
    auto blockHash = parse_hex("94f4552ef9412a85d8728717f09128bb21548a2e5eec2e7e5cefbe72af17aad7");
    auto input = Proto::SigningInput();

    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    input.set_block_hash(blockHash.data(), blockHash.size());
    input.set_nonce(0);
    input.set_spec_version(17);
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    input.set_network(Proto::Network::POLKADOT);
    input.set_transaction_version(3);

    auto stakingCall = input.mutable_staking_call();
    auto &bond = *stakingCall->mutable_bond();

    auto value = store(uint256_t(123456));

    bond.set_controller("1zugcabYjgfQdMLC3cAzQ8tJZMo45tMnGpivpAzpxB4CZyK");
    bond.set_value(value.data(), value.size());
    bond.set_reward_destination(Proto::RewardDestination::STAKED);
    auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.encoded()), "31028488dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee0096fde35692962591e8f41782c5bbe4c3c01022bcfc140a0b11f00502997817263fcaf476477e0db6abbc1d3c9a231aed8ba0ee6d5105db08ac2a3dd68d3c870000000007002c2a55b5a116a4c88aff57e8f2b70ba72dda72dda4b78630e16ad0ca69006f180289070000");
}

} // namespace
