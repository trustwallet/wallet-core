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
    
    func testPrivateKeyGetAccount() throws {
        let url = Bundle(for: type(of: self)).url(forResource: "key", withExtension: "json")!
        let key = StoredKey.load(path: url.path)!
        let wallet = Wallet(keyURL: url, key: key)

        // The wallet already contains `Ethereum` account. No exception expected.
        let ethAccount = try wallet.getAccount(password: "testpassword", coin: .ethereum)
        XCTAssertEqual(ethAccount.address, "0x008AeEda4D805471dF9b2A5B0f38A0C3bCBA786b")

        let accounts1 = try wallet.getAccounts(password: "testpassword", coins: [.ethereum])
        XCTAssertEqual(accounts1.count, 1)

        // Should fail because `getAccount` currently doesn't support creating a new account from PrivateKey.
        XCTAssertThrowsError(try wallet.getAccount(password: "testpassword", coin: .bitcoin))

        // Should return only `Ethereum` account.
        let accounts2 = try wallet.getAccounts(password: "testpassword", coins: [.bitcoin, .ethereum])
        XCTAssertEqual(accounts2.count, 1)
    }
    
    func testTONWalletGetAccount() throws {
        let url = Bundle(for: type(of: self)).url(forResource: "ton_wallet", withExtension: "json")!
        let key = StoredKey.load(path: url.path)!
        let wallet = Wallet(keyURL: url, key: key)

        // The wallet already contains `TON` account. No exception expected.
        let tonAccount = try wallet.getAccount(password: "password", coin: .ton)
        XCTAssertEqual(tonAccount.address, "UQBlm676c6vy6Q9Js732pvf3ivfmIkVc0MVDQy-F6NAFJd4k")

        let accounts1 = try wallet.getAccounts(password: "password", coins: [.ton])
        XCTAssertEqual(accounts1.count, 1)

        // Should fail because `getAccount` currently doesn't support creating a new account from PrivateKey.
        XCTAssertThrowsError(try wallet.getAccount(password: "password", coin: .ethereum))

        // Should return only `TON` account.
        let accounts2 = try wallet.getAccounts(password: "password", coins: [.bitcoin, .ton])
        XCTAssertEqual(accounts2.count, 1)
    }

    func testTONWalletGetPrivateKey() throws {
        let url = Bundle(for: type(of: self)).url(forResource: "ton_wallet", withExtension: "json")!
        let key = StoredKey.load(path: url.path)!
        let wallet = Wallet(keyURL: url, key: key)

        // The wallet already contains `TON` account. No exception expected.
        let privateKey = try wallet.privateKey(password: "password", coin: .ton)
        XCTAssertEqual(privateKey.data.hexString, "cdcea50b87d3f1ca859e7b2bdf9a5339b7b6804b5c70ac85198829f9607dc43b")
    }
}
