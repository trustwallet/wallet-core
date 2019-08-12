// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

@testable import TrustWalletCore

import TrustWalletCore
import XCTest

extension KeyStore {
    var keyWallet: Wallet? {
        return wallets.first(where: { $0.identifier == "key.json" })
    }

    var hdWallet: Wallet? {
        return wallets.first(where: { $0.identifier == "wallet.json" })
    }

    var bitcoinWallet: Wallet {
        return wallets.first(where: { $0.identifier == "btc_missing_address.json"})!
    }
}

class KeyStoreTests: XCTestCase {
    let keyAddress = EthereumAddress(string: "0x008AeEda4D805471dF9b2A5B0f38A0C3bCBA786b")!
    let walletAddress = EthereumAddress(string: "0x32dd55E0BCF509a35A3F5eEb8593fbEb244796b1")!
    let mnemonic = "often tobacco bread scare imitate song kind common bar forest yard wisdom"

    var keyDirectory: URL!

    override func setUp() {
        super.setUp()

        let fileManager = FileManager.default

        keyDirectory = URL(fileURLWithPath: NSTemporaryDirectory()).appendingPathComponent("KeyStoreTests")
        try? fileManager.removeItem(at: keyDirectory)
        try? fileManager.createDirectory(at: keyDirectory, withIntermediateDirectories: true, attributes: nil)

        let keyURL = Bundle(for: type(of: self)).url(forResource: "key", withExtension: "json")!
        let keyDestination = keyDirectory.appendingPathComponent("key.json")

        try? fileManager.removeItem(at: keyDestination)
        try? fileManager.copyItem(at: keyURL, to: keyDestination)

        let walletURL = Bundle(for: type(of: self)).url(forResource: "wallet", withExtension: "json")!
        let walletDestination = keyDirectory.appendingPathComponent("wallet.json")

        try? fileManager.removeItem(at: walletDestination)
        try? fileManager.copyItem(at: walletURL, to: walletDestination)

        let bitcoinWalletURL = Bundle(for: type(of: self)).url(forResource: "btc_missing_address", withExtension: "json")!
        let bitcoinWalletDestination = keyDirectory.appendingPathComponent("btc_missing_address.json")

        try? fileManager.removeItem(at: bitcoinWalletDestination)
        try? fileManager.copyItem(at: bitcoinWalletURL, to: bitcoinWalletDestination)

        let watchesURL = Bundle(for: type(of: self)).url(forResource: "watches", withExtension: "json")!
        let watchesDestination = keyDirectory.appendingPathComponent("watches.json")

        try? fileManager.removeItem(at: watchesDestination)
        try? fileManager.copyItem(at: watchesURL, to: watchesDestination)
    }

    func testLoadKeyStore() {
        let keyStore = try! KeyStore(keyDirectory: keyDirectory)
        XCTAssertEqual(keyStore.wallets.count, 3)
        XCTAssertEqual(keyStore.watches.count, 1)
    }

    func testCreateHDWallet() throws {
        let coins = [CoinType.ethereum]
        let keyStore = try KeyStore(keyDirectory: keyDirectory)
        let newWallet = try keyStore.createWallet(name: "name", password: "password", coins: coins)

        XCTAssertEqual(newWallet.accounts.count, 1)
        XCTAssertEqual(keyStore.wallets.count, 4)
        XCTAssertNoThrow(try newWallet.getAccount(password: "password", coin: .ethereum))
    }

    func testUpdateKey() throws {
        let keyStore = try KeyStore(keyDirectory: keyDirectory)
        let coins = [CoinType.ethereum, .callisto, .poanetwork]
        let wallet = try keyStore.createWallet(name: "name", password: "password", coins: coins)

        try keyStore.update(wallet: wallet, password: "password", newPassword: "testpassword")

        let savedKeyStore = try KeyStore(keyDirectory: keyDirectory)
        let savedWallet = savedKeyStore.wallets.first(where: { $0 == wallet })!

        let data = savedWallet.key.decryptPrivateKey(password: "testpassword")
        let mnemonic = String(data: data!, encoding: .ascii)

        XCTAssertEqual(savedWallet.accounts.count, coins.count)
        XCTAssertNotNil(data)
        XCTAssertNotNil(mnemonic)
        XCTAssert(HDWallet.isValid(mnemonic: mnemonic!))
        XCTAssertEqual(savedWallet.key.name, "name")
    }

    func testUpdateName() throws {
        let keyStore = try KeyStore(keyDirectory: keyDirectory)
        let coins = [CoinType.ethereum, .callisto, .poanetwork]
        let wallet = try keyStore.createWallet(name: "name", password: "password", coins: coins)

        try keyStore.update(wallet: wallet, password: "password", newName: "testname")

        let savedKeyStore = try KeyStore(keyDirectory: keyDirectory)
        let savedWallet = savedKeyStore.wallets.first(where: { $0 == wallet })!

        let data = savedWallet.key.decryptPrivateKey(password: "password")
        let mnemonic = String(data: data!, encoding: .ascii)

        XCTAssertEqual(savedWallet.accounts.count, coins.count)
        XCTAssertNotNil(data)
        XCTAssertNotNil(mnemonic)
        XCTAssert(HDWallet.isValid(mnemonic: mnemonic!))
        XCTAssertEqual(savedWallet.key.name, "testname")
    }

    func testAddAccounts() throws {
        let keyStore = try KeyStore(keyDirectory: keyDirectory)
        let wallet = keyStore.hdWallet!
        _ = try keyStore.addAccounts(wallet: wallet, coins: [.ethereum, .callisto, .poanetwork], password: "password")

        let savedKeyStore = try KeyStore(keyDirectory: keyDirectory)
        let savedWallet = savedKeyStore.hdWallet!
        XCTAssertEqual(savedWallet.accounts.count, 3)
    }

    func testRemoveAccounts() throws {
        let keyStore = try KeyStore(keyDirectory: keyDirectory)
        let coins = [CoinType.ethereum, .callisto, .poanetwork, .bitcoin]
        let wallet = try keyStore.createWallet(name: "name", password: "password", coins: coins)

        _ = try keyStore.removeAccounts(wallet: wallet, coins: coins.dropLast(), password: "password")

        let savedKeyStore = try KeyStore(keyDirectory: keyDirectory)
        let savedWallet = savedKeyStore.wallets.first(where: { $0 == wallet })!
        XCTAssertEqual(savedWallet.accounts.count, 1)
        XCTAssertEqual(savedWallet.accounts[0].coin, coins.last)
    }

    func testDeleteKey() throws {
        let keyStore = try KeyStore(keyDirectory: keyDirectory)
        let wallet = keyStore.keyWallet!
        try keyStore.delete(wallet: wallet, password: "testpassword")
        XCTAssertNil(keyStore.keyWallet)
    }

    func testDeleteWallet() throws {
        let keyStore = try KeyStore(keyDirectory: keyDirectory)
        let wallet = keyStore.hdWallet!
        try keyStore.delete(wallet: wallet, password: "password")
        XCTAssertNil(keyStore.hdWallet)
    }

    func testImportKey() throws {
        let keyStore = try KeyStore(keyDirectory: keyDirectory)
        let privateKeyData = Data(hexString: "9cdb5cab19aec3bd0fcd614c5f185e7a1d97634d4225730eba22497dc89a716c")!
        let key = StoredKey.importPrivateKey(privateKey: privateKeyData, name: "name", password: "password", coin: .ethereum)!
        let json = key.exportJSON()!

        let wallet = try keyStore.import(json: json, name: "name", password: "password", newPassword: "newPassword", coins: [.ethereum])
        let storedData = wallet.key.decryptPrivateKey(password: "newPassword")

        XCTAssertNotNil(keyStore.keyWallet)
        XCTAssertNotNil(storedData)
        XCTAssertNotNil(PrivateKey(data: storedData!))
    }

    func testImportPrivateKey() throws {
        let keyStore = try KeyStore(keyDirectory: keyDirectory)
        let privateKey = PrivateKey(data: Data(hexString: "9cdb5cab19aec3bd0fcd614c5f185e7a1d97634d4225730eba22497dc89a716c")!)!

        let wallet = try keyStore.import(privateKey: privateKey, name: "name", password: "password", coin: .ethereum)
        let storedData = wallet.key.decryptPrivateKey(password: "password")
        XCTAssertNotNil(storedData)
        XCTAssertNotNil(PrivateKey(data: storedData!))

        XCTAssertEqual(wallet.accounts.count, 1)

        _ = try wallet.getAccount(password: "password", coin: .ethereum)
        XCTAssertNotNil(keyStore.keyWallet)
    }

    func testImportWallet() throws {
        let keyStore = try KeyStore(keyDirectory: keyDirectory)
        let wallet = try keyStore.import(mnemonic: mnemonic, name: "name", encryptPassword: "newPassword", coins: [.ethereum])
        let storedData = wallet.key.decryptMnemonic(password: "newPassword")

        XCTAssertNotNil(storedData)
        XCTAssertEqual(wallet.accounts.count, 1)
        XCTAssertNotNil(keyStore.hdWallet)
    }

    func testExportMnemonic() throws {
        let keyStore = try KeyStore(keyDirectory: keyDirectory)
        let wallet = try keyStore.import(mnemonic: mnemonic, name: "name", encryptPassword: "newPassword", coins: [.ethereum])
        let exported = try keyStore.exportMnemonic(wallet: wallet, password: "newPassword")

        XCTAssertEqual(mnemonic, exported)
    }

    func testFileName() {
        let keyStore = try! KeyStore(keyDirectory: keyDirectory)

        let timeZone = TimeZone(secondsFromGMT: -480)!
        let date = DateComponents(calendar: Calendar(identifier: .iso8601), timeZone: timeZone, year: 2018, month: 1, day: 2, hour: 20, minute: 55, second: 25, nanosecond: 186770975).date!
        let fileName = keyStore.generateFileName(identifier: keyAddress.description, date: date, timeZone: timeZone)

        XCTAssertEqual(fileName, "UTC--2018-01-02T20-55-25.186770975-0800--0x008AeEda4D805471dF9b2A5B0f38A0C3bCBA786b")
    }

    func testFileNameUTC() {
        let keyStore = try! KeyStore(keyDirectory: keyDirectory)

        let timeZone = TimeZone(abbreviation: "UTC")!
        let date = DateComponents(calendar: Calendar(identifier: .iso8601), timeZone: timeZone, year: 2018, month: 1, day: 2, hour: 20, minute: 55, second: 25, nanosecond: 186770975).date!
        let fileName = keyStore.generateFileName(identifier: keyAddress.description, date: date, timeZone: timeZone)

        XCTAssertEqual(fileName, "UTC--2018-01-02T20-55-25.186770975Z--0x008AeEda4D805471dF9b2A5B0f38A0C3bCBA786b")
    }

    func testDeriveActiveAccounts() {
        let keyStore = try! KeyStore(keyDirectory: keyDirectory)
        let wallet = try! keyStore.import(mnemonic: mnemonic, name: "name", encryptPassword: "newPassword", coins: [.ethereum])
        let coins = CoinType.allCases
        let accounts = try! keyStore.addAccounts(wallet: wallet, coins: coins, password: "newPassword")

        for account in accounts {
            XCTAssertFalse(account.address.isEmpty)
        }

        XCTAssertEqual(coins.count, wallet.accounts.count)
    }

    func testMissingBitcoinAddressDerivation() {
        let keyStore = try! KeyStore(keyDirectory: keyDirectory)
        let wallet = keyStore.bitcoinWallet
        let coins = CoinType.allCases

        let accounts = try! keyStore.addAccounts(wallet: wallet, coins: coins, password: "bc487bc88da81d8f48b3bf148f1278577d9bff072888a00f75508a922a5d1446")

        for account in accounts {
            XCTAssertFalse(account.address.isEmpty)
        }

        XCTAssertEqual(coins.count, wallet.accounts.count)
    }

    func testSave() throws {
        let fileManager = FileManager.default
        let dir = URL(fileURLWithPath: NSTemporaryDirectory()).appendingPathComponent("keystore")
        try? fileManager.removeItem(at: dir)
        try fileManager.createDirectory(at: dir, withIntermediateDirectories: true, attributes: nil)

        let keyStore = try KeyStore(keyDirectory: dir)
        try keyStore.watch([
            Watch(coin: .ethereum, name: "name", address: "0x008AeEda4D805471dF9b2A5B0f38A0C3bCBA786b", xpub: nil)
        ])
        let wallet = try keyStore.createWallet(name: "", password: "", coins: [.ethereum, .bitcoin])

        XCTAssertTrue(fileManager.fileExists(atPath: dir.appendingPathComponent("watches.json").path))
        XCTAssertTrue(fileManager.fileExists(atPath: wallet.keyURL.path))
    }
}
