// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import Foundation

public struct BitcoinCashAddress {

    public static func == (lhs: BitcoinCashAddress, rhs: BitcoinCashAddress) -> Bool {
        return TWBitcoinCashAddressEqual(lhs.rawValue, rhs.rawValue)
    }

    public static func isValid(data: Data) -> Bool {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        return TWBitcoinCashAddressIsValid(dataData)
    }

    public static func isValidString(string: String) -> Bool {
        let stringString = TWStringCreateWithNSString(string)
        defer {
            TWStringDelete(stringString)
        }
        return TWBitcoinCashAddressIsValidString(stringString)
    }

    var rawValue: TWBitcoinCashAddress

    public var description: String {
        return TWStringNSString(TWBitcoinCashAddressDescription(rawValue))
    }

    public var data: Data {
        return TWDataNSData(TWBitcoinCashAddressData(rawValue))
    }

    public var legacyAddress: BitcoinAddress {
        return BitcoinAddress(rawValue: TWBitcoinCashAddressLegacyAddress(rawValue))
    }

    init(rawValue: TWBitcoinCashAddress) {
        self.rawValue = rawValue
    }

    public init?(string: String) {
        let stringString = TWStringCreateWithNSString(string)
        defer {
            TWStringDelete(stringString)
        }
        rawValue = TWBitcoinCashAddress()
        guard TWBitcoinCashAddressInitWithString(&rawValue, stringString) else {
            return nil
        }
    }

    public init?(data: Data) {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        rawValue = TWBitcoinCashAddress()
        guard TWBitcoinCashAddressInitWithData(&rawValue, dataData) else {
            return nil
        }
    }

    public init(publicKey: PublicKey) {
        rawValue = TWBitcoinCashAddress()
        TWBitcoinCashAddressInitWithPublicKey(&rawValue, publicKey.rawValue)
    }


}
