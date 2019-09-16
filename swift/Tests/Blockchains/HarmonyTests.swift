// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class HarmonyTests: XCTestCase {
    func testSigner() {
        let localNet = "0x02"
        let input = TW_Harmony_Proto_SigningInput.with {
            $0.chainID = Data(hexString: localNet)!
            $0.nonce = Data(hexString: "0x9")!
            $0.gasPrice = Data(hexString: "0x")!
            $0.gasLimit = Data(hexString: "0x5208")!
            $0.fromShardID = Data(hexString: "0x1")!
            $0.toShardID = Data(hexString: "0x0")!
            $0.payload = Data(hexString: "0x")!
            $0.toAddress = "one1d2rngmem4x2c6zxsjjz29dlah0jzkr0k2n88wc"
            $0.amount = Data(hexString: "0x4c53ecdc18a60000")!
            $0.privateKey = Data(hexString: "0xb578822c5c718e510f67a9e291e9c6efdaf753f406020f55223b940e1ddb282e")!
        }
        let output = HarmonySigner.sign(input: input)
        let e1 = "f86909808252080180946a87346f3ba9958d08d09484a2b7fdbbe42b0df6884c53ecdc18a600008028a0325aed6caa01a5235b"
        let e2 = "7a508c8ab67f0c43946b05a1ea6a3e0628de4033fe372da06c19085d3376c30f6dc47cec795991cd37d6d0ebddfa633b0a8f494bc19cd01b"
        XCTAssertEqual(output.encoded.hexString, e1 + e2)
        XCTAssertEqual(output.v.hexString, "28")
        XCTAssertEqual(output.r.hexString, "325aed6caa01a5235b7a508c8ab67f0c43946b05a1ea6a3e0628de4033fe372d")
        XCTAssertEqual(output.s.hexString, "6c19085d3376c30f6dc47cec795991cd37d6d0ebddfa633b0a8f494bc19cd01b")
    }
}
