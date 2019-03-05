// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

/// Ethereum address.
extension EthereumAddress: Address, Hashable {
    public static let size = 20

    /// Validates that the raw data is a valid address.
    static public func isValid(data: Data) -> Bool {
        return data.count == EthereumAddress.size
    }

    /// EIP55 representation of the address.
    public var eip55String: String {
        return description
    }

    public var data: Data {
        return keyHash
    }

    public func hash(into hasher: inout Hasher) {
        hasher.combine(data)
    }
}
