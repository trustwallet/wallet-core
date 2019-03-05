// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

extension PublicKeySecp256k1: Hashable {
    /// Compressed public key size.
    public static let compressedSize = TWPublicKeySecp256k1CompressedSize

    /// Uncompressed public key size.
    public static let uncompressedSize = TWPublicKeySecp256k1UncompressedSize

    // MARK: Hashable

    public static func == (lhs: PublicKeySecp256k1, rhs: PublicKeySecp256k1) -> Bool {
        return lhs.data == rhs.data
    }

    public func hash(into hasher: inout Hasher) {
        hasher.combine(data)
    }
}
