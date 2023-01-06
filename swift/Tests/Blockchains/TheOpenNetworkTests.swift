// Copyright © 2017-2022 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class TheOpenNetworkTests: XCTestCase {
    func testAddressFromPrivateKey() {
        let data = Data(hexString: "63474e5fe9511f1526a50567ce142befc343e71a49b865ac3908f58667319cb8")
        let privateKey = PrivateKey(data: data!)!
        let publicKey = privateKey.getPublicKeyEd25519()
        let address = AnyAddress(publicKey: publicKey, coin: .ton)
        XCTAssertEqual(address.description, "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q")
    }

    func testAddressFromPublicKey() {
        let data = Data(hexString: "f42c77f931bea20ec5d0150731276bbb2e2860947661245b2319ef8133ee8d41")
        let publicKey = PublicKey(data: data!, type: PublicKeyType.ed25519)!
        let address = AnyAddress(publicKey: publicKey, coin: .ton)
        XCTAssertEqual(address.description, "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q")
    }

    func testAddressFromRawString() {
        let addressString = "0:66fbe3c5c03bf5c82792f904c9f8bf28894a6aa3d213d41c20569b654aadedb3"
        let address = AnyAddress(string: addressString, coin: .ton)
        XCTAssertEqual(address!.description, "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q")
    }

    func testAddressFromUserFriendlyString() {
        let addressString = "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q"
        let address = AnyAddress(string: addressString, coin: .ton)
        XCTAssertEqual(address!.description, "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q")
    }

    func testSign() {
        let privateKeyData = Data(hexString: "c38f49de2fb13223a9e7d37d5d0ffbdd89a5eb7c8b0ee4d1c299f2cefe7dc4a0")!

        let transfer = TheOpenNetworkTransfer.with {
            $0.walletVersion = TheOpenNetworkWalletVersion.walletV4R2
            $0.dest = "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q"
            $0.amount = 10
            $0.sequenceNumber = 6
            $0.mode = UInt32(TheOpenNetworkSendMode.payFeesSeparately.rawValue | TheOpenNetworkSendMode.ignoreActionPhaseErrors.rawValue)
            $0.expireAt = 1671132440
        }

        let input = TheOpenNetworkSigningInput.with {
            $0.transfer = transfer
            $0.privateKey = privateKeyData
        }

        let output: TheOpenNetworkSigningOutput = AnySigner.sign(input: input, coin: .ton)

        // tx: https://tonscan.org/tx/3Z4tHpXNLyprecgu5aTQHWtY7dpHXEoo11MAX61Xyg0=
        let expectedString = "te6ccgICAAQAAQAAALAAAAFFiAGwt/q8k4SrjbFbQCjJZfQr64ExRxcUMsWqaQODqTUijgwAAQGcEUPkil2aZ4s8KKparSep/OKHMC8vuXafFbW2HGp/9AcTRv0J5T4dwyW1G0JpHw+g5Ov6QI3Xo0O9RFr3KidICimpoxdjm3UYAAAABgADAAIBYmIAM33x4uAd+uQTyXyCZPxflESlNVHpCeoOECtNsqVW9tmIUAAAAAAAAAAAAAAAAAEAAwAA"

        XCTAssertEqual(output.encoded, expectedString)
    }
}
