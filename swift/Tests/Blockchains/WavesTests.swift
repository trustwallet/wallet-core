// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore

class WavesTests: XCTestCase {

    func testSigner() throws {

        let privateKey = PrivateKey(data: Data(hexString: "68b7a9adb4a655b205f43dac413803785921e22cd7c4d05857b203a62621075f")!)!

        let transferMessage = WavesTransferMessage.with {
            $0.amount = 100_000_000
            $0.asset = "DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD81zq"
            $0.fee = 100_000
            $0.feeAsset = "DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD81zq"
            $0.to = "3PPCZQkvdMJpmx7Zrz1cnYsPe9Bt1XT2Ckx"
            $0.attachment = Data(hexString: "0x68656c6c6f")!
        }

        let input = WavesSigningInput.with {
            $0.timestamp = 1559146613
            $0.privateKey = privateKey.data
            $0.transferMessage = transferMessage
        }

        let output: WavesSigningOutput = AnySigner.sign(input: input, coin: .waves)
        guard let json = try JSONSerialization.jsonObject(with: output.json.data(using: .utf8)!, options: []) as? [String: Any] else {
            XCTFail("unable to decode json")
            return
        }
        XCTAssertNotNil(json["proofs"] as? [String])
        // swiftlint:disable line_length
        XCTAssertEqual(output.json, "{\"amount\":100000000,\"assetId\":\"DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD81zq\",\"attachment\":\"Cn8eVZg\",\"fee\":100000,\"feeAssetId\":\"DacnEpaUVFRCYk8Fcd1F3cqUZuT4XG7qW9mRyoZD81zq\",\"proofs\":[\"2sKtLEXdvK3Yy94S7ypdVyCkUc7BT7RgG6xzLd8qr1NUE24iXcnu1MYF4HKMXRsxnP3akgVS25V3Xet9auv1evte\"],\"recipient\":\"3PPCZQkvdMJpmx7Zrz1cnYsPe9Bt1XT2Ckx\",\"senderPublicKey\":\"9AXmkhYSNGev6iUfR1oJ9X1SjqKsqp518TriHiLLHuKA\",\"timestamp\":1559146613,\"type\":4,\"version\":2}")
        XCTAssertEqual(output.signature.hexString, "5d6a77b1fd9b53d9735cd2543ba94215664f2b07d6c7befb081221fcd49f5b6ad6b9ac108582e8d3e74943bdf35fd80d985edf4b4de1fb1c5c427e84d0879f8f")
        // swiftlint:enable line_length
    }
}
