// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class AvalancheTests: XCTestCase {

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "98cb077f972feb0481f1d894f272c6a1e3c15e272a1658ff716444f465200070")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: false)
        let address = AnyAddress(publicKey: pubkey, coin: .avalancheXChain)
        let addressFromString = AnyAddress(string: "X-avax1apmh7wxg3js48fhacfv5y9md9065jxuft30vup", coin: .avalancheXChain)!

        XCTAssertEqual(pubkey.data.hexString, "04b33c917f2f6103448d7feb42614037d05928433cb25e78f01a825aa829bb3c27299a9fb02dd081d37876f145b8d3aa47fe9764145a9d90d4f941105d7de85c82")
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testAddressData() {
        let address = AnyAddress(string: "X-avax1q99lxu524h34ey36lzvqp0kzsylztp0j4ejsvm", coin: .avalancheXChain)!

        XCTAssertEqual(address.data.hexString, "014bf3728aade35c923af89800bec2813e2585f2")
    }

    func testCChainAddress() {
        let key = PrivateKey(data: Data(hexString: "98cb077f972feb0481f1d894f272c6a1e3c15e272a1658ff716444f465200070")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: false)
        let address = AnyAddress(publicKey: pubkey, coin: .avalancheCChain)
        let addressETH = AnyAddress(publicKey: pubkey, coin: .ethereum)

        XCTAssertEqual(address.description, addressETH.description)
        XCTAssertEqual(address.data.hexString, addressETH.data.hexString)
    }

    func testXpub() {
        let wallet = HDWallet(mnemonic: "chimney noodle canyon tunnel sample stuff scan symbol sight club net own arrive cause suffer purity manage squirrel boost diesel bring cement father slide", passphrase: "")!
        let xpub = wallet.getExtendedPublicKey(purpose: .bip44, coin: .avalancheXChain, version: .xpub)

        XCTAssertEqual(xpub, "xpub6CvdTKLRh3ehvVLR2f3M1GUTFesrz5zoYFbw32iZqRShmoDnxtfSaF7mdCvXwNRfTwce5RYEADGb6YAzhqEAujEkvjTod6s2WEkpUBJZwqf")
    }

    func testDecodeUTXO() {
        let nftUTXO = "0x0000283a7309eb1e2d2df00430de66b2b7abf77716bd06aadfa2fa957be1e748a9680000000a1096e1457b26e622f7b417bc1b41f4d2396fe1d7a1f99a91b9df5ae8f741be850000000b00000000000000a1187b226176616c616e636865223a7b2276657273696f6e223a312c2274797065223a2267656e65726963222c227469746c65223a224361707461696e222c22696d67223a2268747470733a2f2f636c6f7564666c6172652d697066732e636f6d2f697066732f516d50576e4156505770674466535548594b78735741353670714837616e50334e396b4a753571585a69394b5a72222c2264657363223a22227d7d000000000000000000000001000000010d2fbcab2f97ef944fb6a808da9dc4416884c17fe069be69"

        XCTAssertNil(AvalancheDecoder.decodeUTXO(data: Data(hexString: nftUTXO)!))

        let utxo = "0x0000da7ba5f0c8cc5d1c14c2279ffe77abe7c22d1e90f2b0575843d8458e82c7e5320000000021e67317cbc4be2aeb00677ad6462778a8f52274b9d605df2591b23027a87dff00000007000000003b7c458000000000000000000000000100000001b7bce5a6b2263881816034938606a12c014f8bf7363e9c39"
        let decoded = AvalancheDecoder.decodeUTXO(data: Data(hexString: utxo)!)!
        let expectedJSON: String = """
        {
            "addresses": ["b7bce5a6b2263881816034938606a12c014f8bf7"],
            "amount": 998000000,
            "asset": "21e67317cbc4be2aeb00677ad6462778a8f52274b9d605df2591b23027a87dff",
            "codec": 0,
            "hash": "da7ba5f0c8cc5d1c14c2279ffe77abe7c22d1e90f2b0575843d8458e82c7e532",
            "index": 0,
            "locktime": 0,
            "threshold": 1,
            "type": 7
        }
        """

        XCTAssertJSONEqual(decoded, expectedJSON)

        let address = AnyAddress(string: "X-avax1k77wtf4jycugrqtqxjfcvp4p9sq5lzlh5vl83n", coin: .avalancheXChain)
        XCTAssertEqual(address?.data.hexString, "b7bce5a6b2263881816034938606a12c014f8bf7")
    }

    func testSignSimpleTransfer() {
        let key = PrivateKey(data: Data(hexString: "56289e99c94b6912bfc12adc093c9b51124f0dc54ac7a766b2bc5ccf558d8027")!)!
        let blockchainID = Data(hexString: "d891ad56056d9c01f18f43f58b5c784ad07a4a49cf3d1f11623804b5cba2c6bf")!
        let pubkey = key.getPublicKeySecp256k1(compressed: true)
        let netID = UInt32(12345)
        let assetID = Data(hexString: "dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db")!
        let txID = Data(hexString: "f1e1d1c1b1a191817161514131211101f0e0d0c0b0a090807060504030201000")!
        let memo = Data(hexString: "deadbeef")!
        let amount = UInt64(1000)
        let locktime = UInt64(0)
        let threshold = UInt32(1)

        var transfer = AvalancheSimpleTransferTx()
        transfer.typeID = 0
        transfer.networkID = netID
        transfer.blockchainID = blockchainID
        transfer.amount = 180000000
        transfer.fee = 1200000
     
        transfer.toAddresses = [pubkey.data]
        transfer.changeAddresses = [pubkey.data]
        transfer.useMaxAmount = false

        transfer.outputTypeID = 0
        transfer.outputAssetID = assetID
        transfer.memo = memo
        transfer.locktime = 0
        transfer.threshold = 1

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
        transfer.inputs.append(inputOne)

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
        inputTwo.input = wrappedInputTwo
        transfer.inputs.append(inputTwo)

        var input = AvalancheSigningInput()
        input.privateKeys = [key.data]
        input.simpleTransferTx = transfer

        let output: AvalancheSigningOutput = AnySigner.sign(input: input, coin: .avalancheXChain)

        XCTAssertEqual(output.encoded.hexString, "00000000000000003039d891ad56056d9c01f18f43f58b5c784ad07a4a49cf3d1f11623804b5cba2c6bf00000002dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db000000070000000003eab5aa000000000000000000000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29cdbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db00000007000000000aba9500000000000000000000000001000000013cb7d3842e8cee6a0ebd09f1fe884f6861e1b29c00000002f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd15000000020000000300000007f1e1d1c1b1a191817161514131211101f0e0d0c0b0a09080706050403020100000000005dbcf890f77f49b96857648b72b77f9f82937f28a68704af05da0dc12ba53f2db0000000500000000075bcd1500000002000000030000000700000004deadbeef0000000200000009000000022302423a2710d8c5887c99aaad1fed8e3a104995b5dce6b4743e8adecf034c545d8a41afa3fe0308998a1a528f3514df2deec89a125176f1c412693a60939bbf012302423a2710d8c5887c99aaad1fed8e3a104995b5dce6b4743e8adecf034c545d8a41afa3fe0308998a1a528f3514df2deec89a125176f1c412693a60939bbf0100000009000000022302423a2710d8c5887c99aaad1fed8e3a104995b5dce6b4743e8adecf034c545d8a41afa3fe0308998a1a528f3514df2deec89a125176f1c412693a60939bbf012302423a2710d8c5887c99aaad1fed8e3a104995b5dce6b4743e8adecf034c545d8a41afa3fe0308998a1a528f3514df2deec89a125176f1c412693a60939bbf01")
    }
}
