// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class AESTests: XCTestCase {
    func testDecrypt() throws {
        let key = Data(hexString: "5caa3a74154cee16bd1b570a1330be46e086474ac2f4720530662ef1a469662c")!
        let iv = Data(hexString: "89ef1d6728bac2f1dcde2ef9330d2bb8")!
        let cipher = Data(hexString: "1b3db3674de082d65455eba0ae61cfe7e681c8ef1132e60c8dbd8e52daf18f4fea42cc76366c83351dab6dca52682ff81f828753f89a21e1cc46587ca51ccd353914ffdd3b0394acfee392be6c22b3db9237d3f717a3777e3577dd70408c089a4c9c85130a68c43b0a8aadb00f1b8a8558798104e67aa4ff027b35d4b989e7fd3988d5dcdd563105767670be735b21c4")!
        let expected = """
        {"id":1554098597199736,"jsonrpc":"2.0","method":"wc_sessionUpdate","params":[{"approved":false,"chainId":null,"accounts":null}]}
        """

        let decrypted = AES.decryptCBC(key: key, data: cipher, iv: iv, mode: .pkcs7)!

        XCTAssertEqual(String(data: decrypted, encoding: .utf8)!, expected)
    }

    func testEncrypt() throws {
        let key = Data(hexString: "bbc82a01ebdb14698faee4a9e5038de72c995a9f6bcdb21903d62408b0c5ca96")!
        let iv = Data(hexString: "5b3a1a561e395d7ad7fe9c92abdacd17")!
        let plain = """
        {"jsonrpc":"2.0","id":1554343834752446,"error":{"code":-32000,"message":"Session Rejected"}}
        """

        let encrypted = AES.encryptCBC(key: key, data: Data(plain.utf8), iv: iv, mode: .pkcs7)!

        XCTAssertEqual(encrypted.hexString, "6a93788fcd6d266d06ccff35d1ed328d634605a7f2734f1519256b9950d86d6ca34fe4a13ff0ed513025b49427e6fe15268c84d6dfad6c0c8a21abc9306a5308f545b08d946a2a24b7cd18526bcefd6d9740db9b8e21f4511df148d9b9b03ad9")
    }
}
