// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

/// Manages directories of key and wallet files and presents them as accounts.
public final class KeyStore {
    /// The key file directory.
    public let keyDirectory: URL

    /// List of wallets.
    public private(set) var wallets = [Wallet]()

    /// Creates a `KeyStore` for the given directory.
    public init(keyDirectory: URL) throws {
        self.keyDirectory = keyDirectory
        try load()
    }

    private func load() throws {
        let fileManager = FileManager.default
        try? fileManager.createDirectory(at: keyDirectory, withIntermediateDirectories: true, attributes: nil)

        let accountURLs = try fileManager.contentsOfDirectory(at: keyDirectory, includingPropertiesForKeys: [], options: [.skipsHiddenFiles])
        for url in accountURLs {
            guard let key = StoredKey.load(path: url.path) else {
                // Ignore invalid keys
                continue
            }
            let wallet = Wallet(keyURL: url, key: key)
            wallets.append(wallet)
        }
    }

    /// Creates a new wallet. HD default by default
    public func createWallet(password: String, coins: [CoinType]) throws -> Wallet {
        let key = StoredKey(password: password)
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

    /// Imports an encrypted JSON key.
    ///
    /// - Parameters:
    /// - json: json wallet
    /// - password: key password
    /// - newPassword: password to use for the imported key
    /// - coins: coins to use for this wallet
    /// - Returns: new account
    public func `import`(json: Data, password: String, newPassword: String, coins: [CoinType]) throws -> Wallet {
        guard let key = StoredKey.importJSON(json: json) else {
            throw Error.invalidKey
        }
        guard let data = key.decryptPrivateKey(password: password) else {
            throw Error.invalidPassword
        }

        if let mnemonic = checkMnemonic(data) {
            return try self.import(mnemonic: mnemonic, encryptPassword: newPassword, coins: coins)
        }

        guard let privateKey = PrivateKey(data: data) else {
            throw Error.invalidKey
        }
        return try self.import(privateKey: privateKey, password: newPassword, coin: coins.first ?? .ethereum)
    }

    private func checkMnemonic(_ data: Data) -> String? {
        guard let mnemonic = String(data: data, encoding: .ascii), HDWallet.isValid(mnemonic: mnemonic) else {
            return nil
        }
        return mnemonic
    }

    /// Imports a private key.
    ///
    /// - Parameters:
    ///   - privateKey: private key to import
    ///   - password: password to use for the imported private key
    ///   - coin: coin to use for this wallet
    /// - Returns: new wallet
    public func `import`(privateKey: PrivateKey, password: String, coin: CoinType) throws -> Wallet {
        let newKey = StoredKey.importPrivateKey(privateKey: privateKey.data, password: password, coin: coin)
        let url = makeAccountURL()
        let wallet = Wallet(keyURL: url, key: newKey)
        let _ = try wallet.getAccount(password: password, coin: coin)
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
    public func `import`(mnemonic: String, encryptPassword: String, coins: [CoinType]) throws -> Wallet {
        guard HDWallet.isValid(mnemonic: mnemonic) else {
            throw Error.invalidMnemonic
        }

        let key = StoredKey.importHDWallet(mnemonic: mnemonic, password: encryptPassword, coin: coins.first ?? .ethereum)
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
    public func export(wallet: Wallet, password: String, newPassword: String) throws -> Data {
        var privateKeyData = try exportPrivateKey(wallet: wallet, password: password)
        defer {
            privateKeyData.resetBytes(in: 0 ..< privateKeyData.count)
        }

        guard let coin = wallet.key.account(index: 0)?.coin else {
            throw Error.accountNotFound
        }

        if let mnemonic = checkMnemonic(privateKeyData) {
            let newKey = StoredKey.importHDWallet(mnemonic: mnemonic, password: newPassword, coin: coin)
            guard let json = newKey.exportJSON() else {
                throw Error.invalidKey
            }
            return json
        } else {
            let newKey = StoredKey.importPrivateKey(privateKey: privateKeyData, password: newPassword, coin: coin)
            guard let json = newKey.exportJSON() else {
                throw Error.invalidKey
            }
            return json
        }
    }

    /// Exports a wallet as private key data.
    ///
    /// - Parameters:
    ///   - wallet: wallet to export
    ///   - password: account password
    /// - Returns: private key data for encrypted keys or menmonic phrase for HD wallets
    public func exportPrivateKey(wallet: Wallet, password: String) throws -> Data {
        guard let key = wallet.key.decryptPrivateKey(password: password) else {
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
        guard let mnemonic = wallet.key.decryptMnemonic(password: password) else {
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
        guard let index = wallets.index(of: wallet) else {
            fatalError("Missing wallet")
        }

        guard var privateKeyData = wallet.key.decryptPrivateKey(password: password) else {
            throw Error.invalidPassword
        }
        defer {
            privateKeyData.resetBytes(in: 0 ..< privateKeyData.count)
        }

        guard let coin = wallet.key.account(index: 0)?.coin else {
            throw Error.accountNotFound
        }

        if let mnemonic = checkMnemonic(privateKeyData) {
            wallets[index].key = StoredKey.importHDWallet(mnemonic: mnemonic, password: newPassword, coin: coin)
        } else {
            wallets[index].key = StoredKey.importPrivateKey(privateKey: privateKeyData, password: newPassword, coin: coin)
        }
    }

    /// Deletes an account including its key if the password is correct.
    public func delete(wallet: Wallet, password: String) throws {
        guard let index = wallets.index(of: wallet) else {
            fatalError("Missing wallet")
        }

        guard var privateKey = wallet.key.decryptPrivateKey(password: password) else {
            throw KeyStore.Error.invalidKey
        }
        defer {
            privateKey.resetBytes(in: 0..<privateKey.count)
        }
        wallets.remove(at: index)

        try FileManager.default.removeItem(at: wallet.keyURL)
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
        return String(format: "%04d-%02d-%02dT%02d-%02d-%02d.%09d%@", components.year!, components.month!, components.day!, components.hour!, components.minute!, components.second!, components.nanosecond!, tz)
    }
}
