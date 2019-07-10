// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class SteemTests: XCTestCase {
    override func setUp() {
        continueAfterFailure = false
    }

    func testValidAddresses() {
        XCTAssertTrue(CoinType.steem.validate(address: "STM52K5kMmwiRyNQYAf7ymCMz6hieE8siyrqNt1t57ac9hvBrRdaa"))
        XCTAssertTrue(CoinType.steem.validate(address: "TST6EFA9Ge5KQaCS2jGZVf7xHZ6hzcH7uvikf5oR7YnYKemkucxB4"))
    }

    func testInvalidAddresses() {
        for addr in [
            "52K5kMmwiRyNQYAf7ymCMz6hieE8siyrqNt1t57ac9hvBrRdaa",
            "TST52K5kMmwiRyNQYAf7ymCMz6hieE8siyrqNt1t57ac9hvBrRd",
            "TST52K5kMmwiRyNQYAf7ymCMz6hieE8siyrqNt1t57ac9hvBrRdaaaa",
            "STEEM52K5kMmwiRyNQYAf7ymCMz6hieE8siyrqNt1t57ac9hvBrRdaa",
            "TEST52K5kMmwiRyNQYAf7ymCMz6hieE8siyrqNt1t57ac9hvBrRdaa",
            ] {
            XCTAssertFalse(CoinType.steem.validate(address: addr))
        }
    }

     // ensure valid input is signed
    func testSigning() throws {
        let signingInput = BravoSigningInput.with {
            $0.chainID = Data(repeating: 0, count: 32);
            $0.sender = "alice";
            $0.recipient = "bob";
            $0.amount = 90.0;
            $0.testnet = false;
            $0.memo = "Eva";
            $0.referenceBlockID = Data(hexString: "000000c4819e7d5c54c23cca3f438a1f62aa2464")!;
            $0.referenceBlockTime = 1553596224;
            $0.privateKey = Hash.sha256(data: "A".data(using: .utf8)!)
        }
        
        let result = SteemSigner.sign(input: signingInput);
        XCTAssertTrue(result.success, "Error signing: \(result.error)")
        XCTAssertEqual(result.objects.count, 1)

        let signingOutput = try BravoSigningOutput(unpackingAny: result.objects[0])
        
        let signedJSON = signingOutput.jsonEncoded
        let data = signedJSON.data(using: .utf8)
        XCTAssertNotNil(data)

        let jsonArray = try JSONSerialization.jsonObject(with: data!, options : .allowFragments) as? Dictionary<String,Any>
        XCTAssertNotNil(jsonArray, "Error parsing JSON result")
        let signature = (jsonArray!["signatures"] as? NSArray)?.firstObject as? String
        XCTAssertNotNil(signature, "Error parsing JSON result")
        XCTAssertEqual(signature!, "20e8d87c007becf88802ab0e074c001f070cd6522e8d9966feb344b3cd761d43286127787d25fa0aa4cb76969f1facb34e76fc460f7feb029c650bef697080e55e")
    }
    
    // ensure invalid inputs are not signed
    func testFailures() throws {
        let goodInput = BravoSigningInput.with {
            $0.chainID = Data(repeating: 0, count: 32);
            $0.sender = "alice";
            $0.recipient = "bob";
            $0.amount = 90.0;
            $0.testnet = false;
            $0.memo = "Eva";
            $0.referenceBlockID = Data(hexString: "000000c4819e7d5c54c23cca3f438a1f62aa2464")!;
            $0.referenceBlockTime = 1553596224;
            $0.privateKey = Hash.sha256(data: "A".data(using: .utf8)!)
        }
        
        var badInput = goodInput
        badInput.amount = -90.0
        var result = SteemSigner.sign(input: badInput);
        XCTAssertFalse(result.success, "Expected error but signing suceeded!")
        
        badInput = goodInput
        badInput.sender = String(repeating: "A", count: 17)
        result = SteemSigner.sign(input: badInput);
        XCTAssertFalse(result.success, "Expected error but signing suceeded!")
        
        badInput = goodInput
        badInput.memo = String(repeating: "A", count: 2049)
        result = SteemSigner.sign(input: badInput);
        XCTAssertFalse(result.success, "Expected error but signing suceeded!")
        
        badInput = goodInput
        badInput.referenceBlockID = Data(hexString: "0000086bf9e7704509aa41311a66fa0a1b479c")!;
        result = SteemSigner.sign(input: badInput);
        XCTAssertFalse(result.success, "Expected error but signing suceeded!")
    }
}
