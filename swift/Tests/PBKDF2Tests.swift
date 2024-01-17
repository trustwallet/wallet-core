// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class PBKDF2Tests: XCTestCase {

    let password = "password".data(using: .utf8)!
    let salt = "salt".data(using: .utf8)!
    let salt2 = Data(base64Encoded: "kNHS+Mx//slRsmLF9396HQ==")!

    func testSha256Hmac() {

        let key = PBKDF2.hmacSha256(password: password, salt: salt, iterations: 1, dkLen: 20)!

        XCTAssertEqual(key.hexString, "120fb6cffcf8b32c43e7225256c4f837a86548c9")

        let key2 = PBKDF2.hmacSha256(password: password, salt: salt, iterations: 4096, dkLen: 20)!

        XCTAssertEqual(key2.hexString, "c5e478d59288c841aa530db6845c4c8d962893a0")

        let key3 = PBKDF2.hmacSha256(password: password, salt: salt2, iterations: 100, dkLen: 32)!
        XCTAssertEqual(key3.hexString, "9cf33ebd3542c691fac6f61609a8d13355a0adf4d15eed77cc9d13f792b77c3a")
    }

    func testSha512Hmac() {
        let key = PBKDF2.hmacSha512(password: password, salt: salt, iterations: 1, dkLen: 20)!

        XCTAssertEqual(key.hexString, "867f70cf1ade02cff3752599a3a53dc4af34c7a6")

        let key2 = PBKDF2.hmacSha512(password: password, salt: salt, iterations: 4096, dkLen: 20)!

        XCTAssertEqual(key2.hexString, "d197b1b33db0143e018b12f3d1d1479e6cdebdcc")

        let key3 = PBKDF2.hmacSha512(password: password, salt: salt2, iterations: 100, dkLen: 32)!
        XCTAssertEqual(key3.hexString, "6a9a209f35be9212118ea055e11b545451b53b686608a6362d59ddf31a2b3ce0")
    }
}
