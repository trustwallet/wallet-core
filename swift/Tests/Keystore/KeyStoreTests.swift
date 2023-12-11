// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

@testable import WalletCore

import WalletCore
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

    var bnbWallet: Wallet {
        return wallets.first(where: { $0.identifier == "bnb_wallet.json"})!
    }
}

class KeyStoreTests: XCTestCase {
    let keyAddress = AnyAddress(string: "0x008AeEda4D805471dF9b2A5B0f38A0C3bCBA786b", coin: .ethereum)!
    let walletAddress = AnyAddress(string: "0x32dd55E0BCF509a35A3F5eEb8593fbEb244796b1", coin: .ethereum)!
    let mnemonic = "often tobacco bread scare imitate song kind common bar forest yard wisdom"
    let fileManager = FileManager.default

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

        let bnbWalletURL = Bundle(for: type(of: self)).url(forResource: "bnb_wallet", withExtension: "json")!
        let bnbWalletDestination = keyDirectory.appendingPathComponent("bnb_wallet.json")

        try? fileManager.removeItem(at: bnbWalletDestination)
        try? fileManager.copyItem(at: bnbWalletURL, to: bnbWalletDestination)
    }

    func testLoadKeyStore() {
        let keyStore = try! KeyStore(keyDirectory: keyDirectory)
        XCTAssertEqual(keyStore.wallets.count, 4)
        XCTAssertEqual(keyStore.watches.count, 1)
    }

    func testCreateHDWallet() throws {
        let coins = [CoinType.ethereum, .binance, .smartChain]
        let keyStore = try KeyStore(keyDirectory: keyDirectory)
        let newWallet = try keyStore.createWallet(name: "name", password: "password", coins: coins)

        XCTAssertEqual(newWallet.accounts.count, 3)
        XCTAssertEqual(keyStore.wallets.count, 5)
        XCTAssertNoThrow(try newWallet.getAccount(password: "password", coin: .ethereum))
        XCTAssertNoThrow(try newWallet.getAccount(password: "password", coin: .binance))
        XCTAssertNoThrow(try newWallet.getAccount(password: "password", coin: .smartChain))
    }

    func testUpdateKey() throws {
        let keyStore = try KeyStore(keyDirectory: keyDirectory)
        let coins = [CoinType.ethereum, .callisto, .poanetwork]
        let wallet = try keyStore.createWallet(name: "name", password: "password", coins: coins)

        try keyStore.update(wallet: wallet, password: "password", newPassword: "testpassword")

        let savedKeyStore = try KeyStore(keyDirectory: keyDirectory)
        let savedWallet = savedKeyStore.wallets.first(where: { $0 == wallet })!

        let data = savedWallet.key.decryptPrivateKey(password: Data("testpassword".utf8))
        let mnemonic = String(data: data!, encoding: .ascii)

        XCTAssertEqual(savedWallet.accounts.count, coins.count)
        XCTAssertNotNil(data)
        XCTAssertNotNil(mnemonic)
        XCTAssert(Mnemonic.isValid(mnemonic: mnemonic!))
        XCTAssertEqual(savedWallet.key.name, "name")
    }

    func testUpdateName() throws {
        let keyStore = try KeyStore(keyDirectory: keyDirectory)
        let coins = [CoinType.ethereum, .callisto, .poanetwork]
        let wallet = try keyStore.createWallet(name: "name", password: "password", coins: coins)

        try keyStore.update(wallet: wallet, password: "password", newName: "testname")

        let savedKeyStore = try KeyStore(keyDirectory: keyDirectory)
        let savedWallet = savedKeyStore.wallets.first(where: { $0 == wallet })!

        let data = savedWallet.key.decryptPrivateKey(password: Data("password".utf8))
        let mnemonic = String(data: data!, encoding: .ascii)

        XCTAssertEqual(savedWallet.accounts.count, coins.count)
        XCTAssertNotNil(data)
        XCTAssertNotNil(mnemonic)
        XCTAssert(Mnemonic.isValid(mnemonic: mnemonic!))
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
        let key = StoredKey.importPrivateKey(privateKey: privateKeyData, name: "name", password: Data("password".utf8), coin: .ethereum)!
        let json = key.exportJSON()!

        let wallet = try keyStore.import(json: json, name: "name", password: "password", newPassword: "newPassword", coins: [.ethereum])
        let storedData = wallet.key.decryptPrivateKey(password: Data("newPassword".utf8))

        XCTAssertNotNil(keyStore.keyWallet)
        XCTAssertNotNil(storedData)
        XCTAssertNotNil(PrivateKey(data: storedData!))
    }
    
    func testImportPrivateKeyAES256() throws {
        let keyStore = try KeyStore(keyDirectory: keyDirectory)
        let privateKeyData = Data(hexString: "9cdb5cab19aec3bd0fcd614c5f185e7a1d97634d4225730eba22497dc89a716c")!
        let key = StoredKey.importPrivateKeyWithEncryption(privateKey: privateKeyData, name: "name", password: Data("password".utf8), coin: .ethereum, encryption: StoredKeyEncryption.aes256Ctr)!
        let json = key.exportJSON()!

        let wallet = try keyStore.import(json: json, name: "name", password: "password", newPassword: "newPassword", coins: [.ethereum])
        let storedData = wallet.key.decryptPrivateKey(password: Data("newPassword".utf8))

        XCTAssertNotNil(keyStore.keyWallet)
        XCTAssertNotNil(storedData)
        XCTAssertNotNil(PrivateKey(data: storedData!))
    }

    func testImportPrivateKey() throws {
        let keyStore = try KeyStore(keyDirectory: keyDirectory)
        let privateKey = PrivateKey(data: Data(hexString: "9cdb5cab19aec3bd0fcd614c5f185e7a1d97634d4225730eba22497dc89a716c")!)!

        let wallet = try keyStore.import(privateKey: privateKey, name: "name", password: "password", coin: .ethereum)
        let storedData = wallet.key.decryptPrivateKey(password: Data("password".utf8))
        XCTAssertNotNil(storedData)
        XCTAssertNotNil(PrivateKey(data: storedData!))

        XCTAssertEqual(wallet.accounts.count, 1)

        _ = try wallet.getAccount(password: "password", coin: .ethereum)
        XCTAssertNotNil(keyStore.keyWallet)
    }

    func testImportWallet() throws {
        let keyStore = try KeyStore(keyDirectory: keyDirectory)
        let wallet = try keyStore.import(mnemonic: mnemonic, name: "name", encryptPassword: "newPassword", coins: [.ethereum])
        let storedData = wallet.key.decryptMnemonic(password: Data("newPassword".utf8))

        XCTAssertNotNil(storedData)
        XCTAssertEqual(wallet.accounts.count, 1)
        XCTAssertNotNil(keyStore.hdWallet)
    }
    
    func testImportWalletAES256() throws {
        let keyStore = try KeyStore(keyDirectory: keyDirectory)
        let wallet = try keyStore.import(mnemonic: mnemonic, name: "name", encryptPassword: "newPassword", coins: [.ethereum], encryption: .aes256Ctr)
        let storedData = wallet.key.decryptMnemonic(password: Data("newPassword".utf8))

        XCTAssertNotNil(storedData)
        XCTAssertEqual(wallet.accounts.count, 1)
        XCTAssertNotNil(keyStore.hdWallet)
    }

    func testImportJSON() throws {
        let expected = """
        {
            "activeAccounts": [{
                "address": "bc1q4zehq85jqx9zzgzvzn9t64yjy66nunn3vehuv6",
                "coin": 0,
                "derivationPath": "m/84'/0'/0'/0/0",
                "extendedPublicKey": "zpub6qMRMrwcEYaqjf8wSpNqtBfUee6MqpQjrZNKfj5a48EUFUx2yUmfkDJMdHwWvkg8SjdS3ua6dy9ofMrzrytTfdyy2pXg344yFwm2Ta9cm6Q",
                "publicKey": "0334c47fa4eafef196f62eb53192a39bc36c5823ad4bd23db503170b9d3dbe80fd"
            }, {
                "address": "0x33F44330cc4253cCd4ce4224186DB9baCe2190ea",
                "coin": 60,
                "derivationPath": "m/44'/60'/0'/0/0",
                "publicKey": "04906ab3a756b952c1f2ad41daf0c82cc12fb155cd73919b904ffb2630866abfe3feae7169c3e322465d119f4b20465b2a98f8bcb9e19bf22d84ba04e277c1c6ee"
            }, {
                "address": "bnb1njuczq3hgvupu2vnczrjz7rc8x4uxlmhjyq95z",
                "coin": 714,
                "derivationPath": "m/44'/714'/0'/0/0",
                "publicKey": "03397cf6ee9ddfee746dc750e9b1abd9824ff8fec3e29bb09b3b2c330a88b605b8"
            }, {
                "address": "0x5dEc7A9299360aEb44c83B8F730F2BF5Dd1688bC",
                "coin": 10000714,
                "derivationPath": "m/44'/714'/0'/0/0",
                "publicKey": "04397cf6ee9ddfee746dc750e9b1abd9824ff8fec3e29bb09b3b2c330a88b605b81e46b99afe5dd84a5420b9e54f04b26aeb034f12849145a8163255875af1aef7"
            }, {
                "address": "0x33F44330cc4253cCd4ce4224186DB9baCe2190ea",
                "coin": 20000714,
                "derivationPath": "m/44'/60'/0'/0/0",
                "publicKey": "04906ab3a756b952c1f2ad41daf0c82cc12fb155cd73919b904ffb2630866abfe3feae7169c3e322465d119f4b20465b2a98f8bcb9e19bf22d84ba04e277c1c6ee"
            }, {
                "address": "838f8aeba6bb083b5b6e22030fb051eaf1a8b6cd692d4ad533cba60c77e6b8f2",
                "coin": 397,
                "derivationPath": "m/44'/397'/0'",
                "publicKey": "838f8aeba6bb083b5b6e22030fb051eaf1a8b6cd692d4ad533cba60c77e6b8f2"
            }],
            "crypto": {
                "cipher": "aes-128-ctr",
                "cipherparams": {
                    "iv": "cfeacebdf0d0c57cbbe6260094cdf3a9"
                },
                "ciphertext": "60358be4204c0d9c723775159bcadd63a51f0c06fce4024294d8ed4c19acb85cba3ca769dc3521fb572a06f8986d8bbc5736d6900e3e215f9bc112acffa470b178a621922041300bd7",
                "kdf": "scrypt",
                "kdfparams": {
                    "dklen": 32,
                    "n": 4096,
                    "p": 6,
                    "r": 8,
                    "salt": "14198d7e5f2afbfde2b00539d0c9abaec99e708dd4a2242448c57248e3e07c77"
                },
                "mac": "90b65f299a9ac59f50d24c6f80f4cdcffe6500c86687df716a15d79461992085"
            },
            "id": "3c937d42-443d-4acf-9311-2d9dfa857e1c",
            "name": "",
            "type": "mnemonic",
            "version": 3
        }
        """

        let password = "e28ddf66cec05c1fc09939a00628b230459202b2493fccac288038ef37815723"
        let keyStore = try KeyStore(keyDirectory: keyDirectory)

        // Fill public key if needed
        let btcAccount = try keyStore.bnbWallet.getAccount(password: password, coin: .bitcoin)
        XCTAssertEqual(btcAccount.publicKey, "0334c47fa4eafef196f62eb53192a39bc36c5823ad4bd23db503170b9d3dbe80fd")

        // Fix all empty
        _ = keyStore.bnbWallet.key.fixAddresses(password: Data(password.utf8))
        _ = try keyStore.addAccounts(wallet: keyStore.bnbWallet, coins: [.smartChainLegacy, .smartChain], password: password)

        // simulate migration code
        _ = try keyStore.removeAccounts(wallet: keyStore.bnbWallet, coins: [.near], password: password)
        _ = try keyStore.addAccounts(wallet: keyStore.bnbWallet, coins: [.near], password: password)

        XCTAssertEqual(keyStore.bnbWallet.accounts.count, 6)

        let accountLegacy = keyStore.bnbWallet.accounts[3]
        let account = keyStore.bnbWallet.accounts[4]
        let accountNear = keyStore.bnbWallet.accounts[5]

        XCTAssertEqual(accountLegacy.coin, CoinType.smartChainLegacy)
        XCTAssertEqual(accountLegacy.address, "0x5dEc7A9299360aEb44c83B8F730F2BF5Dd1688bC")

        XCTAssertEqual(account.coin, CoinType.smartChain)
        XCTAssertEqual(account.address, "0x33F44330cc4253cCd4ce4224186DB9baCe2190ea")

        XCTAssertEqual(accountNear.coin, CoinType.near)
        XCTAssertEqual(accountNear.address, "838f8aeba6bb083b5b6e22030fb051eaf1a8b6cd692d4ad533cba60c77e6b8f2")

        let saved = try String(contentsOf: keyStore.bnbWallet.keyURL)
        XCTAssertJSONEqual(saved, expected)
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
            XCTAssertFalse(account.publicKey.isEmpty)
        }

        XCTAssertEqual(coins.count, wallet.accounts.count)
    }

    func testSave() throws {
        let dir = try createTempDirURL()
        let keyStore = try KeyStore(keyDirectory: dir)
        try keyStore.watch([
            Watch(coin: .ethereum, name: "name", address: "0x008AeEda4D805471dF9b2A5B0f38A0C3bCBA786b", xpub: nil)
        ])
        let wallet = try keyStore.createWallet(name: "", password: "", coins: [.ethereum, .bitcoin])

        XCTAssertTrue(fileManager.fileExists(atPath: dir.appendingPathComponent("watches.json").path))
        XCTAssertTrue(fileManager.fileExists(atPath: wallet.keyURL.path))
    }

    func testImportError() throws {
        let url = try createTempDirURL()
        let keystore = try KeyStore(keyDirectory: url)

        do {
            _ = try keystore.import(json: Data(mnemonic.utf8), name: "", password: "", newPassword: "", coins: [])
        } catch {
            guard
                let err = error as? KeyStore.Error,
                err == .invalidJSON
            else {
                XCTFail("Should be invalid json error")
                return
            }
        }

        do {
            _ = try keystore.import(json: Data("{}".utf8), name: "", password: "", newPassword: "", coins: [])
        } catch {
            guard
                let err = error as? KeyStore.Error,
                err == .invalidJSON
            else {
                XCTFail("Should be invalid json error")
                return
            }
        }
    }

    func testCreateMultiAccount() throws {
        let mnemonic = "team engine square letter hero song dizzy scrub tornado fabric divert saddle"
        let password = "password"
        let keyStore = try KeyStore(keyDirectory: keyDirectory)
        let wallet = try keyStore.import(mnemonic: mnemonic, name: "name", encryptPassword: password, coins: [.bitcoin, .solana])

        _ = try keyStore.addAccounts(wallet: wallet, coins: [.bitcoin, .solana], password: password)

        let btc1 = try wallet.getAccount(password: password, coin: .bitcoin, derivation: .default)
        XCTAssertEqual(btc1.address, "bc1qturc268v0f2srjh4r2zu4t6zk4gdutqd5a6zny")
        XCTAssertEqual(btc1.extendedPublicKey, "zpub6qbsWdbcKW9sC6shTKK4VEhfWvDCoWpfLnnVfYKHLHt31wKYUwH3aFDz4WLjZvjHZ5W4qVEyk37cRwzTbfrrT1Gnu8SgXawASnkdQ994atn")

        let btc2 = try wallet.getAccount(password: password, coin: .bitcoin, derivation: .bitcoinLegacy)
        XCTAssertEqual(btc2.address, "1NyRyFewhZcWMa9XCj3bBxSXPXyoSg8dKz")
        XCTAssertEqual(btc2.extendedPublicKey, "xpub6CR52eaUuVb4kXAVyHC2i5ZuqJ37oWNPZFtjXaazFPXZD45DwWBYEBLdrF7fmCR9pgBuCA9Q57zZfyJjDUBDNtWkhWuGHNYKLgDHpqrHsxV")

        let solana1 = try wallet.getAccount(password: password, coin: .solana, derivation: .default)
        XCTAssertEqual(solana1.address, "HiipoCKL8hX2RVmJTz3vaLy34hS2zLhWWMkUWtw85TmZ")
        XCTAssertEqual(solana1.derivationPath, "m/44'/501'/0'")

        let solana2 = try wallet.getAccount(password: password, coin: .solana, derivation: .solanaSolana)
        XCTAssertEqual(solana2.address, "CgWJeEWkiYqosy1ba7a3wn9HAQuHyK48xs3LM4SSDc1C")
        XCTAssertEqual(solana2.derivationPath, "m/44'/501'/0'/0'")
    }

    func createTempDirURL() throws -> URL {
        let dir = URL(fileURLWithPath: NSTemporaryDirectory()).appendingPathComponent("keystore")
        try? fileManager.removeItem(at: dir)
        try fileManager.createDirectory(at: dir, withIntermediateDirectories: true, attributes: nil)
        return dir
    }
}
