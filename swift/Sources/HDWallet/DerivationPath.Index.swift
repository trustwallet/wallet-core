// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

extension DerivationPath {
    /// Derivation path index.
    public struct Index: Codable, Hashable, CustomStringConvertible {
        /// Index value.
        public var value: UInt32

        /// Whether the index is hardened.
        public var hardened: Bool

        /// The derivation index.
        public var derivationIndex: UInt32 {
            if hardened {
                return UInt32(value) | 0x80000000
            } else {
                return UInt32(value)
            }
        }

        public init(_ value: UInt32, hardened: Bool = true) {
            self.value = value
            self.hardened = hardened
        }

        public func hash(into hasher: inout Hasher) {
            hasher.combine(value)
            hasher.combine(hardened)
        }

        public static func == (lhs: Index, rhs: Index) -> Bool {
            return lhs.value == rhs.value && lhs.hardened == rhs.hardened
        }

        public var description: String {
            if hardened {
                return "\(value)'"
            } else {
                return value.description
            }
        }
    }
}
