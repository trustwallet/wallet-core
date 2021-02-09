// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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

        XCTAssertEqual(output.encoded.hexString, "a273756e747275737465645f7261775f76616c7565585ea4656e6f6e636500666d6574686f64707374616b696e672e5472616e7366657263666565a2636761730066616d6f756e74410064626f6479a262746f5500c73cc001463434915ba3f39751beb7c0905b45eb66616d6f756e744400989680697369676e6174757265a26a7075626c69635f6b6579582093d8f8a455f50527976a8aa87ebde38d5606efa86cb985d3fb466aff37000e3b697369676e61747572655840e331ce731ed819106586152b13cd98ecf3248a880bdc71174ee3d83f6d5f3f8ee8fc34c19b22032f2f1e3e06d382720125d7a517fba9295c813228cc2b63170b")
    } 
}
