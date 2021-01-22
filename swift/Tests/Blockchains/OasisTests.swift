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
                $0.method = "staking.Transfer"
                $0.gasPrice = 0
                $0.gasAmount = Data("0".utf8)
                $0.amount = Data("10000000".utf8)
                $0.nonce = 0
                $0.context = "oasis-core/consensus: tx for chain bc1c715319132305795fa86bd32e93291aaacbfb5b5955f3ba78bdba413af9e1"
            }
        }

        let output: OasisSigningOutput = AnySigner.sign(input: input, coin: .oasis)

        XCTAssertEqual(output.encoded.hexString, "82880000420030404900313030303030303000707374616b696e672e5472616e7366657278636f617369732d636f72652f636f6e73656e7375733a20747820666f7220636861696e20626331633731353331393133323330353739356661383662643332653933323931616161636266623562353935356633626137386264626134313361663965315840cfe091da8d2d148a053591f435d711adf6f79105a73343bdfe32a30520c5a421d32429a52eb2d5d70ef3d161712abf3696d4072d4289c9771a6e78f5c0299d0b")
    } 
}
