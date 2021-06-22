// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class TopTests: XCTestCase {

    func testAddress() {
        let privateKey = PrivateKey(data: Data(hexString: "b0032f8057051b611a7c0ea373da4d7a6764351030ed497e6134fd9e11775b19")!)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: false)
        let address = AnyAddress(publicKey: publicKey, coin: .top)
        XCTAssertEqual(address.description, "T80000968927100f3cb7b23e8d477298311648978d8613")
    }

    func testSigner() {
        let input = TopSigningInput.with {
            $0.from = "T0000066ab344963eaa071f9636faac26b0d1a39900325"
            $0.to = "T0000066ab344963eaa071f9636faac26b0d1a39900325"
            $0.amount = 1
            $0.txDeposit = 2
            $0.lastTxNonce = 3
            $0.lastTxHash = "12345678"
            $0.token = "hello world"
            $0.note = "top unit test"
            $0.privateKey = Data(hexString: "2ff271ab38849388c49e24fbc52d357384c24ed929df4f0c7b81afca7c775b62")!
        }

        let output: TopSigningOutput = AnySigner.sign(input: input, coin: .top)

        XCTAssertEqual(output.signature.hexString, "0488227084554fae2b4b270cb56c9d8ee2b6bb1ee06863945ecce5bff9f8a197aa4546799dbe85decd6f3a43f52a2b06f77efd53af8e8530463d27c1b5945608bf216da6f9276fb23e718ef5c85bdcc8b63d5f88540b1b68978837e35334beb60d1de3efbe6948bb10636f9f014553b8b0ca5697a66b0c69c9dc0ce893289fce5100")
        XCTAssertEqual(output.encoded.hexString, "f8509466ab344963eaa071f9636faac26b0d1a399003259466ab344963eaa071f9636faac26b0d1a399003250102038831323334353637388b68656c6c6f20776f726c648d746f7020756e69742074657374")
    }

}
