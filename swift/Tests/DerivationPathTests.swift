// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class DerivationPathTests: XCTestCase {
    func testInitWithIndices() {
        let path = DerivationPath(purpose: .bip44, coinType: .ethereum, account: 0, change: 0, address: 0)
        XCTAssertEqual(path.indices[0], DerivationPath.Index(44, hardened: true))
        XCTAssertEqual(path.indices[1], DerivationPath.Index(60, hardened: true))
        XCTAssertEqual(path.indices[2], DerivationPath.Index(0, hardened: true))
        XCTAssertEqual(path.indices[3], DerivationPath.Index(0, hardened: false))
        XCTAssertEqual(path.indices[4], DerivationPath.Index(0, hardened: false))
    }

    func testInitWithString() {
        let path = DerivationPath("m/44'/60'/0'/0/0")

        XCTAssertNotNil(path)
        XCTAssertEqual(path?.indices[0], DerivationPath.Index(44, hardened: true))
        XCTAssertEqual(path?.indices[1], DerivationPath.Index(60, hardened: true))
        XCTAssertEqual(path?.indices[2], DerivationPath.Index(0, hardened: true))
        XCTAssertEqual(path?.indices[3], DerivationPath.Index(0, hardened: false))
        XCTAssertEqual(path?.indices[4], DerivationPath.Index(0, hardened: false))

        XCTAssertEqual(path?.purpose, Purpose(rawValue: 44)!)
        XCTAssertEqual(path?.coinType, CoinType(rawValue: 60)!)
        XCTAssertEqual(path?.account, 0)
        XCTAssertEqual(path?.change, 0)
        XCTAssertEqual(path?.address, 0)
    }

    func testInitInvalid() {
        XCTAssertNil(DerivationPath("a/b/c"))
        XCTAssertNil(DerivationPath("m/44'/60''/"))
    }

    func testDescription() {
        let path = DerivationPath("m/44'/60'/0'/0/0")
        XCTAssertEqual(path?.description, "m/44'/60'/0'/0/0")
    }

    func testEqual() {
        let path1 = DerivationPath("m/44'/60'/0'/0/0")
        let path2 = DerivationPath("44'/60'/0'/0/0")
        XCTAssertNotNil(path1)
        XCTAssertNotNil(path2)
        XCTAssertEqual(path1, path2)
    }
}
