// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import Foundation

public final class EthereumAddress {

    public static func == (lhs: EthereumAddress, rhs: EthereumAddress) -> Bool {
        return TWEthereumAddressEqual(lhs.rawValue, rhs.rawValue)
    }

    public static func isValidString(string: String) -> Bool {
        let stringString = TWStringCreateWithNSString(string)
        defer {
            TWStringDelete(stringString)
        }
        return TWEthereumAddressIsValidString(stringString)
    }

    public var description: String {
        return TWStringNSString(TWEthereumAddressDescription(rawValue))
    }

    public var keyHash: Data {
        return TWDataNSData(TWEthereumAddressKeyHash(rawValue))
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
        guard let rawValue = TWEthereumAddressCreateWithString(stringString) else {
            return nil
        }
        self.rawValue = rawValue
    }

    public init?(keyHash: Data) {
        let keyHashData = TWDataCreateWithNSData(keyHash)
        defer {
            TWDataDelete(keyHashData)
        }
        guard let rawValue = TWEthereumAddressCreateWithKeyHash(keyHashData) else {
            return nil
        }
        self.rawValue = rawValue
    }

    public init?(publicKey: PublicKey) {
        guard let rawValue = TWEthereumAddressCreateWithPublicKey(publicKey.rawValue) else {
            return nil
        }
        self.rawValue = rawValue
    }

    deinit {
        TWEthereumAddressDelete(rawValue)
    }

}
