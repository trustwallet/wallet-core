// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

extension PublicKey: Hashable {
    /// Compressed public key size.
    public static let compressedSize = TWPublicKeyCompressedSize

    /// Uncompressed public key size.
    public static let uncompressedSize = TWPublicKeyUncompressedSize

    // MARK: Hashable

    public static func == (lhs: PublicKey, rhs: PublicKey) -> Bool {
        return lhs.data == rhs.data
    }

    public func hash(into hasher: inout Hasher) {
        hasher.combine(data)
    }
}
