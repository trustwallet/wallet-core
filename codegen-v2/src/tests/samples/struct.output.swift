// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import Foundation

public struct MainStruct {
    init() {}

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

    public static func firstFunction(first_param: Int32) -> Bool {
        let result = MainStructFirstFunction(first_param)
        return result
    }

    public var firstProperty: Bool {
        let obj = self.rawValue
        let result = MainStructFirstProperty(obj)
        return result
    }
}
