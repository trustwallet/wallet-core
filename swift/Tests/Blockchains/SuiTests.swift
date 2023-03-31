// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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

    func testSign() {
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
