// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import Foundation

public func CardanoMinAdaAmount(tokenBundle: Data) -> UInt64 {
    let tokenBundleData = TWDataCreateWithNSData(tokenBundle)
    defer {
        TWDataDelete(tokenBundleData)
    }
    return TWCardanoMinAdaAmount(tokenBundleData)
}
