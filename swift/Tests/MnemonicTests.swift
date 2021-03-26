// Copyright © 2017-2021 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class MnemonicTests: XCTestCase {
    func testIsValid() {
        XCTAssertTrue(Mnemonic.isValid(mnemonic: "credit expect life fade cover suit response wash pear what skull force"));
        XCTAssertFalse(Mnemonic.isValid(mnemonic: "ripple scissors hisc mammal hire column oak again sun offer wealth tomorrow"));
    }

    func testIsWordValid() {
        XCTAssertTrue(Mnemonic.isWordValid(mnemonic: "credit"));
        XCTAssertFalse(Mnemonic.isWordValid(mnemonic: "hybridous"));
    }

    func testSuggest() {
        XCTAssertEqual(Mnemonic.suggest(prefix: "air"), "air airport")
        XCTAssertEqual(Mnemonic.suggest(prefix: "rob"), "robot robust")
    }
}
