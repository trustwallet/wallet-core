// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public extension HDWallet {
    static func derive(from extended: String, at path: DerivationPath) -> PublicKey? {
        let xpubVersion = path.coinType.xpubVersion
        let xprvVersion = path.coinType.xprvVersion
        return HDWallet.getPublicKeyFromExtended(extended: extended, coin: path.coinType, versionPublic: xpubVersion, versionPrivate: xprvVersion, change: path.change, address: path.address)
    }
}
