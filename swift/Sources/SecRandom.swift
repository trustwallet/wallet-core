// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Security

@_cdecl("random32")
public func random32() -> UInt32 {
    var bytes = [UInt8](repeating: 0, count: 32)
    guard SecRandomCopyBytes(kSecRandomDefault, bytes.count, &bytes) == errSecSuccess else {
        // failed to generate random number
        abort()
    }

    return bytes.withUnsafeBytes { ptr -> UInt32 in
        ptr.baseAddress!.assumingMemoryBound(to: UInt32.self).pointee.littleEndian
    }
}

@_cdecl("random_buffer")
public func random_buffer(buf: UnsafeMutableRawPointer, len: Int) {
    guard SecRandomCopyBytes(kSecRandomDefault, len, buf) == errSecSuccess else {
        // failed to generate random number
        abort()
    }
}
