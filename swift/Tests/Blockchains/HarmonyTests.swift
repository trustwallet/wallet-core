// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

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

    func testStakingDelegate() {
        let localNet = "0x02"
        let delegate = TW_Harmony_Proto_DirectiveDelegate.with {
            $0.delegatorAddress = "one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9"
            $0.validatorAddress = "one1a0x3d6xpmr6f8wsyaxd9v36pytvp48zckswvv9"
            $0.amount = Data(hexString: "0xa")!
        }
        let input = TW_Harmony_Proto_StakingTransactionInput.with {
            $0.chainID = Data(hexString: localNet)!
            $0.privateKey = Data(hexString: "4edef2c24995d15b0e25cbd152fb0e2c05d3b79b9c2afd134e6f59f91bf99e48")!
            $0.nonce = Data(hexString: "0x2")!
            $0.gasPrice = Data(hexString: "0x")!
            $0.gasLimit = Data(hexString: "0x64")!
            $0.delegateMessage = delegate
        }
        let output = HarmonyStakingSigner.sign(input: input)

        let e1 = "f87302eb94ebcd16e8c1d8f493ba04e99a56474122d81a9c5894ebcd16e8c1d8f493ba04e99a56474122d81a9c"
        let e2 = "580a02806428a0ada9a8fb49eb3cd74f0f861e16bc1f1d56a0c6e3c25b0391f9e07a7963317e80a05c28dbc417"
        let e3 = "63dc2391263e1aae30f842f90734d7ec68cee2352af0d4b0662b54"
        XCTAssertEqual(output.encoded.hexString, e1 + e2 + e3)
        XCTAssertEqual(output.v.hexString, "28")
        XCTAssertEqual(output.r.hexString, "ada9a8fb49eb3cd74f0f861e16bc1f1d56a0c6e3c25b0391f9e07a7963317e80")
        XCTAssertEqual(output.s.hexString, "5c28dbc41763dc2391263e1aae30f842f90734d7ec68cee2352af0d4b0662b54")
    }
}