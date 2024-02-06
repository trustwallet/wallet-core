// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

extension MainEnum {
    public static func firstFunction(first_param: Int32) -> Bool {
        let result = MainEnumFirstFunction(first_param)
        return result
    }

    public static func secondFunction(first_param: SomeStruct) -> Bool {
        let first_param = first_param.rawValue
        let result = MainEnumSecondFunction(first_param)
        return result
    }
}
