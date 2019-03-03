// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import Foundation

public struct TezosAddress {

    public static func == (lhs: TezosAddress, rhs: TezosAddress) -> Bool {
        return TWTezosAddressEqual(lhs.rawValue, rhs.rawValue)
    }

    public static func isValid(data: Data) -> Bool {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        return TWTezosAddressIsValid(dataData)
    }

    public static func isValidString(string: String) -> Bool {
        let stringString = TWStringCreateWithNSString(string)
        defer {
            TWStringDelete(stringString)
        }
        return TWTezosAddressIsValidString(stringString)
    }

    public static func forge() -> String? {
        return TWTezosAddressForge()
    }

    var rawValue: TWTezosAddress

    public var description: String {
        return TWStringNSString(TWTezosAddressDescription(rawValue))
    }

    public var data: Data {
        return TWDataNSData(TWTezosAddressData(rawValue))
    }

    init(rawValue: TWTezosAddress) {
        self.rawValue = rawValue
    }

    public init?(string: String) {
        let stringString = TWStringCreateWithNSString(string)
        defer {
            TWStringDelete(stringString)
        }
        rawValue = TWTezosAddress()
        guard TWTezosAddressInitWithString(&rawValue, stringString) else {
            return nil
        }
    }

    public init?(data: Data) {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        rawValue = TWTezosAddress()
        guard TWTezosAddressInitWithData(&rawValue, dataData) else {
            return nil
        }
    }

    public init?(publicKey: PublicKey, prefix: UInt8) {
        rawValue = TWTezosAddress()
        guard TWTezosAddressInitWithPublicKey(&rawValue, publicKey.rawValue, prefix) else {
            return nil
        }
    }


}
