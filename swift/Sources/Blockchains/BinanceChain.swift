// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public class BinanceChain: Blockchain {

    public override init(purpose: Purpose = .bip44) {
        super.init(purpose: purpose)
    }

    public var hrp: HRP {
        return .binance
    }

    override public var coinType: CoinType {
        return .binance
    }

    override public func address(for publicKey: PublicKey) -> Address {
        return TendermintAddress(hrp: hrp, publicKey: publicKey.compressed)!
    }

    override open func address(string: String) -> Address? {
        return TendermintAddress(string: string)
    }

    override open func address(data: Data) -> Address? {
        return TendermintAddress(hrp: hrp, keyHash: data)
    }
}

public class BinanceChainTestnet: BinanceChain {
    override public var hrp: HRP {
        return .binanceTest
    }
}
