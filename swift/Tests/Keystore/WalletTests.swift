// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import WalletCore

class WalletTests: XCTestCase {
    func testSeparateAccounts() throws {
        let url = Bundle(for: type(of: self)).url(forResource: "wallet", withExtension: "json")!
        let key = StoredKey.load(path: url.path)!
        let wallet = Wallet(keyURL: url, key: key)
        _ = try wallet.getAccount(password: "password", coin: .ethereum)
        _ = try wallet.getAccount(password: "password", coin: .callisto)
        _ = try wallet.getAccount(password: "password", coin: .poanetwork)

        XCTAssertEqual(wallet.key.accountCount, 3)
    }

    func testIdentifier() throws {
        let url = URL(string: "UTC--2018-07-23T15-42-07.380692005-42000--6E199F01-FA96-4ADF-9A4B-36EE4B1E08C7")!
        let key = StoredKey(name: "name", password: Data("password".utf8))
        let wallet = Wallet(keyURL: url, key: key)
        XCTAssertEqual(wallet.identifier, "UTC--2018-07-23T15-42-07.380692005-42000--6E199F01-FA96-4ADF-9A4B-36EE4B1E08C7")
    }
}
