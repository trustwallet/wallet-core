// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import Foundation

public final class OntologyAddress {

    public static func == (lhs: OntologyAddress, rhs: OntologyAddress) -> Bool {
        return TWOntologyAddressEqual(lhs.rawValue, rhs.rawValue)
    }

    public static func isValidString(string: String) -> Bool {
        let stringString = TWStringCreateWithNSString(string)
        defer {
            TWStringDelete(stringString)
        }
        return TWOntologyAddressIsValidString(stringString)
    }

    public var description: String {
        return TWStringNSString(TWOntologyAddressDescription(rawValue))
    }

    public var keyHash: Data {
        return TWDataNSData(TWOntologyAddressKeyHash(rawValue))
    }

    let rawValue: OpaquePointer

    init(rawValue: OpaquePointer) {
        self.rawValue = rawValue
    }

    public init?(data: Data) {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        guard let rawValue = TWOntologyAddressCreateWithData(dataData) else {
            return nil
        }
        self.rawValue = rawValue
    }

    public init?(string: String) {
        let stringString = TWStringCreateWithNSString(string)
        defer {
            TWStringDelete(stringString)
        }
        guard let rawValue = TWOntologyAddressCreateWithString(stringString) else {
            return nil
        }
        self.rawValue = rawValue
    }

    public init(publicKey: PublicKey) {
        rawValue = TWOntologyAddressCreateWithPublicKey(publicKey.rawValue)
    }

    deinit {
        TWOntologyAddressDelete(rawValue)
    }

}
