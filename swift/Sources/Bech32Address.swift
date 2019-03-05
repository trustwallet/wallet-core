// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import Foundation

public final class Bech32Address {

    public static func == (lhs: Bech32Address, rhs: Bech32Address) -> Bool {
        return TWBech32AddressEqual(lhs.rawValue, rhs.rawValue)
    }

    public static func isValidString(string: String) -> Bool {
        let stringString = TWStringCreateWithNSString(string)
        defer {
            TWStringDelete(stringString)
        }
        return TWBech32AddressIsValidString(stringString)
    }

    public var description: String {
        return TWStringNSString(TWBech32AddressDescription(rawValue))
    }

    public var hrp: HRP {
        return HRP(rawValue: TWBech32AddressHRP(rawValue).rawValue)!
    }

    public var witnessProgram: Data {
        return TWDataNSData(TWBech32AddressWitnessProgram(rawValue))
    }

    let rawValue: OpaquePointer

    init(rawValue: OpaquePointer) {
        self.rawValue = rawValue
    }

    public init?(string: String) {
        let stringString = TWStringCreateWithNSString(string)
        defer {
            TWStringDelete(stringString)
        }
        guard let rawValue = TWBech32AddressCreateWithString(stringString) else {
            return nil
        }
        self.rawValue = rawValue
    }

    public init?(hrp: HRP, data: Data) {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        guard let rawValue = TWBech32AddressCreateWithData(TWHRP(rawValue: hrp.rawValue), dataData) else {
            return nil
        }
        self.rawValue = rawValue
    }

    public init(hrp: HRP, publicKey: PublicKey) {
        rawValue = TWBech32AddressCreateWithPublicKey(TWHRP(rawValue: hrp.rawValue), publicKey.rawValue)
    }

    deinit {
        TWBech32AddressDelete(rawValue)
    }

}
