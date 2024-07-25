// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class TheOpenNetworkTests: XCTestCase {
    func testAddressFromPrivateKey() {
        let data = Data(hexString: "63474e5fe9511f1526a50567ce142befc343e71a49b865ac3908f58667319cb8")
        let privateKey = PrivateKey(data: data!)!
        let publicKey = privateKey.getPublicKeyEd25519()
        let address = AnyAddress(publicKey: publicKey, coin: .ton)
        XCTAssertEqual(address.description, "UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV")
    }

    func testAddressFromPublicKey() {
        let data = Data(hexString: "f42c77f931bea20ec5d0150731276bbb2e2860947661245b2319ef8133ee8d41")
        let publicKey = PublicKey(data: data!, type: PublicKeyType.ed25519)!
        let address = AnyAddress(publicKey: publicKey, coin: .ton)
        XCTAssertEqual(address.description, "UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV")
    }

    func testAddressFromRawString() {
        let addressString = "0:66fbe3c5c03bf5c82792f904c9f8bf28894a6aa3d213d41c20569b654aadedb3"
        let address = AnyAddress(string: addressString, coin: .ton)
        XCTAssertEqual(address!.description, "UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV")
    }

    func testAddressFromBounceableString() {
        let addressString = "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q"
        let address = AnyAddress(string: addressString, coin: .ton)
        XCTAssertEqual(address!.description, "UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV")
    }

    func testAddressFromUserFriendlyString() {
        let addressString = "UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV"
        let address = AnyAddress(string: addressString, coin: .ton)
        XCTAssertEqual(address!.description, "UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV")
    }

    func testAddressToBounceable() {
        let addressString = "UQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts4DV"
        let address = TONAddressConverter.toUserFriendly(address: addressString, bounceable: true, testnet: false)
        XCTAssertEqual(address, "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q")
    }

    func testGenerateJettonAddress() {
        let mainAddress = "UQBjKqthWBE6GEcqb_epTRFrQ1niS6Z1Z1MHMwR-mnAYRoYr"
        let mainAddressBoc = TONAddressConverter.toBoc(address: mainAddress)
        XCTAssertEqual(mainAddressBoc, "te6cckEBAQEAJAAAQ4AMZVVsKwInQwjlTf71KaItaGs8SXTOrOpg5mCP004DCNAptHQU")

        // curl --location 'https://toncenter.com/api/v2/runGetMethod' --header 'Content-Type: application/json' --data \
        // '{"address":"EQAvlWFDxGF2lXm67y4yzC17wYKD9A0guwPkMs1gOsM__NOT","method":"get_wallet_address","method":"get_wallet_address","stack":[["tvm.Slice","te6ccgICAAEAAQAAACQAAABDgAxlVWwrAidDCOVN/vUpoi1oazxJdM6s6mDmYI/TTgMI0A=="]]}'

        // Parse the `get_wallet_address` RPC response.
        let jettonAddressBocEncoded = "te6cckEBAQEAJAAAQ4AFvT5rqwxcbKfITqnkwL+go4Zi9bulRHAtLt4cjjFdK7B8L+Cq"
        let jettonAddress = TONAddressConverter.fromBoc(boc: jettonAddressBocEncoded)
        XCTAssertEqual(jettonAddress, "UQAt6fNdWGLjZT5CdU8mBf0FHDMXrd0qI4FpdvDkcYrpXV5H")
    }

    func testSign() {
        let privateKeyData = Data(hexString: "c38f49de2fb13223a9e7d37d5d0ffbdd89a5eb7c8b0ee4d1c299f2cefe7dc4a0")!

        let transfer = TheOpenNetworkTransfer.with {
            $0.walletVersion = TheOpenNetworkWalletVersion.walletV4R2
            $0.dest = "EQBm--PFwDv1yCeS-QTJ-L8oiUpqo9IT1BwgVptlSq3ts90Q"
            $0.amount = 10
            $0.mode = UInt32(TheOpenNetworkSendMode.payFeesSeparately.rawValue | TheOpenNetworkSendMode.ignoreActionPhaseErrors.rawValue)
            $0.bounceable = true
        }

        let input = TheOpenNetworkSigningInput.with {
            $0.messages = [transfer]
            $0.privateKey = privateKeyData
            $0.sequenceNumber = 6
            $0.expireAt = 1671132440
        }

        let output: TheOpenNetworkSigningOutput = AnySigner.sign(input: input, coin: .ton)

        // tx: https://tonscan.org/tx/3Z4tHpXNLyprecgu5aTQHWtY7dpHXEoo11MAX61Xyg0=
        let expectedString = "te6cckEBBAEArQABRYgBsLf6vJOEq42xW0AoyWX0K+uBMUcXFDLFqmkDg6k1Io4MAQGcEUPkil2aZ4s8KKparSep/OKHMC8vuXafFbW2HGp/9AcTRv0J5T4dwyW1G0JpHw+g5Ov6QI3Xo0O9RFr3KidICimpoxdjm3UYAAAABgADAgFiYgAzffHi4B365BPJfIJk/F+URKU1UekJ6g4QK02ypVb22YhQAAAAAAAAAAAAAAAAAQMAAA08Nzs="

        XCTAssertEqual(output.encoded, expectedString)
    }

    func testJettonTransferSign() {
        let privateKeyData = Data(hexString: "c054900a527538c1b4325688a421c0469b171c29f23a62da216e90b0df2412ee")!
        
        let jettonTransfer = TheOpenNetworkJettonTransfer.with {
            $0.jettonAmount = 500 * 1000 * 1000
            $0.toOwner = "EQAFwMs5ha8OgZ9M4hQr80z9NkE7rGxUpE1hCFndiY6JnDx8"
            $0.responseAddress = "EQBaKIMq5Am2p_rfR1IFTwsNWHxBkOpLTmwUain5Fj4llTXk"
            $0.forwardAmount = 1
        }
        
        let transfer = TheOpenNetworkTransfer.with {
            $0.walletVersion = TheOpenNetworkWalletVersion.walletV4R2
            $0.dest = "EQBiaD8PO1NwfbxSkwbcNT9rXDjqhiIvXWymNO-edV0H5lja"
            $0.amount = 100 * 1000 * 1000
            $0.mode = UInt32(TheOpenNetworkSendMode.payFeesSeparately.rawValue | TheOpenNetworkSendMode.ignoreActionPhaseErrors.rawValue)
            $0.comment = "test comment"
            $0.bounceable = true
            $0.jettonTransfer = jettonTransfer
        }

        let input = TheOpenNetworkSigningInput.with {
            $0.messages = [transfer]
            $0.privateKey = privateKeyData
            $0.sequenceNumber = 1
            $0.expireAt = 1787693046
        }

        let output: TheOpenNetworkSigningOutput = AnySigner.sign(input: input, coin: .ton)

        // tx: https://testnet.tonscan.org/tx/Er_oT5R3QK7D-qVPBKUGkJAOOq6ayVls-mgEphpI9Ck=
        let expectedString = "te6cckECBAEAARUAAUWIALRRBlXIE21P9b6OpAqeFhqw+IMh1Jac2CjUU/IsfEsqDAEBnGiFlaLItV573gJqBvctP5j3jVKlLuxmO+pnW0QGlXjXgzjw5YeTNwRG9upJHOl6GA3pFetKNojqGzfkxku+owUpqaMXao4H9gAAAAEAAwIBaGIAMTQfh52puD7eKUmDbhqfta4cdUMRF662Uxp3zzqug/MgL68IAAAAAAAAAAAAAAAAAAEDAMoPin6lAAAAAAAAAABB3NZQCAALgZZzC14dAz6ZxChX5pn6bIJ3WNipSJrCELO7Ex0TOQAWiiDKuQJtqf630dSBU8LDVh8QZDqS05sFGop+RY+JZUICAAAAAHRlc3QgY29tbWVudG/bd5c="

        XCTAssertEqual(output.encoded, expectedString)
    }
}
