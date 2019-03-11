// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

/// Coin wallet.
public final class Wallet: Hashable {
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

    /// Returns the only account for non HD-wallets.
    ///
    /// - Parameters:
    ///   - password: wallet encryption password
    ///   - type: blockchain type
    /// - Returns: the account
    /// - Throws: `KeyStore.Error.invalidPassword` if the password is incorrect.
    public func getAccount(password: String, coin: CoinType) throws -> Account {
        guard let account = key.accountForCoin(coin: coin, password: password) else {
            throw KeyStore.Error.invalidPassword
        }
        return account
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
