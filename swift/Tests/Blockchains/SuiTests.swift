// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class SuiTests: XCTestCase {
    func testAddress() {
        let anyAddress = AnyAddress(string: "0x259ff8074ab425cbb489f236e18e08f03f1a7856bdf7c7a2877bd64f738b5015", coin: .sui)

        XCTAssertEqual(anyAddress?.description, "0x259ff8074ab425cbb489f236e18e08f03f1a7856bdf7c7a2877bd64f738b5015")
        XCTAssertEqual(anyAddress?.coin, .sui)

        let invalid = "MQqpqMQgCBuiPkoXfgZZsJvuzCeI1zc00z6vHJj4"
        XCTAssertNil(Data(hexString: invalid))
        XCTAssertNil(AnyAddress(string: invalid, coin: .sui))
        XCTAssertFalse(AnyAddress.isValid(string: invalid, coin: .sui))
    }

    func testSignDirect() {
        // Successfully broadcasted: https://suiscan.xyz/mainnet/tx/D4Ay9TdBJjXkGmrZSstZakpEWskEQHaWURP6xWPRXbAm
        let privateKeyData = Data(hexString: "7e6682f7bf479ef0f627823cffd4e1a940a7af33e5fb39d9e0f631d2ecc5daff")!
        let txBytes = """
AAAEAAjoAwAAAAAAAAAIUMMAAAAAAAAAIKcXWr3V7ZLr4605DbNmxqcGR4zfUXzebPmGMAZc2jd6ACBU6A1215DCd/WkTzzpL1PSb1iUiSvzld7mN1mIh2vmsgMCAAIBAAABAQABAQMAAAAAAQIAAQEDAAABAAEDAFToDXbXkMJ39aRPPOkvU9JvWJSJK/OV3uY3WYiHa+ayAWNgILOn3HsRw6pvQZsX+KnBLn95ox0b3S3mcLTt1jAFeHEaBQAAAAAgGGuNnxrqusosgjP3gQ3jBjnhapGNBlcU0yTaupXpa0BU6A1215DCd/WkTzzpL1PSb1iUiSvzld7mN1mIh2vmsu4CAAAAAAAAwMYtAAAAAAAA
"""

        let input = SuiSigningInput.with {
            $0.paySui = SuiPaySui.with {
                $0.inputCoins = [SuiObjectRef.with {
                    $0.objectID = "0x636020b3a7dc7b11c3aa6f419b17f8a9c12e7f79a31d1bdd2de670b4edd63005"
                    $0.version = 85619064
                    $0.objectDigest = "2eKuWbZSVfpFVfg8FXY9wP6W5AFXnTchSoUdp7obyYZ5"
                }]
                $0.recipients = [
                    "0xa7175abdd5ed92ebe3ad390db366c6a706478cdf517cde6cf98630065cda377a",
                    "0x54e80d76d790c277f5a44f3ce92f53d26f5894892bf395dee6375988876be6b2"
                ]
                $0.amounts = [1000, 50000]
            }
            $0.privateKey = privateKeyData
            // 0.003 SUI
            $0.gasBudget = 3000000
            $0.referenceGasPrice = 750
        }
        let output: SuiSigningOutput = AnySigner.sign(input: input, coin: .sui)
        XCTAssertEqual(output.unsignedTx, txBytes)
        let expectedSignature = "AEh44B7iGArEHF1wOLAQJMLNgGnaIwn3gKPC92vtDJqITDETAM5z9plaxio1xomt6/cZReQ5FZaQsMC6l7E0BwmF69FEH+T5VPvl3GB3vwCOEZpeJpKXxvcIPQAdKsh2/g=="
        XCTAssertEqual(output.signature, expectedSignature)
    }
    
    func testTransferSui() {
        // Successfully broadcasted https://explorer.sui.io/txblock/HkPo6rYPyDY53x1MBszvSZVZyixVN7CHvCJGX381czAh?network=devnet
        let privateKeyData = Data(hexString: "3823dce5288ab55dd1c00d97e91933c613417fdb282a0b8b01a7f5f5a533b266")!
        let txBytes = """
AAACAAgQJwAAAAAAAAAgJZ/4B0q0Jcu0ifI24Y4I8D8aeFa998eih3vWT3OLUBUCAgABAQAAAQEDAAAAAAEBANV1rX8Y6UhGKlz2mPVk7zlKdSpx/sYkk6+KBVwBLA1QAQbywsjB2JZN8QGdZhbpcFcZvrq9kx2idVy5SM635olk7AIAAAAAAAAgYEVuxmf1zRBGdoDr+VDtMpIFF12s2Ua7I2ru1XyGF8/Vda1/GOlIRipc9pj1ZO85SnUqcf7GJJOvigVcASwNUAEAAAAAAAAA0AcAAAAAAAAA
"""
        let input = SuiSigningInput.with {
            $0.signDirectMessage = SuiSignDirect.with {
                $0.unsignedTxMsg = txBytes
            }
            $0.privateKey = privateKeyData
        }
        let output: SuiSigningOutput = AnySigner.sign(input: input, coin: .sui)
        XCTAssertEqual(output.unsignedTx, txBytes)
        let expectedSignature = "APxPduNVvHj2CcRcHOtiP2aBR9qP3vO2Cb0g12PI64QofDB6ks33oqe/i/iCTLcop2rBrkczwrayZuJOdi7gvwNqfN7sFqdcD/Z4e8I1YQlGkDMCK7EOgmydRDqfH8C9jg=="
        XCTAssertEqual(output.signature, expectedSignature)
    }
}
