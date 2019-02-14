// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import Foundation

public struct AES {

    public static func cbcencrypt(key: Data, data: Data, iv: Data) -> Data? {
        let keyData = TWDataCreateWithNSData(key)
        defer {
            TWDataDelete(keyData)
        }
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        let ivData = TWDataCreateWithNSData(iv)
        defer {
            TWDataDelete(ivData)
        }
        guard let result = TWAESCBCEncrypt(keyData, dataData, ivData) else {
            return nil
        }
        return TWDataNSData(result)
    }

    public static func cbcdecrypt(key: Data, data: Data, iv: Data) -> Data? {
        let keyData = TWDataCreateWithNSData(key)
        defer {
            TWDataDelete(keyData)
        }
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        let ivData = TWDataCreateWithNSData(iv)
        defer {
            TWDataDelete(ivData)
        }
        guard let result = TWAESCBCDecrypt(keyData, dataData, ivData) else {
            return nil
        }
        return TWDataNSData(result)
    }

    public static func ctrencrypt(key: Data, data: Data, iv: Data) -> Data? {
        let keyData = TWDataCreateWithNSData(key)
        defer {
            TWDataDelete(keyData)
        }
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        let ivData = TWDataCreateWithNSData(iv)
        defer {
            TWDataDelete(ivData)
        }
        guard let result = TWAESCTREncrypt(keyData, dataData, ivData) else {
            return nil
        }
        return TWDataNSData(result)
    }

    public static func ctrdecrypt(key: Data, data: Data, iv: Data) -> Data? {
        let keyData = TWDataCreateWithNSData(key)
        defer {
            TWDataDelete(keyData)
        }
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        let ivData = TWDataCreateWithNSData(iv)
        defer {
            TWDataDelete(ivData)
        }
        guard let result = TWAESCTRDecrypt(keyData, dataData, ivData) else {
            return nil
        }
        return TWDataNSData(result)
    }

    var rawValue: TWAES

    init(rawValue: TWAES) {
        self.rawValue = rawValue
    }


}
