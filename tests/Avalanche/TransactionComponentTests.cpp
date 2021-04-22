// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "Avalanche/Transaction.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Avalanche;

std::vector<Address> generateAddresses() {
    // build some arbitrary addresses for this test
    std::vector<Address> arbitraryAddresses;
    arbitraryAddresses.push_back(Address("X-avax18lasc0q97jlurnnxuyyqyz0rm6tvl0ecsvkvjj"));
    arbitraryAddresses.push_back(Address("X-avax1c83k5c3ezrynj3ah2nzppt6zsqz7l6z3cgkgxh"));
    arbitraryAddresses.push_back(Address("X-avax1kancmd604dq8wut9plw6qxv2f0sy4ndxn6tng0"));
    return arbitraryAddresses;
}

TEST(AvalancheTransactionComponents, TestInitialState) {
    auto arbitraryAddresses = generateAddresses();
    // build some arbitrary outputs for this test
    std::vector<std::unique_ptr<TransactionOutput>> arbitraryOutputs;
    // fill outputs with arbitrary transactionoutputs
    arbitraryOutputs.push_back(std::make_unique<SECP256k1TransferOutput>(12345, 54321, 333, arbitraryAddresses));
    arbitraryOutputs.push_back(std::make_unique<SECP256k1TransferOutput>(54321, 54321, 333, arbitraryAddresses));
    arbitraryOutputs.push_back(std::make_unique<SECP256k1TransferOutput>(111, 111, 111, arbitraryAddresses));
    auto stateOne = InitialState(InitialState::FeatureExtension::SECP256k1, std::move(arbitraryOutputs));
    auto stateTwo = InitialState(stateOne); // should be able to build a state from another
    EXPECT_EQ(stateTwo.FxID, InitialState::FeatureExtension::SECP256k1);
    stateOne.FxID = InitialState::FeatureExtension::NFT;
    stateTwo = stateOne; // should be able to assign states to each other
    EXPECT_EQ(stateTwo.FxID, InitialState::FeatureExtension::NFT);
    stateOne.FxID = InitialState::FeatureExtension::SECP256k1;
    auto stateOneLessThanStateTwo = stateOne < stateTwo; // state one should now be less than state two by lexicographical sort
    EXPECT_TRUE(stateOneLessThanStateTwo);
}

TEST(AvalancheTransactionComponents, TestTransferableInput) {
    auto arbitraryAddresses = generateAddresses();
    auto assetID = parse_hex("0xdbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db"); 
    
    auto addressesInOne = std::vector<uint32_t>{3, 7};
    auto txidOne = parse_hex("0xf1e1d1c1b1a191817161514131211101f0e0d0c0b0a090807060504030201000");
    auto utxoIndexOne = 6;
    auto coreInputOne = std::make_unique<SECP256k1TransferInput>(123456789, addressesInOne);
    auto inputOne = TransferableInput(txidOne, utxoIndexOne, assetID, std::move(coreInputOne), arbitraryAddresses);

    auto addressesInTwo = std::vector<uint32_t>{4, 8};
    auto txidTwo = parse_hex("0xf1e1d1c1b1a191817161514131211101f0e0d0c0b0a090807060504030201000");
    auto utxoIndexTwo = 9;
    auto coreInputTwo = std::make_unique<SECP256k1TransferInput>(987654321, addressesInTwo);
    auto inputTwo = TransferableInput(txidTwo, utxoIndexTwo, assetID, std::move(coreInputTwo), arbitraryAddresses);

    auto inputThree = TransferableInput(inputTwo); // should be able to build from each other
    EXPECT_EQ(inputTwo.UTXOIndex, inputThree.UTXOIndex);
    auto sameUTXOs = inputOne.UTXOIndex == inputThree.UTXOIndex;
    EXPECT_FALSE(sameUTXOs);

    inputThree = inputOne; // should be able to assign inputs to each other
    sameUTXOs = inputOne.UTXOIndex == inputThree.UTXOIndex;
    EXPECT_TRUE(sameUTXOs);

    auto inputOneLessThanInputTwo = inputThree < inputTwo; // inputThree should be less than inputTwo by lexicographical sort if it was assigned properly
    EXPECT_TRUE(inputOneLessThanInputTwo); 
}

TEST(AvalancheTransactionComponents, TestTransferableOutput) {
}

TEST(AvalancheTransactionComponents, TestOutput) {
}

TEST(AvalancheTransactionComponents, TestTransactionFailureCases) {
}
