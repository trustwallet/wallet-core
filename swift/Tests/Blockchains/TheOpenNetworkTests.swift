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
            $0.bounceable = true
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

    func testJettonTransferSign() {
        let privateKeyData = Data(hexString: "c054900a527538c1b4325688a421c0469b171c29f23a62da216e90b0df2412ee")!

        let transferData = TheOpenNetworkTransfer.with {
            $0.walletVersion = TheOpenNetworkWalletVersion.walletV4R2
            $0.dest = "EQBiaD8PO1NwfbxSkwbcNT9rXDjqhiIvXWymNO-edV0H5lja"
            $0.amount = 100 * 1000 * 1000
            $0.sequenceNumber = 1
            $0.mode = UInt32(TheOpenNetworkSendMode.payFeesSeparately.rawValue | TheOpenNetworkSendMode.ignoreActionPhaseErrors.rawValue)
            $0.expireAt = 1787693046
            $0.comment = "test comment"
            $0.bounceable = true
        }
        
        let jettonTransfer = TheOpenNetworkJettonTransfer.with {
            $0.transfer = transferData
            $0.jettonAmount = 500 * 1000 * 1000
            $0.toOwner = "EQAFwMs5ha8OgZ9M4hQr80z9NkE7rGxUpE1hCFndiY6JnDx8"
            $0.responseAddress = "EQBaKIMq5Am2p_rfR1IFTwsNWHxBkOpLTmwUain5Fj4llTXk"
            $0.forwardAmount = 1
        }

        let input = TheOpenNetworkSigningInput.with {
            $0.jettonTransfer = jettonTransfer
            $0.privateKey = privateKeyData
        }

        let output: TheOpenNetworkSigningOutput = AnySigner.sign(input: input, coin: .ton)

        // tx: https://testnet.tonscan.org/tx/Er_oT5R3QK7D-qVPBKUGkJAOOq6ayVls-mgEphpI9Ck=
        let expectedString = "te6ccgICAAQAAQAAARgAAAFFiAC0UQZVyBNtT/W+jqQKnhYasPiDIdSWnNgo1FPyLHxLKgwAAQGcaIWVosi1XnveAmoG9y0/mPeNUqUu7GY76mdbRAaVeNeDOPDlh5M3BEb26kkc6XoYDekV60o2iOobN+TGS76jBSmpoxdqjgf2AAAAAQADAAIBaGIAMTQfh52puD7eKUmDbhqfta4cdUMRF662Uxp3zzqug/MgL68IAAAAAAAAAAAAAAAAAAEAAwDKD4p+pQAAAAAAAAAAQdzWUAgAC4GWcwteHQM+mcQoV+aZ+myCd1jYqUiawhCzuxMdEzkAFoogyrkCban+t9HUgVPCw1YfEGQ6ktObBRqKfkWPiWVCAgAAAAB0ZXN0IGNvbW1lbnQ="

        XCTAssertEqual(output.encoded, expectedString)
    }
}
