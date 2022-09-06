// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

extension DerivationPath: Equatable, Hashable, CustomStringConvertible {

    public typealias Index = DerivationPathIndex

    public static func == (lhs: DerivationPath, rhs: DerivationPath) -> Bool {
        return lhs.description == rhs.description
    }

    public var coinType: UInt32 {
        coin
    }

    public var indices: [Index] {
        var result = [Index]()
        for i in 0..<indicesCount() {
            guard let index = indexAt(index: i) else {
                continue
            }
            result.append(index)
        }
        return result
    }

    public convenience init(purpose: Purpose, coin: UInt32) {
        self.init(purpose: purpose, coin: coin, account: 0, change: 0, address: 0)
    }

    public convenience init?(_ string: String) {
        self.init(string: string)
    }

    public subscript(index: Int) -> DerivationPathIndex? {
        return self.indexAt(index: UInt32(index))
    }

    public func hash(into hasher: inout Hasher) {
        let count = indicesCount()
        for i in 0..<count {
            hasher.combine(self[Int(i)])
        }
    }
}

extension DerivationPath: Codable {
    private enum CodingKeys: String, CodingKey {
        case purpose
        case coin
        case account
        case change
        case address
    }

    public func encode(to encoder: Encoder) throws {
        var container = encoder.container(keyedBy: CodingKeys.self)
        try container.encode(purpose.rawValue, forKey: .purpose)
        try container.encode(coin, forKey: .coin)
        try container.encode(account, forKey: .account)
        try container.encode(change, forKey: .change)
        try container.encode(address, forKey: .address)
    }

    public convenience init(from decoder: Decoder) throws {
        let container = try decoder.container(keyedBy: CodingKeys.self)
        let purpose = try container.decode(UInt32.self, forKey: .purpose)
        guard
            let purposeEnum = Purpose(rawValue: purpose)
        else {
            throw DecodingError.dataCorruptedError(forKey: CodingKeys.purpose, in: container, debugDescription: "purpose value is not valid")
        }
        let coin = try container.decode(UInt32.self, forKey: .coin)
        let account = try container.decode(UInt32.self, forKey: .account)
        let change = try container.decode(UInt32.self, forKey: .change)
        let address = try container.decode(UInt32.self, forKey: .address)
        self.init(purpose: purposeEnum, coin: coin, account: account, change: change, address: address)
    }
}

extension DerivationPathIndex: Equatable, Hashable, CustomStringConvertible {
    public static func == (lhs: DerivationPathIndex, rhs: DerivationPathIndex) -> Bool {
        return lhs.value == rhs.value && lhs.hardened == rhs.hardened
    }

    public convenience init(_ value: UInt32, hardened: Bool) {
        self.init(value: value, hardened: hardened)
    }

    public func hash(into hasher: inout Hasher) {
        hasher.combine(value)
        hasher.combine(hardened)
    }
}

extension DerivationPathIndex: Codable {
    private enum CodingKeys: String, CodingKey {
        case value
        case hardened
    }

    public func encode(to encoder: Encoder) throws {
        var container = encoder.container(keyedBy: CodingKeys.self)
        try container.encode(value, forKey: .value)
        try container.encode(hardened, forKey: .hardened)
    }

    public convenience init(from decoder: Decoder) throws {
        let container = try decoder.container(keyedBy: CodingKeys.self)
        let value = try container.decode(UInt32.self, forKey: .value)
        let hardened = try container.decode(Bool.self, forKey: .hardened)
        self.init(value: value, hardened: hardened)
    }
}
