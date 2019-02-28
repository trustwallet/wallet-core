// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

extension PrivateKey: Hashable {
    /// Private key size in bytes.
    public static let size = TWPrivateKeySize

    // MARK: Hashable

    public func hash(into hasher: inout Hasher) {
        hasher.combine(data)
    }

    public static func == (lhs: PrivateKey, rhs: PrivateKey) -> Bool {
        return lhs.data == rhs.data
    }
}
