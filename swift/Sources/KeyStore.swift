// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

/// Manages directories of key and wallet files and presents them as accounts.
public final class KeyStore {
    static let watchesFileName = "watches.json"

    /// The key file directory.
    public let keyDirectory: URL

    /// The watches file URL.
    public let watchesFile: URL

    /// List of wallets.
    public private(set) var wallets = [Wallet]()

    /// List of accounts being watched
    public var watches = [Watch]()

    /// Creates a `KeyStore` for the given directory.
    public init(keyDirectory: URL) throws {
        self.keyDirectory = keyDirectory
        self.watchesFile = keyDirectory.appendingPathComponent(KeyStore.watchesFileName)

        try load()
    }

    private func load() throws {
        let fileManager = FileManager.default
        try? fileManager.createDirectory(at: keyDirectory, withIntermediateDirectories: true, attributes: nil)

        if fileManager.fileExists(atPath: watchesFile.path) {
            let data = try Data(contentsOf: watchesFile)
            watches = try JSONDecoder().decode([Watch].self, from: data)
        }

        let accountURLs = try fileManager.contentsOfDirectory(at: keyDirectory, includingPropertiesForKeys: [], options: [.skipsHiddenFiles])
        for url in accountURLs {
            if url.lastPathComponent == KeyStore.watchesFileName {
                // Skip watches file
                continue
            }
            guard let key = StoredKey.load(path: url.path) else {
                // Ignore invalid keys
                continue
            }
            let wallet = Wallet(keyURL: url, key: key)
            wallets.append(wallet)
        }
    }

    /// Watches a list of accounts.
    public func watch(_ watches: [Watch]) throws {
        self.watches.append(contentsOf: watches)

        let data = try JSONEncoder().encode(watches)
        try data.write(to: watchesFile)
    }

    /// Stop watching an account.
    public func removeWatch(_ watch: Watch) throws {
        guard let index = watches.firstIndex(of: watch) else {
            return
        }
        watches.remove(at: index)

        let data = try JSONEncoder().encode(watches)
        try data.write(to: watchesFile)
    }

    /// Creates a new wallet. HD default by default
    public func createWallet(name: String, password: String, coins: [CoinType], encryption: StoredKeyEncryption = .aes128Ctr) throws -> Wallet {
        let key = StoredKey(name: name, password: Data(password.utf8), encryption: encryption)
        return try saveCreatedWallet(for: key, password: password, coins: coins)
    }

    private func saveCreatedWallet(for key: StoredKey, password: String, coins: [CoinType]) throws -> Wallet {
        let url = makeAccountURL()
        let wallet = Wallet(keyURL: url, key: key)
        for coin in coins {
            _ = try wallet.getAccount(password: password, coin: coin)
        }
        wallets.append(wallet)

        try save(wallet: wallet)

        return wallet
    }

    /// Adds accounts to a wallet.
    public func addAccounts(wallet: Wallet, coins: [CoinType], password: String) throws -> [Account] {
        let accounts = try wallet.getAccounts(password: password, coins: coins)
        try save(wallet: wallet)
        return accounts
    }

    /// Remove accounts from a wallet.
    public func removeAccounts(wallet: Wallet, coins: [CoinType], password: String) throws -> Wallet {
        guard wallet.key.decryptPrivateKey(password: Data(password.utf8)) != nil else {
            throw Error.invalidPassword
        }

        guard let index = wallets.firstIndex(of: wallet) else {
            fatalError("Missing wallet")
        }

        for coin in coins {
            wallet.key.removeAccountForCoin(coin: coin)
        }

        wallets[index] = wallet
        try save(wallet: wallet)
        return wallet
    }

    /// Imports an encrypted JSON key.
    ///
    /// - Parameters:
    /// - json: json wallet
    /// - password: key password
    /// - newPassword: password to use for the imported key
    /// - coins: coins to use for this wallet
    /// - Returns: new account
    public func `import`(json: Data, name: String, password: String, newPassword: String, coins: [CoinType]) throws -> Wallet {
        guard let key = StoredKey.importJSON(json: json) else {
            throw Error.invalidJSON
        }
        guard let data = key.decryptPrivateKey(password: Data(password.utf8)) else {
            throw Error.invalidPassword
        }

        if let mnemonic = checkMnemonic(data) {
            return try self.import(mnemonic: mnemonic, name: name, encryptPassword: newPassword, coins: coins)
        }

        guard let privateKey = PrivateKey(data: data) else {
            throw Error.invalidKey
        }
        if key.hasPrivateKeyEncoded {
            guard let encodedPrivateKey = key.decryptPrivateKeyEncoded(password: Data(password.utf8)) else {
                throw Error.invalidPassword
            }
            return try self.import(encodedPrivateKey: encodedPrivateKey, name: name, password: newPassword, coin: coins.first ?? .ethereum)
        } else {
            return try self.import(privateKey: privateKey, name: name, password: newPassword, coin: coins.first ?? .ethereum)
        }
    }

    private func checkMnemonic(_ data: Data) -> String? {
        guard let mnemonic = String(data: data, encoding: .ascii), Mnemonic.isValid(mnemonic: mnemonic) else {
            return nil
        }
        return mnemonic
    }
    
    private func checkEncoded(wallet: Wallet, password: String) -> String? {
        guard wallet.key.hasPrivateKeyEncoded else {
            return nil
        }
        return wallet.key.decryptPrivateKeyEncoded(password: Data(password.utf8))
    }

    /// Imports a private key.
    ///
    /// - Parameters:
    ///   - privateKey: private key to import
    ///   - password: password to use for the imported private key
    ///   - coin: coin to use for this wallet
    /// - Returns: new wallet
    public func `import`(privateKey: PrivateKey, name: String, password: String, coin: CoinType, encryption: StoredKeyEncryption = .aes128Ctr) throws -> Wallet {
        guard let newKey = StoredKey.importPrivateKeyWithEncryption(privateKey: privateKey.data, name: name, password: Data(password.utf8), coin: coin, encryption: encryption) else {
            throw Error.invalidKey
        }
        let url = makeAccountURL()
        let wallet = Wallet(keyURL: url, key: newKey)
        _ = try wallet.getAccount(password: password, coin: coin)
        wallets.append(wallet)

        try save(wallet: wallet)

        return wallet
    }

    /// Imports an encoded private key.
    ///
    /// - Parameters:
    ///   - privateKey: private key to import
    ///   - password: password to use for the imported private key
    ///   - coin: coin to use for this wallet
    /// - Returns: new wallet
    public func `import`(encodedPrivateKey: String, name: String, password: String, coin: CoinType, encryption: StoredKeyEncryption = .aes128Ctr) throws -> Wallet {
        guard let newKey = StoredKey.importPrivateKeyEncodedWithEncryption(privateKey: encodedPrivateKey, name: name, password: Data(password.utf8), coin: coin, encryption: encryption) else {
            throw Error.invalidKey
        }
        let url = makeAccountURL()
        let wallet = Wallet(keyURL: url, key: newKey)
        _ = try wallet.getAccount(password: password, coin: coin)
        wallets.append(wallet)

        try save(wallet: wallet)

        return wallet
    }

    /// Imports a wallet.
    ///
    /// - Parameters:
    ///   - mnemonic: wallet's mnemonic phrase
    ///   - encryptPassword: password to use for encrypting
    ///   - coins: coins to add
    /// - Returns: new account
    public func `import`(mnemonic: String, name: String, encryptPassword: String, coins: [CoinType], encryption: StoredKeyEncryption = .aes128Ctr) throws -> Wallet {
        guard let key = StoredKey.importHDWalletWithEncryption(mnemonic: mnemonic, name: name, password: Data(encryptPassword.utf8), coin: coins.first ?? .ethereum, encryption: encryption) else {
            throw Error.invalidMnemonic
        }
        let url = makeAccountURL()
        let wallet = Wallet(keyURL: url, key: key)
        _ = try wallet.getAccounts(password: encryptPassword, coins: coins)

        wallets.append(wallet)

        try save(wallet: wallet)

        return wallet
    }

    /// Exports a wallet as JSON data.
    ///
    /// - Parameters:
    ///   - wallet: wallet to export
    ///   - password: account password
    ///   - newPassword: password to use for exported key
    /// - Returns: encrypted JSON key
    public func export(wallet: Wallet, password: String, newPassword: String, encryption: StoredKeyEncryption = .aes128Ctr) throws -> Data {
        var privateKeyData = try exportPrivateKey(wallet: wallet, password: password)
        defer {
            privateKeyData.resetBytes(in: 0 ..< privateKeyData.count)
        }

        guard let coin = wallet.key.account(index: 0)?.coin else {
            throw Error.accountNotFound
        }

        if let mnemonic = checkMnemonic(privateKeyData), let newKey = StoredKey.importHDWalletWithEncryption(mnemonic: mnemonic, name: "", password: Data(newPassword.utf8), coin: coin, encryption: encryption) {
            guard let json = newKey.exportJSON() else {
                throw Error.invalidKey
            }
            return json
        } else if let privateKey = checkEncoded(wallet: wallet, password: password), let newKey = StoredKey.importPrivateKeyEncodedWithEncryption(privateKey: privateKey, name: "", password: Data(newPassword.utf8), coin: coin, encryption: encryption) {
            guard let json = newKey.exportJSON() else {
                throw Error.invalidKey
            }
            return json
        } else if let newKey = StoredKey.importPrivateKeyWithEncryption(privateKey: privateKeyData, name: "", password: Data(newPassword.utf8), coin: coin, encryption: encryption) {
            guard let json = newKey.exportJSON() else {
                throw Error.invalidKey
            }
            return json
        }

        throw Error.invalidKey
    }

    /// Exports a wallet as private key data.
    ///
    /// - Parameters:
    ///   - wallet: wallet to export
    ///   - password: account password
    /// - Returns: private key data for encrypted keys or mnemonic phrase for HD wallets
    public func exportPrivateKey(wallet: Wallet, password: String) throws -> Data {
        guard let key = wallet.key.decryptPrivateKey(password: Data(password.utf8)) else {
            throw Error.invalidPassword
        }
        return key
    }
    
    /// Exports a wallet as encoded private key data.
    ///
    /// - Parameters:
    ///   - wallet: wallet to export
    ///   - password: account password
    /// - Returns: encoded private key data
    public func exportPrivateKeyEncoded(wallet: Wallet, password: String) throws -> String {
        guard let key = wallet.key.decryptPrivateKeyEncoded(password: Data(password.utf8)) else {
            throw Error.invalidPassword
        }
        return key
    }

    /// Exports a wallet as a mnemonic phrase.
    ///
    /// - Parameters:
    ///   - wallet: wallet to export
    ///   - password: account password
    /// - Returns: mnemonic phrase
    /// - Throws: `EncryptError.invalidMnemonic` if the account is not an HD wallet.
    public func exportMnemonic(wallet: Wallet, password: String) throws -> String {
        guard let mnemonic = wallet.key.decryptMnemonic(password: Data(password.utf8)) else {
            throw Error.invalidPassword
        }
        return mnemonic
    }

    /// Updates the password of an existing account.
    ///
    /// - Parameters:
    ///   - wallet: wallet to update
    ///   - password: current password
    ///   - newPassword: new password
    public func update(wallet: Wallet, password: String, newPassword: String) throws {
        try update(wallet: wallet, password: password, newPassword: newPassword, newName: wallet.key.name)
    }

    /// Updates the name of an existing account.
    ///
    /// - Parameters:
    ///   - wallet: wallet to update
    ///   - password: current password
    ///   - newName: new name
    public func update(wallet: Wallet, password: String, newName: String, encryption: StoredKeyEncryption = .aes128Ctr) throws {
        try update(wallet: wallet, password: password, newPassword: password, newName: newName, encryption: encryption)
    }

    private func update(wallet: Wallet, password: String, newPassword: String, newName: String, encryption: StoredKeyEncryption = .aes128Ctr) throws {
        guard let index = wallets.firstIndex(of: wallet) else {
            fatalError("Missing wallet")
        }

        guard var privateKeyData = wallet.key.decryptPrivateKey(password: Data(password.utf8)) else {
            throw Error.invalidPassword
        }
        defer {
            privateKeyData.resetBytes(in: 0 ..< privateKeyData.count)
        }

        let coins = wallet.accounts.map({ $0.coin })
        guard !coins.isEmpty else {
            throw Error.accountNotFound
        }

        if let mnemonic = checkMnemonic(privateKeyData),
            let key = StoredKey.importHDWalletWithEncryption(mnemonic: mnemonic, name: newName, password: Data(newPassword.utf8), coin: coins[0], encryption: encryption) {
            wallets[index].key = key
        } else if let key = StoredKey.importPrivateKeyWithEncryption(
                privateKey: privateKeyData, name: newName, password: Data(newPassword.utf8), coin: coins[0], encryption: encryption) {
            wallets[index].key = key
        } else {
            throw Error.invalidKey
        }

        _ = try wallets[index].getAccounts(password: newPassword, coins: coins)
        try save(wallet: wallets[index])
    }

    /// Deletes an account including its key if the password is correct.
    public func delete(wallet: Wallet, password: String) throws {
        guard let index = wallets.firstIndex(of: wallet) else {
            fatalError("Missing wallet")
        }

        guard var privateKey = wallet.key.decryptPrivateKey(password: Data(password.utf8)) else {
            throw KeyStore.Error.invalidKey
        }
        defer {
            privateKey.resetBytes(in: 0..<privateKey.count)
        }
        wallets.remove(at: index)

        try FileManager.default.removeItem(at: wallet.keyURL)
    }

    /// Removes all wallets.
    public func destroy() throws {
        wallets.removeAll(keepingCapacity: false)

        let fileManager = FileManager.default
        let accountURLs = try fileManager.contentsOfDirectory(at: keyDirectory, includingPropertiesForKeys: [], options: [.skipsHiddenFiles])
        for url in accountURLs {
            try? fileManager.removeItem(at: url)
        }
    }

    // MARK: Helpers

    private func makeAccountURL(for address: Address) -> URL {
        return keyDirectory.appendingPathComponent(generateFileName(identifier: address.description))
    }

    private func makeAccountURL() -> URL {
        return keyDirectory.appendingPathComponent(generateFileName(identifier: UUID().uuidString))
    }

    private func save(wallet: Wallet) throws {
        _ = wallet.key.store(path: wallet.keyURL.path)
    }

    /// Generates a unique file name for an address.
    func generateFileName(identifier: String, date: Date = Date(), timeZone: TimeZone = .current) -> String {
        // keyFileName implements the naming convention for keyfiles:
        // UTC--<created_at UTC ISO8601>-<address hex>
        return "UTC--\(filenameTimestamp(for: date, in: timeZone))--\(identifier)"
    }

    private func filenameTimestamp(for date: Date, in timeZone: TimeZone = .current) -> String {
        var tz = ""
        let offset = timeZone.secondsFromGMT()
        if offset == 0 {
            tz = "Z"
        } else {
            tz = String(format: "%03d00", offset/60)
        }

        let components = Calendar(identifier: .iso8601).dateComponents(in: timeZone, from: date)
        return String(format: "%04d-%02d-%02dT%02d-%02d-%02d.%09d%@",
                      components.year!, components.month!,
                      components.day!, components.hour!,
                      components.minute!, components.second!,
                      components.nanosecond!, tz)
    }
}
