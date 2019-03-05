// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import Foundation

public final class IconAddress {

    public static func == (lhs: IconAddress, rhs: IconAddress) -> Bool {
        return TWIconAddressEqual(lhs.rawValue, rhs.rawValue)
    }

    public static func isValidString(string: String) -> Bool {
        let stringString = TWStringCreateWithNSString(string)
        defer {
            TWStringDelete(stringString)
        }
        return TWIconAddressIsValidString(stringString)
    }

    public var description: String {
        return TWStringNSString(TWIconAddressDescription(rawValue))
    }

    public var keyHash: Data {
        return TWDataNSData(TWIconAddressKeyHash(rawValue))
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
        guard let rawValue = TWIconAddressCreateWithString(stringString) else {
            return nil
        }
        self.rawValue = rawValue
    }

    public init?(keyHash: Data, type: IconAddressType) {
        let keyHashData = TWDataCreateWithNSData(keyHash)
        defer {
            TWDataDelete(keyHashData)
        }
        guard let rawValue = TWIconAddressCreateWithKeyHash(keyHashData, TWIconAddressType(rawValue: type.rawValue)) else {
            return nil
        }
        self.rawValue = rawValue
    }

    public init(publicKey: PublicKey, type: IconAddressType) {
        rawValue = TWIconAddressCreateWithPublicKey(publicKey.rawValue, TWIconAddressType(rawValue: type.rawValue))
    }

    deinit {
        TWIconAddressDelete(rawValue)
    }

}
