// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import Foundation

/// Converts a Data struct to TWData/UnsafeRawPointer caller must delete it after use.
public func TWDataCreateWithNSData(_ data: Data) -> UnsafeRawPointer {
    return TWDataCreateWithBytes([UInt8](data), data.count)
}

/// Converts a TWData/UnsafeRawPointer (will be deleted within this call) to a Data struct.
public func TWDataNSData(_ data: UnsafeRawPointer) -> Data {
    defer {
        TWDataDelete(data)
    }
    return Data(bytes: TWDataBytes(data), count: TWDataSize(data))
}
