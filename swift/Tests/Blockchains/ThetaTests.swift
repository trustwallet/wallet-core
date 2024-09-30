// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class ThetaTests: XCTestCase {
    func testSigner() {
        let input = ThetaSigningInput.with {
            $0.chainID = "privatenet"
            $0.toAddress = "0x9F1233798E905E173560071255140b4A8aBd3Ec6"
            $0.thetaAmount = Data(hexString: "0a")!
            $0.tfuelAmount = Data(hexString: "14")!
            $0.sequence = 1
            $0.fee = Data(hexString: "e8d4a51000")!
            $0.privateKey = Data(hexString: "0x93a90ea508331dfdf27fb79757d4250b4e84954927ba0073cd67454ac432c737")!
        }

        let output: ThetaSigningOutput = AnySigner.sign(input: input, coin: .theta)

        XCTAssertEqual(output.signature.hexString, "5190868498d587d074d57298f41853d0109d997f15ddf617f471eb8cbb7fff267cb8fe9134ccdef053ec7cabd18070325c9c436efe1abbacd14eb7561d3fc10501")
        XCTAssertEqual(output.encoded.hexString, "02f887c78085e8d4a51000f863f861942e833968e5bb786ae419c4d13189fb081cc43babc70a85e8d4a5101401b8415190868498d587d074d57298f41853d0109d997f15ddf617f471eb8cbb7fff267cb8fe9134ccdef053ec7cabd18070325c9c436efe1abbacd14eb7561d3fc10501d9d8949f1233798e905e173560071255140b4a8abd3ec6c20a14")
    }
}
