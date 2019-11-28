// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public extension Data {
    static func fromTWData(_ ptr: UnsafeRawPointer) -> Data {
        let nsdata = TWDataNSData(ptr)
        let data = nsdata as Data
        return data
    }

    static func fromTWData(_ ptr: UnsafeRawPointer?) -> Data? {
        guard let ptr = ptr else {
            return nil
        }
        return fromTWData(ptr)
    }
}
