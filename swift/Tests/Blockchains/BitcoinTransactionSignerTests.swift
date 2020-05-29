// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class BitcoinTransactionSignerTests: XCTestCase {
    override func setUp() {
        continueAfterFailure = false
    }

    func testSignP2WSH() throws {
        var input = BitcoinSigningInput.with {
            $0.hashType = BitcoinSigHashType.all.rawValue
            $0.amount = 1000
            $0.byteFee = 1
            $0.toAddress = "1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx"
            $0.changeAddress = "1FQc5LdgGHMHEN9nwkjmz6tWkxhPpxBvBU"
        }

        input.privateKey.append(Data(hexString: "ed00a0841cd53aedf89b0c616742d1d2a930f8ae2b0fb514765a17bb62c7521a")!)
        input.privateKey.append(Data(hexString: "619c335025c7f4012e556c2a58b2506e30b8511b53ade95ea316fd8c3286feb9")!)

        input.scripts["593128f9f90e38b706c18623151e37d2da05c229"] = Data(hexString: "2103596d3451025c19dbbdeb932d6bf8bfb4ad499b95b6f88db8899efac102e5fc71ac")!

        let p2sh = BitcoinScript.buildPayToWitnessScriptHash(scriptHash: Data(hexString: "ff25429251b5a84f452230a3c75fd886b7fc5a7865ce4a7bb7a9d7c5be6da3db")!)
        let utxo0 = BitcoinUnspentTransaction.with {
            $0.script = p2sh.data
            $0.amount = 1226
            $0.outPoint.hash = Data(hexString: "0001000000000000000000000000000000000000000000000000000000000000")!
            $0.outPoint.index = 0
            $0.outPoint.sequence = UInt32.max
        }
        input.utxo.append(utxo0)

        let plan: BitcoinTransactionPlan = AnySigner.plan(input: input, coin: .bitcoin)

        XCTAssertEqual(plan.amount, 1000)
        XCTAssertEqual(plan.fee, 147)
        XCTAssertEqual(plan.change, 79)

        let output: BitcoinSigningOutput = AnySigner.sign(input: input, coin: .bitcoin)
        XCTAssertTrue(output.error.isEmpty)

        let signedTx = output.transaction
        XCTAssertEqual(signedTx.version, 1)

        let txId = output.transactionID
        XCTAssertEqual(txId, "dc60991ff61a6061f55854ce6fb3203b7c8291ed7b2ce799040114c608391583")

        XCTAssertEqual(signedTx.inputs.count, 1)  // Only one UTXO available
        XCTAssertEqual(signedTx.inputs[0].script.hexString, "")

        XCTAssertEqual(signedTx.outputs.count, 2) // Exact amount
        XCTAssertEqual(signedTx.outputs[0].value, 1000)
        XCTAssertEqual(signedTx.outputs[1].value, 79)

        let encoded = output.encoded
        let witnessHash = Data(Hash.sha256SHA256(data: encoded).reversed())
        XCTAssertEqual(witnessHash.hexString, "ec57de0d46eb45e8019b82e388e458e72fb834dba971e3a45ff8fa7bb7bdb799")
        XCTAssertEqual(encoded.hexString,
            "01000000" + // version
            "0001" + // marker & flag
            "01" + // inputs
                "0001000000000000000000000000000000000000000000000000000000000000" + "00000000" + "00" + "ffffffff" +
            "02" + // outputs
                "e803000000000000" + "19" + "76a914769bdff96a02f9135a1d19b749db6a78fe07dc9088ac" +
                "4f00000000000000" + "19" + "76a9149e089b6889e032d46e3b915a3392edfd616fb1c488ac" +
            // witness
                "02" +
                    "48" + "30450221009eefc1befe96158f82b74e6804f1f713768c6172636ca11fcc975c316ea86f75022057914c48bc24f717498b851a47a2926f96242e3943ebdf08d5a97a499efc8b9001" +
                    "23" + "2103596d3451025c19dbbdeb932d6bf8bfb4ad499b95b6f88db8899efac102e5fc71ac" +
            "00000000" // nLockTime
        )
    }
}
