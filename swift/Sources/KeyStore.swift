// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

private enum StoredKeyType {
    case privateKey(PrivateKey)
    case mnemonic(String)
    case tonMnemonic(String)
}

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

        switch try decryptSecret(key: key, password: Data(password.utf8)) {
        case .privateKey(let privateKey):
            return try self.import(privateKey: privateKey, name: name, password: newPassword, coin: coins.first ?? .ethereum)
        case .mnemonic(let mnemonic):
            return try self.import(mnemonic: mnemonic, name: name, encryptPassword: newPassword, coins: coins)
        case .tonMnemonic(let tonMnemonic):
            return try self.importTON(tonMnemonic: tonMnemonic, name: name, encryptPassword: newPassword, coin: coins.first ?? .ton)
        }
    }

    /// Decrypts an inner secret as a `privateKey`, `mnemonic` or another type.
    /// - Returns: a `StoredKeyType` enum.
    /// - Note: `StoredKey::type` is not always set, and mnemonic or private key can be stored encrypted without any tag.
    private func decryptSecret(key: StoredKey, password: Data) throws -> StoredKeyType {
        guard var secretData = key.decryptPrivateKey(password: password) else {
            throw Error.invalidPassword
        }
        defer {
            secretData.resetBytes(in: 0 ..< secretData.count)
        }

        // First, check whether the key is init with a TON mnemonic.
        // That's because TON Wallet is a new feature, and `StoredKey::type` is always set for these kind of keys.
        if key.isTONMnemonic {
            guard let tonMnemonic = String(data: secretData, encoding: .ascii), TONWallet.isValidMnemonic(mnemonic: tonMnemonic, passphrase: nil) else {
                throw Error.invalidMnemonic
            }
            return StoredKeyType.tonMnemonic(tonMnemonic)
        }

        // The next, we should try to convert the secret into a string and check whether it's a valid BIP39 mnemonic phrase.
        if let mnemonic = String(data: secretData, encoding: .ascii), Mnemonic.isValid(mnemonic: mnemonic) {
            return StoredKeyType.mnemonic(mnemonic)
        }

        // Otherwise, we consider the secret as a private key.

        if let privateKey = PrivateKey(data: secretData) {
            return StoredKeyType.privateKey(privateKey)
        }

        throw Error.invalidKey
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
    
    /// Imports a TON wallet.
    ///
    /// - Parameters:
    ///   - tonMnemonic: TON wallet's mnemonic phrase
    ///   - encryptPassword: password to use for encrypting
    ///   - coin: coins to use for this wallet
    /// - Returns: new account
    public func `importTON`(tonMnemonic: String, name: String, encryptPassword: String, coin: CoinType, encryption: StoredKeyEncryption = .aes128Ctr) throws -> Wallet {
        guard let newKey = StoredKey.importTONWalletWithEncryption(tonMnemonic: tonMnemonic, name: name, password: Data(encryptPassword.utf8), coin: coin, encryption: encryption) else {
            throw Error.invalidKey
        }

        let url = makeAccountURL()
        let wallet = Wallet(keyURL: url, key: newKey)
        // `StoredKey.importTONWalletWithEncryption` should create exactly one account only.
        if wallet.accounts.count != 1 {
            throw Error.invalidKey
        }
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
        // TODO why `importHDWalletWithEncryption` is called with a single coin?
        // I guess that's because we don't want other wallets to know all user accounts.
        guard let coin = wallet.key.account(index: 0)?.coin else {
            throw Error.accountNotFound
        }

        let secretType = try decryptSecret(key: wallet.key, password: Data(password.utf8))

        let maybeNewKey: StoredKey? = switch secretType {
        case .privateKey(let privateKey):
            StoredKey.importPrivateKeyWithEncryption(privateKey: privateKey.data, name: "", password: Data(newPassword.utf8), coin: coin, encryption: encryption)

        case .mnemonic(let mnemonic):
            StoredKey.importHDWalletWithEncryption(mnemonic: mnemonic, name: "", password: Data(newPassword.utf8), coin: coin, encryption: encryption)

        case .tonMnemonic(let tonMnemonic):
            StoredKey.importTONWalletWithEncryption(tonMnemonic: tonMnemonic, name: "", password: Data(newPassword.utf8), coin: coin, encryption: encryption)
        }

        guard let newKey = maybeNewKey, let json = newKey.exportJSON() else {
            throw Error.invalidKey
        }
        return json
    }

    /// Exports a wallet as private key data.
    ///
    /// - Parameters:
    ///   - wallet: wallet to export
    ///   - password: account password
    /// - Returns: private key data for encrypted keys or mnemonic phrase for HD wallets
    /// - Throws: `EncryptError.invalidPassword` if the password is incorrect.
    public func exportPrivateKey(wallet: Wallet, password: String) throws -> Data {
        guard let key = wallet.key.decryptPrivateKey(password: Data(password.utf8)) else {
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
    /// - Throws: `EncryptError.invalidPassword` if the password is incorrect.
    public func exportMnemonic(wallet: Wallet, password: String) throws -> String {
        guard let mnemonic = wallet.key.decryptMnemonic(password: Data(password.utf8)) else {
            throw Error.invalidPassword
        }
        return mnemonic
    }
    
    /// Exports a wallet as a TON mnemonic phrase.
    ///
    /// - Parameters:
    ///   - wallet: wallet to export
    ///   - password: account password
    /// - Returns: TON mnemonic phrase
    /// - Throws: `EncryptError.invalidPassword` if the password is incorrect.
    public func exportTONMnemonic(wallet: Wallet, password: String) throws -> String {
        guard let tonMnemonic = wallet.key.decryptTONMnemonic(password: Data(password.utf8)) else {
            throw Error.invalidPassword
        }
        return tonMnemonic
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

        let coins = wallet.accounts.map({ $0.coin })
        guard !coins.isEmpty else {
            throw Error.accountNotFound
        }

        let secretType = try decryptSecret(key: wallet.key, password: Data(password.utf8))

        let maybeNewKey: StoredKey? = switch secretType {
        case .privateKey(let privateKey):
            StoredKey.importPrivateKeyWithEncryption(
                privateKey: privateKey.data, name: newName, password: Data(newPassword.utf8), coin: coins[0], encryption: encryption)

        case .mnemonic(let mnemonic):
            StoredKey.importHDWalletWithEncryption(mnemonic: mnemonic, name: newName, password: Data(newPassword.utf8), coin: coins[0], encryption: encryption)

        case .tonMnemonic(let tonMnemonic):
            StoredKey.importTONWalletWithEncryption(tonMnemonic: tonMnemonic, name: newName, password: Data(newPassword.utf8), coin: coins[0], encryption: encryption)
        }

        guard let newKey = maybeNewKey else {
            throw Error.invalidKey
        }

        wallets[index].key = newKey
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
