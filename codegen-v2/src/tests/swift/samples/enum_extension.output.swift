// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
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
