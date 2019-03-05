// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

public typealias ZcoinAddress = BitcoinAddress

public final class Zcoin: Bitcoin {
    override public var coinType: CoinType {
        return .zcoin
    }

    override public var privateKeyPrefix: UInt8 {
        return 0xd2
    }

    override public var p2pkhPrefix: UInt8 {
         return P2PKHPrefix.zcoin.rawValue
    }

    override public var p2shPrefix: UInt8 {
        return P2SHPrefix.zcoin.rawValue
    }

    override public var supportSegwit: Bool {
        return false
    }

    override public init(purpose: Purpose = .bip44) {
        super.init(purpose: purpose)
    }

    override public func address(for publicKey: PublicKeySecp256k1) -> Address {
        return BitcoinAddress(publicKey: publicKey, prefix: p2pkhPrefix)
    }
}
