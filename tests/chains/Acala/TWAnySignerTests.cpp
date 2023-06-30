// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "uint256.h"
#include "proto/Polkadot.pb.h"
#include "Polkadot/Extrinsic.h"
#include "PrivateKey.h"
#include <TrustWalletCore/TWAnySigner.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Polkadot::tests {

TEST(TWAnySignerAcala, Sign) {
    // Successfully broadcasted: https://acala.subscan.io/extrinsic/3893620-3
    auto secret = parse_hex("9066aa168c379a403becb235c15e7129c133c244e56a757ab07bc369288bcab0");

    auto genesisHash = parse_hex("fc41b9bd8ef8fe53d58c7ea67c794c7ec9a73daf05e6d54b14ff6342c99ba64c");
    auto blockHash = parse_hex("707ffa05b7dc6cdb6356bd8bd51ff20b2757c3214a76277516080a10f1bc7537");

    auto input = Proto::SigningInput();
    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    input.set_block_hash(blockHash.data(), blockHash.size());

    input.set_nonce(0);
    input.set_spec_version(2170);
    input.set_private_key(secret.data(), secret.size());
    input.set_network(10); // Acala
    input.set_transaction_version(2);
    input.set_multi_address(true);

    auto &era = *input.mutable_era();
    era.set_block_number(3893613);
    era.set_period(64);

    auto balanceCall = input.mutable_balance_call();
    auto &transfer = *balanceCall->mutable_transfer();
    auto value = store(uint256_t(1'000'000'000'000));
    transfer.set_to_address("25Qqz3ARAvnZbahGZUzV3xpP1bB3eRrupEprK7f2FNbHbvsz");
    transfer.set_value(value.data(), value.size());

    auto* callIndices = transfer.mutable_call_indices()->mutable_custom();
    callIndices->set_module_index(0x0a);
    callIndices->set_method_index(0x00);

    auto extrinsic = Extrinsic(input);
    auto preimage = extrinsic.encodePayload();
    EXPECT_EQ(hex(preimage), "0a0000c8c602ded977c56076ae38d98026fa669ca10d6a2b5a0bfc4086ae7668ed1c60070010a5d4e8d50200007a08000002000000fc41b9bd8ef8fe53d58c7ea67c794c7ec9a73daf05e6d54b14ff6342c99ba64c707ffa05b7dc6cdb6356bd8bd51ff20b2757c3214a76277516080a10f1bc7537");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypePolkadot);

    EXPECT_EQ(hex(output.encoded()), "41028400e9590e4d99264a14a85e21e69537e4a64f66a875d38cb8f76b305f41fabe24a900dd54466dffd1e3c80b76013e9459fbdcd17805bd5fdbca0961a643bad1cbd2b7fe005c62c51c18b67f31eb9e61b187a911952fee172ef18402d07c703eec3100d50200000a0000c8c602ded977c56076ae38d98026fa669ca10d6a2b5a0bfc4086ae7668ed1c60070010a5d4e8");
}

} // namespace TW::Polkadot::tests
