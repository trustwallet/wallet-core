// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

public enum HRP: UInt32, CustomStringConvertible  {
    case unknown = 0
    case bitcoin = 1
    case litecoin = 2
    case bitcoinCash = 3
    case binance = 4
    case binanceTest = 5

    public var description: String {
        switch self {
        case .unknown: return ""
        case .bitcoin: return "bc"
        case .litecoin: return "ltc"
        case .bitcoinCash: return "bitcoincash"
        case .binance: return "bnb"
        case .binanceTest: return "tbnb"
        }
    }
}
