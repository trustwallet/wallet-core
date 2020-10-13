// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
        }

        let output: NimiqSigningOutput = AnySigner.sign(input: input, coin: .nimiq)

        XCTAssertEqual(output.encoded.hexString, "0070c7492aaa9c9ac7a05bc0d9c5db2dae9372029654f71f0c7f95deed5099b7021450ffc385cd4e7c6ac9a7e91614ca67ff90568a00000000028182ba00000000000003e80004cb2f2a74dc7f6e0ab58a0bf52cc6e8801b0cca132dd4229d9a3e3a3d2f90e4d8f045d981b771bf5fc3851a98f3c617b1a943228f963e910e061808a721cfa0e3cad50b")
    }
}
