// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
