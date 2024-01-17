// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class OasisTests: XCTestCase {

    func testAddress() {

        let key = PrivateKey(data: Data(hexString: "4f8b5676990b00e23d9904a92deb8d8f428ff289c8939926358f1d20537c21a0")!)!
        let pubkey = key.getPublicKeyEd25519()
        let address = AnyAddress(publicKey: pubkey, coin: .oasis)
        let addressFromString = AnyAddress(string: "oasis1qzawzy5kaa2xgphenf3r0f5enpr3mx5dps559yxm", coin: .oasis)!

        XCTAssertEqual(pubkey.data.hexString, "93d8f8a455f50527976a8aa87ebde38d5606efa86cb985d3fb466aff37000e3b")
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSign() {
        let privateKey = PrivateKey(data: Data(hexString: "0x4f8b5676990b00e23d9904a92deb8d8f428ff289c8939926358f1d20537c21a0")!)!
        let input = OasisSigningInput.with {
            $0.privateKey = privateKey.data
            $0.transfer = OasisTransferMessage.with {
                $0.to = "oasis1qrrnesqpgc6rfy2m50eew5d7klqfqk69avhv4ak5"
                $0.gasPrice = 0
                $0.gasAmount = "0"
                $0.amount = "10000000"
                $0.nonce = 0
                $0.context = "oasis-core/consensus: tx for chain a245619497e580dd3bc1aa3256c07f68b8dcc13f92da115eadc3b231b083d3c4"
            }
        }

        let output: OasisSigningOutput = AnySigner.sign(input: input, coin: .oasis)

        XCTAssertEqual(output.encoded.hexString, "a2697369676e6174757265a2697369676e617475726558406e51c18c9b2015c9b49414b3307336597f51ff331873d214ce2db81c9651a34d99529ccaa294a39ccd01c6b0bc2c2239d87c624e5ba4840cf99ac8f9283e240c6a7075626c69635f6b6579582093d8f8a455f50527976a8aa87ebde38d5606efa86cb985d3fb466aff37000e3b73756e747275737465645f7261775f76616c7565585ea463666565a2636761730066616d6f756e74410064626f6479a262746f5500c73cc001463434915ba3f39751beb7c0905b45eb66616d6f756e744400989680656e6f6e636500666d6574686f64707374616b696e672e5472616e73666572")
    } 
}
