// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

/// An account watch.
public struct Watch: Codable, Equatable {
    /// Coin type.
    public var coin: CoinType

    /// Account name
    public var name: String

    /// Account address.
    public var address: String

    /// Account xpub.
    public var xpub: String?

    public init(coin: CoinType, name: String, address: String, xpub: String?) {
        self.coin = coin
        self.name = name
        self.address = address
        self.xpub = xpub
    }

    enum CodingKeys: CodingKey {
        case coin
        case name
        case address
        case xpub
    }

    public init(from decoder: Decoder) throws {
        let container = try decoder.container(keyedBy: CodingKeys.self)
        guard let coin = CoinType(rawValue: try container.decode(CoinType.RawValue.self, forKey: .coin)) else {
            throw DecodingError.dataCorruptedError(forKey: CodingKeys.coin, in: container, debugDescription: "Invalid coin type")
        }
        self.coin = coin
        name = try container.decodeIfPresent(String.self, forKey: .name) ?? ""
        address = try container.decode(String.self, forKey: .address)
        xpub = try container.decodeIfPresent(String.self, forKey: .xpub)
    }

    public func encode(to encoder: Encoder) throws {
        var container = encoder.container(keyedBy: CodingKeys.self)
        try container.encode(coin.rawValue, forKey: .coin)
        try container.encode(name, forKey: .name)
        try container.encode(address, forKey: .address)
        try container.encodeIfPresent(xpub, forKey: .xpub)
    }
}
