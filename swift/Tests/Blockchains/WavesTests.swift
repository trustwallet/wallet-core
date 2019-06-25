// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class WavesTests: XCTestCase {

    func testSigner() throws {

        let privateKey = PrivateKey(data: Data(hexString: "68b7a9adb4a655b205f43dac413803785921e22cd7c4d05857b203a62621075f")!)!

        let input = WavesSigningInput.with {
            $0.amount = 100_000_000
            $0.asset = "DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD81zq"
            $0.fee = 100_000
            $0.feeAsset = "DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD81zq"
            $0.to = "3PPCZQkvdMJpmx7Zrz1cnYsPe9Bt1XT2Ckx"
            $0.attachment = Data(hexString: "0x68656c6c6f")!
            $0.timestamp = 1559146613
            $0.privateKey = privateKey.data
        }

        let output = WavesSigner.sign(input: input)
        guard let json = try JSONSerialization.jsonObject(with: output.json.data(using: .utf8)!, options: []) as? [String: Any] else {
            XCTFail("unable to decode json")
            return
        }
        XCTAssertNotNil(json["proofs"] as? [String])
        XCTAssertEqual(output.signature.hexString, "5d6a77b1fd9b53d9735cd2543ba94215664f2b07d6c7befb081221fcd49f5b6ad6b9ac108582e8d3e74943bdf35fd80d985edf4b4de1fb1c5c427e84d0879f8f");
    }
}
