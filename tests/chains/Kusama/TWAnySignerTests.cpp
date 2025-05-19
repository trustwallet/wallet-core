// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "TestUtilities.h"
#include "proto/Polkadot.pb.h"
#include "uint256.h"

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWAnySigner.h>
#include <TrustWalletCore/TWTransactionCompiler.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Polkadot::tests {
extern uint32_t kusamaPrefix;
extern PrivateKey privateKey;
extern TWPublicKey* publicKey;
auto genesisHashKSM = parse_hex("b0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe");

extern Data helper_encodePayload(TWCoinType coin, const Proto::SigningInput& input);

TEST(TWAnySignerKusama, SignTransferKSM) {
    auto blockHash = parse_hex("4955dd4813f3e91ef3fd5a825b928af2fc50a71380085f753ccef00bb1582891");

    auto input = TW::Polkadot::Proto::SigningInput();
    input.set_block_hash(blockHash.data(), blockHash.size());
    input.set_genesis_hash(genesisHashKSM.data(), genesisHashKSM.size());
    input.set_nonce(0);
    input.set_spec_version(2019);
    input.set_private_key(privateKey.bytes.data(), privateKey.bytes.size());
    input.set_network(kusamaPrefix);
    input.set_transaction_version(2);

    auto balanceCall = input.mutable_balance_call();
    auto& transfer = *balanceCall->mutable_transfer();
    auto value = store(uint256_t(12345));
    transfer.set_to_address("FoQJpPyadYccjavVdTWxpxU7rUEaYhfLCPwXgkfD6Zat9QP");
    transfer.set_value(value.data(), value.size());

    auto preimage = helper_encodePayload(TWCoinTypeKusama, input);
    ASSERT_EQ(hex(preimage), "04008eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48e5c0000000e307000002000000b0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe4955dd4813f3e91ef3fd5a825b928af2fc50a71380085f753ccef00bb1582891");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeKusama);
    ASSERT_EQ(hex(output.encoded()), "25028488dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee000765cfa76cfe19499f4f19ef7dc4527652ec5b2e6b5ecfaf68725dafd48ae2694ad52e61f44152a544784e847de10ddb2c56bee4406574dcbcfdb5e5d35b6d0300000004008eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48e5c0");
}

TEST(TWAnySignerKusama, Sign) {
    auto key = parse_hex("0x8cdc538e96f460da9d639afc5c226f477ce98684d77fb31e88db74c1f1dd86b2");
    auto genesisHash = parse_hex("0xb0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe");

    Proto::SigningInput input;
    input.set_block_hash(genesisHash.data(), genesisHash.size());
    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    input.set_nonce(1);
    input.set_spec_version(2019);
    input.set_private_key(key.data(), key.size());
    input.set_network(TWCoinTypeSS58Prefix(TWCoinTypeKusama));
    input.set_transaction_version(2);

    auto balanceCall = input.mutable_balance_call();
    auto& transfer = *balanceCall->mutable_transfer();
    auto value = store(uint256_t(10000000000));
    transfer.set_to_address("CtwdfrhECFs3FpvCGoiE4hwRC4UsSiM8WL899HjRdQbfYZY");
    transfer.set_value(value.data(), value.size());

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeKusama);

    ASSERT_EQ(hex(output.encoded()), "350284f41296779fd61a5bed6c2f506cc6c9ea93d6aeb357b9c69717193f434ba24ae700cd78b46eff36c433e642d7e9830805aab4f43eef70067ef32c8b2a294c510673a841c5f8a6e8900c03be40cfa475ae53e6f8aa61961563cb7cc0fa169ef9630d00040004000e33fdfb980e4499e5c3576e742a563b6a4fc0f6f598b1917fd7a6fe393ffc720700e40b5402");
}

} // namespace TW::Polkadot::tests
