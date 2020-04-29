// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class ElrondTests: XCTestCase {

    let aliceBech32 = "erd1l453hd0gt5gzdp7czpuall8ggt2dcv5zwmfdf3sd3lguxseux2fsmsgldz"
    let aliceSeedHex = "1a927e2af5306a9bb2ea777f73e06ecc0ac9aaa72fb4ea3fecf659451394cccf"
    let alicePubKeyHex = "fd691bb5e85d102687d81079dffce842d4dc328276d2d4c60d8fd1c3433c3293"
    let bobBech32 = "erd1cux02zersde0l7hhklzhywcxk4u9n4py5tdxyx7vrvhnza2r4gmq4vw35r"

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: aliceSeedHex)!)!
        let pubkey = key.getPublicKeyEd25519()
        let address = AnyAddress(publicKey: pubkey, coin: .elrond)
        let addressFromString = AnyAddress(string: aliceBech32, coin: .elrond)!

        XCTAssertEqual(pubkey.data.hexString, alicePubKeyHex)
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSign() {
        let privateKey = PrivateKey(data: Data(hexString: aliceSeedHex)!)!

        let input = ElrondSigningInput.with {
            $0.transaction = ElrondTransactionMessage.with {
                $0.nonce = 0
                $0.value = "0"
                $0.sender = aliceBech32
                $0.receiver = bobBech32 
                $0.gasPrice = 200000000000000
                $0.gasLimit = 500000000
                $0.data = "foo"
            }

            $0.privateKey = privateKey.data
        }

        let output: ElrondSigningOutput = AnySigner.sign(input: input, coin: .elrond)
        let expectedSignature = "b88ad2fe98a7316ea432a0a76c18cd87200fe75f27a8f053ea6532b40317dbec5136c5463aef132ae951b7e60d45d921caaa5903e70821dcda98f237d4ec4308"
        let expectedEncoded = #"{"nonce":0,"value":"0","receiver":"\#(bobBech32)","sender":"\#(aliceBech32)","gasPrice":200000000000000,"gasLimit":500000000,"data":"foo","signature":"\#(expectedSignature)"}"#

        XCTAssertEqual(output.signature, expectedSignature)
        XCTAssertEqual(output.encoded, expectedEncoded)
    }
}
