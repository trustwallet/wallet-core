// Copyright © 2017-2019 Binance.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

typealias TWBitcoinSigningInput = TW_Bitcoin_Proto_SigningInput
typealias TWBitcoinSigningOutput = TW_Bitcoin_Proto_SigningOutput
typealias TWBitcoinUnspentTransaction = TW_Bitcoin_Proto_UnspentTransaction
typealias TWBitcoinTransactionPlan = TW_Bitcoin_Proto_TransactionPlan
typealias TWBitcoinOutPoint = TW_Bitcoin_Proto_OutPoint

class BitcoinTransactionSignerTests: XCTestCase {
    override func setUp() {
        continueAfterFailure = false
    }

    func testSignP2WSH() throws {
        var input = TWBitcoinSigningInput.with {
            $0.hashType = TWSignatureHashTypeAll
            $0.amount = 1000
            $0.byteFee = 1
            $0.toAddress = "1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx"
            $0.changeAddress = "1FQc5LdgGHMHEN9nwkjmz6tWkxhPpxBvBU"
        }

        input.privateKey.append(Data(hexString: "ed00a0841cd53aedf89b0c616742d1d2a930f8ae2b0fb514765a17bb62c7521a")!)
        input.privateKey.append(Data(hexString: "619c335025c7f4012e556c2a58b2506e30b8511b53ade95ea316fd8c3286feb9")!)

        input.scripts["593128f9f90e38b706c18623151e37d2da05c229"] = Data(hexString: "2103596d3451025c19dbbdeb932d6bf8bfb4ad499b95b6f88db8899efac102e5fc71ac")!

        let p2sh = BitcoinScript.buildPayToWitnessScriptHash(scriptHash: Data(hexString: "ff25429251b5a84f452230a3c75fd886b7fc5a7865ce4a7bb7a9d7c5be6da3db")!)
        let utxo0 = TWBitcoinUnspentTransaction.with {
            $0.script = p2sh.data
            $0.amount = 1226
            $0.outPoint.hash = Data(hexString: "0001000000000000000000000000000000000000000000000000000000000000")!
            $0.outPoint.index = 0
            $0.outPoint.sequence = UInt32.max
        }
        input.utxo.append(utxo0)

        let signer = BitcoinTransactionSigner(input: input)
        let plan = signer.plan()

        XCTAssertEqual(plan.amount, 1000)
        XCTAssertEqual(plan.fee, 226)
        XCTAssertEqual(plan.change, 0)

        let result = signer.sign()
        XCTAssertTrue(result.success, "Error signing: \(result.error)")
        XCTAssertEqual(result.objects.count, 1)

        let output = try TWBitcoinSigningOutput(unpackingAny: result.objects[0])
        let signedTx = output.transaction
        XCTAssertEqual(signedTx.version, 1)

        XCTAssertEqual(signedTx.inputs.count, 1)  // Only one UTXO available
        XCTAssertEqual(signedTx.inputs[0].script.hexString, "")

        XCTAssertEqual(signedTx.outputs.count, 1) // Exact amount
        XCTAssertEqual(signedTx.outputs[0].value, 1000)

        let encoded = output.encoded
        XCTAssertEqual(encoded.hexString, "01000000" +
            "0001" +
            "01" +
                "0001000000000000000000000000000000000000000000000000000000000000" + "00000000" + "00" + "ffffffff" +
            "01" +
                "e803000000000000" + "1976a914769bdff96a02f9135a1d19b749db6a78fe07dc9088ac" +
            "02" +
                "4730440220252e92b8757f1e5577c54ce5deb8072914c1f03333128777dee96ebceeb6a99b02202b7298789316779d0aa7595abeedc03054405c42ab9859e67d9253d2c9a0cdfa01232103596d3451025c" +
                "19dbbdeb932d6bf8bfb4ad499b95b6f88db8899efac102e5fc71ac" +
            "00000000"
        );
    }
}
