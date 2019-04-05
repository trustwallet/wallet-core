// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public extension HDWallet {
    static func derive(from extended: String, at path: DerivationPath) -> PublicKey? {
        let xpubVersion: HDVersion
        let xprvVersion: HDVersion
        switch path.purpose {
        case .bip44:
            switch path.coinType {
            case .litecoin:
                xpubVersion = .ltub
                xprvVersion = .ltpv
            case .decred:
                xpubVersion = .dpub
                xprvVersion = .dprv
            default:
                xpubVersion = .xpub
                xprvVersion = .xprv
            }
        case .bip49:
            xpubVersion = path.coinType == .litecoin ? .mtub: .ypub
            xprvVersion = path.coinType == .litecoin ? .mtpv: .yprv
        case .bip84:
            xpubVersion = .zpub
            xprvVersion = .zprv
        }
        return HDWallet.getPublicKeyFromExtended(extended: extended, coin: path.coinType, versionPublic: xpubVersion, versionPrivate: xprvVersion, change: path.change, address: path.address)
    }
}

extension ZcashTAddress: Address {}
