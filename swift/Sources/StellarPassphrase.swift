// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

public enum StellarPassphrase: UInt32, CaseIterable, CustomStringConvertible  {
    case stellar = 0
    case kin = 1

    public var description: String {
        switch self {
        case .stellar: return "Public Global Stellar Network ; September 2015"
        case .kin: return "Kin Mainnet ; December 2018"
        }
    }
}
