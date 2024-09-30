// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import Foundation

public final class MainStruct {
    let rawValue: OpaquePointer

    init(rawValue: OpaquePointer) {
        self.rawValue = rawValue
    }

    public init(string: String) {
        let string = TWStringCreateWithNSString(string)
        defer {
            TWStringDelete(string)
        }

        let result = MainStructCreate(string)

        self.rawValue = result
    }

    deinit {
        MainStructDelete(self.rawValue)
    }

    public static func secondFunction(first_param: Int32) -> Bool {
        let result = MainStructSecondFunction(first_param)
        return result
    }

    public var firstProperty: Bool {
        let obj = self.rawValue
        let result = MainStructFirstProperty(obj)
        return result
    }
}
