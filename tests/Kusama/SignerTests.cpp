// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Polkadot/Signer.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "proto/Polkadot.pb.h"
#include "uint256.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Polkadot;

TEST(PolkadotSigner, SignTransfer) {
    auto privateKey = PrivateKey(parse_hex("0xabf8e5bdbe30c65656c0a3cbd181ff8a56294a69dfedd27982aace4a76909115"));
    auto genesisHash = parse_hex("0xb0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe");

    auto input = Proto::SigningInput();
    input.set_block_hash(genesisHash.data(), genesisHash.size());
    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    input.set_nonce(0);
    input.set_spec_version(1031);
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    input.set_network(Proto::Network::KUSAMA);
    input.set_extrinsic_version(4);

    auto balanceCall = input.mutable_balance_call();
    auto &transfer = *balanceCall->mutable_transfer();
    auto value = store(uint256_t(12345));
    transfer.set_to_address("FoQJpPyadYccjavVdTWxpxU7rUEaYhfLCPwXgkfD6Zat9QP");
    transfer.set_value(value.data(), value.size());

    auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.encoded()), "2d0284ff88dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee0034a113577b56545c45e18969471eebe11ed434f3b2f06e2e3dc8dc137ba804caf60757787ebdeb298327e2f29d68c5520965405ef5582db0445c06e1c11a8a0e0000000400ff8eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48e5c0");
}

TEST(PolkadotSigner, SignTransferEra) {
    auto privateKey = PrivateKey(parse_hex("0xabf8e5bdbe30c65656c0a3cbd181ff8a56294a69dfedd27982aace4a76909115"));
    auto blockHash = parse_hex("0xee17a6af2ea3f8c230e184e6821f7dfd56b60b9ca46b06e713acd9c8c834a5ee");
    auto genesisHash = parse_hex("0xb0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe");

    auto input = Proto::SigningInput();
    input.set_block_hash(blockHash.data(), blockHash.size());
    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    input.set_nonce(0);
    input.set_spec_version(1031);
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    input.set_network(Proto::Network::KUSAMA);
    input.set_extrinsic_version(4);

    auto &era = *input.mutable_era();
    era.set_phase(429119);
    era.set_period(8);

    auto balanceCall = input.mutable_balance_call();
    auto &transfer = *balanceCall->mutable_transfer();
    auto value = store(uint256_t(12345));
    transfer.set_to_address("FoQJpPyadYccjavVdTWxpxU7rUEaYhfLCPwXgkfD6Zat9QP");
    transfer.set_value(value.data(), value.size());

    auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.encoded()), "310284ff88dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee004898684cbde8f04384e381636326f5a7334dfa9617e66771eedb2b8cb1d3b9415531305ad70e4e38accd06ef0fbe5963d928b59979f1807500170981af335306720000000400ff8eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48e5c0");
}

TEST(PolkadotSigner, SignNominate) {
    auto privateKey = PrivateKey(parse_hex("0xabf8e5bdbe30c65656c0a3cbd181ff8a56294a69dfedd27982aace4a76909115"));
    auto genesisHash = parse_hex("0xb0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe");

    auto input = Proto::SigningInput();
    input.set_block_hash(genesisHash.data(), genesisHash.size());
    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    input.set_nonce(0);
    input.set_spec_version(1031);
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    input.set_network(Proto::Network::KUSAMA);
    input.set_extrinsic_version(4);

    auto stakingCall = input.mutable_staking_call();
    auto &nominate = *stakingCall->mutable_nominate();
    nominate.add_nominators("FoQJpPyadYccjavVdTWxpxU7rUEaYhfLCPwXgkfD6Zat9QP");
    nominate.add_nominators("CtwdfrhECFs3FpvCGoiE4hwRC4UsSiM8WL899HjRdQbfYZY");

    auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.encoded()), "ad0284ff88dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee00855fe8ba83a200c7b363fe7c9f315cc5578e25745a4131584713c3642644ed43ae503a2fe10bcc6a4d5eb6ebc96d5f17e4035f3658e8d6de8998f68a66a3df01000000060508ff8eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48ff0e33fdfb980e4499e5c3576e742a563b6a4fc0f6f598b1917fd7a6fe393ffc72");
}

TEST(PolkadotSigner, SignNominate2) {
    auto privateKey = PrivateKey(parse_hex("0xabf8e5bdbe30c65656c0a3cbd181ff8a56294a69dfedd27982aace4a76909115"));
    auto genesisHash = parse_hex("0xb0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe");

    auto input = Proto::SigningInput();
    input.set_block_hash(genesisHash.data(), genesisHash.size());
    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    input.set_nonce(0);
    input.set_spec_version(1031);
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    input.set_network(Proto::Network::KUSAMA);
    input.set_extrinsic_version(4);

    auto stakingCall = input.mutable_staking_call();
    auto &nominate = *stakingCall->mutable_nominate();
    // payload size larger than 256, will be hashed
    nominate.add_nominators("FoQJpPyadYccjavVdTWxpxU7rUEaYhfLCPwXgkfD6Zat9QP");
    nominate.add_nominators("CtwdfrhECFs3FpvCGoiE4hwRC4UsSiM8WL899HjRdQbfYZY");
    nominate.add_nominators("HJ1dGPxVr13KHGiCTGQfZjZMKGc8J52CsHEjcEXNMDyeGxf");
    nominate.add_nominators("F4LocUbsPrcC8xVap4wiTgDakzn3xFyXneuYDHRaHxnb6dH");
    nominate.add_nominators("DriCrAgdVV57NeQm5bWn5KQpVndVnXnm55BjRpe6qzZ5ktJ");
    nominate.add_nominators("GiBnzCGFofhmAvsUv9FUShUb8YJYYwWex3ThQNkbDDNprS6");

    auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.encoded()), "bd0484ff88dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee006d071e9e29e9108f07b9127a6a71f5e2b44b8c30452878aeb4d0fe12f7f0c43bca5e7eca7e199e1287ab26422903069ce14afead5fb3878f9f036578c369a80e000000060518ff8eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48ff0e33fdfb980e4499e5c3576e742a563b6a4fc0f6f598b1917fd7a6fe393ffc72ffd0bd030f88d88e9746e7e684a210d0e1b8b7db04d6a3dad1da047e7200c21e10ff6dd7ab69a1bccd2dbb782487f801509bf3aed97db9492b99f0d4e7ad08896e1cff38bc883d8fb7ae97eb3a7862cff1252172f4a901264c84b7feee0a900a64f77dffb6f0e5386adc413a67e628050d4ca4fc8ee2f170ed959465feff17bd88b7cf3c");
}

TEST(PolkadotSigner, SignChill) {
    auto privateKey = PrivateKey(parse_hex("0xabf8e5bdbe30c65656c0a3cbd181ff8a56294a69dfedd27982aace4a76909115"));
    auto genesisHash = parse_hex("0xb0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe");

    auto input = Proto::SigningInput();
    input.set_block_hash(genesisHash.data(), genesisHash.size());
    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    input.set_nonce(0);
    input.set_spec_version(1031);
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    input.set_network(Proto::Network::KUSAMA);
    input.set_extrinsic_version(4);

    auto stakingCall = input.mutable_staking_call();
    auto __attribute__((unused)) &chill = *stakingCall->mutable_chill();
    auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.encoded()), "a10184ff88dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee004c4801e3edba279dbb6e017f4c5fc9fc263a51a5d1e2f59e44e0f709541522d3c3cdd3d7ed1225913901e177ac4161df846632ddb7bad1f5b504612fa1ee620b0000000606");
}

TEST(PolkadotSigner, SignWithdraw) {
    auto privateKey = PrivateKey(parse_hex("0xabf8e5bdbe30c65656c0a3cbd181ff8a56294a69dfedd27982aace4a76909115"));
    auto genesisHash = parse_hex("0xb0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe");

    auto input = Proto::SigningInput();
    input.set_block_hash(genesisHash.data(), genesisHash.size());
    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    input.set_nonce(0);
    input.set_spec_version(1031);
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    input.set_network(Proto::Network::KUSAMA);
    input.set_extrinsic_version(4);

    auto stakingCall = input.mutable_staking_call();
    auto __attribute__((unused)) &withdraw = *stakingCall->mutable_withdraw_unbonded();
    auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.encoded()), "a10184ff88dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee00c595893548338fa3f90f1cf1e9b650f357925e29d26148ae732d6307fd630d56639448786841b36d7ff13023a633891c47d2cf0e79f03a26105f369a69f8c2010000000603");
}

TEST(PolkadotSigner, SignUnbond) {
    auto privateKey = PrivateKey(parse_hex("0xabf8e5bdbe30c65656c0a3cbd181ff8a56294a69dfedd27982aace4a76909115"));
    auto genesisHash = parse_hex("0xb0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe");

    auto input = Proto::SigningInput();
    input.set_block_hash(genesisHash.data(), genesisHash.size());
    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    input.set_nonce(0);
    input.set_spec_version(1031);
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    input.set_network(Proto::Network::KUSAMA);
    input.set_extrinsic_version(4);

    auto stakingCall = input.mutable_staking_call();
    auto &unbond = *stakingCall->mutable_unbond();

    auto value = store(uint256_t(123456));
    unbond.set_value(value.data(), value.size());
    auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.encoded()), "b10184ff88dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee005e29b1763a6287af1daa0487097850040038b8517df8cd47c57d8f9e8cb203872ff9f755c7b69ddefee98a0ad0dfbbcde757f33e4cba67331b323f8816af100a000000060202890700");
}

TEST(PolkadotSigner, SignBond) {
    auto privateKey = PrivateKey(parse_hex("0xabf8e5bdbe30c65656c0a3cbd181ff8a56294a69dfedd27982aace4a76909115"));
    auto genesisHash = parse_hex("0xb0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe");

    auto input = Proto::SigningInput();
    input.set_block_hash(genesisHash.data(), genesisHash.size());
    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    input.set_nonce(0);
    input.set_spec_version(1031);
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    input.set_network(Proto::Network::KUSAMA);
    input.set_extrinsic_version(4);

    auto stakingCall = input.mutable_staking_call();
    auto &bond = *stakingCall->mutable_bond();

    auto value = store(uint256_t(123456));

    bond.set_validator("FfmSiZNJP72xtSaXiP2iUhBwWeMEvmjPrxY2ViVkWaeChDC");
    bond.set_value(value.data(), value.size());
    bond.set_reward_destination(Proto::RewardDestination::STAKED);
    auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.encoded()), "390284ff88dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee006589694e6696e2173c0c4a1a0353267410546fb8f0970d037a6d079719c0e440a1af26bafae266fb383b41936682d74b68bb4018f7565f27fb7a6646a5c7cb0d0000000600ff88dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee0289070000");
}
