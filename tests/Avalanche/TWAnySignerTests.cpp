// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.


#include "proto/Avalanche.pb.h"
#include "proto/Bitcoin.pb.h"
#include <TrustWalletCore/TWAnySigner.h>
#include "HexCoding.h"
#include "Avalanche/CB58.h"
#include "PrivateKey.h"
#include "Avalanche/Address.h"
#include "Avalanche/Transaction.h"
#include "../interface/TWTestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Avalanche;

std::vector<PublicKey> generateAddressesForAnySigner();
const std::vector<uint8_t> getDefaultPrivateKeyBytes();
void setUpDefaultPrivateKeyData(Proto::SigningInput& input);
void setUpDefaultSimpleTx(TW::Avalanche::Proto::SimpleTransferTx& simpleTx);
void setUpDefaultBaseTx(TW::Avalanche::Proto::BaseTx& baseTx);

const char* DefaultAssetId = "0xdbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db";

TEST(TWAnySignerAvalanche, SignSimpleTransaction) {
    Proto::SigningInput input;
    setUpDefaultPrivateKeyData(input);
    auto& simpleTx = *input.mutable_simple_transfer_tx();
    setUpDefaultSimpleTx(simpleTx);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeAvalancheXChain);

    ASSERT_EQ(hex(output.encoded()), "00000000000000003039d891ad56056d9c01f18f43f58b5c784ad07a4a49cf3d1f11623804b5cba2c6bf00000002dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db000000070000000003eab5aa000000000000000000000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29cdbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db00000007000000000aba9500000000000000000000000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29c00000002f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd15000000020000000300000007f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd1500000002000000030000000700000004deadbeef0000000200000009000000022302423a2710d8c5887c99aaad1fed8e3a104995b5dce6b4743e8adecf034c545d8a41afa3fe0308998a1a528f3514df2deec89a125176f1c412693a60939bbf012302423a2710d8c5887c99aaad1fed8e3a104995b5dce6b4743e8adecf034c545d8a41afa3fe0308998a1a528f3514df2deec89a125176f1c412693a60939bbf0100000009000000022302423a2710d8c5887c99aaad1fed8e3a104995b5dce6b4743e8adecf034c545d8a41afa3fe0308998a1a528f3514df2deec89a125176f1c412693a60939bbf012302423a2710d8c5887c99aaad1fed8e3a104995b5dce6b4743e8adecf034c545d8a41afa3fe0308998a1a528f3514df2deec89a125176f1c412693a60939bbf01");
}

TEST(TWAnySignerAvalanche, PlanSimpleTransaction) {
    Proto::SigningInput input;
    setUpDefaultPrivateKeyData(input);
    auto& simpleTx = *input.mutable_simple_transfer_tx();
    setUpDefaultSimpleTx(simpleTx);

    Proto::TransactionPlan plan;
    ANY_PLAN(input, plan, TWCoinTypeAvalancheXChain);

    EXPECT_EQ(plan.amount(), 180000000);
    EXPECT_EQ(plan.available_amount(), 246913578); // 2 * 123456789
    EXPECT_EQ(plan.fee(), 1200000);
    EXPECT_EQ(plan.change(), 65713578);
    EXPECT_EQ(plan.utxos_size(), 2);
}

TEST(TWAnySignerAvalanche, PlanSmallAmount) {
    Proto::SigningInput input;
    setUpDefaultPrivateKeyData(input);
    auto& simpleTx = *input.mutable_simple_transfer_tx();
    setUpDefaultSimpleTx(simpleTx);
    simpleTx.set_amount(50000000);

    Proto::TransactionPlan plan;
    ANY_PLAN(input, plan, TWCoinTypeAvalancheXChain);

    EXPECT_EQ(plan.amount(), 50000000);
    EXPECT_EQ(plan.available_amount(), 123456789);
    EXPECT_EQ(plan.fee(), 1200000);
    EXPECT_EQ(plan.change(), 72256789);
    EXPECT_EQ(plan.utxos_size(), 1);
}

TEST(TWAnySignerAvalanche, PlanMaxAmount) {
    Proto::SigningInput input;
    setUpDefaultPrivateKeyData(input);
    auto& simpleTx = *input.mutable_simple_transfer_tx();
    setUpDefaultSimpleTx(simpleTx);
    simpleTx.set_amount(123); // does not matter with max amount
    simpleTx.set_use_max_amount(true);

    Proto::TransactionPlan plan;
    ANY_PLAN(input, plan, TWCoinTypeAvalancheXChain);

    EXPECT_EQ(plan.amount(), 246913578 - 1200000);
    EXPECT_EQ(plan.available_amount(), 246913578);
    EXPECT_EQ(plan.fee(), 1200000);
    EXPECT_EQ(plan.change(), 0);
    EXPECT_EQ(plan.utxos_size(), 2);
}

TEST(TWAnySignerAvalanche, SignBasicTransaction) {
    Proto::SigningInput input;
    setUpDefaultPrivateKeyData(input);
    auto& baseTx = *input.mutable_base_tx();
    setUpDefaultBaseTx(baseTx);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeAvalancheXChain);

    ASSERT_EQ(hex(output.encoded()), "00000000000000003039d891ad56056d9c01f18f43f58b5c784ad07a4a49cf3d1f11623804b5cba2c6bf00000002dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000700000000000003e8000000000000000000000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29cdbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db000000070000000000003039000000000000d43100000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29c00000002f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd15000000020000000300000007f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd1500000002000000030000000700000004deadbeef00000002000000090000000244ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d0044ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d00000000090000000244ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d0044ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d00");
}

std::vector<PublicKey> generateAddressesForAnySigner() {
    // build some arbitrary addresses for this test
    std::vector<PublicKey> arbitraryAddresses;
    {   
        auto privateKey = PrivateKey(parse_hex("0x98cb077f972feb0481f1d894f272c6a1e3c15e272a1658ff716444f465200070"));
        const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
        arbitraryAddresses.push_back(publicKey);
    }
    {   
        auto privateKey = PrivateKey(parse_hex("0x88cb770f972feb0481f1d894f272c6a1e3c15e272a1658ff716444f465200070"));
        const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
        arbitraryAddresses.push_back(publicKey);
    }
    {   
        auto privateKey = PrivateKey(parse_hex("0x78cb077f972fe00481f0d894f272c6a1e3c15e272a1658ff716444f465200070"));
        const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
        arbitraryAddresses.push_back(publicKey);
    }
    return arbitraryAddresses;
}

const std::vector<uint8_t> getDefaultPrivateKeyBytes() {
    const auto privateKeyBytes = CB58::avalanche.decode("ewoqjP7PxY4yr3iLTpLisriqt94hdyDFNgchSxGGztUrTXtNN");
    const std::vector<uint8_t> privateKeyBytesNoChecksum(privateKeyBytes.begin(), privateKeyBytes.begin() + 32); // we just want the first 32 bytes
    return privateKeyBytesNoChecksum; 
}

void setUpDefaultPrivateKeyData(Proto::SigningInput& input) {
    auto privateKeyBytes = getDefaultPrivateKeyBytes();
    input.add_private_keys();
    input.set_private_keys(0, privateKeyBytes.data(), privateKeyBytes.size());
}

Data getDefaultBlockchainID() {
    auto id = CB58::avalanche.decode("2eNy1mUFdmaxXNj1eQHUe7Np4gju9sJsEtWQ4MX3ToiNKuADed");
    return Data(id.begin(), id.begin() + 32); // we just want the first 32 bytes, no checksum
}

Proto::TransferableInput buildTransferableInput(const PublicKey& publicKey, const Data& txId, const Data& assetId, uint64_t amount) {
    auto coreInput = new Proto::SECP256K1TransferInput();
    coreInput->set_amount(amount);
    coreInput->add_address_indices(3);
    coreInput->add_address_indices(7);
    auto wrappedInput = new Proto::TransactionInput();
    wrappedInput->set_allocated_secp_transfer_input(coreInput);
    auto input = Proto::TransferableInput();
    input.set_utxo_index(5);
    input.set_tx_id(txId.data(), txId.size());
    input.set_asset_id(assetId.data(), assetId.size());
    for (auto i = 0; i < 8; ++i) {
        input.add_spendable_addresses();
        input.set_spendable_addresses(i, publicKey.bytes.data(), publicKey.bytes.size());
    }
    input.set_allocated_input(wrappedInput);
    return input;
}

void setUpDefaultSimpleTx(TW::Avalanche::Proto::SimpleTransferTx& simpleTx) {
    const auto privateKey = PrivateKey(getDefaultPrivateKeyBytes()); 
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    // keep in mind the expected formats in Signer!
    // for example, addresses are expected as secp pubkey, not avalanche-formatted address
    
    auto blockchainIDBytes = getDefaultBlockchainID();
    uint32_t netID = 12345;
    auto assetIDBytes = parse_hex(DefaultAssetId);
    auto txIDBytes = parse_hex("0xf1e1d1c1b1a191817161514131211101f0e0d0c0b0a090807060504030201000"); 
    Data memo = parse_hex("deadbeef");
    auto locktime = 0;
    auto threshold = 1;

    simpleTx.set_type_id(0);
    simpleTx.set_network_id(netID);
    simpleTx.set_blockchain_id(blockchainIDBytes.data(), blockchainIDBytes.size());

    simpleTx.set_amount(180000000);
    simpleTx.set_fee(1200000);
    simpleTx.add_to_addresses();
    simpleTx.set_to_addresses(0, publicKey.bytes.data(), publicKey.bytes.size());
    simpleTx.add_change_addresses();
    simpleTx.set_change_addresses(0, publicKey.bytes.data(), publicKey.bytes.size());
    simpleTx.set_use_max_amount(false);

    simpleTx.set_output_type_id(0);
    simpleTx.set_output_asset_id(assetIDBytes.data(), assetIDBytes.size());
    simpleTx.set_memo(memo.data(), memo.size());
    simpleTx.set_locktime(locktime);
    simpleTx.set_threshold(threshold);

    auto inputOne = simpleTx.add_inputs();
    *inputOne = buildTransferableInput(publicKey, txIDBytes, assetIDBytes, 123456789);

    auto inputTwo = simpleTx.add_inputs();
    *inputTwo = buildTransferableInput(publicKey, txIDBytes, assetIDBytes, 123456789);
}

void setUpDefaultBaseTx(TW::Avalanche::Proto::BaseTx& baseTx) {
    const auto privateKey = PrivateKey(getDefaultPrivateKeyBytes()); 
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    // keep in mind the expected formats in Signer!
    // for example, addresses are expected as secp pubkey, not avalanche-formatted address
    
    auto blockchainIDBytes = getDefaultBlockchainID();
    uint32_t netID = 12345;
    auto assetIDBytes = parse_hex(DefaultAssetId);
    auto txIDBytes = parse_hex("0xf1e1d1c1b1a191817161514131211101f0e0d0c0b0a090807060504030201000"); 
    Data memo = parse_hex("deadbeef");
    auto amount = 1000;   
    auto locktime = 0;
    auto threshold = 1;

    baseTx.set_type_id(0);
    baseTx.set_network_id(netID);
    baseTx.set_blockchain_id(blockchainIDBytes.data(), blockchainIDBytes.size());
    baseTx.set_memo(memo.data(), memo.size());
    
    auto inputOne = baseTx.add_inputs();
    *inputOne = buildTransferableInput(publicKey, txIDBytes, assetIDBytes, 123456789);

    auto inputTwo = baseTx.add_inputs();
    *inputTwo = buildTransferableInput(publicKey, txIDBytes, assetIDBytes, 123456789);

    auto coreOutputOne = new Proto::SECP256K1TransferOutput();
    coreOutputOne->set_amount(12345);
    coreOutputOne->set_locktime(54321);
    coreOutputOne->set_threshold(threshold);
    coreOutputOne->add_addresses();
    coreOutputOne->set_addresses(0, publicKey.bytes.data(), publicKey.bytes.size());
    auto wrappedOutputOne = new Proto::TransactionOutput();
    wrappedOutputOne->set_allocated_secp_transfer_output(coreOutputOne);
    auto outputOne = baseTx.add_outputs();
    outputOne->set_asset_id(assetIDBytes.data(), assetIDBytes.size());
    outputOne->set_allocated_output(wrappedOutputOne);

    auto coreOutputTwo = new Proto::SECP256K1TransferOutput();
    coreOutputTwo->set_amount(amount);
    coreOutputTwo->set_locktime(locktime);
    coreOutputTwo->set_threshold(threshold);
    coreOutputTwo->add_addresses();
    coreOutputTwo->set_addresses(0, publicKey.bytes.data(), publicKey.bytes.size());    
    auto wrappedOutputTwo = new Proto::TransactionOutput();
    wrappedOutputTwo->set_allocated_secp_transfer_output(coreOutputTwo);
    auto outputTwo = baseTx.add_outputs();
    outputTwo->set_asset_id(assetIDBytes.data(), assetIDBytes.size());
    outputTwo->set_allocated_output(wrappedOutputTwo);
}
