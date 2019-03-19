// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

extension CoinType {
    var keyPrefix: UInt8 {
        switch self {
        case .bitcoin:
            return 0x80
        case .litecoin:
            return 0xb0
        case .dash:
            return 0xcc
        case .zcoin:
            return 0xd2
        default:
            fatalError("no private prefix: \(self)")
        }
    }
}

extension PrivateKey {
    /// Creates a `PrivateKey` from a Bitcoin WIF (wallet import format) string.

    static let prefixSet = Set<UInt8>([CoinType.bitcoin, .litecoin, .dash, .zcoin].map {$0.keyPrefix})

    public convenience init?(wif: String) {
        guard let decoded = Base58.decode(string: wif) else {
            return nil
        }
        guard PrivateKey.prefixSet.contains(decoded[0]) else {
            return nil
        }
        if decoded.count == 34 && decoded.last != 0x01 {
            return nil
        }
        self.init(data: Data(decoded[1 ..< 33]))
    }

    public func wif(for coin: CoinType = .bitcoin) -> String {
        let result = Data(bytes: [coin.keyPrefix]) + data
        let check = Hash.sha256SHA256(data: result)[0..<4]
        return Base58.encodeNoCheck(data: result + check)
    }
}
