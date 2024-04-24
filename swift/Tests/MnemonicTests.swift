// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class MnemonicTests: XCTestCase {
    func testIsValid() {
        XCTAssertTrue(Mnemonic.isValid(mnemonic: "credit expect life fade cover suit response wash pear what skull force"))
        XCTAssertFalse(Mnemonic.isValid(mnemonic: "ripple scissors hisc mammal hire column oak again sun offer wealth tomorrow"))
    }

    func testValidate() {
        let words = "credit expect life3 fade cover suit response wash8 pear what skull force"
            .split(separator: " ")
            .map { String($0) }

        let result = Mnemonic.validate(mnemonic: words)

        XCTAssertEqual(result.count, 2)
        XCTAssertEqual(result[0].word, "life3")
        XCTAssertEqual(result[1].index, 7)
    }

    func testIsWordValid() {
        XCTAssertTrue(Mnemonic.isValidWord(word: "credit"))

        XCTAssertFalse(Mnemonic.isValidWord(word: "di"))
        XCTAssertFalse(Mnemonic.isValidWord(word: "cr"))
        XCTAssertFalse(Mnemonic.isValidWord(word: "hybridous"))
    }

    func testSuggest() {
        XCTAssertEqual(Mnemonic.search(prefix: "air"), ["air", "airport"])
        XCTAssertEqual(Mnemonic.search(prefix: "rob"), ["robot", "robust"])
    }
}
