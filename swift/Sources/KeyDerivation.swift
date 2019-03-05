// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import Foundation

public struct KeyDerivation {

    public static func scrypt(password: String, salt: Data, n: UInt64, r: UInt32, p: UInt32, keyLength: Int) -> Data? {
        let passwordString = TWStringCreateWithNSString(password)
        defer {
            TWStringDelete(passwordString)
        }
        let saltData = TWDataCreateWithNSData(salt)
        defer {
            TWDataDelete(saltData)
        }
        guard let result = TWKeyDerivationScrypt(passwordString, saltData, n, r, p, keyLength) else {
            return nil
        }
        return TWDataNSData(result)
    }

    public static func pbkdf2_256(password: String, salt: Data, iterations: UInt32, keyLength: Int) -> Data {
        let passwordString = TWStringCreateWithNSString(password)
        defer {
            TWStringDelete(passwordString)
        }
        let saltData = TWDataCreateWithNSData(salt)
        defer {
            TWDataDelete(saltData)
        }
        return TWDataNSData(TWKeyDerivationPBKDF2_256(passwordString, saltData, iterations, keyLength))
    }

    public static func pbkdf2_512(password: String, salt: Data, iterations: UInt32, keyLength: Int) -> Data {
        let passwordString = TWStringCreateWithNSString(password)
        defer {
            TWStringDelete(passwordString)
        }
        let saltData = TWDataCreateWithNSData(salt)
        defer {
            TWDataDelete(saltData)
        }
        return TWDataNSData(TWKeyDerivationPBKDF2_512(passwordString, saltData, iterations, keyLength))
    }

    var rawValue: TWKeyDerivation

    init(rawValue: TWKeyDerivation) {
        self.rawValue = rawValue
    }


}
