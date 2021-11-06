// Copyright © 2017-2020 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

// https://developer.trustwallet.com/add_new_asset/universal_asset_id
public struct UniversalAssetID: CustomStringConvertible, Equatable, Hashable {
    public enum Prefix: String {
        case coin = "c"
        case token = "t"
    }

    public let coin: CoinType
    public let token: String

    public var description: String {
        let prefix = "\(Prefix.coin.rawValue)\(coin.rawValue)"
        guard !token.isEmpty else { return prefix }
        let suffix = "\(Prefix.token.rawValue)\(token)"
        return [prefix, suffix].joined(separator: "_")
    }

    public init(coin: CoinType, token: String = "") {
        self.coin = coin
        self.token = token
    }

    public init?(string: String) {
        guard !string.isEmpty else { return nil }
        let parts = string.split(separator: "_")

        var prefix = Prefix.coin.rawValue
        guard
            parts[0].hasPrefix(prefix),
            let int = UInt32(String(parts[0].dropFirst(prefix.count))),
            let coin = CoinType(rawValue: int)
        else {
            return nil
        }
        self.coin = coin

        prefix = Prefix.token.rawValue
        if parts.count == 2 {
            if !parts[1].hasPrefix(prefix) {
                return nil
            }
            self.token = String(parts[1].dropFirst(prefix.count))
        } else {
            self.token = ""
        }
    }
}

extension UniversalAssetID: Codable {
    public init(from decoder: Decoder) throws {
        let container = try decoder.singleValueContainer()
        let value = try container.decode(String.self)
        guard let id = UniversalAssetID(string: value) else {
            throw DecodingError.dataCorruptedError(in: container, debugDescription: "Invalid asset id")
        }
        self = id
    }

    public func encode(to encoder: Encoder) throws {
        var container = encoder.singleValueContainer()
        try container.encode(description)
    }
}
