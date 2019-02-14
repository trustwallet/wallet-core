// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public extension String {
    var twString: UnsafeRawPointer {
        return TWStringCreateWithNSString(self)
    }

    static func fromTWString(_ ptr: UnsafeRawPointer) -> String {
        return TWStringNSString(ptr) as String
    }

    static func fromTWString(_ ptr: UnsafeRawPointer?) -> String? {
        guard let ptr = ptr else {
            return nil
        }
        return TWStringNSString(ptr) as String
    }
}
