// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Polkadot/Signer.h"
#include "Polkadot/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "proto/Polkadot.pb.h"
#include "uint256.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Polkadot;

TEST(PolkadotSigner, SignTransfer) {
    auto privateKey = PrivateKey(parse_hex("0xabf8e5bdbe30c65656c0a3cbd181ff8a56294a69dfedd27982aace4a76909115"));
    auto blockHash = parse_hex("0xb0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe");
    auto genesisHash = parse_hex("0xb0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe");

    auto input = Proto::SigningInput();
    input.set_block_hash(blockHash.data(), blockHash.size());
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

TEST(PolkadotSigner, SignNominate) {
    auto privateKey = PrivateKey(parse_hex("0xabf8e5bdbe30c65656c0a3cbd181ff8a56294a69dfedd27982aace4a76909115"));
    auto blockHash = parse_hex("0xb0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe");
    auto genesisHash = parse_hex("0xb0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe");

    auto input = Proto::SigningInput();
    input.set_block_hash(blockHash.data(), blockHash.size());
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

TEST(PolkadotSigner, SignChill) {
    auto privateKey = PrivateKey(parse_hex("0xabf8e5bdbe30c65656c0a3cbd181ff8a56294a69dfedd27982aace4a76909115"));
    auto blockHash = parse_hex("0xb0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe");
    auto genesisHash = parse_hex("0xb0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe");

    auto input = Proto::SigningInput();
    input.set_block_hash(blockHash.data(), blockHash.size());
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
