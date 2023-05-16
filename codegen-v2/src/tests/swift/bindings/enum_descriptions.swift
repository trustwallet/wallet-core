// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

public enum MainEnum: UInt32, CaseIterable, CustomStringConvertible {
    case `one` = 0
    case `two` = 1
    case `three` = 2

    public var description: String {
        switch self {
            case .one: return "one_string"
            case .two: return ""
            case .three: return "three_string"
        }
    }
}
