// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import Foundation

/// Converts a String struct to TWString/UnsafeRawPointer, caller must delete it after use.
public func TWStringCreateWithNSString(_ string: String) -> UnsafeRawPointer {
    return TWStringCreateWithUTF8Bytes([CChar](string.utf8CString))
}

/// Converts a TWString/UnsafeRawPointer (will be deleted within this call) to a String struct.
public func TWStringNSString(_ string: UnsafeRawPointer) -> String {
    defer {
        TWStringDelete(string)
    }
    return String(utf8String: TWStringUTF8Bytes(string))!
}
