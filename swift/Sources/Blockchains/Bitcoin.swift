// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public typealias TWBitcoinSigningInput = TW_Bitcoin_Proto_SigningInput
public typealias TWBitcoinSigningOutput = TW_Bitcoin_Proto_SigningOutput
public typealias TWBitcoinUnspentTransaction = TW_Bitcoin_Proto_UnspentTransaction
public typealias TWBitcoinTransactionPlan = TW_Bitcoin_Proto_TransactionPlan
public typealias TWBitcoinOutPoint = TW_Bitcoin_Proto_OutPoint

public extension HDWallet {
    static func derive(from extended: String, at path: DerivationPath) -> PublicKey? {
        let xpubVersion: HDVersion
        let xprvVersion: HDVersion
        switch path.purpose {
        case .bip44:
            xpubVersion = path.coinType == .litecoin ? .ltub: .xpub
            xprvVersion = path.coinType == .litecoin ? .ltpv: .xprv
        case .bip49:
            xpubVersion = path.coinType == .litecoin ? .mtub: .ypub
            xprvVersion = path.coinType == .litecoin ? .mtpv: .yprv
        case .bip84:
            xpubVersion = .zpub
            xprvVersion = .zprv
        }
        return HDWallet.getPublicKeyFromExtended(extended: extended, curve: path.coinType.curve, versionPublic: xpubVersion, versionPrivate: xprvVersion, change: path.change, address: path.address)
    }
}
