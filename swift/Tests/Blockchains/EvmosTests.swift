// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import WalletCore

class EvmosTests: XCTestCase {
    func testAddressData() throws {
        let wallet = HDWallet(strength: 256, passphrase: "")!
        let nativeEvmos = wallet.getAddressForCoin(coin: .nativeEvmos)
        let evmos = wallet.getAddressForCoin(coin: .evmos)

        let addr1 = AnyAddress(string: nativeEvmos, coin: .nativeEvmos)
        let addr2 = AnyAddress(string: evmos, coin: .evmos)

        XCTAssertEqual(addr1?.data.hexString, addr2?.data.hexString)
    }
}
