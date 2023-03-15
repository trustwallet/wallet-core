// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "proto/Polkadot.pb.h"
#include "uint256.h"
#include "TestUtilities.h"
#include <TrustWalletCore/TWAnySigner.h>

#include <gtest/gtest.h>

namespace TW::Polkadot::tests {

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
