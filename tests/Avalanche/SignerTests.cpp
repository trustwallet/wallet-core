// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Avalanche/Signer.h"
#include "Avalanche/Address.h"
#include "HexCoding.h"
#include "PrivateKey.h"
#include "PublicKey.h"
#include "Avalanche/CB58.h"

#include <gtest/gtest.h>

using namespace TW;
using namespace TW::Avalanche;

// This test is modeled after the avalanchejs signer tests
// many example values are likewise modeled after the docs.avax.network tx serialization examples

std::pair<PrivateKey, BaseTransaction> getPrivateKeyAndTransaction();
std::vector<Address> generateAddressesForSigner();

TEST(AvalancheSigner, SignBasicTransaction) {
    auto defaults = getPrivateKeyAndTransaction();
    auto privateKey = defaults.first;
    auto transaction = defaults.second;
    
    Data encodedUnsignedTransaction;
    transaction.encode(encodedUnsignedTransaction);

    ASSERT_EQ(hex(encodedUnsignedTransaction), "0000000000003039d891ad56056d9c01f18f43f58b5c784ad07a4a49cf3d1f11623804b5cba2c6bf00000002dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000700000000000003e8000000000000000000000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29cdbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db000000070000000000003039000000000000d43100000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29c00000002f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd15000000020000000300000007f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd1500000002000000030000000700000004deadbeef");
    std::vector<PrivateKey> keyRing = {privateKey};
    auto encodedSignedTransaction = Signer::sign(keyRing, transaction);
    ASSERT_EQ(hex(encodedSignedTransaction), "00000000000000003039d891ad56056d9c01f18f43f58b5c784ad07a4a49cf3d1f11623804b5cba2c6bf00000002dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000700000000000003e8000000000000000000000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29cdbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db000000070000000000003039000000000000d43100000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29c00000002f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd15000000020000000300000007f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd1500000002000000030000000700000004deadbeef00000002000000090000000244ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d0044ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d00000000090000000244ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d0044ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d00");
}

std::pair<PrivateKey, BaseTransaction> getPrivateKeyAndTransaction() {
    const auto privateKeyBytes = CB58::avalanche.decode("ewoqjP7PxY4yr3iLTpLisriqt94hdyDFNgchSxGGztUrTXtNN");
    const std::vector<uint8_t> privateKeyBytesNoChecksum(privateKeyBytes.begin(), privateKeyBytes.begin() + 32); // we just want the first 32 bytes
    const auto privateKey = PrivateKey(privateKeyBytesNoChecksum); 
    const auto publicKeyOne = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
    const auto addressOne = Address(publicKeyOne);

    std::vector<Address> spendableAddresses = {addressOne, addressOne, addressOne, addressOne, addressOne, addressOne, addressOne, addressOne, addressOne, addressOne};

    auto blockchainID = CB58::avalanche.decode("2eNy1mUFdmaxXNj1eQHUe7Np4gju9sJsEtWQ4MX3ToiNKuADed");
    Data blockchainIDBytes(blockchainID.begin(), blockchainID.begin() + BLOCKCHAIN_ID_SIZE); // we just want the first 32 bytes, no checksum
    uint32_t netID = 12345;
    auto assetID = parse_hex("0xdbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db"); 
    uint32_t baseTypeID = 0;
    Data memo = {0xde, 0xad, 0xbe, 0xef};
    auto amount = 1000;   
    auto locktime = 0;
    auto threshold = 1;

    auto addressesInOne = std::vector<uint32_t>{3, 7};
    auto txidOne = parse_hex("0xf1e1d1c1b1a191817161514131211101f0e0d0c0b0a090807060504030201000");
    auto utxoIndexOne = 5;
    auto coreInputOne = std::make_unique<SECP256k1TransferInput>(123456789, addressesInOne);
    auto inputOne = TransferableInput(txidOne, utxoIndexOne, assetID, std::move(coreInputOne), spendableAddresses);

    auto addressesInTwo = std::vector<uint32_t>{3, 7};
    auto txidTwo = parse_hex("0xf1e1d1c1b1a191817161514131211101f0e0d0c0b0a090807060504030201000");
    auto utxoIndexTwo = 5;
    auto coreInputTwo = std::make_unique<SECP256k1TransferInput>(123456789, addressesInTwo);
    auto inputTwo = TransferableInput(txidTwo, utxoIndexTwo, assetID, std::move(coreInputTwo), spendableAddresses);
    
    auto inputs = std::vector<TransferableInput>{inputOne, inputTwo};

    auto addressesOutOne = std::vector<Address>{addressOne};
    auto coreOutputOne = std::make_unique<SECP256k1TransferOutput>(12345, 54321, threshold, addressesOutOne);
    auto outputOne = TransferableOutput(assetID, std::move(coreOutputOne));

    auto addressesOutTwo = std::vector<Address>{addressOne};
    auto coreOutputTwo = std::make_unique<SECP256k1TransferOutput>(amount, locktime, threshold, addressesOutTwo);
    auto outputTwo = TransferableOutput(assetID, std::move(coreOutputTwo));

    auto outputs = std::vector<TransferableOutput>{outputOne, outputTwo};

    auto transaction = BaseTransaction(baseTypeID, netID, blockchainIDBytes, inputs, outputs, memo);

    return std::make_pair(privateKey, transaction);
}

std::vector<Address> generateAddressesForSigner() {
    // build some arbitrary addresses for this test
    std::vector<Address> arbitraryAddresses;
    {   
        auto privateKey = PrivateKey(parse_hex("0x98cb077f972feb0481f1d894f272c6a1e3c15e272a1658ff716444f465200070"));
        const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
        arbitraryAddresses.push_back(Address(publicKey));
    }
    {   
        auto privateKey = PrivateKey(parse_hex("0x88cb770f972feb0481f1d894f272c6a1e3c15e272a1658ff716444f465200070"));
        const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
        arbitraryAddresses.push_back(Address(publicKey));
    }
    {   
        auto privateKey = PrivateKey(parse_hex("0x78cb077f972fe00481f0d894f272c6a1e3c15e272a1658ff716444f465200070"));
        const auto publicKey = privateKey.getPublicKey(TWPublicKeyTypeSECP256k1);
        arbitraryAddresses.push_back(Address(publicKey));
    }
    return arbitraryAddresses;
}
