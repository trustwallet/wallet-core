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

TEST(PolkadotSigner, Sign) {
    auto privateKey = PrivateKey(parse_hex("0xabf8e5bdbe30c65656c0a3cbd181ff8a56294a69dfedd27982aace4a76909115"));
    auto blockHash = parse_hex("0xb0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe");
    auto genesisHash = parse_hex("0xb0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe");

    auto input = Proto::SigningInput();
    input.set_block_hash(blockHash.data(), blockHash.size());
    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    input.set_nonce(0);
    input.set_spec_version(1030);
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    input.set_network(Proto::Network::KUSAMA);
    input.set_extrinsic_version(4);

    auto balanceCall = input.mutable_balance_call();
    auto &transfer = *balanceCall->mutable_transfer();
    auto value = store(uint256_t(12345));
    transfer.set_to_address("FoQJpPyadYccjavVdTWxpxU7rUEaYhfLCPwXgkfD6Zat9QP");
    transfer.set_value(value.data(), value.size());

    auto output = Signer::sign(input);

    ASSERT_EQ(hex(output.encoded()), "0x2d0284ff88dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee0011549108231dc42c75599fbb3fd61a3155c79ddbd51d237bfb4403ab04e63ea667f6616989e2e2bdc7670225be8fd3824ca33792ee82d73bcff34181b43106060000000400ff8eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48e5c0");
}
