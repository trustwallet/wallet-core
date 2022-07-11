// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import WalletCore

class Base32Tests: XCTestCase {
    func testDecode()  {
        guard let decoded = Base32.decode(string: "JBSWY3DPK5XXE3DE") else {
            return XCTFail();
        }
        let toCompare = String(data: decoded, encoding:.utf8);

        XCTAssertEqual(toCompare, "HelloWorld");
    }
    
    func testDecodeWithAlphabet() {
        guard let decoded = Base32.decodeWithAlphabet(string: "g52w64jworydimrxov5hmn3gpj2gwyttnzxdmndjo5xxiztsojuxg5dxobzhs6i", alphabet:"abcdefghijklmnopqrstuvwxyz234567") else {
            return XCTFail();
        }
        let toCompare = String(data: decoded, encoding:.utf8);

        XCTAssertEqual(toCompare, "7uoq6tp427uzv7fztkbsnn64iwotfrristwpryy");
    }
}
