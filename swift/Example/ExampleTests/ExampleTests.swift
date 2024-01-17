// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import XCTest
import WalletCore
@testable import Example

class ExampleTests: XCTestCase {

    func testEthereum() {
        let wallet = HDWallet(mnemonic: "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal", passphrase: "TREZOR")!
        let address = wallet.getAddressForCoin(coin: .ethereum)

        XCTAssertEqual(address, "0x27Ef5cDBe01777D62438AfFeb695e33fC2335979")
    }
}
