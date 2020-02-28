// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "proto/Polkadot.pb.h"
#include "uint256.h"
#include "../interface/TWTestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Polkadot;

TEST(TWAnySignerPolkadot, Sign) {
    auto key = parse_hex("0xabf8e5bdbe30c65656c0a3cbd181ff8a56294a69dfedd27982aace4a76909115");
    auto genesisHash = parse_hex("0xb0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe");

    Proto::SigningInput input;
    input.set_block_hash(genesisHash.data(), genesisHash.size());
    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    input.set_nonce(0);
    input.set_spec_version(1031);
    input.set_private_key(key.data(), key.size());
    input.set_network(Proto::Network::POLKADOT);
    input.set_extrinsic_version(4);

    auto balanceCall = input.mutable_balance_call();
    auto &transfer = *balanceCall->mutable_transfer();
    auto value = store(uint256_t(12345));
    transfer.set_to_address("15KRsCq9LLNmCxNFhGk55s5bEyazKefunDxUH24GFZwsTxyu");
    transfer.set_value(value.data(), value.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypePolkadot);

    ASSERT_EQ(hex(output.encoded()), "2d0284ff88dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee00693cab6d47abb4129bd7a1c87aeca2427b5f1f8c3e5ef500a6174c02fe09c20ccb003ab9e4da6e50c5e8ee8e105091aed60c441d9e040f01730d990d92b8c3050000000400ffbeff0e5d6f6e6e6d573d3044f3e2bfb353400375dc281da3337468d4aa527908e5c0");
}
