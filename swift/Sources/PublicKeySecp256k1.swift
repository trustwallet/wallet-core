// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import Foundation

public struct PublicKeySecp256k1 {

    public static func isValid(data: Data) -> Bool {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        return TWPublicKeySecp256k1IsValid(dataData)
    }

    public static func recover(signature: Data, message: Data) -> PublicKeySecp256k1 {
        let signatureData = TWDataCreateWithNSData(signature)
        defer {
            TWDataDelete(signatureData)
        }
        let messageData = TWDataCreateWithNSData(message)
        defer {
            TWDataDelete(messageData)
        }
        return PublicKeySecp256k1(rawValue: TWPublicKeySecp256k1Recover(signatureData, messageData))
    }

    var rawValue: TWPublicKeySecp256k1

    public var isCompressed: Bool {
        return TWPublicKeySecp256k1IsCompressed(rawValue)
    }

    public var compressed: PublicKeySecp256k1 {
        return PublicKeySecp256k1(rawValue: TWPublicKeySecp256k1Compressed(rawValue))
    }

    public var data: Data {
        return TWDataNSData(TWPublicKeySecp256k1Data(rawValue))
    }

    public var description: String {
        return TWStringNSString(TWPublicKeySecp256k1Description(rawValue))
    }

    init(rawValue: TWPublicKeySecp256k1) {
        self.rawValue = rawValue
    }

    public init?(data: Data) {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        rawValue = TWPublicKeySecp256k1()
        guard TWPublicKeySecp256k1InitWithData(&rawValue, dataData) else {
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
        return TWPublicKeySecp256k1Verify(rawValue, signatureData, messageData)
    }

}
