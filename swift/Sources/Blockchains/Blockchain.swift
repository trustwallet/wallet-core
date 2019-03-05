// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

/// Blockchain represents what is unique about every blockchain.
open class Blockchain: Hashable {
    /// Coin type for Level 2 of BIP44.
    ///
    /// - SeeAlso: https://github.com/satoshilabs/slips/blob/master/slip-0044.md
    open var coinType: CoinType {
        fatalError("Use a specific Blockchain subclass")
    }

    open var coinPurpose: Purpose

    open var xpubVersion: HDVersion? {
        return nil
    }

    open var xprvVersion: HDVersion? {
        return nil
    }

    public init(purpose: Purpose = .bip44) {
        self.coinPurpose = purpose
    }

    /// Returns the address associated with a public key.
    open func address(for publicKey: PublicKey) -> Address {
        fatalError("Use a specific Blockchain subclass")
    }

    /// Returns the address given its string representation.
    open func address(string: String) -> Address? {
        fatalError("Use a specific Blockchain subclass")
    }

    /// Returns the address given its raw representation.
    open func address(data: Data) -> Address? {
        fatalError("Use a specific Blockchain subclass")
    }

    // MARK: Hashable

    public static func == (lhs: Blockchain, rhs: Blockchain) -> Bool {
        return lhs.coinType == rhs.coinType
    }

    public func hash(into hasher: inout Hasher) {
        hasher.combine(coinType)
    }

    public func derivationPath(account: UInt32 = 0, change: UInt32 = 0, at index: UInt32) -> DerivationPath {
        return DerivationPath(purpose: coinPurpose, coinType: coinType, account: account, change: change, address: index)
    }
}

public extension Blockchain {
    func derive(from extendedPubkey: String, at path: DerivationPath) -> String? {
        return TrustWalletCore.HDWallet.getAddressFromExtended(extended: extendedPubkey, curve: .secp256k1, coinType: path.coinType, change: path.change, address: path.address)
    }

    func derivePubkey(from extendedPubkey: String, at path: DerivationPath) -> PublicKey? {
        guard let xpubVer = xpubVersion,
            let xprvVer = xprvVersion else {
            return nil
        }
        return TrustWalletCore.HDWallet.getPublicKeyFromExtended(extended: extendedPubkey, curve: .secp256k1, versionPublic: xpubVer, versionPrivate: xprvVer, change: path.change, address: path.address)
    }
}
