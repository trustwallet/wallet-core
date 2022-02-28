// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public extension CoinType {
    /// Converts a string to an address for this coin type.
    func address(string: String) -> AnyAddress? {
        return AnyAddress(string: string, coin: self)
    }
}
