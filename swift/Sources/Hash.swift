// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import Foundation

public struct Hash {

    public static func sha1(data: Data) -> Data {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        return TWDataNSData(TWHashSHA1(dataData))
    }

    public static func sha256(data: Data) -> Data {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        return TWDataNSData(TWHashSHA256(dataData))
    }

    public static func sha512(data: Data) -> Data {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        return TWDataNSData(TWHashSHA512(dataData))
    }

    public static func keccak256(data: Data) -> Data {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        return TWDataNSData(TWHashKeccak256(dataData))
    }

    public static func keccak512(data: Data) -> Data {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        return TWDataNSData(TWHashKeccak512(dataData))
    }

    public static func sha3_256(data: Data) -> Data {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        return TWDataNSData(TWHashSHA3_256(dataData))
    }

    public static func sha3_512(data: Data) -> Data {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        return TWDataNSData(TWHashSHA3_512(dataData))
    }

    public static func ripemd(data: Data) -> Data {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        return TWDataNSData(TWHashRIPEMD(dataData))
    }

    public static func blake2b(data: Data, size: Int) -> Data {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        return TWDataNSData(TWHashBlake2b(dataData, size))
    }

    public static func sha256RIPEMD(data: Data) -> Data {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        return TWDataNSData(TWHashSHA256RIPEMD(dataData))
    }

    public static func sha256SHA256(data: Data) -> Data {
        let dataData = TWDataCreateWithNSData(data)
        defer {
            TWDataDelete(dataData)
        }
        return TWDataNSData(TWHashSHA256SHA256(dataData))
    }

    var rawValue: TWHash

    init(rawValue: TWHash) {
        self.rawValue = rawValue
    }


}
