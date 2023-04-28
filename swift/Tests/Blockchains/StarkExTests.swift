// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import WalletCore

class StarkExTests: XCTestCase {
    func testMessageAndVerifySigner() {
        let privateKey = PrivateKey(data: Data(hexString: "04be51a04e718c202e4dca60c2b72958252024cfc1070c090dd0f170298249de")!)!
        let msg = "463a2240432264a3aa71a5713f2a4e4c1b9e12bbb56083cd56af6d878217cf"
        let signature = StarkExMessageSigner.signMessage(privateKey: privateKey, message: msg)
        XCTAssertEqual(signature, "04cf5f21333dd189ada3c0f2a51430d733501a9b1d5e07905273c1938cfb261e05b6013d74adde403e8953743a338c8d414bb96bf69d2ca1a91a85ed2700a528")
        let pubKey = privateKey.getPublicKeyByType(pubkeyType: .starkex)
        XCTAssertTrue(StarkExMessageSigner.verifyMessage(pubKey: pubKey, message: msg, signature: signature))
    }
}
