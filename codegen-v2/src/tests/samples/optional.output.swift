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

    public init?(string: String?) {
        guard let result = MainStructCreate(string) else {
            return nil
        }

        self.rawValue = result
    }

    deinit {
        MainStructDelete(self.rawValue)
    }

    public static func withOptionalInt(first_param: Int32?) -> Bool? {
        guard let result = MainStructWithOptionalInt(first_param) else {
            return nil
        }
        return result
    }

    public static func withOptionalStruct(first_param: SomeStruct?) -> Bool? {
        let first_param = first_param?.rawValue
        guard let result = MainStructWithOptionalStruct(first_param) else {
            return nil
        }
        return result
    }

    public static func withOptionalString(first_param: String?) -> Bool? {
        let ptr: UnsafeRawPointer?
        if let first_param = first_param {
            ptr = TWStringCreateWithNSString(first_param)
        } else {
            ptr = nil
        }
        defer {
            if let first_param  = ptr {
                TWStringDelete(first_param)
            }
        }
        let first_param = ptr

        guard let result = MainStructWithOptionalString(first_param) else {
            return nil
        }
        return result
    }

    public static func withOptionalEnum(first_param: SomeEnum?) -> Bool? {
        let first_param = SomeEnum(rawValue: first_param.rawValue)
        guard let result = MainStructWithOptionalEnum(first_param) else {
            return nil
        }
        return result
    }

    public var withOptionalInt: Int32? {
        let obj = self.rawValue
        guard let result = MainStructWithOptionalInt(obj) else {
            return nil
        }
        return result
    }

    public var withOptionalString: String? {
        let obj = self.rawValue
        guard let result = MainStructWithOptionalString(obj) else {
            return nil
        }
        return TWStringNSString(result)
    }

    public var withOptionalStruct: SomeStruct? {
        let obj = self.rawValue
        guard let result = MainStructWithOptionalStruct(obj) else {
            return nil
        }
        return SomeStruct(rawValue: result)
    }

    public var withOptionalEnum: SomeEnum? {
        let obj = self.rawValue
        guard let result = MainStructWithOptionalEnum(obj) else {
            return nil
        }
        return SomeEnum(rawValue: result.rawValue)!
    }
}
