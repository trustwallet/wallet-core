// Copyright © 2017-2020 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

/// Coin wallet. Wrapper for wallet core StoreWallet.
public final class Wallet: Hashable, Equatable {
    public let storeWallet: StoreWallet

    /// Creates a `Wallet` from an encrypted key.
    public init(keyURL: URL, key: StoredKey) {
        self.storeWallet = StoreWallet(path: keyURL.absoluteString, key: key)
    }

    /// Returns the account for a specific coin.
    ///
    /// - Parameters:
    ///   - password: wallet encryption password
    ///   - coin: coin type
    /// - Returns: the account
    /// - Throws: `KeyStore.Error.invalidPassword` if the password is incorrect.
    public func getAccount(password: String, coin: CoinType) throws -> Account {
        return storeWallet.getAccount(password: password, coin: coin)
    }

    /// Returns the accounts for a specific coins.
    ///
    /// - Parameters:
    ///   - password: wallet encryption password
    ///   - coins: coins to add accounts for
    /// - Returns: the added accounts
    /// - Throws: `KeyStore.Error.invalidPassword` if the password is incorrect.
    public func getAccounts(password: String, coins: [CoinType]) throws -> [Account] {
        return coins.compactMap({ storeWallet.getAccount(password: password, coin: $0) })
    }

    /// Returns the private key for a specific coin.
    ///
    /// - Parameters:
    ///   - password: wallet encryption password
    ///   - coin: coin type
    /// - Returns: the private key
    /// - Throws: `KeyStore.Error.invalidPassword` if the password is incorrect.
    public func privateKey(password: String, coin: CoinType) throws -> PrivateKey {
        return storeWallet.privateKey(password: password, coin: coin)
    }

    public func hash(into hasher: inout Hasher) {
        hasher.combine(storeWallet.indentifier)
    }

    public static func == (lhs: Wallet, rhs: Wallet) -> Bool {
        return lhs.storeWallet == rhs.storeWallet
    }
}
