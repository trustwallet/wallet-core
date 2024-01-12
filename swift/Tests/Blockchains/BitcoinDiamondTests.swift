// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class BitcoinDiamondTests: XCTestCase {
    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "d2b9f2846d3adcead910ee0124a3ba7ae29e8a4729787d27f9bea1f532928eee")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: true)
        let address = AnyAddress(publicKey: pubkey, coin: .bitcoinDiamond)
        let addressFromString = AnyAddress(string: "1G15VvshDxwFTnahZZECJfFwEkq9fP79o8", coin: .bitcoinDiamond)!

        XCTAssertEqual(pubkey.data.hexString, "02485a209514cc896f8ed736e205bc4c35bd5299ef3f9e84054475336b964c02a3")
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSign() {
        let key = PrivateKey(data: Data(hexString: "d2b9f2846d3adcead910ee0124a3ba7ae29e8a4729787d27f9bea1f532928eee")!)!

        let script = BitcoinScript.lockScriptForAddress(address: "1G15VvshDxwFTnahZZECJfFwEkq9fP79o8", coin: .bitcoinDiamond)
        let utxos = [
            BitcoinUnspentTransaction.with {
                $0.outPoint.hash = Data(hexString: "034f4667301711e8a69236a93476ed798f9c11aaae472da5b315191a0453461d")!
                $0.outPoint.index = 0
                $0.outPoint.sequence = UINT32_MAX
                $0.script = script.data
                $0.amount = 27615
            }
        ]

        let plan = BitcoinTransactionPlan.with {
            $0.amount = 17615
            $0.fee = 10000
            $0.change = 0
            $0.utxos = utxos
            $0.preblockhash = Data(hexString: "99668daf7c77c6462f3ba8fab61b4f62abaabd62911e7e59729ee9e1929dfa4b")!
        }

        let input = BitcoinSigningInput.with {
            $0.amount = 17615
            $0.toAddress = "1HevQVTSEc8cEpDm65UpfCdj5erd4xxBhx"
            $0.hashType = BitcoinScript.hashTypeForCoin(coinType: .bitcoinDiamond)
            $0.coinType = CoinType.bitcoinDiamond.rawValue
            $0.privateKey = [key.data]
            $0.plan = plan
        }

        let output: BitcoinSigningOutput = AnySigner.sign(input: input, coin: .bitcoinDiamond)
        XCTAssertEqual(output.error, .ok)
        XCTAssertEqual(output.encoded.hexString, "0c00000099668daf7c77c6462f3ba8fab61b4f62abaabd62911e7e59729ee9e1929dfa4b01034f4667301711e8a69236a93476ed798f9c11aaae472da5b315191a0453461d000000006a473044022078e0d3a9e1eb270ab02c15f8fcf1d3bfc95a324839690b7de4f011a4266132ff02204679e8103c4d3f0bb5192a5f53cc273732fd0e8392ab3b00dc708fd24d0160b3012102485a209514cc896f8ed736e205bc4c35bd5299ef3f9e84054475336b964c02a3ffffffff01cf440000000000001976a914b6adfbbf15c8f6fa53f1edb37054dce5c7c145c688ac00000000")
    }
}
