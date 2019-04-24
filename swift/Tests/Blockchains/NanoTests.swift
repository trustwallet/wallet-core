// Copyright © 2019 Mart Roosmaa.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class NanoTests: XCTestCase {

    func testAddressFromPublicKey() {
        let pubkey = PublicKey(data: Data(hexString: "5114aad86a390897d2a91b33b931b3a59a7df9e63eb3694f9430122f5622ae50")!, type: .ed25519Blake2b)!
        let address = NanoAddress(publicKey: pubkey)

        XCTAssertEqual(address.description, "nano_1nanode8ngaakzbck8smq6ru9bethqwyehomf79sae1k7xd47dkidjqzffeg")
    }

    func testSigner() {
        let input = NanoSigningInput.with {
            $0.privateKey = Data(hexString: "173c40e97fe2afcd24187e74f6b603cb949a5365e72fbdd065a6b165e2189e34")!
            $0.linkBlock = Data(hexString: "491fca2c69a84607d374aaf1f6acd3ce70744c5be0721b5ed394653e85233507")!
            $0.representative = "xrb_3arg3asgtigae3xckabaaewkx3bzsh7nwz7jkmjos79ihyaxwphhm6qgjps4"
            $0.balance = "96242336390000000000000000000"
        }

        let output = NanoSigner.sign(input: input)
        XCTAssertEqual(output.blockHash.hexString, "f9a323153daefe041efb94d69b9669c882c935530ed953bbe8a665dfedda9696")
        XCTAssertEqual(output.signature.hexString, "d247f6b90383b24e612569c75a12f11242f6e03b4914eadc7d941577dcf54a3a7cb7f0a4aba4246a40d9ebb5ee1e00b4a0a834ad5a1e7bef24e11f62b95a9e09")
    }

}
