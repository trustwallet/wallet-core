// Copyright © 2017-2019 Binance.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

typealias RippleSigningInput = TW_Ripple_Proto_SigningInput
typealias RiipleSigningOutput = TW_Ripple_Proto_SigningOutput

class RippleTests: XCTestCase {

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "9c3d42d0515f0406ed350ab2abf3eaf761f8907802469b64052ac17e2250ae13")!)!
        let pubkey = key.getPublicKey(compressed: true)
        let address = RippleAddress(publicKey: pubkey)

        XCTAssertEqual(address?.description, "r36yxStAh7qgTQNHTzjZvXybCTzUFhrfav")
    }

    func testSigner() {
        let input = RippleSigningInput.with {
            $0.amount = 29_000_000
            $0.fee = 200_000
            $0.sequence = 1 // from account info api
            $0.account = "rDpysuumkweqeC7XdNgYNtzL5GxbdsmrtF"
            $0.destination = "rU893viamSnsfP3zjzM2KPxjqZjXSXK6VF"
            $0.privateKey = Data(hexString: "e008b363373ceedbec3c3e9b1c28c9ff455b0a6bd612b205b7ab750144c284d7")!
        }

        let output = RippleSigner.sign(input: input)
        XCTAssertEqual(output.encoded.hexString, "12000022800000002400000001614000000001ba8140684000000000030d40732103417452a634b07d2f8bd9f0bfa2a31e1fe828bf3fa77beecb0d0fc32ee18ecfe074473045022100e5d9cdc97625ed1eabb4e649e45b46fc2cec80f9f97b642e84bfa15ae8c52d9e02206c0992307a01cbceb559788f36fccb197a8af6fc271dd2733362887c16008a2681148400b6b6d08d5d495653d73eda6804c249a5148883148132e4e20aecf29090ac428a9c43f230a829220d")
    }
}
