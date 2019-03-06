// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

public enum CoinType: UInt32 {
    case binance = 714
    case bitcoin = 0
    case bitcoinCash = 145
    case callisto = 820
    case dash = 5
    case eos = 194
    case ethereum = 60
    case ethereumClassic = 61
    case go = 6060
    case icon = 74
    case litecoin = 2
    case poa = 178
    case ripple = 144
    case tezos = 1729
    case thunderToken = 1001
    case tomoChain = 889
    case tron = 195
    case veChain = 818
    case wanChain = 5718350
    case zcash = 133
    case zcoin = 136

    public var purpose: Purpose {
        return Purpose(rawValue: TWCoinTypePurpose(TWCoinType(rawValue: rawValue)).rawValue)!
    }

    public var curve: Curve {
        return Curve(rawValue: TWCoinTypeCurve(TWCoinType(rawValue: rawValue)).rawValue)!
    }
}
