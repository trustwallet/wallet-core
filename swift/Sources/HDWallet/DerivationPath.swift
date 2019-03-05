// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

/// Represents a hierarchical determinisic derivation path.
public struct DerivationPath: Codable, Hashable, CustomStringConvertible {
    let indexCount = 5

    /// List of indices in the derivation path.
    public private(set) var indices = [Index]()

    /// Address purpose, each coin will have a different value.
    public var purpose: Purpose {
        get {
            return Purpose(rawValue: indices[0].value)!
        }
        set {
            indices[0] = Index(newValue.rawValue, hardened: true)
        }
    }

    /// Coin type distinguishes between main net, test net, and forks.
    public var coinType: CoinType {
        get {
            return CoinType(rawValue: indices[1].value)!
        }
        set {
            indices[1] = Index(newValue.rawValue, hardened: true)
        }
    }

    /// Account number.
    public var account: UInt32 {
        get {
            return indices[2].value
        }
        set {
            indices[2] = Index(newValue, hardened: true)
        }
    }

    /// Change or private addresses will set this to 1.
    public var change: UInt32 {
        get {
            return indices[3].value
        }
        set {
            indices[3] = Index(newValue, hardened: false)
        }
    }

    /// Address number
    public var address: UInt32 {
        get {
            return indices[4].value
        }
        set {
            indices[4] = Index(newValue, hardened: false)
        }
    }

    init(indices: [Index]) {
        precondition(indices.count == indexCount, "Not enough indices")
        self.indices = indices
    }

    /// Creates a `DerivationPath` by components.
    public init(purpose: Purpose, coinType: CoinType, account: UInt32 = 0, change: UInt32 = 0, address: UInt32 = 0) {
        self.indices = [Index](repeating: Index(0), count: indexCount)
        self.purpose = purpose
        self.coinType = coinType
        self.account = account
        self.change = change
        self.address = address
    }

    /// Creates a derivation path with a string description like `m/10/0/2'/3`
    public init?(_ string: String) {
        let components = string.split(separator: "/")
        for component in components {
            if component == "m" {
                continue
            }
            if component.hasSuffix("'") {
                guard let index = UInt32(component.dropLast()) else {
                    return nil
                }
                indices.append(Index(index, hardened: true))
            } else {
                guard let index = UInt32(component) else {
                    return nil
                }
                indices.append(Index(index, hardened: false))
            }
        }
        guard indices.count == indexCount else {
            return nil
        }
    }

    /// String representation.
    public var description: String {
        return "m/" + indices.map({ $0.description }).joined(separator: "/")
    }

    public func hash(into hasher: inout Hasher) {
        indices.forEach { hasher.combine($0) }
    }

    public static func == (lhs: DerivationPath, rhs: DerivationPath) -> Bool {
        return lhs.indices == rhs.indices
    }
}
