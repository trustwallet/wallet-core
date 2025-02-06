// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#include "AnyAddress.h"
#include "Coin.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "proto/Polymesh.pb.h"
#include "proto/TransactionCompiler.pb.h"
#include "uint256.h"

#include <TrustWalletCore/TWAnyAddress.h>
#include <TrustWalletCore/TWPrivateKey.h>
#include <TrustWalletCore/TWPublicKey.h>
#include <TrustWalletCore/TWString.h>
#include <TrustWalletCore/TWTransactionCompiler.h>

#include "TestUtilities.h"
#include <gtest/gtest.h>
#include <pthread.h>

using namespace TW;
using namespace TW::Polymesh;

namespace TW::Polymesh::tests {
uint32_t polymeshPrefix = 12;

Data helper_encodeTransaction(TWCoinType coin, const Proto::SigningInput& input, const Data& pubKey, const Data& signature) {
    auto txInputData = data(input.SerializeAsString());
    auto txInputDataPtr = WRAPD(TWDataCreateWithBytes(txInputData.data(), txInputData.size()));

    const auto outputData = WRAPD(TWTransactionCompilerCompileWithSignatures(
        coin, txInputDataPtr.get(),
        WRAP(TWDataVector, TWDataVectorCreateWithData((TWData*)&signature)).get(),
        WRAP(TWDataVector, TWDataVectorCreateWithData((TWData*)&pubKey)).get()));

    Polymesh::Proto::SigningOutput output;
    output.ParseFromArray(TWDataBytes(outputData.get()),
                          (int)TWDataSize(outputData.get()));
    EXPECT_EQ(output.error(), Common::Proto::OK);

    return data(output.encoded());
}

TEST(TWAnySignerPolymesh, PolymeshEncodeAndSign) {
    // tx on mainnet
    // https://polymesh.subscan.io/extrinsic/0x9a4283cc38f7e769c53ad2d1c5cf292fc85a740ec1c1aa80c180847e51928650

    /// Step 1: Prepare transaction input (protobuf)
    const auto coin = TWCoinTypePolymesh;

    Polymesh::Proto::SigningInput input;
    input.set_network(12);
    auto blockHash = parse_hex("898bba6413c38f79a284aec8749f297f6c8734c501f67517b5a6aadc338d1102");
    auto vGenesisHash = parse_hex("6fbd74e5e1d0a61d52ccfe9d4adaed16dd3a7caa37c6bc4d0c2fa12e8b2f4063");
    input.set_block_hash(std::string(blockHash.begin(), blockHash.end()));
    input.set_genesis_hash(std::string(vGenesisHash.begin(), vGenesisHash.end()));
    input.set_nonce(1UL);
    input.set_spec_version(3010u);
    input.set_transaction_version(2u);

    auto* era = input.mutable_era();
    era->set_block_number(4298130UL);
    era->set_period(64UL);

    auto* transfer = input.mutable_runtime_call()->mutable_balance_call()->mutable_transfer();
    transfer->set_to_address("2FSoQykVV3uWe5ChZuazMDHBoaZmCPPuoYx5KHL5VqXooDQW");
    auto value = store(1000000);
    transfer->set_value(std::string(value.begin(), value.end()));
    transfer->set_memo("MEMO PADDED WITH SPACES");

    auto* callIndices = transfer->mutable_call_indices()->mutable_custom();
    callIndices->set_module_index(0x05);
    callIndices->set_method_index(0x01);

    /// Step 2: Obtain preimage hash
    auto txInputData = data(input.SerializeAsString());
    auto txInputDataPtr = WRAPD(TWDataCreateWithBytes(txInputData.data(), txInputData.size()));
    const auto preImageHashes = WRAPD(TWTransactionCompilerPreImageHashes(coin, txInputDataPtr.get()));
    auto preImageHash = data(TWDataBytes(preImageHashes.get()), TWDataSize(preImageHashes.get()));

    TxCompiler::Proto::PreSigningOutput preSigningOutput;
    ASSERT_TRUE(preSigningOutput.ParseFromArray(preImageHash.data(), int(preImageHash.size())));
    ASSERT_EQ(preSigningOutput.error(), Common::Proto::OK);

    const auto preImage = data(preSigningOutput.data());

    ASSERT_EQ(hex(preImage), "050100849e2f6b165d4b28b39ef3d98f86c0520d82bc349536324365c10af08f323f8302093d00014d454d4f2050414444454420574954482053504143455300000000000000000025010400c20b0000020000006fbd74e5e1d0a61d52ccfe9d4adaed16dd3a7caa37c6bc4d0c2fa12e8b2f4063898bba6413c38f79a284aec8749f297f6c8734c501f67517b5a6aadc338d1102");

    auto pubKey = parse_hex("4322cf71da08f9d56181a707af7c0c437dfcb93e6caac9825a5aba57548142ee");
    auto signature = parse_hex("0791ee378775eaff34ef7e529ab742f0d81d281fdf20ace0aa765ca484f5909c4eea0a59c8dbbc534c832704924b424ba3230c38acd0ad5360cef023ca2a420f");

    /// Step 3: Compile transaction info
    const auto outputData = WRAPD(TWTransactionCompilerCompileWithSignatures(
        coin, txInputDataPtr.get(),
        WRAP(TWDataVector, TWDataVectorCreateWithData((TWData*)&signature)).get(),
        WRAP(TWDataVector, TWDataVectorCreateWithData((TWData*)&pubKey)).get()));

    const auto ExpectedTx =
        "bd0284004322cf71da08f9d56181a707af7c0c437dfcb93e6caac9825a5aba57548142ee000791ee378775eaff34ef7e529ab742f0d81d281fdf20ace0aa765ca484f5909c4eea0a59c8dbbc534c832704924b424ba3230c38acd0ad5360cef023ca2a420f25010400050100849e2f6b165d4b28b39ef3d98f86c0520d82bc349536324365c10af08f323f8302093d00014d454d4f20504144444544205749544820535041434553000000000000000000";
    {
        Polymesh::Proto::SigningOutput output;
        ASSERT_TRUE(output.ParseFromArray(TWDataBytes(outputData.get()),
                                          (int)TWDataSize(outputData.get())));

        EXPECT_EQ(hex(output.encoded()), ExpectedTx);
    }
}

TEST(TWAnySignerPolymesh, encodeTransaction_Add_authorization) {
    // tx on mainnet
    // https://polymesh.subscan.io/extrinsic/0x7d9b9109027b36b72d37ba0648cb70e5254524d3d6752cc6b41601f4bdfb1af0

    Polymesh::Proto::SigningInput input;
    input.set_network(12);
    auto blockHash = parse_hex("ce0c2109db498e45abf8fd447580dcfa7b7a07ffc2bfb1a0fbdd1af3e8816d2b");
    auto vGenesisHash = parse_hex("6fbd74e5e1d0a61d52ccfe9d4adaed16dd3a7caa37c6bc4d0c2fa12e8b2f4063");
    input.set_block_hash(std::string(blockHash.begin(), blockHash.end()));
    input.set_genesis_hash(std::string(vGenesisHash.begin(), vGenesisHash.end()));
    input.set_nonce(5UL);
    input.set_spec_version(3010U);
    input.set_transaction_version(2U);

    auto* era = input.mutable_era();
    era->set_block_number(4395451UL);
    era->set_period(64UL);

    auto* addAuthorization = input.mutable_runtime_call()->mutable_identity_call()->mutable_add_authorization();
    addAuthorization->set_target("2HEVN4PHYKj7B1krQ9bctAQXZxHQQkANVNCcfbdYk2gZ4cBR");
    auto* keyPerms = addAuthorization->mutable_authorization()->mutable_join_identity();
    // Set empty "These".
    auto* assets = keyPerms->mutable_asset();
    assets->set_kind(Polymesh::Proto::SecondaryKeyPermissions_RestrictionKind_These);
    auto* extrinsics = keyPerms->mutable_extrinsic();
    extrinsics->set_kind(Polymesh::Proto::SecondaryKeyPermissions_RestrictionKind_These);
    auto* portfolios = keyPerms->mutable_portfolio();
    portfolios->set_kind(Polymesh::Proto::SecondaryKeyPermissions_RestrictionKind_These);

    auto* callIndices = addAuthorization->mutable_call_indices()->mutable_custom();
    callIndices->set_module_index(0x07);
    callIndices->set_method_index(0x0d);

    auto pubKey = parse_hex("4322cf71da08f9d56181a707af7c0c437dfcb93e6caac9825a5aba57548142ee");
    auto signature = parse_hex("81e6561e4391862b5da961d7033baced1c4b25f0e27f938b02321af1118e0b859e1c2bd5607576a258f2c2befbc5f397ea4adb62938f30eb73c8060ab0eabf01");
    auto encoded = helper_encodeTransaction(TWCoinTypePolymesh, input, pubKey, signature);
    ASSERT_EQ(hex(encoded), "490284004322cf71da08f9d56181a707af7c0c437dfcb93e6caac9825a5aba57548142ee0081e6561e4391862b5da961d7033baced1c4b25f0e27f938b02321af1118e0b859e1c2bd5607576a258f2c2befbc5f397ea4adb62938f30eb73c8060ab0eabf01b5031400070d01d3b2f1c41b9b4522eb3e23329b81aca6cc0231167ecfa3580c5a71ff6d0610540501000100010000");
}

TEST(TWAnySignerPolymesh, encodeTransaction_JoinIdentityAsKey) {
    // tx on mainnet
    // https://polymesh.subscan.io/extrinsic/0x9d7297d8b38af5668861996cb115f321ed681989e87024fda64eae748c2dc542

    Polymesh::Proto::SigningInput input;
    input.set_network(12);
    auto blockHash = parse_hex("45c80153c47f5d16acc7a66d473870e8d4574437a7d8c813f47da74cae3812c2");
    auto vGenesisHash = parse_hex("6fbd74e5e1d0a61d52ccfe9d4adaed16dd3a7caa37c6bc4d0c2fa12e8b2f4063");
    input.set_block_hash(std::string(blockHash.begin(), blockHash.end()));
    input.set_genesis_hash(std::string(vGenesisHash.begin(), vGenesisHash.end()));
    input.set_nonce(0UL);
    input.set_spec_version(3010U);
    input.set_transaction_version(2U);

    auto* era = input.mutable_era();
    era->set_block_number(4395527UL);
    era->set_period(64UL);

    auto* key = input.mutable_runtime_call()->mutable_identity_call()->mutable_join_identity_as_key();
    key->set_auth_id(21435);
    auto* callIndices = key->mutable_call_indices()->mutable_custom();
    callIndices->set_module_index(0x07);
    callIndices->set_method_index(0x05);

    auto pubKey = parse_hex("d3b2f1c41b9b4522eb3e23329b81aca6cc0231167ecfa3580c5a71ff6d061054");
    auto signature = parse_hex("7f5adbb2749e2f0ace29b409c41dd717681495b1f22dc5358311646a9fb8af8a173fc47f1b19748fb56831c2128773e2976986685adee83c741ab49934d80006");
    auto encoded = helper_encodeTransaction(TWCoinTypePolymesh, input, pubKey, signature);
    ASSERT_EQ(hex(encoded), "c5018400d3b2f1c41b9b4522eb3e23329b81aca6cc0231167ecfa3580c5a71ff6d061054007f5adbb2749e2f0ace29b409c41dd717681495b1f22dc5358311646a9fb8af8a173fc47f1b19748fb56831c2128773e2976986685adee83c741ab49934d80006750000000705bb53000000000000");
}

} // namespace TW::Polymesh::tests
