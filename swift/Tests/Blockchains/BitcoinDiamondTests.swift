// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
            $0.preblockhash = Data(hexString: "4bfa9d92e1e99e72597e1e9162bdaaab624f1bb6faa83b2f46c6777caf8d6699")!
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
        XCTAssertEqual(output.encoded.hexString, "0c0000004bfa9d92e1e99e72597e1e9162bdaaab624f1bb6faa83b2f46c6777caf8d669901034f4667301711e8a69236a93476ed798f9c11aaae472da5b315191a0453461d000000006b483045022100da0146d41744f53d4af823d5901785014cbcad874941f72984d58a53f1bbeafa02207b4f77aa97bc3a08cf0ae5c6c36fe97d68496294286bbdaa9933e392fcb54fa0012102485a209514cc896f8ed736e205bc4c35bd5299ef3f9e84054475336b964c02a3ffffffff01cf440000000000001976a914b6adfbbf15c8f6fa53f1edb37054dce5c7c145c688ac00000000")
    }
}
