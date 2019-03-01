// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public typealias LitecoinAddress = BitcoinAddress
public typealias LitecoinBech32Address = Bech32Address

public final class Litecoin: Bitcoin {
    override public var coinType: CoinType {
        return .litecoin
    }

    override public var privateKeyPrefix: UInt8 {
        return 0xB0
    }

    override public var p2pkhPrefix: UInt8 {
        return P2PKHPrefix.litecoin.rawValue
    }

    override public var p2shPrefix: UInt8 {
        return P2SHPrefix.litecoin.rawValue
    }

    override public var hrp: HRP {
        return .litecoin
    }

    override public var xpubVersion: HDVersion? {
        switch self.coinPurpose {
        case .bip44:
            return HDVersion.ltub
        case .bip49:
            return HDVersion.mtub
        case .bip84:
            return HDVersion.zpub
        }
    }

    override public var xprvVersion: HDVersion? {
        switch self.coinPurpose {
        case .bip44:
            return HDVersion.ltpv
        case .bip49:
            return HDVersion.mtpv
        case .bip84:
            return HDVersion.zprv
        }
    }
}
