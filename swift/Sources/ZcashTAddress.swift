// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import Foundation

public struct ZcashTAddress {

    public static func == (lhs: ZcashTAddress, rhs: ZcashTAddress) -> Bool {
        return TWZcashTAddressEqual(lhs.rawValue, rhs.rawValue)
    }

    public static func isValid(data: Data) -> Bool {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        return TWZcashTAddressIsValid(dataData)
    }

    public static func isValidString(string: String) -> Bool {
        let stringString = TWStringCreateWithNSString(string)
        defer {
            TWStringDelete(stringString)
        }
        return TWZcashTAddressIsValidString(stringString)
    }

    var rawValue: TWZcashTAddress

    public var description: String {
        return TWStringNSString(TWZcashTAddressDescription(rawValue))
    }

    public var data: Data {
        return TWDataNSData(TWZcashTAddressData(rawValue))
    }

    init(rawValue: TWZcashTAddress) {
        self.rawValue = rawValue
    }

    public init?(string: String) {
        let stringString = TWStringCreateWithNSString(string)
        defer {
            TWStringDelete(stringString)
        }
        rawValue = TWZcashTAddress()
        guard TWZcashTAddressInitWithString(&rawValue, stringString) else {
            return nil
        }
    }

    public init?(data: Data) {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        rawValue = TWZcashTAddress()
        guard TWZcashTAddressInitWithData(&rawValue, dataData) else {
            return nil
        }
    }

    public init?(publicKey: PublicKeySecp256k1, prefix: UInt8) {
        rawValue = TWZcashTAddress()
        guard TWZcashTAddressInitWithPublicKey(&rawValue, publicKey.rawValue, prefix) else {
            return nil
        }
    }


}
