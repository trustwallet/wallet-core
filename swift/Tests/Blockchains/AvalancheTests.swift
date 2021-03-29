// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class AvalancheTests: XCTestCase {

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "0x98cb077f972feb0481f1d894f272c6a1e3c15e272a1658ff716444f465200070")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: false)
        let address = AnyAddress(publicKey: pubkey, coin: .avalancheXChain)
        let addressFromString = AnyAddress(string: "X-avax1apmh7wxg3js48fhacfv5y9md9065jxuft30vup", coin: .avalancheXChain)!

        XCTAssertEqual(pubkey.data.hexString, "0x02b33c917f2f6103448d7feb42614037d05928433cb25e78f01a825aa829bb3c27")
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSign() {
        let key = PrivateKey(data: Data(hexString: "0x56289e99c94b6912bfc12adc093c9b51124f0dc54ac7a766b2bc5ccf558d8027")!)!
        let blockchainID = Data(hexString: "0xd891ad56056d9c01f18f43f58b5c784ad07a4a49cf3d1f11623804b5cba2c6bf")!
        let pubkey = key.getPublicKeySecp256k1(compressed: false)
        let address = AnyAddress(publicKey: pubkey, coin: .avalancheXChain)
        let netID = UInt32(12345)
        let assetID = Data(hexString: "0xdbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db")!
        let txID = Data(hexString: "0xf1e1d1c1b1a191817161514131211101f0e0d0c0b0a090807060504030201000")!
        let memo = Data(hexString: "0xdeadbeef")!
        let amount = UInt64(1000)
        let locktime = UInt64(0)
        let threshold = UInt32(1)

        
        var baseTx = AvalancheBaseTx()
        baseTx.typeID = 0
        baseTx.networkID = netID
        baseTx.blockchainID = blockchainID
        baseTx.memo = memo

        var coreInputOne = AvalancheSECP256K1TransferInput()
        coreInputOne.amount = 123456789
        coreInputOne.addressIndices = [3, 7]
        var wrappedInputOne = AvalancheTransactionInput()
        wrappedInputOne.secpTransferInput = coreInputOne
        var inputOne = AvalancheTransferableInput()
        inputOne.utxoIndex = 5
        inputOne.txID = txID
        inputOne.assetID = assetID
        inputOne.spendableAddresses = [pubkey.data, pubkey.data, pubkey.data, pubkey.data, pubkey.data, pubkey.data, pubkey.data, pubkey.data]
        inputOne.input = wrappedInputOne
        
        var coreInputTwo = AvalancheSECP256K1TransferInput()
        coreInputTwo.amount = 123456789
        coreInputTwo.addressIndices = [3, 7]
        var wrappedInputTwo = AvalancheTransactionInput()
        wrappedInputTwo.secpTransferInput = coreInputTwo
        var inputTwo = AvalancheTransferableInput()
        inputTwo.utxoIndex = 5
        inputTwo.txID = txID
        inputTwo.assetID = assetID
        inputTwo.spendableAddresses = [pubkey.data, pubkey.data, pubkey.data, pubkey.data, pubkey.data, pubkey.data, pubkey.data, pubkey.data]
        inputTwo.input = wrappedInputOne
        
        var coreOutputOne = AvalancheSECP256K1TransferOutput()
        coreOutputOne.amount = 12345
        coreOutputOne.locktime = 54321
        coreOutputOne.threshold = threshold
        coreOutputOne.addresses = [pubkey.data]
        var wrappedOutputOne = AvalancheTransactionOutput()
        wrappedOutputOne.secpTransferOutput = coreOutputOne
        var outputOne = AvalancheTransferableOutput()
        outputOne.assetID = assetID
        outputOne.output = wrappedOutputOne
        
        var coreOutputTwo = AvalancheSECP256K1TransferOutput()
        coreOutputTwo.amount = amount
        coreOutputTwo.locktime = locktime
        coreOutputTwo.threshold = threshold
        coreOutputTwo.addresses = [pubkey.data]
        var wrappedOutputTwo = AvalancheTransactionOutput()
        wrappedOutputTwo.secpTransferOutput = coreOutputTwo
        var outputTwo = AvalancheTransferableOutput()
        outputTwo.assetID = assetID
        outputTwo.output = wrappedOutputTwo

        var input = AvalancheSigningInput()
        input.privateKeys = [key.data]
        let inputTx = AvalancheUnsignedTx.with {
            $0.baseTx = baseTx
        }
        input.inputTx = inputTx

        let output: AvalancheSigningOutput = AnySigner.sign(input: input, coin: .avalanche)

        XCTAssertEqual(output.encoded.hexString, "00000000000000003039d891ad56056d9c01f18f43f58b5c784ad07a4a49cf3d1f11623804b5cba2c6bf00000002dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000700000000000003e8000000000000000000000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29cdbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db000000070000000000003039000000000000d43100000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29c00000002f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd15000000020000000300000007f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd1500000002000000030000000700000004deadbeef00000002000000090000000244ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d0044ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d00000000090000000244ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d0044ef527f47cab3ed82eb267c27c04869e46531b05db643f5bc97da21148afe161f17634a90f4e22adb810b472062f7e809dde19059fa7048f9972a481fe9390d00")

    }
}
