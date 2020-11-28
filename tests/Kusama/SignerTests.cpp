// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Polkadot/Signer.h"
#include "Polkadot/Extrinsic.h"
#include "SS58Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "proto/Polkadot.pb.h"
#include "uint256.h"

#include <TrustWalletCore/TWSS58AddressType.h>
#include <gtest/gtest.h>


namespace TW::Polkadot {
    extern PrivateKey privateKey;
    extern PublicKey toPublicKey;
    auto genesisHashKSM = parse_hex("b0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe");

TEST(PolkadotSigner, SignTransferKSM) {
    auto blockHash = parse_hex("4955dd4813f3e91ef3fd5a825b928af2fc50a71380085f753ccef00bb1582891");
    auto toAddress = SS58Address(toPublicKey, TWSS58AddressTypeKusama);

    auto input = Proto::SigningInput();
    input.set_block_hash(blockHash.data(), blockHash.size());
    input.set_genesis_hash(genesisHashKSM.data(), genesisHashKSM.size());
    input.set_nonce(0);
    input.set_spec_version(2019);
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    input.set_network(Proto::Network::KUSAMA);
    input.set_transaction_version(2);

    auto balanceCall = input.mutable_balance_call();
    auto &transfer = *balanceCall->mutable_transfer();
    auto value = store(uint256_t(12345));
    transfer.set_to_address(toAddress.string());
    transfer.set_value(value.data(), value.size());

    auto extrinsic = Extrinsic(input);
    auto preimage = extrinsic.encodePayload();
    auto output = Signer::sign(input);

    ASSERT_EQ(hex(preimage), "0400beff0e5d6f6e6e6d573d3044f3e2bfb353400375dc281da3337468d4aa527908e5c0000000e307000002000000b0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe4955dd4813f3e91ef3fd5a825b928af2fc50a71380085f753ccef00bb1582891");
    ASSERT_EQ(hex(output.encoded()), "25028488dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee00845a6ae1042cfaaf6e94314ae3f9653cb5c5dd3a5601e2e091d03509c331cc1f3dc66060f818cf88451187f727dca4388dc2370818f6d18d8776c1e7dfa6290d0000000400beff0e5d6f6e6e6d573d3044f3e2bfb353400375dc281da3337468d4aa527908e5c0");
}

} // namespace
