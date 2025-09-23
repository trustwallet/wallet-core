// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class PBKDF2Tests: XCTestCase {

    let password = "password".data(using: .utf8)!
    let salt = "saltwithsufficientlength".data(using: .utf8)!
    let salt2 = Data(base64Encoded: "kNHS+Mx//slRsmLF9396HQ==")!

    func testSha256Hmac() {

        let key = PBKDF2.hmacSha256(password: password, salt: salt, iterations: 1, dkLen: 20)!

        XCTAssertEqual(key.hexString, "168ff47855244ffef87254a2c4ac9c954bc59c44")

        let key2 = PBKDF2.hmacSha256(password: password, salt: salt, iterations: 4096, dkLen: 20)!

        XCTAssertEqual(key2.hexString, "77a724e0e92ec460a6f3b01a7b5eceb86abffc04")

        let key3 = PBKDF2.hmacSha256(password: password, salt: salt2, iterations: 100, dkLen: 32)!
        XCTAssertEqual(key3.hexString, "9cf33ebd3542c691fac6f61609a8d13355a0adf4d15eed77cc9d13f792b77c3a")
    }

    func testSha512Hmac() {
        let key = PBKDF2.hmacSha512(password: password, salt: salt, iterations: 1, dkLen: 20)!

        XCTAssertEqual(key.hexString, "8f7ce2eabfe88358b223e04c0462f0d00febf35c")

        let key2 = PBKDF2.hmacSha512(password: password, salt: salt, iterations: 4096, dkLen: 20)!

        XCTAssertEqual(key2.hexString, "0db9217fc2a56c8ce7f2f6e78c3466b29f9c9d45")

        let key3 = PBKDF2.hmacSha512(password: password, salt: salt2, iterations: 100, dkLen: 32)!
        XCTAssertEqual(key3.hexString, "6a9a209f35be9212118ea055e11b545451b53b686608a6362d59ddf31a2b3ce0")
    }
}
