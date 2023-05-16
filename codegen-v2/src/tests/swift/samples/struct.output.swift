// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
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
