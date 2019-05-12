// Copyright © 2017-2019 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
import TrustWalletCore
import XCTest

class ARKTests: XCTestCase {
    
    func testAddress() {
        let privateKey = PrivateKey(data: Data(hexString: "db33ffdf82c7ba903daf68d961d3c23c20471a8ce6b408e52d579fd8add80cc9")!)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true);
        let address = ARKAddress(publicKey: publicKey)
        XCTAssertEqual(address.description, "AHqJoKaLeEAeJ6e3dk9DXgbNUpxGJebSvt")
    }
    
    func testSigner() throws {
        let input = ARKSigningInput.with {
            $0.amount = 1000000000
            $0.privateKey = Data(hexString: "d8839c2432bfd0a67ef10a804ba991eabba19f154a3d707917681d45822a5712")!
            $0.timestamp = 50686854
            $0.recipientID = "ANBkoGqWeTSiaEVgVzSKZd3jS7UWzv9PSo"
        }
        
        let output = ARKSigner.sign(input: input)

        let jsonArray = try JSONSerialization.jsonObject(with: output.json.data(using: .utf8)!, options: .allowFragments) as? Dictionary<String,Any>
        
        let signature = jsonArray!["signature"] as! String
        let senderPublicKey = jsonArray!["senderPublicKey"] as! String

        XCTAssertEqual(signature, "304402201ace9afcaf9d0ec64a31fd98c589767c76b5360d5b22dfe3cde2dfffdfef61dc022026d276a6140e6abbd80775541479cc71cf52590895bd24c0c577a9c57ecae581")
        XCTAssertEqual(senderPublicKey, "034151a3ec46b5670a682b0a63394f863587d1bc97483b1b6c70eb58e7f0aed192")
    }
    
}
