// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "AnyAddress.h"
#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "proto/Polkadot.pb.h"
#include "proto/TransactionCompiler.pb.h"
#include "uint256.h"

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWString.h>
#include <TrustWalletCore/TWTransactionCompiler.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>

namespace TW::Polkadot::tests {
uint32_t polkadotPrefix = ss58Prefix(TWCoinTypePolkadot);
uint32_t kusamaPrefix = ss58Prefix(TWCoinTypeKusama);
uint32_t astarPrefix = 5;
uint32_t parallelPrefix = 172;

auto privateKey = PrivateKey(parse_hex("0xabf8e5bdbe30c65656c0a3cbd181ff8a56294a69dfedd27982aace4a76909115"));
auto privateKeyThrow2Data = DATA("70a794d4f1019c3ce002f33062f45029c4f930a56b3d20ec477f7668c6bbc37f");
auto privateKeyThrow2 = TWPrivateKeyCreateWithData(privateKeyThrow2Data.get());
auto addressThrow2 = "14Ztd3KJDaB9xyJtRkREtSZDdhLSbm7UUKt8Z7AwSv7q85G2";
auto genesisHash = parse_hex("91b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c3");

Data helper_encodePayload(TWCoinType coin, const Proto::SigningInput& input) {
    auto txInputData = data(input.SerializeAsString());
    auto txInputDataPtr = WRAPD(TWDataCreateWithBytes(txInputData.data(), txInputData.size()));
    const auto preImageHashes = WRAPD(TWTransactionCompilerPreImageHashes(coin, txInputDataPtr.get()));
    auto preImageHash = data(TWDataBytes(preImageHashes.get()), TWDataSize(preImageHashes.get()));

    TxCompiler::Proto::PreSigningOutput preSigningOutput;
    preSigningOutput.ParseFromArray(preImageHash.data(), int(preImageHash.size()));

    EXPECT_EQ(preSigningOutput.error(), Common::Proto::OK);
    return data(preSigningOutput.data());
}

TEST(TWAnySignerPolkadot, SignTransfer_9fd062) {
    auto toAddressStr = STRING("13ZLCqJNPsRZYEbwjtZZFpWt9GyFzg5WahXCVWKpWdUJqrQ5");

    auto input = Proto::SigningInput();
    input.set_genesis_hash(genesisHash.data(), genesisHash.size());
    auto blockHash = parse_hex("0x5d2143bb808626d63ad7e1cda70fa8697059d670a992e82cd440fbb95ea40351");
    input.set_block_hash(blockHash.data(), blockHash.size());
    input.set_nonce(3);
    input.set_spec_version(26);
    {
        auto pubKey = WRAP(TWPublicKey, TWPrivateKeyGetPublicKeyEd25519(privateKeyThrow2));
        const auto address = WRAP(TWAnyAddress, TWAnyAddressCreateWithPublicKey(pubKey.get(), TWCoinTypePolkadot));
        auto addressStr = WRAPS(TWAnyAddressDescription(address.get()));
        ASSERT_STREQ(TWStringUTF8Bytes(addressStr.get()), addressThrow2);
    }
    input.set_private_key(TWDataBytes(privateKeyThrow2Data.get()), TWDataSize(privateKeyThrow2Data.get()));
    input.set_network(ss58Prefix(TWCoinTypePolkadot));
    input.set_transaction_version(5);

    // era: for blockhash and block number, use curl -H "Content-Type: application/json" -H "Accept: text/plain" https://<polkadot-rpc-url>/transaction/material?noMeta=true
    auto era = input.mutable_era();
    era->set_block_number(3541050);
    era->set_period(64);

    auto balanceCall = input.mutable_balance_call();
    auto transfer = balanceCall->mutable_transfer();
    auto value = store(uint256_t(2000000000)); // 0.2
    transfer->set_to_address(TWStringUTF8Bytes(toAddressStr.get()));
    transfer->set_value(value.data(), value.size());

    auto preimage = helper_encodePayload(TWCoinTypePolkadot, input);
    EXPECT_EQ(hex(preimage), "05007120f76076bcb0efdf94c7219e116899d0163ea61cb428183d71324eb33b2bce0300943577a5030c001a0000000500000091b171bb158e2d3848fa23a9f1c25182fb8e20313b2c1eb49219da7a70ce90c35d2143bb808626d63ad7e1cda70fa8697059d670a992e82cd440fbb95ea40351");

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypePolkadot);

    // https://polkadot.subscan.io/extrinsic/0x9fd06208a6023e489147d8d93f0182b0cb7e45a40165247319b87278e08362d8
    EXPECT_EQ(hex(output.encoded()), "3502849dca538b7a925b8ea979cc546464a3c5f81d2398a3a272f6f93bdf4803f2f7830073e59cef381aedf56d7af076bafff9857ffc1e3bd7d1d7484176ff5b58b73f1211a518e1ed1fd2ea201bd31869c0798bba4ffe753998c409d098b65d25dff801a5030c0005007120f76076bcb0efdf94c7219e116899d0163ea61cb428183d71324eb33b2bce0300943577");
}

} // namespace TW::Polkadot::tests
