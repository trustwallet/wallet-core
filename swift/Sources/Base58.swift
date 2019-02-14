// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import Foundation

public struct Base58 {

    public static func encode(data: Data) -> String {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        return TWStringNSString(TWBase58Encode(dataData))
    }

    public static func encodeNoCheck(data: Data) -> String {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        return TWStringNSString(TWBase58EncodeNoCheck(dataData))
    }

    public static func decode(string: String) -> Data? {
        let stringString = TWStringCreateWithNSString(string)
        defer {
            TWStringDelete(stringString)
        }
        guard let result = TWBase58Decode(stringString) else {
            return nil
        }
        return TWDataNSData(result)
    }

    public static func decodeNoCheck(string: String) -> Data? {
        let stringString = TWStringCreateWithNSString(string)
        defer {
            TWStringDelete(stringString)
        }
        guard let result = TWBase58DecodeNoCheck(stringString) else {
            return nil
        }
        return TWDataNSData(result)
    }

    var rawValue: TWBase58

    init(rawValue: TWBase58) {
        self.rawValue = rawValue
    }


}
