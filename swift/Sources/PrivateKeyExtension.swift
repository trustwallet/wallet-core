// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

extension PrivateKeySecp256k1: Hashable {
    /// Private key size in bytes.
    public static let size = TWPrivateKeySecp256k1Size

    // MARK: Hashable

    public func hash(into hasher: inout Hasher) {
        hasher.combine(data)
    }

    public static func == (lhs: PrivateKeySecp256k1, rhs: PrivateKeySecp256k1) -> Bool {
        return lhs.data == rhs.data
    }
}
