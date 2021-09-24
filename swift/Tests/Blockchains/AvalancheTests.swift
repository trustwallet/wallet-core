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
//        let wallet = HDWallet(mnemonic: "", passphrase: "")!
//        let key = wallet.getKey(coin: .avalancheXChain, derivationPath: "m/44'/9000'/0'/0/4")

        let key = PrivateKey(data: Data(hexString: "56289e99c94b6912bfc12adc093c9b51124f0dc54ac7a766b2bc5ccf558d8027")!)!
        let address = AnyAddress(string: "X-avax1k77wtf4jycugrqtqxjfcvp4p9sq5lzlh5vl83n", coin: .avalancheXChain)!
        print("m/44'/9000'/0'/0/4 address: \(address.data.hexString)")

        let toAddress = AnyAddress(string: "X-avax1f0ee803c7jdue3slku7azyf78zs0fs04cxuvv9", coin: .avalancheXChain)!
        let blockchainID = Data(hexString: "ed5f38341e436e5d46e2bb00b45d62ae97d1b050c64bc634ae10626739e35c4b")! // CB58: 2oYMBNV4eNHyqk2fjjV5nVQLDbtmNJzq5s3qs3Lo6ftnC6FByM

        let pubkey = key.getPublicKeySecp256k1(compressed: true)
        let netID = UInt32(1)
        let assetID = Data(hexString: "21e67317cbc4be2aeb00677ad6462778a8f52274b9d605df2591b23027a87dff")!

        let txID = Data(hexString: "da7ba5f0c8cc5d1c14c2279ffe77abe7c22d1e90f2b0575843d8458e82c7e532")! // CB58: 2fDrbHifgfLG9oSy2en9FsZK1SGWXRLVQDmkHKQsB1ri43bTi3
        let memo = Data("hello".utf8)

        var transfer = AvalancheSimpleTransferTx.with {
            $0.typeID = 0
            $0.networkID = netID
            $0.blockchainID = blockchainID
            $0.amount = 10000000 // 0.01
            $0.fee = 1200000

            $0.toAddresses = [toAddress.data]
            $0.changeAddresses = [toAddress.data]
            $0.useMaxAmount = false

            $0.outputTypeID = 0
            $0.outputAssetID = assetID
            $0.memo = memo
            $0.locktime = 0
            $0.threshold = 1
        }

        let inputOne = AvalancheTransferableInput.with {
            $0.utxoIndex = 0
            $0.txID = txID
            $0.assetID = assetID
            $0.spendableAddresses = [pubkey.data]
            $0.input = AvalancheTransactionInput.with {
                $0.secpTransferInput = AvalancheSECP256K1TransferInput.with {
                    $0.amount = 998000000
                    $0.addressIndices = [0]
                }
            }
        }

        transfer.inputs.append(inputOne)

        let input = AvalancheSigningInput.with {
            $0.privateKeys = [key.data]
            $0.simpleTransferTx = transfer
        }

        let output: AvalancheSigningOutput = AnySigner.sign(input: input, coin: .avalancheXChain)

        XCTAssertEqual(output.encoded.hexString, "00000000000000000001ed5f38341e436e5d46e2bb00b45d62ae97d1b050c64bc634ae10626739e35c4b0000000221e67317cbc4be2aeb00677ad6462778a8f52274b9d605df2591b23027a87dff000000070000000000989680000000000000000000000001000000014bf393be38f49bccc61fb73dd1113e38a0f4c1f521e67317cbc4be2aeb00677ad6462778a8f52274b9d605df2591b23027a87dff00000007000000003ad15f80000000000000000000000001000000014bf393be38f49bccc61fb73dd1113e38a0f4c1f500000001da7ba5f0c8cc5d1c14c2279ffe77abe7c22d1e90f2b0575843d8458e82c7e5320000000021e67317cbc4be2aeb00677ad6462778a8f52274b9d605df2591b23027a87dff00000005000000003b7c458000000001000000000000000568656c6c6f000000010000000900000001a2fd0500b784683e56116ecddd31bac12751f46ac6867f20bdb2c9374f413bf50b9b429c45ad1225b28c59ff35eeff46eaa700ab3c178417ac16b3962a7ea5a900")
    }
}
