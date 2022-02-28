// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

extension Account: Equatable {
    public static func == (lhs: Account, rhs: Account) -> Bool {
        return lhs.coin == rhs.coin &&
        lhs.address == rhs.address &&
        lhs.derivationPath == rhs.address &&
        lhs.extendedPublicKey == rhs.extendedPublicKey
    }
}

extension Account: Hashable {
    public func hash(into hasher: inout Hasher) {
        hasher.combine(coin)
        hasher.combine(address)
        hasher.combine(derivationPath)
        hasher.combine(extendedPublicKey)
    }
}

extension Account: Codable {
    private enum CodingKeys: String, CodingKey {
        case coin
        case address
        case derivationPath
        case extendedPublicKey
    }

    public func encode(to encoder: Encoder) throws {
        var container = encoder.container(keyedBy: CodingKeys.self)
        try container.encode(coin.rawValue, forKey: .coin)
        try container.encode(address, forKey: .address)
        try container.encode(derivationPath, forKey: .derivationPath)
        try container.encode(extendedPublicKey, forKey: .extendedPublicKey)
    }

    public convenience init(from decoder: Decoder) throws {
        let container = try decoder.container(keyedBy: CodingKeys.self)
        let rawCoin           = try container.decode(UInt32.self, forKey: .coin)
        let address           = try container.decode(String.self, forKey: .address)
        let derivationPath    = try container.decode(String.self, forKey: .derivationPath)
        let extendedPublicKey = try container.decode(String.self, forKey: .extendedPublicKey)

        self.init(
            address: address,
            coin: CoinType(rawValue: rawCoin)!,
            derivationPath: derivationPath,
            extendedPublicKey: extendedPublicKey
        )
    }
}
