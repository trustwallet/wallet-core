// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.


#include "proto/Avalanche.pb.h"
#include <TrustWalletCore/TWAnySigner.h>
#include "HexCoding.h"
#include "Avalanche/CB58.h"
#include "PrivateKey.h"
#include "Avalanche/Address.h"
#include "Avalanche/InitialState.h"
#include "../interface/TWTestUtilities.h"
#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Avalanche;

std::pair<PrivateKey, Proto::SigningInput> getPrivateKeyAndTransactionAnySigner();
std::vector<PublicKey> generateAddressesForAnySigner();

TEST(TWAnySignerAvalanche, SignBasicTransaction) {
    auto defaults = getPrivateKeyAndTransactionAnySigner();
    auto privateKey = defaults.first;
    auto input = defaults.second;

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeAvalancheXChain);

    ASSERT_EQ(hex(output.encoded()), "00000000000000003039d891ad56056d9c01f18f43f58b5c784ad07a4a49cf3d1f11623804b5cba2c6bf00000002dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000700000000000003e8000000000000000000000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29cdbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db000000070000000000003039000000000000d43100000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29c00000002f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd15000000020000000300000007f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd1500000002000000030000000700000004deadbeef00000002000000090000000244ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d0044ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d00000000090000000244ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d0044ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d00");
}

TEST(TWAnySignerAvalanche, SignCreateAssetTransaction) {
    // auto defaults = getPrivateKeyAndTransactionAnySigner();
    // auto privateKey = defaults.first;
    // auto defaultInput = defaults.second;
        const auto privateKeyBytes = CB58::avalanche.decode("ewoqjP7PxY4yr3iLTpLisriqt94hdyDFNgchSxGGztUrTXtNN");
    const std::vector<uint8_t> privateKeyBytesNoChecksum(privateKeyBytes.begin(), privateKeyBytes.begin() + 32); // we just want the first 32 bytes
    const auto privateKey = PrivateKey(privateKeyBytesNoChecksum); 
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    
    auto blockchainID = CB58::avalanche.decode("2eNy1mUFdmaxXNj1eQHUe7Np4gju9sJsEtWQ4MX3ToiNKuADed");
    Data blockchainIDBytes(blockchainID.begin(), blockchainID.begin() + 32); // we just want the first 32 bytes, no checksum
    uint32_t netID = 12345;
    auto assetID = "0xdbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db";
    auto assetIDBytes = parse_hex(assetID);
    auto txIDBytes = parse_hex("0xf1e1d1c1b1a191817161514131211101f0e0d0c0b0a090807060504030201000"); 
    Data memo = {0xde, 0xad, 0xbe, 0xef};
    auto amount = 1000;   
    auto locktime = 0;
    auto threshold = 1;

    Proto::SigningInput input;

    // keep in mind the expected formats in Signer!
    // for example, addresses are expected as secp pubkey, not avalanche-formatted address
    input.add_private_keys();
    input.set_private_keys(0, privateKeyBytesNoChecksum.data(), privateKeyBytesNoChecksum.size());
    auto &inputTx = *input.mutable_input_tx();
    auto &baseTx = *inputTx.mutable_base_tx();
    baseTx.set_typeid_(0);
    baseTx.set_network_id(netID);
    baseTx.set_blockchain_id(blockchainIDBytes.data(), blockchainIDBytes.size());
    baseTx.set_memo(memo.data(), memo.size());
    
    auto coreInputOne = new Proto::SECP256K1TransferInput();
    coreInputOne->set_amount(123456789);
    coreInputOne->add_address_indices(3);
    coreInputOne->add_address_indices(7);
    auto wrappedInputOne = new Proto::TransactionInput();
    wrappedInputOne->set_allocated_secp_transfer_input(coreInputOne);
    auto inputOne = baseTx.add_inputs();
    inputOne->set_utxo_index(5);
    inputOne->set_tx_id(txIDBytes.data(), txIDBytes.size());
    inputOne->set_asset_id(assetIDBytes.data(), assetIDBytes.size());
    for (auto i = 0; i < 8; ++i) {
        inputOne->add_spendable_addresses();
        inputOne->set_spendable_addresses(i, publicKey.bytes.data(), publicKey.bytes.size());
    }
    inputOne->set_allocated_input(wrappedInputOne);
    auto coreInputTwo = new Proto::SECP256K1TransferInput();
    coreInputTwo->set_amount(123456789);
    coreInputTwo->add_address_indices(3);
    coreInputTwo->add_address_indices(7);
    auto wrappedInputTwo = new Proto::TransactionInput();
    wrappedInputTwo->set_allocated_secp_transfer_input(coreInputTwo);
    auto inputTwo = baseTx.add_inputs();
    inputTwo->set_utxo_index(5);
    inputTwo->set_tx_id(txIDBytes.data(), txIDBytes.size());
    inputTwo->set_asset_id(assetIDBytes.data(), assetIDBytes.size());
    for (auto i = 0; i < 8; ++i) {
        inputTwo->add_spendable_addresses();
        inputTwo->set_spendable_addresses(i, publicKey.bytes.data(), publicKey.bytes.size());
    }
    inputTwo->set_allocated_input(wrappedInputTwo);

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

    auto arbitraryAddresses = generateAddressesForAnySigner();
    auto &inputTx = *input.mutable_input_tx();
    auto &createAssetTx = *inputTx.mutable_create_asset_tx();

    auto stateOne = createAssetTx.add_initial_states();
    stateOne->set_fx_id(Avalanche::InitialState::FeatureExtension::SECP256k1);
    // amount, locktime, threshold, addresses
    {
        auto firstCoreOutput = new Proto::SECP256K1TransferOutput(); //12345, 54321, 333, arbitraryAddresses
        firstCoreOutput->set_amount(12345);
        firstCoreOutput->set_amount(54321);
        firstCoreOutput->set_threshold(333);
        for (auto i = 0; i < arbitraryAddresses.size(); ++i) {
            firstCoreOutput->add_addresses();
            firstCoreOutput->set_addresses(i, arbitraryAddresses[i].bytes.data(), arbitraryAddresses[i].bytes.size());
        }
        stateOne->add_outputs();
        auto wrappedOutputOne = stateOne->mutable_outputs(0);
        wrappedOutputOne->set_allocated_secp_transfer_output(firstCoreOutput);
   
        auto secondCoreOutput = new Proto::SECP256K1TransferOutput(); //54321, 54321, 333, arbitraryAddresses
        secondCoreOutput->set_amount(54321);
        secondCoreOutput->set_amount(54321);
        secondCoreOutput->set_threshold(333);
        for (auto i = 0; i < arbitraryAddresses.size(); ++i) {
            secondCoreOutput->add_addresses();
            secondCoreOutput->set_addresses(i, arbitraryAddresses[i].bytes.data(), arbitraryAddresses[i].bytes.size());
        }
        stateOne->add_outputs();
        auto wrappedOutputTwo = stateOne->mutable_outputs(1);
        wrappedOutputTwo->set_allocated_secp_transfer_output(secondCoreOutput);
   
        auto thirdCoreOutput = new Proto::SECP256K1TransferOutput(); //111, 111, 111, arbitraryAddresses
        thirdCoreOutput->set_amount(111);
        thirdCoreOutput->set_amount(111);
        thirdCoreOutput->set_threshold(111);
        for (auto i = 0; i < arbitraryAddresses.size(); ++i) {
            thirdCoreOutput->add_addresses();
            thirdCoreOutput->set_addresses(i, arbitraryAddresses[i].bytes.data(), arbitraryAddresses[i].bytes.size());
        }
        stateOne->add_outputs();
        auto wrappedOutputThree = stateOne->mutable_outputs(2);
        wrappedOutputThree->set_allocated_secp_transfer_output(thirdCoreOutput);
    }
    auto stateTwo = createAssetTx.add_initial_states();
    stateTwo->set_fx_id(Avalanche::InitialState::FeatureExtension::SECP256k1);
    // amount, locktime, threshold, addresses
    {
        auto firstCoreOutput = new Proto::SECP256K1TransferOutput(); //12345, 54321, 333, arbitraryAddresses
        firstCoreOutput->set_amount(12345);
        firstCoreOutput->set_amount(54321);
        firstCoreOutput->set_threshold(333);
        for (auto i = 0; i < arbitraryAddresses.size(); ++i) {
            firstCoreOutput->add_addresses();
            firstCoreOutput->set_addresses(i, arbitraryAddresses[i].bytes.data(), arbitraryAddresses[i].bytes.size());
        }
        stateTwo->add_outputs();
        auto wrappedOutputOne = stateTwo->mutable_outputs(0);
        wrappedOutputOne->set_allocated_secp_transfer_output(firstCoreOutput);
   
        auto secondCoreOutput = new Proto::SECP256K1TransferOutput(); //54321, 54321, 333, arbitraryAddresses
        secondCoreOutput->set_amount(54321);
        secondCoreOutput->set_amount(54321);
        secondCoreOutput->set_threshold(333);
        for (auto i = 0; i < arbitraryAddresses.size(); ++i) {
            secondCoreOutput->add_addresses();
            secondCoreOutput->set_addresses(i, arbitraryAddresses[i].bytes.data(), arbitraryAddresses[i].bytes.size());
        }
        stateTwo->add_outputs();
        auto wrappedOutputTwo = stateTwo->mutable_outputs(1);
        wrappedOutputTwo->set_allocated_secp_transfer_output(secondCoreOutput);
   
        auto thirdCoreOutput = new Proto::SECP256K1TransferOutput(); //111, 111, 111, arbitraryAddresses
        thirdCoreOutput->set_amount(111);
        thirdCoreOutput->set_amount(111);
        thirdCoreOutput->set_threshold(111);
        for (auto i = 0; i < arbitraryAddresses.size(); ++i) {
            thirdCoreOutput->add_addresses();
            thirdCoreOutput->set_addresses(i, arbitraryAddresses[i].bytes.data(), arbitraryAddresses[i].bytes.size());
        }
        stateTwo->add_outputs();
        auto wrappedOutputThree = stateTwo->mutable_outputs(2);
        wrappedOutputThree->set_allocated_secp_transfer_output(thirdCoreOutput);
    }
    std::string name = "testcoin";
    createAssetTx.set_name(name);
    
    std::string symbol = "test";
    createAssetTx.set_symbol(symbol);

    uint8_t denomination = 1;
    createAssetTx.set_denomination(denomination);

    Proto::SigningOutput output;
    ANY_SIGN(input, TWCoinTypeAvalancheXChain);

    ASSERT_EQ(hex(output.encoded()), "00000000000100003039d891ad56056d9c01f18f43f58b5c784ad07a4a49cf3d1f11623804b5cba2c6bf00000002dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000700000000000003e8000000000000000000000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29cdbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db000000070000000000003039000000000000d43100000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29c00000002f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd15000000020000000300000007f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd1500000002000000030000000700000004deadbeef0000000200000009000000021690ee4d10718b83bd43ce9cec43bf91d23b9380755de49d59331f7c25be60eb228ffcc273393597dcd7109af08eb59e9fcc09e256f2c24f04c58dbd6d1f04af011690ee4d10718b83bd43ce9cec43bf91d23b9380755de49d59331f7c25be60eb228ffcc273393597dcd7109af08eb59e9fcc09e256f2c24f04c58dbd6d1f04af0100000009000000021690ee4d10718b83bd43ce9cec43bf91d23b9380755de49d59331f7c25be60eb228ffcc273393597dcd7109af08eb59e9fcc09e256f2c24f04c58dbd6d1f04af011690ee4d10718b83bd43ce9cec43bf91d23b9380755de49d59331f7c25be60eb228ffcc273393597dcd7109af08eb59e9fcc09e256f2c24f04c58dbd6d1f04af01");
}

TEST(TWAnySignerAvalanche, SignOperationTransaction) {
    // auto defaults = getPrivateKeyAndTransaction();
    // auto privateKey = defaults.first;
    // auto baseTransaction = defaults.second;
    // auto assetID = parse_hex("0xdbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db"); 
    // auto txid = parse_hex("0xf1e1d1c1b1a191817161514131211101f0e0d0c0b0a090807060504030201000");

    // auto utxoIDOne = std::make_pair(txid, uint32_t(1));
    // auto utxoIDTwo = std::make_pair(txid, uint32_t(2));
    // std::vector<TransferableOp::UTXOID> utxoIDsOne = {utxoIDTwo, utxoIDOne}; // create it backwards to force a sort
    // std::vector<uint32_t> addressIndicesOne = {1, 2, 3};
    // uint32_t groupIDOne = 1;
    // Data payloadOne = {0xfe, 0xed, 0xfe, 0xed};
    // std::vector<Output> outputsOne;
    // auto outputOne = std::make_tuple(uint64_t(1), uint32_t(2), generateAddressesForSigner());
    // outputsOne.push_back(outputOne);
    // auto nftOp = std::make_unique<NFTMintOperation>(addressIndicesOne, groupIDOne, payloadOne, outputsOne);
    // auto opOne = TransferableOp(assetID, utxoIDsOne, std::move(nftOp));
    
    // auto differingTxid = parse_hex("0x0000d1c1b1a191817161514131211101f0e0d0c0b0a090807060504030201000");
    // auto utxoIDThree = std::make_pair(differingTxid, uint32_t(3));
    // auto utxoIDFour = std::make_pair(txid, uint32_t(3));
    // std::vector<TransferableOp::UTXOID> utxoIDsTwo = {utxoIDFour, utxoIDThree}; // create it backwards to force a sort
    // std::vector<uint32_t> addressIndicesTwo = {2, 3, 4};
    // auto mintOutputAddrs = generateAddressesForSigner();
    // auto mintOutput = SECP256k1MintOutput(uint64_t(10), uint32_t(20), mintOutputAddrs);
    // auto transferOutputAddrs = generateAddressesForSigner();
    // auto transferOutput = SECP256k1TransferOutput(uint64_t(30), uint64_t(40), uint32_t(5), transferOutputAddrs);
    // auto secpOp = std::make_unique<SECP256k1MintOperation>(addressIndicesTwo, mintOutput, transferOutput);
    // auto opTwo = TransferableOp(assetID, utxoIDsTwo, std::move(secpOp));
    
    // auto utxoIDFive = std::make_pair(txid, uint32_t(5));
    // auto utxoIDSix = std::make_pair(txid, uint32_t(6));
    // std::vector<TransferableOp::UTXOID> utxoIDsThree = {utxoIDSix, utxoIDFive}; // create it backwards to force a sort
    // std::vector<uint32_t> addressIndicesThree = {3, 4, 5};
    // auto nftTransferAddrs = generateAddressesForSigner();
    // auto nftPayload = Data{0xfe, 0xde, 0xfe, 0xde};
    // auto nftTransferOut = NFTTransferOutput(uint32_t(1), nftPayload, uint64_t(2), uint32_t(3), nftTransferAddrs);
    // auto nftTransferOp = std::make_unique<NFTTransferOperation>(addressIndicesThree, nftTransferOut);
    // auto opThree = TransferableOp(assetID, utxoIDsThree, std::move(nftTransferOp));

    // std::vector<TransferableOp> ops = {opOne, opTwo, opThree};
    // auto transaction = UnsignedOperationTransaction(baseTransaction, ops); 
    // Data encodedUnsignedTransaction;
    // transaction.encode(encodedUnsignedTransaction);

    // ASSERT_EQ(hex(encodedUnsignedTransaction), "0000000200003039d891ad56056d9c01f18f43f58b5c784ad07a4a49cf3d1f11623804b5cba2c6bf00000002dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000700000000000003e8000000000000000000000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29cdbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db000000070000000000003039000000000000d43100000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29c00000002f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd15000000020000000300000007f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd1500000002000000030000000700000004deadbeef00000003dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db000000020000d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000003f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000003000000080000000300000002000000030000000400000006000000000000000a00000014000000033ffb0c3c05f4bfc1ce66e1080209e3de96cfbf38b7678db74fab407771650fdda0198a4be04acda6c1e36a623910c93947b754c410af428005efe85100000007000000000000001e000000000000002800000005000000033ffb0c3c05f4bfc1ce66e1080209e3de96cfbf38b7678db74fab407771650fdda0198a4be04acda6c1e36a623910c93947b754c410af428005efe851dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db00000002f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000001f1e1d1c1b1a191817161514131211101f0e0d0c0b0a090807060504030201000000000020000000c000000030000000100000002000000030000000100000004feedfeed00000001000000000000000100000002000000033ffb0c3c05f4bfc1ce66e1080209e3de96cfbf38c1e36a623910c93947b754c410af428005efe851b7678db74fab407771650fdda0198a4be04acda6dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db00000002f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005f1e1d1c1b1a191817161514131211101f0e0d0c0b0a090807060504030201000000000060000000d000000030000000300000004000000050000000100000004fedefede000000000000000200000003000000033ffb0c3c05f4bfc1ce66e1080209e3de96cfbf38b7678db74fab407771650fdda0198a4be04acda6c1e36a623910c93947b754c410af428005efe851");
    // std::vector<PrivateKey> keyRing = {privateKey};
    // auto encodedSignedTransaction = Signer::sign(keyRing, transaction);
    // ASSERT_EQ(hex(encodedSignedTransaction), "00000000000200003039d891ad56056d9c01f18f43f58b5c784ad07a4a49cf3d1f11623804b5cba2c6bf00000002dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000700000000000003e8000000000000000000000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29cdbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db000000070000000000003039000000000000d43100000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29c00000002f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd15000000020000000300000007f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd1500000002000000030000000700000004deadbeef0000000200000009000000028d718dfbf41202d03e495cda3eeb2464dfdc0e7e1e73e3189e5b4752dae1a919360e43aeb3c31c1258c8cc80028bbcd83f15d00aa40dffbcbc9ad35e4f57ac8b008d718dfbf41202d03e495cda3eeb2464dfdc0e7e1e73e3189e5b4752dae1a919360e43aeb3c31c1258c8cc80028bbcd83f15d00aa40dffbcbc9ad35e4f57ac8b0000000009000000028d718dfbf41202d03e495cda3eeb2464dfdc0e7e1e73e3189e5b4752dae1a919360e43aeb3c31c1258c8cc80028bbcd83f15d00aa40dffbcbc9ad35e4f57ac8b008d718dfbf41202d03e495cda3eeb2464dfdc0e7e1e73e3189e5b4752dae1a919360e43aeb3c31c1258c8cc80028bbcd83f15d00aa40dffbcbc9ad35e4f57ac8b00");
}

TEST(TWAnySignerAvalanche, SignImportTransaction) {
}

TEST(TWAnySignerAvalanche, SignExportTransaction) {
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

std::pair<PrivateKey, Proto::SigningInput> getPrivateKeyAndTransactionAnySigner() {
    const auto privateKeyBytes = CB58::avalanche.decode("ewoqjP7PxY4yr3iLTpLisriqt94hdyDFNgchSxGGztUrTXtNN");
    const std::vector<uint8_t> privateKeyBytesNoChecksum(privateKeyBytes.begin(), privateKeyBytes.begin() + 32); // we just want the first 32 bytes
    const auto privateKey = PrivateKey(privateKeyBytesNoChecksum); 
    const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    
    auto blockchainID = CB58::avalanche.decode("2eNy1mUFdmaxXNj1eQHUe7Np4gju9sJsEtWQ4MX3ToiNKuADed");
    Data blockchainIDBytes(blockchainID.begin(), blockchainID.begin() + 32); // we just want the first 32 bytes, no checksum
    uint32_t netID = 12345;
    auto assetID = "0xdbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db";
    auto assetIDBytes = parse_hex(assetID);
    auto txIDBytes = parse_hex("0xf1e1d1c1b1a191817161514131211101f0e0d0c0b0a090807060504030201000"); 
    Data memo = {0xde, 0xad, 0xbe, 0xef};
    auto amount = 1000;   
    auto locktime = 0;
    auto threshold = 1;

    Proto::SigningInput input;

    // keep in mind the expected formats in Signer!
    // for example, addresses are expected as secp pubkey, not avalanche-formatted address
    input.add_private_keys();
    input.set_private_keys(0, privateKeyBytesNoChecksum.data(), privateKeyBytesNoChecksum.size());
    auto &inputTx = *input.mutable_input_tx();
    auto &baseTx = *inputTx.mutable_base_tx();
    baseTx.set_typeid_(0);
    baseTx.set_network_id(netID);
    baseTx.set_blockchain_id(blockchainIDBytes.data(), blockchainIDBytes.size());
    baseTx.set_memo(memo.data(), memo.size());
    
    auto coreInputOne = new Proto::SECP256K1TransferInput();
    coreInputOne->set_amount(123456789);
    coreInputOne->add_address_indices(3);
    coreInputOne->add_address_indices(7);
    auto wrappedInputOne = new Proto::TransactionInput();
    wrappedInputOne->set_allocated_secp_transfer_input(coreInputOne);
    auto inputOne = baseTx.add_inputs();
    inputOne->set_utxo_index(5);
    inputOne->set_tx_id(txIDBytes.data(), txIDBytes.size());
    inputOne->set_asset_id(assetIDBytes.data(), assetIDBytes.size());
    for (auto i = 0; i < 8; ++i) {
        inputOne->add_spendable_addresses();
        inputOne->set_spendable_addresses(i, publicKey.bytes.data(), publicKey.bytes.size());
    }
    inputOne->set_allocated_input(wrappedInputOne);
    auto coreInputTwo = new Proto::SECP256K1TransferInput();
    coreInputTwo->set_amount(123456789);
    coreInputTwo->add_address_indices(3);
    coreInputTwo->add_address_indices(7);
    auto wrappedInputTwo = new Proto::TransactionInput();
    wrappedInputTwo->set_allocated_secp_transfer_input(coreInputTwo);
    auto inputTwo = baseTx.add_inputs();
    inputTwo->set_utxo_index(5);
    inputTwo->set_tx_id(txIDBytes.data(), txIDBytes.size());
    inputTwo->set_asset_id(assetIDBytes.data(), assetIDBytes.size());
    for (auto i = 0; i < 8; ++i) {
        inputTwo->add_spendable_addresses();
        inputTwo->set_spendable_addresses(i, publicKey.bytes.data(), publicKey.bytes.size());
    }
    inputTwo->set_allocated_input(wrappedInputTwo);

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

    return std::make_pair(privateKey, input);
}