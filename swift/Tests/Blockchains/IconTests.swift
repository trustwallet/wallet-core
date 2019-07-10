// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class IconTests: XCTestCase {

    func testInvalid() {
        XCTAssertNil(IconAddress(string: "abc"))
        XCTAssertNil(IconAddress(string: "dshadghasdghsadadsadjsad"))

        XCTAssertFalse(IconAddress.isValidString(string: "abc"))
        XCTAssertFalse(IconAddress.isValidString(string: "0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed"))
    }

    func testIsValid() {
        XCTAssertTrue(IconAddress.isValidString(string: "hx116f042497e5f34268b1b91e742680f84cf4e9f3"))
        XCTAssertTrue(IconAddress.isValidString(string: "cx116f042497e5f34268b1b91e742680f84cf4e9f3"))
    }

    func testFromPrivateKey() {
        let privateKey = PrivateKey(data: Data(hexString: "94d1a980d5e528067d44bf8a60d646f556e40ca71e17cd4ead2d56f89e4bd20f")!)!
        let address = IconAddress(publicKey: privateKey.getPublicKeySecp256k1(compressed: false), type: .address)
        XCTAssertEqual(address.description, "hx98c0832ca5bd8e8bf355ca9491888aa9725c2c48")
    }

    func testDescription() {
        let address = IconAddress(string: "hx116f042497e5f34268b1b91e742680f84cf4e9f3")!
        XCTAssertEqual(address.description, "hx116f042497e5f34268b1b91e742680f84cf4e9f3")
    }

    func testSigning() {
        let privateKey = PrivateKey(data: Data(hexString: "2d42994b2f7735bbc93a3e64381864d06747e574aa94655c516f9ad0a74eed79")!)!
        let input = IconSigningInput.with {
            $0.fromAddress = "hxbe258ceb872e08851f1f59694dac2558708ece11"
            $0.toAddress = "hx5bfdb090f43a808005ffc27c25b213145e80b7cd"
            $0.value = Data(hexString: "0de0b6b3a7640000")!
            $0.stepLimit = Data(hexString: "012345")!
            $0.networkID = Data([0x1])
            $0.timestamp = 1516942975500598
            $0.privateKey = privateKey.data
            $0.nonce = Data([0x1])
        }
        let output = IconSigner.sign(input: input)
        // swiftlint:disable line_length
        let expected = """
{"from":"hxbe258ceb872e08851f1f59694dac2558708ece11","nid":"0x1","nonce":"0x1","signature":"xR6wKs+IA+7E91bT8966jFKlK5mayutXCvayuSMCrx9KB7670CsWa0B7LQzgsxU0GLXaovlAT2MLs1XuDiSaZQE=","stepLimit":"0x12345","timestamp":"0x563a6cf330136","to":"hx5bfdb090f43a808005ffc27c25b213145e80b7cd","value":"0xde0b6b3a7640000","version":"0x3"}
"""
        // swiftlint:enable line_length
        XCTAssertEqual(output.encoded, expected)
    }
}
