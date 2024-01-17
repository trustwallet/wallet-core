// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class ZenTests: XCTestCase {
    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "3a8e0a528f62f4ca2c77744c8a571def2845079b50105a9f7ef6b1b823def67a")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: true)
        let address = AnyAddress(publicKey: pubkey, coin: .zen)
        let addressFromString = AnyAddress(string: "znk19H1wcARcCa7TM6zgmJUbWoWWtZ8k5cg", coin: .zen)!

        XCTAssertEqual(pubkey.data.hexString, "02b4ac9056d20c52ac11b0d7e83715dd3eac851cfc9cb64b8546d9ea0d4bb3bdfe")
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSign() {
        let key = PrivateKey(data: Data(hexString: "3a8e0a528f62f4ca2c77744c8a571def2845079b50105a9f7ef6b1b823def67a")!)!
        let blockHash = Data(hexString: "81dc725fd33fada1062323802eefb54d3325d924d4297a692214560400000000")!
        let blockHeight = Int64(1147624)
        let utxos = [
            BitcoinUnspentTransaction.with {
                $0.outPoint.hash = Data(hexString: "a39e13b5ab406547e31284cd96fb40ed271813939c195ae7a86cd67fb8a4de62")!
                $0.outPoint.index = 0
                $0.outPoint.sequence = UINT32_MAX
                $0.script = Data(hexString: "76a914cf83669620de8bbdf2cefcdc5b5113195603c56588ac2081dc725fd33fada1062323802eefb54d3325d924d4297a69221456040000000003e88211b4")!
                $0.amount = 17600
            }
        ]

        // Plan
        let plan = BitcoinTransactionPlan.with {
            $0.amount = 10000
            $0.fee = 226
            $0.change = 7374
            $0.utxos = utxos
            $0.preblockhash = blockHash
            $0.preblockheight = blockHeight
        }

        let input = BitcoinSigningInput.with {
            $0.amount = 10000
            $0.toAddress = "zngBGZGKaaBamofSuFw5WMnvU2HQAtwGeb5"
            $0.changeAddress = "znk19H1wcARcCa7TM6zgmJUbWoWWtZ8k5cg"
            $0.hashType = BitcoinScript.hashTypeForCoin(coinType: .zen)
            $0.coinType = CoinType.zen.rawValue
            $0.privateKey = [key.data]
            $0.plan = plan
        }

        // Sign
        let output: BitcoinSigningOutput = AnySigner.sign(input: input, coin: .zen)
        XCTAssertEqual(output.error, .ok)
        XCTAssertEqual(output.encoded.hexString, "0100000001a39e13b5ab406547e31284cd96fb40ed271813939c195ae7a86cd67fb8a4de62000000006a473044022014d687c0bee0b7b584db2eecbbf73b545ee255c42b8edf0944665df3fa882cfe02203bce2412d93c5a56cb4806ddd8297ff05f8fc121306e870bae33377a58a02f05012102b4ac9056d20c52ac11b0d7e83715dd3eac851cfc9cb64b8546d9ea0d4bb3bdfeffffffff0210270000000000003f76a914a58d22659b1082d1fa8698fc51996b43281bfce788ac2081dc725fd33fada1062323802eefb54d3325d924d4297a69221456040000000003e88211b4ce1c0000000000003f76a914cf83669620de8bbdf2cefcdc5b5113195603c56588ac2081dc725fd33fada1062323802eefb54d3325d924d4297a69221456040000000003e88211b400000000")

    }
}
