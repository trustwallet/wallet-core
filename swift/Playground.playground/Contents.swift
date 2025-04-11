// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import UIKit
import TrustWalletCore
import WalletCoreSwiftProtobuf

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
