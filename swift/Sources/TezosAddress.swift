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

    public static func description(address: TezosAddress) -> String {
        return TWStringNSString(TWTezosAddressDescription(address.rawValue))
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

    public static func data(address: TezosAddress) -> Data {
        return TWDataNSData(TWTezosAddressData(address.rawValue))
    }

    var rawValue: TWTezosAddress

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

    public init?(publicKey: PublicKey) {
        rawValue = TWTezosAddress()
        guard TWTezosAddressInitWithPublicKey(&rawValue, publicKey.rawValue) else {
            return nil
        }
    }


}
