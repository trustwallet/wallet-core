// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import Foundation

public extension CoinType {
    /// Converts a string to an address for this coin type.
    func address(string: String) -> AnyAddress? {
        return AnyAddress(string: string, coin: self)
    }
}
