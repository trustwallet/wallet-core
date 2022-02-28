// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
