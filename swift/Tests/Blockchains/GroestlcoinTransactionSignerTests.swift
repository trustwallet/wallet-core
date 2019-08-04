// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class GroestlcoinTransactionSignerTests: XCTestCase {
    override func setUp() {
        continueAfterFailure = false
    }

    func testSignP2WPKH() throws {
        var input = BitcoinSigningInput.with {
            $0.hashType = BitcoinSigHashType.all.rawValue
            $0.amount = 2500
            $0.byteFee = 1
            $0.toAddress = "31inaRqambLsd9D7Ke4USZmGEVd3PHkh7P"
            $0.changeAddress = "Fj62rBJi8LvbmWu2jzkaUX1NFXLEqDLoZM"
        }
        input.privateKey.append(Data(hexString: "dc334e7347f2f9f72fce789b11832bdf78adf0158bc6617e6d2d2a530a0d4bc6")!)

        let utxo0 = BitcoinUnspentTransaction.with {
            $0.script = Data(hexString: "00147557920fbc32a1ef4ef26bae5e8ce3f95abf09ce")!
            $0.amount = 4774
            $0.outPoint.hash = Data(hexString: "9568b09e6c6d940302ec555a877c9e5f799de8ee473e18d3a19ae14478cc4e8f")!
            $0.outPoint.index = 1
            $0.outPoint.sequence = UInt32.max
        }
        input.utxo.append(utxo0)

        let signer = GroestlcoinTransactionSigner(input: input)
        let plan = signer.plan()

        XCTAssertEqual(plan.amount, 2500)
        XCTAssertEqual(plan.fee, 226)
        XCTAssertEqual(plan.change, 2048)

        // https://blockbook.groestlcoin.org/tx/40b539c578934c9863a93c966e278fbeb3e67b0da4eb9e3030092c1b717e7a64
        let result = signer.sign()
        XCTAssertTrue(result.success, "Error signing: \(result.error)")
        XCTAssertEqual(result.objects.count, 1)

        let output = try BitcoinSigningOutput(unpackingAny: result.objects[0])
        let signedTx = output.transaction
        XCTAssertEqual(signedTx.version, 1)

        let txId = output.transactionID
        XCTAssertEqual(txId, "40b539c578934c9863a93c966e278fbeb3e67b0da4eb9e3030092c1b717e7a64")

        XCTAssertEqual(signedTx.inputs.count, 1)
        XCTAssertEqual(signedTx.inputs[0].script.hexString, "")

        XCTAssertEqual(signedTx.outputs.count, 2)
        XCTAssertEqual(signedTx.outputs[0].value, 2500)
        XCTAssertEqual(signedTx.outputs[1].value, 2048)

        let encoded = output.encoded
        let witnessHash = Data(Hash.sha256(data: encoded).reversed())
        XCTAssertEqual(witnessHash.hexString, "9bce4e2e16646416b6105a4c161a005b53b2afedfc56b89071141199fbcd01fb")
        XCTAssertEqual(encoded.hexString, "010000000001019568b09e6c6d940302ec555a877c9e5f799de8ee473e18d3a19ae14478cc4e8f0100000000ffffffff02c40900000000000017a9140055b0c94df477ee6b9f75185dfc9aa8ce2e52e48700080000000000001976a91498af0aaca388a7e1024f505c033626d908e3b54a88ac024830450221009bbd0228dcb7343828633ded99d216555d587b74db40c4a46f560187eca222dd022032364cf6dbf9c0213076beb6b4a20935d4e9c827a551c3f6f8cbb22d8b464467012102e9c9b9b76e982ad8fa9a7f48470eafbeeba9bf6d287579318c517db5157d936e00000000");
    }

    func testSignP2PKH() throws {
        var input = BitcoinSigningInput.with {
            $0.hashType = BitcoinSigHashType.all.rawValue
            $0.amount = 2500
            $0.byteFee = 1
            $0.toAddress = "grs1qw4teyraux2s77nhjdwh9ar8rl9dt7zww8r6lne"
            $0.changeAddress = "31inaRqambLsd9D7Ke4USZmGEVd3PHkh7P"
        }
        input.privateKey.append(Data(hexString: "3c3385ddc6fd95ba7282051aeb440bc75820b8c10db5c83c052d7586e3e98e84")!)

        let utxo0 = BitcoinUnspentTransaction.with {
            $0.script = Data(hexString: "76a91498af0aaca388a7e1024f505c033626d908e3b54a88ac")!
            $0.amount = 5000
            $0.outPoint.hash = Data(hexString: "9568b09e6c6d940302ec555a877c9e5f799de8ee473e18d3a19ae14478cc4e8f")!
            $0.outPoint.index = 0
            $0.outPoint.sequence = UInt32.max
        }
        input.utxo.append(utxo0)

        let signer = GroestlcoinTransactionSigner(input: input)
        let plan = signer.plan()

        XCTAssertEqual(plan.amount, 2500)
        XCTAssertEqual(plan.fee, 226)
        XCTAssertEqual(plan.change, 2274)

        // https://blockbook.groestlcoin.org/tx/74a0dd12bc178cfcc1e0982a2a5b2c01a50e41abbb63beb031bcd21b3e28eac0
        let result = signer.sign()
        XCTAssertTrue(result.success, "Error signing: \(result.error)")
        XCTAssertEqual(result.objects.count, 1)

        let output = try BitcoinSigningOutput(unpackingAny: result.objects[0])
        let signedTx = output.transaction
        XCTAssertEqual(signedTx.version, 1)

        let txId = output.transactionID
        XCTAssertEqual(txId, "74a0dd12bc178cfcc1e0982a2a5b2c01a50e41abbb63beb031bcd21b3e28eac0")

        XCTAssertEqual(signedTx.inputs.count, 1)
        XCTAssertEqual(signedTx.inputs[0].script.hexString, "47304402202163ab98b028aa13563f0de00b785d6df81df5eac0b7c91d23f5be7ea674aa3702202bf6cd7055c6f8f697ce045b1a4f9b997cf6e5761a661d27696ac34064479d19012103b85cc59b67c35851eb5060cfc3a759a482254553c5857075c9e247d74d412c91")

        XCTAssertEqual(signedTx.outputs.count, 2)
        XCTAssertEqual(signedTx.outputs[0].value, 2500)
        XCTAssertEqual(signedTx.outputs[1].value, 2274)

        let encoded = output.encoded
        XCTAssertEqual(encoded.hexString, "01000000019568b09e6c6d940302ec555a877c9e5f799de8ee473e18d3a19ae14478cc4e8f000000006a47304402202163ab98b028aa13563f0de00b785d6df81df5eac0b7c91d23f5be7ea674aa3702202bf6cd7055c6f8f697ce045b1a4f9b997cf6e5761a661d27696ac34064479d19012103b85cc59b67c35851eb5060cfc3a759a482254553c5857075c9e247d74d412c91ffffffff02c4090000000000001600147557920fbc32a1ef4ef26bae5e8ce3f95abf09cee20800000000000017a9140055b0c94df477ee6b9f75185dfc9aa8ce2e52e48700000000");
    }

    func testSignP2SH_P2WPKH() throws {
        var input = BitcoinSigningInput.with {
            $0.hashType = BitcoinSigHashType.all.rawValue
            $0.amount = 5000
            $0.byteFee = 1
            $0.toAddress = "Fj62rBJi8LvbmWu2jzkaUX1NFXLEqDLoZM"
            $0.changeAddress = "grs1qw4teyraux2s77nhjdwh9ar8rl9dt7zww8r6lne"
        }
        input.privateKey.append(Data(hexString: "302fc195a8fc96c5a581471e67e4c1ac2efda252f76ad5c77a53764c70d58f91")!)

        input.scripts["0055b0c94df477ee6b9f75185dfc9aa8ce2e52e4"] = Data(hexString: "00142fc7d70acef142d1f7b5ef2f20b1a9b759797674")!

        let utxo0 = BitcoinUnspentTransaction.with {
            $0.script = Data(hexString: "a9140055b0c94df477ee6b9f75185dfc9aa8ce2e52e487")!
            $0.amount = 10000
            $0.outPoint.hash = Data(hexString: "fdae0772d7d1d33804a6b1ca0e391668b116bb7a70028427d3d82232189ce863")!
            $0.outPoint.index = 0
            $0.outPoint.sequence = UInt32.max
        }
        input.utxo.append(utxo0)

        let signer = GroestlcoinTransactionSigner(input: input)
        let plan = signer.plan()

        XCTAssertEqual(plan.amount, 5000)
        XCTAssertEqual(plan.fee, 226)
        XCTAssertEqual(plan.change, 4774)

        // https://blockbook.groestlcoin.org/tx/8f4ecc7844e19aa1d3183e47eee89d795f9e7c875a55ec0203946d6c9eb06895
        let result = signer.sign()
        XCTAssertTrue(result.success, "Error signing: \(result.error)")
        XCTAssertEqual(result.objects.count, 1)

        let output = try BitcoinSigningOutput(unpackingAny: result.objects[0])
        let signedTx = output.transaction
        XCTAssertEqual(signedTx.version, 1)

        let txId = output.transactionID
        XCTAssertEqual(txId, "8f4ecc7844e19aa1d3183e47eee89d795f9e7c875a55ec0203946d6c9eb06895")

        XCTAssertEqual(signedTx.inputs.count, 1)
        XCTAssertEqual(signedTx.inputs[0].script.hexString, "1600142fc7d70acef142d1f7b5ef2f20b1a9b759797674")

        XCTAssertEqual(signedTx.outputs.count, 2)
        XCTAssertEqual(signedTx.outputs[0].value, 5000)
        XCTAssertEqual(signedTx.outputs[1].value, 4774)

        let encoded = output.encoded
        let witnessHash = Data(Hash.sha256(data: encoded).reversed())
        XCTAssertEqual(witnessHash.hexString, "940dde32c750c826562e72bbb8df6c9840f32f34e31b032f92b7f27f71a83de3")
        XCTAssertEqual(encoded.hexString, "01000000000101fdae0772d7d1d33804a6b1ca0e391668b116bb7a70028427d3d82232189ce86300000000171600142fc7d70acef142d1f7b5ef2f20b1a9b759797674ffffffff0288130000000000001976a91498af0aaca388a7e1024f505c033626d908e3b54a88aca6120000000000001600147557920fbc32a1ef4ef26bae5e8ce3f95abf09ce024730440220614df57babf74029afaa6dda202afa47d3555cca7a0f20a22e466aeb7029e7d002207974b4c16f346811aff6720d09b9c58d0c4e01e8d258c3d203cc3c1ad228c61a012102fb6ad115761ea928f1367befb2bee79c0b3497314b45e0b734cd150f0601706c00000000");
    }

}
