// Copyright © 2017-2020 Trust Wallet.
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



// These tests are modeled after the avalanchejs signer tests
// many example values are likewise modeled after the docs.avax.network tx serialization examples

TEST(AvalancheSigner, SignWithLocktimeThreshold) {
    const auto privateKeyOneBytes = CB58::avalanche.decode("ewoqjP7PxY4yr3iLTpLisriqt94hdyDFNgchSxGGztUrTXtNN");
    const std::vector<uint8_t> privateKeyOneBytesNoChecksum(privateKeyOneBytes.begin(), privateKeyOneBytes.begin() + 32); // we just want the first 32 bytes
    const auto privateKeyOne = PrivateKey(privateKeyOneBytesNoChecksum); 
    const auto publicKeyOne = privateKeyOne.getPublicKey(TWPublicKeyTypeSECP256k1);
    const auto addressOne = Address(publicKeyOne); // hopefully comes out as X-avax18jma8ppw3nhx5r4ap8clazz0dps7rv5u00z96u
    // const auto publicKeyTwo = PublicKey(parse_hex("0x02b33c917f2f6103448d7feb42614037d05928433cb25e78f01a825aa829bb3c27"), TWPublicKeyTypeSECP256k1);
    // const auto addressTwo = Address(publicKeyTwo); 
    // const auto publicKeyThree = PublicKey(parse_hex("0xc3344128e060128ede3523a24a461c8943ab0859"), TWPublicKeyTypeSECP256k1);
    // const auto addressThree = Address(publicKeyTwo); 

    std::vector<Address> spendableAddresses = {addressOne, addressOne, addressOne, addressOne, addressOne, addressOne, addressOne, addressOne, addressOne, addressOne};

    auto blockchainID = CB58::avalanche.decode("2eNy1mUFdmaxXNj1eQHUe7Np4gju9sJsEtWQ4MX3ToiNKuADed");
    Data blockchainIDBytes(blockchainID.begin(), blockchainID.begin() + 32); // we just want the first 32 bytes, no checksum
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
    auto coreInputOne = new SECP256k1TransferInput(123456789, addressesInOne);
    auto inputOne = TransferableInput(txidOne, utxoIndexOne, assetID, coreInputOne, spendableAddresses);

    auto addressesInTwo = std::vector<uint32_t>{3, 7};
    auto txidTwo = parse_hex("0xf1e1d1c1b1a191817161514131211101f0e0d0c0b0a090807060504030201000");
    auto utxoIndexTwo = 5;
    auto coreInputTwo = new SECP256k1TransferInput(123456789, addressesInTwo);
    auto inputTwo = TransferableInput(txidTwo, utxoIndexTwo, assetID, coreInputTwo, spendableAddresses);
    
    auto inputs = std::vector<TransferableInput>{inputOne, inputTwo};

    auto addressesOutOne = std::vector<Address>{addressOne};
    auto coreOutputOne = new SECP256k1TransferOutput(12345, 54321, threshold, addressesOutOne);
    auto outputOne = TransferableOutput(assetID, coreOutputOne);

    auto addressesOutTwo = std::vector<Address>{addressOne};
    auto coreOutputTwo = new SECP256k1TransferOutput(amount, locktime, threshold, addressesOutTwo);
    auto outputTwo = TransferableOutput(assetID, coreOutputTwo);

    auto outputs = std::vector<TransferableOutput>{outputOne, outputTwo};

    auto transaction = BaseTransaction(baseTypeID, netID, blockchainIDBytes, inputs, outputs, memo);
    Data encodedUnsignedTransaction;
    transaction.encode(encodedUnsignedTransaction);

    ASSERT_EQ(hex(encodedUnsignedTransaction), "0000000000003039d891ad56056d9c01f18f43f58b5c784ad07a4a49cf3d1f11623804b5cba2c6bf00000002dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000700000000000003e8000000000000000000000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29cdbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db000000070000000000003039000000000000d43100000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29c00000002f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd15000000020000000300000007f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd1500000002000000030000000700000004deadbeef");
    std::vector<PrivateKey> keyRing = {privateKeyOne};
    auto encodedSignedTransaction = Signer::sign(keyRing, transaction);
    ASSERT_EQ(hex(encodedSignedTransaction), "00000000000000003039d891ad56056d9c01f18f43f58b5c784ad07a4a49cf3d1f11623804b5cba2c6bf00000002dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000700000000000003e8000000000000000000000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29cdbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db000000070000000000003039000000000000d43100000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29c00000002f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd15000000020000000300000007f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd1500000002000000030000000700000004deadbeef00000002000000090000000244ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d0044ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d00000000090000000244ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d0044ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d00");
}

TEST(AvalancheSigner, SignWithoutAsofLocktimeThreshold) {
  /*
  test('Creation Tx2 without asof, locktime, threshold', () => {
    const txu:UnsignedTx = set.buildBaseTx(
      netid, blockchainID,
      new BN(9000), assetID,
      addrs3, addrs1, addrs1
    );
    const tx:Tx = txu.sign(keymgr1);
    const tx2:Tx = new Tx();
    tx2.fromBuffer(tx.toBuffer());
    expect(tx2.toBuffer().toString('hex')).toBe(tx.toBuffer().toString('hex'));
    expect(tx2.toString()).toBe(tx.toString());
  });
  */
}

TEST(AvalancheSigner, SignOperationTx) {
  /*
  test('Creation Tx3 using OperationTx', () => {
    const txu:UnsignedTx = set.buildNFTTransferTx(
      netid, blockchainID, 
      addrs3, addrs1, addrs2, nftutxoids, new BN(90), avaxAssetID, undefined,
      UnixNow(), UnixNow().add(new BN(50)), 1,
    );
    const tx:Tx = txu.sign(keymgr1);
    const tx2:Tx = new Tx();
    tx2.fromBuffer(tx.toBuffer());
    expect(tx2.toBuffer().toString('hex')).toBe(tx.toBuffer().toString('hex'));
  });
  */
}

TEST(AvalancheSigner, SignImportTx) {
/*
  test('Creation Tx4 using ImportTx', () => {
    const txu:UnsignedTx = set.buildImportTx(
      netid, blockchainID, addrs3, addrs1, addrs2, importUTXOs, bintools.cb58Decode(PlatformChainID), new BN(90), assetID,
      new UTF8Payload("hello world").getPayload(), UnixNow());
    const tx:Tx = txu.sign(keymgr1);
    const tx2:Tx = new Tx();
    tx2.fromBuffer(tx.toBuffer());
    expect(tx2.toBuffer().toString('hex')).toBe(tx.toBuffer().toString('hex'));
  });
*/
}

TEST(AvalancheSigner, SignExportTx) {
/*
  test('Creation Tx5 using ExportTx', () => {
    const txu:UnsignedTx = set.buildExportTx(
      netid, blockchainID, new BN(90), avaxAssetID,
      addrs3, addrs1, addrs2, bintools.cb58Decode(PlatformChainID), 
      undefined, undefined, new UTF8Payload("hello world").getPayload(), UnixNow()
    )
    const tx:Tx = txu.sign(keymgr1);
    const tx2:Tx = new Tx();
    tx2.fromBuffer(tx.toBuffer());
    expect(tx.toBuffer().toString('hex')).toBe(tx2.toBuffer().toString('hex'));
  });
*/
}

