// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class NimiqTests: XCTestCase {

    func testSign() {
        let input = NimiqSigningInput.with {
            $0.privateKey = Data(hexString: "e3cc33575834add098f8487123cd4bca543ee859b3e8cfe624e7e6a97202b756")!
            $0.destination = "NQ86 2H8F YGU5 RM77 QSN9 LYLH C56A CYYR 0MLA"
            $0.value = 42042042
            $0.fee = 1000
            $0.validityStartHeight = 314159
            $0.networkID = .mainnetAlbatross
        }

        let output: NimiqSigningOutput = AnySigner.sign(input: input, coin: .nimiq)

        XCTAssertEqual(output.encoded.hexString, "000070c7492aaa9c9ac7a05bc0d9c5db2dae9372029654f71f0c7f95deed5099b7021450ffc385cd4e7c6ac9a7e91614ca67ff90568a00000000028182ba00000000000003e80004cb2f180ba678744be3bf9cd44fbcdabfb5be209f21739934836e26055610ab02720fa99489219d9f3581664473a1b40b30ad1f6e13150d59f8234a42c3f0de3d505405")
    }
}
