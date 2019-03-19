// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public final class Decred: Bitcoin {
    override public var coinType: CoinType {
        return .decred
    }

    /// Public key hash address prefix.
    ///
    /// - SeeAlso: https://dash-docs.github.io/en/developer-reference#address-conversion
    override public var p2pkhPrefix: UInt8 {
        return P2PKHPrefix.decred.rawValue
    }

    override public var p2shPrefix: UInt8 {
        return P2SHPrefix.decred.rawValue
    }

    override public var supportSegwit: Bool {
        return false
    }

    override public init(purpose: Purpose = .bip44) {
        super.init(purpose: purpose)
    }

    override public func address(for publicKey: PublicKey) -> Address {
        return DecredAddress(publicKey: publicKey)
    }
}

extension DecredAddress: Address {
    public static func isValid(data: Data) -> Bool {
        fatalError("Data representation not supported")
    }

    public var data: Data {
        fatalError("Data representation not supported")
    }
}
