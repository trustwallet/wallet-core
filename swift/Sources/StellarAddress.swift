// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import Foundation

public final class StellarAddress {

    public static func == (lhs: StellarAddress, rhs: StellarAddress) -> Bool {
        return TWStellarAddressEqual(lhs.rawValue, rhs.rawValue)
    }

    public static func isValidString(string: String) -> Bool {
        let stringString = TWStringCreateWithNSString(string)
        defer {
            TWStringDelete(stringString)
        }
        return TWStellarAddressIsValidString(stringString)
    }

    public var description: String {
        return TWStringNSString(TWStellarAddressDescription(rawValue))
    }

    public var keyHash: Data {
        return TWDataNSData(TWStellarAddressKeyHash(rawValue))
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
        guard let rawValue = TWStellarAddressCreateWithString(stringString) else {
            return nil
        }
        self.rawValue = rawValue
    }

    public init(publicKey: PublicKey) {
        rawValue = TWStellarAddressCreateWithPublicKey(publicKey.rawValue)
    }

    deinit {
        TWStellarAddressDelete(rawValue)
    }

}
