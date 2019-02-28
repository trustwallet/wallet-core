// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

extension PrivateKey {
    /// Creates a `PrivateKey` from a Bitcoin WIF (wallet import format) string.

    static let prefixSet = Set([
        Bitcoin().privateKeyPrefix,
        Litecoin().privateKeyPrefix,
        Dash().privateKeyPrefix,
        Zcoin().privateKeyPrefix,
    ])

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

    public func wif(for coin: Bitcoin = Bitcoin()) -> String {
        let result = Data(bytes: [coin.privateKeyPrefix]) + data
        let check = Hash.sha256SHA256(data: result)[0..<4]
        return Base58.encodeNoCheck(data: result + check)
    }
}
