// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

/// Coin wallet.
public final class Wallet: Hashable, Equatable {
    /// Unique wallet identifier.
    public let identifier: String

    /// URL for the key file on disk.
    public var keyURL: URL

    /// Encrypted wallet key
    public var key: StoredKey

    public var accounts: [Account] {
        return (0..<key.accountCount).compactMap({ key.account(index: $0) })
    }

    /// Creates a `Wallet` from an encrypted key.
    public init(keyURL: URL, key: StoredKey) {
        identifier = keyURL.lastPathComponent
        self.keyURL = keyURL
        self.key = key
    }

    /// Returns the account for a specific coin.
    ///
    /// - Parameters:
    ///   - password: wallet encryption password
    ///   - coin: coin type
    /// - Returns: the account
    /// - Throws: `KeyStore.Error.invalidPassword` if the password is incorrect.
    public func getAccount(password: String, coin: CoinType) throws -> Account {
        let wallet = key.wallet(password: Data(password.utf8))
        guard let account = key.accountForCoin(coin: coin, wallet: wallet) else {
            throw KeyStore.Error.invalidPassword
        }
        return account
    }

    /// Returns the account for a specific coin and derivation.
    ///
    /// - Parameters:
    ///   - password: wallet encryption password
    ///   - coin: coin type
    ///   - derivation: derivation, a specific or default
    /// - Returns: the account
    /// - Throws: `KeyStore.Error.invalidPassword` if the password is incorrect.
    public func getAccount(password: String, coin: CoinType, derivation: Derivation) throws -> Account {
        let wallet = key.wallet(password: Data(password.utf8))
        guard let account = key.accountForCoinDerivation(coin: coin, derivation: derivation, wallet: wallet) else {
            throw KeyStore.Error.invalidPassword
        }
        return account
    }

    /// Returns the accounts for a specific coins.
    ///
    /// - Parameters:
    ///   - password: wallet encryption password
    ///   - coins: coins to add accounts for
    /// - Returns: the added accounts
    /// - Throws: `KeyStore.Error.invalidPassword` if the password is incorrect.
    public func getAccounts(password: String, coins: [CoinType]) throws -> [Account] {
        guard let wallet = key.wallet(password: Data(password.utf8)) else {
            throw KeyStore.Error.invalidPassword
        }
        return coins.compactMap({ key.accountForCoin(coin: $0, wallet: wallet) })
    }

    /// Returns the private key for a specific coin.
    ///
    /// - Parameters:
    ///   - password: wallet encryption password
    ///   - coin: coin type
    /// - Returns: the private key
    /// - Throws: `KeyStore.Error.invalidPassword` if the password is incorrect.
    public func privateKey(password: String, coin: CoinType) throws -> PrivateKey {
        guard let pk = key.privateKey(coin: coin, password: Data(password.utf8)) else {
            throw KeyStore.Error.invalidPassword
        }
        return pk
    }

    public func hash(into hasher: inout Hasher) {
        hasher.combine(identifier)
    }

    public static func == (lhs: Wallet, rhs: Wallet) -> Bool {
        return lhs.identifier == rhs.identifier
    }
}

/// Support account types.
public enum WalletType {
    case encryptedKey
    case hierarchicalDeterministicWallet
}

public enum WalletError: LocalizedError {
    case invalidKeyType
}
