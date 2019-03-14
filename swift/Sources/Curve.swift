// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

public enum Curve: UInt32, CustomStringConvertible  {
    case secp256k1 = 0
    case ed25519 = 1
    case nist256p1 = 2

    public var description: String {
        switch self {
        case .secp256k1: return "secp256k1"
        case .ed25519: return "ed25519"
        case .nist256p1: return "nist256p1"
        }
    }
}
