// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import WalletCore

class BitcoinTransactionSignerTests: XCTestCase {
    override func setUp() {
        continueAfterFailure = false
    }

    func testSignP2WSH() throws {
        // set up input
        var input = BitcoinSigningInput.with {
            $0.hashType = BitcoinScript.hashTypeForCoin(coinType: .bitcoin)
            $0.amount = 1000
            $0.byteFee = 1
            $0.toAddress = "1Bp9U1ogV3A14FMvKbRJms7ctyso4Z4Tcx"
            $0.changeAddress = "1FQc5LdgGHMHEN9nwkjmz6tWkxhPpxBvBU"
        }

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

        // Plan
        let plan: BitcoinTransactionPlan = AnySigner.plan(input: input, coin: .bitcoin)

        XCTAssertEqual(plan.amount, 1000)
        XCTAssertEqual(plan.fee, 147)
        XCTAssertEqual(plan.change, 79)

        // Extend input with private key
        input.privateKey.append(Data(hexString: "ed00a0841cd53aedf89b0c616742d1d2a930f8ae2b0fb514765a17bb62c7521a")!)
        input.privateKey.append(Data(hexString: "619c335025c7f4012e556c2a58b2506e30b8511b53ade95ea316fd8c3286feb9")!)

        // Sign
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

    func testSignP2SH_P2WPKH() {
        let address = "3LGoLac9mtCwDy2q8PYyvwL8kMyrCWCYQW"
        let lockScript = BitcoinScript.lockScriptForAddress(address: address, coin: .bitcoin)
        let key = PrivateKey(data: Data(hexString: "e240ef3419d038577e48426c8c37c3c13bec1a0ed3f5270b82e7377bc48699dd")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: true)
        let utxos = [
            BitcoinUnspentTransaction.with {
                $0.outPoint.hash = Data(Data(hexString: "8b5f4861c6d4a4ea361aa4066d720067f73854d9a1b1d01e2b0e3c9e150bc5a3")!.reversed())
                $0.outPoint.index = 0
                $0.outPoint.sequence = UINT32_MAX
                $0.script = lockScript.data
                $0.amount = 54700
            }
        ]

        let plan = BitcoinTransactionPlan.with {
            $0.amount = 43980
            $0.fee = 10720
            $0.change = 0
            $0.utxos = utxos
        }

        // redeem p2wpkh nested in p2sh
        let scriptHash = lockScript.matchPayToScriptHash()!
        let input = BitcoinSigningInput.with {
            $0.toAddress = "3NqULUrjZ7NL36YtBGsSVzqr5q1x9CJWwu"
            $0.hashType = BitcoinScript.hashTypeForCoin(coinType: .bitcoin)
            $0.coinType = CoinType.bitcoin.rawValue
            $0.scripts = [
                scriptHash.hexString: BitcoinScript.buildPayToWitnessPubkeyHash(hash: pubkey.bitcoinKeyHash).data
            ]
            $0.privateKey = [key.data]
            $0.plan = plan
        }

        let output: BitcoinSigningOutput = AnySigner.sign(input: input, coin: .bitcoin)

        // https://blockchair.com/bitcoin/transaction/da2a9ce5d71ff7490bc9025e2888ca109b68ec0bd0e7d26195e1783305c00117
        XCTAssertEqual(output.encoded.hexString, "01000000000101a3c50b159e3c0e2b1ed0b1a1d95438f76700726d06a41a36eaa4d4c661485f8b00000000171600140a3cca78017f46ac23e463148adb7231aef81956ffffffff01ccab00000000000017a914e7f40472c54fc93078c5129568cf95c27be3b2c287024830450221008dc29a5430facd4078ad93e72517d87b298d7a73b55d2828acab040ccf713ed5022063a13e348655fa7cdcfff084380611629babf165607b529bcc35bf6ddfab1f8101210370386469db8302c3092955724f56bcca9a36f31df82655aa79be46b08744cd1200000000")
    }

    func testHashTypeForCoin() {
        XCTAssertEqual(BitcoinScript.hashTypeForCoin(coinType: .bitcoin), TWBitcoinSigHashTypeAll.rawValue)
        XCTAssertEqual(BitcoinScript.hashTypeForCoin(coinType: .bitcoinCash), 0x41)
        XCTAssertEqual(BitcoinScript.hashTypeForCoin(coinType: .bitcoinGold), 0x4f41)
    }
}
