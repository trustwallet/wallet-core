// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public protocol Address: CustomStringConvertible {
    /// Validates that the string is a valid address.
    static func isValidString(string: String) -> Bool

    /// Raw representation of the address.
    var data: Data { get }

    /// Creates a address from a string representation.
    init?(string: String)
}
