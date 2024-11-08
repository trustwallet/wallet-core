// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class Bech32Tests: XCTestCase {
    func testEncode()  {
        let data = Data(hexString: "00443214c74254b635cf84653a56d7c675be77df")!
        let encoded = Bech32.encode(hrp: "abcdef", data: data)
        XCTAssertEqual(encoded, "abcdef1qpzry9x8gf2tvdw0s3jn54khce6mua7lmqqqxw")
    }

    func testDecode()  {
        let decoded = Bech32.decode(string: "abcdef1qpzry9x8gf2tvdw0s3jn54khce6mua7lmqqqxw")!
        XCTAssertEqual(decoded.hexString, "00443214c74254b635cf84653a56d7c675be77df")
    }

    func testDecodeWrongChecksumVariant()  {
        // This is a Bech32m variant, not Bech32 variant. So it should fail using Bech32 decoder.
        let decoded = Bech32.decode(string: "abcdef1l7aum6echk45nj3s0wdvt2fg8x9yrzpqzd3ryx")
        XCTAssertNil(decoded)
    }

    func testEncodeM()  {
        let data = Data(hexString: "ffbbcdeb38bdab49ca307b9ac5a928398a418820")!
        let encoded = Bech32.encodeM(hrp: "abcdef", data: data)
        XCTAssertEqual(encoded, "abcdef1l7aum6echk45nj3s0wdvt2fg8x9yrzpqzd3ryx")
    }

    func testDecodeM()  {
        let decoded = Bech32.decodeM(string: "abcdef1l7aum6echk45nj3s0wdvt2fg8x9yrzpqzd3ryx")!
        XCTAssertEqual(decoded.hexString, "ffbbcdeb38bdab49ca307b9ac5a928398a418820")
    }

    func testDecodeMWrongChecksumVariant()  {
        // This is a Bech32 variant, not Bech32m variant. So it should fail using Bech32M decoder.
        let decoded = Bech32.decodeM(string: "abcdef1qpzry9x8gf2tvdw0s3jn54khce6mua7lmqqqxw")
        XCTAssertNil(decoded)
    }
}
