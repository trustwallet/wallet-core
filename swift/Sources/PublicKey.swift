// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import Foundation

public struct PublicKey {

    public static func isValid(data: Data) -> Bool {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        return TWPublicKeyIsValid(dataData)
    }

    public static func recover(signature: Data, message: Data) -> PublicKey {
        let signatureData = TWDataCreateWithNSData(signature)
        defer {
            TWDataDelete(signatureData)
        }
        let messageData = TWDataCreateWithNSData(message)
        defer {
            TWDataDelete(messageData)
        }
        return PublicKey(rawValue: TWPublicKeyRecover(signatureData, messageData))
    }

    var rawValue: TWPublicKey

    public var isCompressed: Bool {
        return TWPublicKeyIsCompressed(rawValue)
    }

    public var compressed: PublicKey {
        return PublicKey(rawValue: TWPublicKeyCompressed(rawValue))
    }

    public var data: Data {
        return TWDataNSData(TWPublicKeyData(rawValue))
    }

    public var description: String {
        return TWStringNSString(TWPublicKeyDescription(rawValue))
    }

    init(rawValue: TWPublicKey) {
        self.rawValue = rawValue
    }

    public init?(data: Data) {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        rawValue = TWPublicKey()
        guard TWPublicKeyInitWithData(&rawValue, dataData) else {
            return nil
        }
    }


    public func verify(signature: Data, message: Data) -> Bool {
        let signatureData = TWDataCreateWithNSData(signature)
        defer {
            TWDataDelete(signatureData)
        }
        let messageData = TWDataCreateWithNSData(message)
        defer {
            TWDataDelete(messageData)
        }
        return TWPublicKeyVerify(rawValue, signatureData, messageData)
    }

}
