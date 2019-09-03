// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class RippleTests: XCTestCase {

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "9c3d42d0515f0406ed350ab2abf3eaf761f8907802469b64052ac17e2250ae13")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: true)
        let address = RippleAddress(publicKey: pubkey)

        XCTAssertEqual(address.description, "r36yxStAh7qgTQNHTzjZvXybCTzUFhrfav")
    }

    func testSigner() {
        let input = RippleSigningInput.with {
            $0.amount = 29_000_000
            $0.fee = 200_000
            $0.sequence = 1 // from account info api
            $0.account = "rDpysuumkweqeC7XdNgYNtzL5GxbdsmrtF"
            $0.destination = "rU893viamSnsfP3zjzM2KPxjqZjXSXK6VF"
            $0.privateKey = Data(hexString: "ba005cd605d8a02e3d5dfd04234cef3a3ee4f76bfbad2722d1fb5af8e12e6764")!
        }

        let output = RippleSigner.sign(input: input)
        XCTAssertEqual(output.encoded.hexString, "12000022800000002400000001614000000001ba8140684000000000030d407321026cc34b92cefb3a4537b3edb0b6044c04af27c01583c577823ecc69a9a21119b6744630440220067f20b3eebfc7107dd0bcc72337a236ac3be042c0469f2341d76694a17d4bb9022048393d7ee7dcb729783b33f5038939ddce1bb8337e66d752974626854556bbb681148400b6b6d08d5d495653d73eda6804c249a5148883148132e4e20aecf29090ac428a9c43f230a829220d")
    }
}
