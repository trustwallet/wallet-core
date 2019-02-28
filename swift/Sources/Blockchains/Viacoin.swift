// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public typealias ViacoinAddress = BitcoinAddress
public typealias ViacoinBech32Address = Bech32Address

public final class Viacoin: Bitcoin {
    override public var coinType: CoinType {
        return .viacoin
    }

    override public var privateKeyPrefix: UInt8 {
        return 0xC7
    }

    override public var p2pkhPrefix: UInt8 {
        return P2PKHPrefix.viacoin.rawValue
    }

    override public var p2shPrefix: UInt8 {
        return P2SHPrefix.viacoin.rawValue
    }

    override public var hrp: HRP {
        return .viacoin
    }

    override public var xpubVersion: HDVersion? {
        switch self.coinPurpose {
        case .bip44:
            return HDVersion.xpub
        case .bip49:
            return HDVersion.ypub
        case .bip84:
            return HDVersion.zpub
        }
    }

    override public var xprvVersion: HDVersion? {
        switch self.coinPurpose {
        case .bip44:
            return HDVersion.xprv
        case .bip49:
            return HDVersion.yprv
        case .bip84:
            return HDVersion.zprv
        }
    }
}
