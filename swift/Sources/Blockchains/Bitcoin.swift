// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

typealias TWBitcoinSigningInput = TW_Bitcoin_Proto_SigningInput
typealias TWBitcoinSigningOutput = TW_Bitcoin_Proto_SigningOutput
typealias TWBitcoinUnspentTransaction = TW_Bitcoin_Proto_UnspentTransaction
typealias TWBitcoinTransactionPlan = TW_Bitcoin_Proto_TransactionPlan
typealias TWBitcoinOutPoint = TW_Bitcoin_Proto_OutPoint

/// Bitcoin blockchain.
///
/// Bitcoin-based blockchains should inherit from this class.
open class Bitcoin: Blockchain {
    /// SLIP-044 coin type.
    override open var coinType: CoinType {
        return .bitcoin
    }

    override open var xpubVersion: HDVersion? {
        switch self.coinPurpose {
        case .bip44:
            return HDVersion.xpub
        case .bip49:
            return HDVersion.ypub
        case .bip84:
            return HDVersion.zpub
        }
    }

    override open var xprvVersion: HDVersion? {
        switch self.coinPurpose {
        case .bip44:
            return HDVersion.xprv
        case .bip49:
            return HDVersion.yprv
        case .bip84:
            return HDVersion.zprv
        }
    }

    /// Private key prefix.
    ///
    /// - SeeAlso: https://en.bitcoin.it/wiki/List_of_address_prefixes
    open var privateKeyPrefix: UInt8 {
        return 0x80
    }

    /// Public key hash address prefix.
    ///
    /// - SeeAlso: https://en.bitcoin.it/wiki/List_of_address_prefixes
    open var p2pkhPrefix: UInt8 {
        return P2PKHPrefix.bitcoin.rawValue
    }

    /// Pay to script hash (P2SH) address prefix.
    ///
    /// - SeeAlso: https://en.bitcoin.it/wiki/List_of_address_prefixes
    open var p2shPrefix: UInt8 {
        return P2SHPrefix.bitcoin.rawValue
    }

    public var prefixSet: Set<UInt8> {
        return Set([p2shPrefix, p2pkhPrefix])
    }

    open var hrp: HRP {
        return .bitcoin
    }

    open var supportSegwit: Bool {
        return true
    }

    override open func address(for publicKey: PublicKey) -> Address {
        switch coinPurpose {
        case .bip44:
            let hash = Data([p2pkhPrefix]) + publicKey.bitcoinKeyHash
            return BitcoinAddress(data: hash)!
        case .bip49:
            return BitcoinAddress.compatibleAddress(publicKey: publicKey, prefix: p2shPrefix)
        case .bip84:
            let address = Bech32Address(hrp: hrp, publicKey: publicKey.compressed)
            return address
        }
    }

    override open func address(string: String) -> Address? {
        if let bech32Address = Bech32Address(string: string) {
            return bech32Address
        } else {
            return BitcoinAddress(string: string)
        }
    }

    override open func address(data: Data) -> Address? {
        if let bech32Address = Bech32Address(hrp: hrp, data: data) {
            return bech32Address
        } else {
            return BitcoinAddress(data: data)
        }
    }

    override public init(purpose: Purpose = .bip84) {
        super.init(purpose: purpose)
    }

    open func compatibleAddress(for publicKey: PublicKey) -> Address {
        return BitcoinAddress.compatibleAddress(publicKey: publicKey, prefix: p2shPrefix)
    }

    open func compatibleAddress(string: String) -> Address? {
        return BitcoinAddress(string: string)
    }

    open func compatibleAddress(data: Data) -> Address? {
        return BitcoinAddress(data: data)
    }

    open func legacyAddress(for publicKey: PublicKey) -> Address {
        return BitcoinAddress(publicKey: publicKey, prefix: p2pkhPrefix)
    }

    open func legacyAddress(string: String) -> Address? {
        return BitcoinAddress(string: string)
    }

    open func legacyAddress(data: Data) -> Address? {
        return BitcoinAddress(data: data)
    }

    override public func derivationPath(account: UInt32 = 0, change: UInt32 = 0, at index: UInt32) -> DerivationPath {
        return DerivationPath(purpose: coinPurpose, coinType: coinType, account: account, change: change, address: index)
    }

    public func validate(address: String) -> Bool {
        if let data = Base58.decode(string: address) {
            guard data.count == 21, prefixSet.contains(data[0]) else {
                return false
            }
            return true
        } else if let bech32Address = BitcoinSegwitAddress(string: address), supportSegwit {
            return bech32Address.hrp == self.hrp
        } else {
            return false
        }
    }
}
