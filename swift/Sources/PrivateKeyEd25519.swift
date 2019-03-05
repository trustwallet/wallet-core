// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import Foundation

public final class PrivateKeyEd25519 {

    public static func isValid(data: Data) -> Bool {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        return TWPrivateKeyEd25519IsValid(dataData)
    }

    public var data: Data {
        return TWDataNSData(TWPrivateKeyEd25519Data(rawValue))
    }

    let rawValue: OpaquePointer

    init(rawValue: OpaquePointer) {
        self.rawValue = rawValue
    }

    public init() {
        rawValue = TWPrivateKeyEd25519Create()
    }

    public init?(data: Data) {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        guard let rawValue = TWPrivateKeyEd25519CreateWithData(dataData) else {
            return nil
        }
        self.rawValue = rawValue
    }

    public init?(key: PrivateKeyEd25519) {
        guard let rawValue = TWPrivateKeyEd25519CreateCopy(key.rawValue) else {
            return nil
        }
        self.rawValue = rawValue
    }

    deinit {
        TWPrivateKeyEd25519Delete(rawValue)
    }

    public func getPublicKey() -> PublicKeyEd25519 {
        return PublicKeyEd25519(rawValue: TWPrivateKeyEd25519GetPublicKey(rawValue))
    }

    public func sign(digest: Data) -> Data? {
        let digestData = TWDataCreateWithNSData(digest)
        defer {
            TWDataDelete(digestData)
        }
        guard let result = TWPrivateKeyEd25519Sign(rawValue, digestData) else {
            return nil
        }
        return TWDataNSData(result)
    }

}
