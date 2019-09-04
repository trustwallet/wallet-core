// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class HarmonyTests: XCTestCase {
    func testSigner() {
        let input = TW_Harmony_Proto_SigningInput.with {
            $0.chainID = Data(hexString: "0x0190")!
            $0.nonce = Data(hexString: "0x09")!
            $0.gasPrice = Data(hexString: "0x00")!
            $0.gasLimit = Data(hexString: "0x5208")!
            $0.fromShardID = Data(hexString: "0x03")!
            $0.toShardID = Data(hexString: "0x02")!
            $0.payload = Data(hexString: "0x")!
            $0.toAddress = "0x587c66b4b973a7b231d02ebbc7e7d9f6c5a49ef2"
            $0.amount = Data(hexString: "0x0168d28e3f00280000")!
            $0.privateKey = Data(hexString: "0xe2f88b4974ae763ca1c2db49218802c2e441293a09eaa9ab681779e05d1b7b94")!
        }
        let output = HarmonySigner.sign(input: input)
        XCTAssertEqual(output.v.hexString, "0343")
        XCTAssertEqual(output.r.hexString, "f4757c9ffad127996f788fb388be3e3e03440f6980b36dc6cee7230e390f0c13")
        XCTAssertEqual(output.s.hexString, "42f0ff332bd552e8ad7a1cf6a0af4ebebfb1f8aae413c54d3464b9babba5f28d")
    }
}
