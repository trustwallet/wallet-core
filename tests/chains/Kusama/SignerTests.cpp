// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "HexCoding.h"
#include "Polkadot/Extrinsic.h"
#include "Polkadot/SS58Address.h"
#include "Polkadot/Signer.h"
#include "Coin.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "proto/Polkadot.pb.h"
#include "uint256.h"

#include <TrustWalletCore/TWSS58AddressType.h>
#include <gtest/gtest.h>


namespace TW::Polkadot::tests {
    extern PrivateKey privateKey;
    extern PublicKey toPublicKey;
    auto genesisHashKSM = parse_hex("b0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe");

TEST(KusamaSigner, SignTransferKSM) {
    auto blockHash = parse_hex("4955dd4813f3e91ef3fd5a825b928af2fc50a71380085f753ccef00bb1582891");
    auto toAddress = SS58Address(toPublicKey, TWSS58AddressTypeKusama);

    auto input = TW::Polkadot::Proto::SigningInput();
    input.set_block_hash(blockHash.data(), blockHash.size());
    input.set_genesis_hash(genesisHashKSM.data(), genesisHashKSM.size());
    input.set_nonce(0);
    input.set_spec_version(2019);
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    input.set_network(ss58Prefix(TWCoinType::TWCoinTypeKusama));
    input.set_transaction_version(2);

    auto balanceCall = input.mutable_balance_call();
    auto& transfer = *balanceCall->mutable_transfer();
    auto value = store(uint256_t(12345));
    transfer.set_to_address(toAddress.string());
    transfer.set_value(value.data(), value.size());

    auto extrinsic = TW::Polkadot::Extrinsic(input);
    auto preimage = extrinsic.encodePayload();
    auto output = TW::Polkadot::Signer::sign(input);

    ASSERT_EQ(hex(preimage), "04008eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48e5c0000000e307000002000000b0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe4955dd4813f3e91ef3fd5a825b928af2fc50a71380085f753ccef00bb1582891");
    ASSERT_EQ(hex(output.encoded()), "25028488dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee000765cfa76cfe19499f4f19ef7dc4527652ec5b2e6b5ecfaf68725dafd48ae2694ad52e61f44152a544784e847de10ddb2c56bee4406574dcbcfdb5e5d35b6d0300000004008eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48e5c0");
}

} // namespace TW::Polkadot::tests
