// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.
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
