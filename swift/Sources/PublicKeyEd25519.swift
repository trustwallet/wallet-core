// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import Foundation

public struct PublicKeyEd25519 {

    public static func isValid(data: Data) -> Bool {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        return TWPublicKeyEd25519IsValid(dataData)
    }

    var rawValue: TWPublicKeyEd25519

    public var data: Data {
        return TWDataNSData(TWPublicKeyEd25519Data(rawValue))
    }

    public var description: String {
        return TWStringNSString(TWPublicKeyEd25519Description(rawValue))
    }

    init(rawValue: TWPublicKeyEd25519) {
        self.rawValue = rawValue
    }

    public init?(data: Data) {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        rawValue = TWPublicKeyEd25519()
        guard TWPublicKeyEd25519InitWithData(&rawValue, dataData) else {
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
        return TWPublicKeyEd25519Verify(rawValue, signatureData, messageData)
    }

}
