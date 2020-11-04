// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import WalletCore

class FilecoinTests: XCTestCase {

    func testAddress() {
        let privateKey = PrivateKey(data: Data(hexString: "1d969865e189957b9824bd34f26d5cbf357fda1a6d844cbf0c9ab1ed93fa7dbe")!)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: false)
        let address = AnyAddress(publicKey: publicKey, coin: .filecoin)
        XCTAssertEqual(address.description, "f1z4a36sc7mfbv4z3qwutblp2flycdui3baffytbq")
    }

    func testSigner() {
        let input = FilecoinSigningInput.with {
            $0.privateKey = Data(hexString: "1d969865e189957b9824bd34f26d5cbf357fda1a6d844cbf0c9ab1ed93fa7dbe")!
            $0.to = "f3um6uo3qt5of54xjbx3hsxbw5mbsc6auxzrvfxekn5bv3duewqyn2tg5rhrlx73qahzzpkhuj7a34iq7oifsq"
            $0.nonce = 2
            // 600 FIL
            $0.value = Data(hexString: "2086ac351052600000")!
            $0.gasLimit = 1000
            $0.gasFeeCap = Data(hexString: "25f273933db5700000")!
            $0.gasPremium = Data(hexString: "2b5e3af16b18800000")!
        }

        let output: FilecoinSigningOutput = AnySigner.sign(input: input, coin: .filecoin)
        let json = """
        {
            "Message": {
                "From": "f1z4a36sc7mfbv4z3qwutblp2flycdui3baffytbq",
                "GasFeeCap": "700000000000000000000",
                "GasLimit": 1000,
                "GasPremium": "800000000000000000000",
                "Nonce": 2,
                "To": "f3um6uo3qt5of54xjbx3hsxbw5mbsc6auxzrvfxekn5bv3duewqyn2tg5rhrlx73qahzzpkhuj7a34iq7oifsq",
                "Value": "600000000000000000000"
            },
            "Signature": {
                "Data": "jMRu+OZ/lfppgmqSfGsntFrRLWZnUg3ZYmJTTRLsVt4V1310vR3VKGJpaE6S4sNvDOE6sEgmN9YmfTkPVK2qMgE=",
                "Type": 1
            }
        }
        """
        XCTAssertJSONEqual(output.json, json)
    }
}
