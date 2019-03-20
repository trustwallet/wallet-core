// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import Foundation

public final class PrivateKey {

    public static func isValid(data: Data) -> Bool {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        return TWPrivateKeyIsValid(dataData)
    }

    public var data: Data {
        return TWDataNSData(TWPrivateKeyData(rawValue))
    }

    let rawValue: OpaquePointer

    init(rawValue: OpaquePointer) {
        self.rawValue = rawValue
    }

    public init() {
        rawValue = TWPrivateKeyCreate()
    }

    public init?(data: Data) {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        guard let rawValue = TWPrivateKeyCreateWithData(dataData) else {
            return nil
        }
        self.rawValue = rawValue
    }

    public init?(key: PrivateKey) {
        guard let rawValue = TWPrivateKeyCreateCopy(key.rawValue) else {
            return nil
        }
        self.rawValue = rawValue
    }

    deinit {
        TWPrivateKeyDelete(rawValue)
    }

    public func getPublicKeySecp256k1(compressed: Bool) -> PublicKey {
        return PublicKey(rawValue: TWPrivateKeyGetPublicKeySecp256k1(rawValue, compressed))
    }

    public func getPublicKeyNist256p1() -> PublicKey {
        return PublicKey(rawValue: TWPrivateKeyGetPublicKeyNist256p1(rawValue))
    }

    public func getPublicKeyEd25519() -> PublicKey {
        return PublicKey(rawValue: TWPrivateKeyGetPublicKeyEd25519(rawValue))
    }

    public func sign(digest: Data, curve: Curve) -> Data? {
        let digestData = TWDataCreateWithNSData(digest)
        defer {
            TWDataDelete(digestData)
        }
        guard let result = TWPrivateKeySign(rawValue, digestData, TWCurve(rawValue: curve.rawValue)) else {
            return nil
        }
        return TWDataNSData(result)
    }

    public func signAsDER(digest: Data, curve: Curve) -> Data? {
        let digestData = TWDataCreateWithNSData(digest)
        defer {
            TWDataDelete(digestData)
        }
        guard let result = TWPrivateKeySignAsDER(rawValue, digestData, TWCurve(rawValue: curve.rawValue)) else {
            return nil
        }
        return TWDataNSData(result)
    }

}
