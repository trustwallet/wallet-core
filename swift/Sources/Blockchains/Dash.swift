// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public typealias DashAddress = BitcoinAddress

public final class Dash: Bitcoin {
    override public var coinType: CoinType {
        return .dash
    }

    /// Private key prefix.
    ///
    /// - SeeAlso: https://dash-docs.github.io/en/developer-guide#wallet-import-format-wif
    override public var privateKeyPrefix: UInt8 {
        return 0xCC
    }

    /// Public key hash address prefix.
    ///
    /// - SeeAlso: https://dash-docs.github.io/en/developer-reference#address-conversion
    override public var p2pkhPrefix: UInt8 {
        return P2PKHPrefix.dash.rawValue
    }

    override public var p2shPrefix: UInt8 {
        return P2SHPrefix.dash.rawValue
    }

    override public var supportSegwit: Bool {
        return false
    }

    override public init(purpose: Purpose = .bip44) {
        super.init(purpose: purpose)
    }

    override public func address(for publicKey: PublicKey) -> Address {
        return BitcoinAddress(publicKey: publicKey, prefix: p2pkhPrefix)
    }
}
