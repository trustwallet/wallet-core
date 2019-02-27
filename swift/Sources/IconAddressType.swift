// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

public enum IconAddressType: UInt32, CustomStringConvertible  {
    case address = 0
    case contract = 1

    public var description: String {
        switch self {
        case .address: return "hx"
        case .contract: return "cx"
        }
    }
}
