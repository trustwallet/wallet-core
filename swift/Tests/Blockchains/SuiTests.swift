// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class SuiTests: XCTestCase {
    func testAddress() {
        let anyAddress = AnyAddress(string: "0x061ce2b2100a71bb7aa0da98998887ad82597948", coin: .sui)

        XCTAssertEqual(anyAddress?.description, "0x061ce2b2100a71bb7aa0da98998887ad82597948")
        XCTAssertEqual(anyAddress?.coin, .sui)

        let invalid = "MQqpqMQgCBuiPkoXfgZZsJvuzCeI1zc00z6vHJj4"
        XCTAssertNil(Data(hexString: invalid))
        XCTAssertNil(AnyAddress(string: invalid, coin: .sui))
        XCTAssertFalse(AnyAddress.isValid(string: invalid, coin: .sui))
    }

    func testSign() {
        // Successfully broadcasted https://explorer.sui.io/transaction/rxLgxcAqgMg8gphp6eCsSGQcdZnwFYx2SRdwEhnAUC4
        let privateKeyData = Data(hexString: "3823dce5288ab55dd1c00d97e91933c613417fdb282a0b8b01a7f5f5a533b266")!
        let txBytes = """
AAUCLiNiMy/EzosKCk5EZr5QQZmMVLnvAAAAAAAAACDqj/OT+1+qyLZKV4YLw8kpK3/bTZKspTUmh1pBuUfHPLb0crwkV1LQcBARaxER8XhTNJmK7wAAAAAAAAAgaQEguOdXa+m16IM536nsveakQ4u/GYJAc1fpYGGKEvgBQUP35yxF+cEL5qm153kw18dVeuYB6AMAAAAAAAAttQCskZzd41GsNuNxHYMsbbl2aS4jYjMvxM6LCgpORGa+UEGZjFS57wAAAAAAAAAg6o/zk/tfqsi2SleGC8PJKSt/202SrKU1JodaQblHxzwBAAAAAAAAAOgDAAAAAAAA
"""
        let input = SuiSigningInput.with {
            $0.signDirectMessage = SuiSignDirect.with {
                $0.unsignedTxMsg = txBytes
            }
            $0.privateKey = privateKeyData
        }
        let output: SuiSigningOutput = AnySigner.sign(input: input, coin: .sui)
        XCTAssertEqual(output.unsignedTx, txBytes)
        let expectedSignature = "AIYRmHDpQesfAx3iWBCMwInf3MZ56ZQGnPWNtECFjcSq0ssAgjRW6GLnFCX24tfDNjSm9gjYgoLmn1No15iFJAtqfN7sFqdcD/Z4e8I1YQlGkDMCK7EOgmydRDqfH8C9jg=="
        XCTAssertEqual(output.signature, expectedSignature)
    }
}
