// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import Foundation

public final class PrivateKeySecp256k1 {

    public static func isValid(data: Data) -> Bool {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        return TWPrivateKeySecp256k1IsValid(dataData)
    }

    public var data: Data {
        return TWDataNSData(TWPrivateKeySecp256k1Data(rawValue))
    }

    let rawValue: OpaquePointer

    init(rawValue: OpaquePointer) {
        self.rawValue = rawValue
    }

    public init() {
        rawValue = TWPrivateKeySecp256k1Create()
    }

    public init?(data: Data) {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        guard let rawValue = TWPrivateKeySecp256k1CreateWithData(dataData) else {
            return nil
        }
        self.rawValue = rawValue
    }

    public init?(key: PrivateKeySecp256k1) {
        guard let rawValue = TWPrivateKeySecp256k1CreateCopy(key.rawValue) else {
            return nil
        }
        self.rawValue = rawValue
    }

    deinit {
        TWPrivateKeySecp256k1Delete(rawValue)
    }

    public func getPublicKey(compressed: Bool) -> PublicKeySecp256k1 {
        return PublicKeySecp256k1(rawValue: TWPrivateKeySecp256k1GetPublicKey(rawValue, compressed))
    }

    public func sign(digest: Data) -> Data? {
        let digestData = TWDataCreateWithNSData(digest)
        defer {
            TWDataDelete(digestData)
        }
        guard let result = TWPrivateKeySecp256k1Sign(rawValue, digestData) else {
            return nil
        }
        return TWDataNSData(result)
    }

    public func signAsDER(digest: Data) -> Data? {
        let digestData = TWDataCreateWithNSData(digest)
        defer {
            TWDataDelete(digestData)
        }
        guard let result = TWPrivateKeySecp256k1SignAsDER(rawValue, digestData) else {
            return nil
        }
        return TWDataNSData(result)
    }

}
