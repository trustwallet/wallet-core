// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import UIKit
import TrustWalletCore
import SwiftProtobuf

enum PlaygroundError: Error {
    case invalidHexString
}

func reverseHex(string: String) throws -> String {
    guard let data = Data(hexString: string) else {
        throw PlaygroundError.invalidHexString
    }
    return Data(data.reversed()).hexString
}

try reverseHex(string: "0xdeadbeef")
