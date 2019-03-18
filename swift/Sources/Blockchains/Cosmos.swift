// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public class Cosmos: Blockchain {

    public override init(purpose: Purpose = .bip44) {
        super.init(purpose: purpose)
    }

    public let hrp: HRP = .cosmos

    override public var coinType: CoinType {
        return .cosmos
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
