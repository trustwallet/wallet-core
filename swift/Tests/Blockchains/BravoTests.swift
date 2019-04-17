import XCTest
import TrustWalletCore

class BravoSignerTests: XCTestCase {
    override func setUp() {
        continueAfterFailure = false
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
            $0.referenceBlockID = Data(hexString: "0000086bf9e7704509aa41311a66fa0a1b479c6b")!;
            $0.referenceBlockTime = 1552464180;
            $0.privateKey = Hash.sha256(data: "A".data(using: .utf8)!)
        }
        
        let result = BravoSigner.sign(input: signingInput);
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
        XCTAssertEqual(signature!, "1f05b3dddfcbca5748b02562645fe7c3f01044b78983ce673f84c230b2dc97beed19b2e8462f1e45f2ac7600c2900f9f90510efe0891141ad2c6b1ae33b21bcace")
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
            $0.referenceBlockID = Data(hexString: "0000086bf9e7704509aa41311a66fa0a1b479c6b")!;
            $0.referenceBlockTime = 1552464180;
            $0.privateKey = Hash.sha256(data: "A".data(using: .utf8)!)
        }
        
        var badInput = goodInput
        badInput.amount = -90.0
        var result = BravoSigner.sign(input: badInput);
        XCTAssertFalse(result.success, "Expected error but signing suceeded!")
        
        badInput = goodInput
        badInput.sender = String(repeating: "A", count: 17)
        result = BravoSigner.sign(input: badInput);
        XCTAssertFalse(result.success, "Expected error but signing suceeded!")
        
        badInput = goodInput
        badInput.memo = String(repeating: "A", count: 2049)
        result = BravoSigner.sign(input: badInput);
        XCTAssertFalse(result.success, "Expected error but signing suceeded!")
        
        badInput = goodInput
        badInput.referenceBlockID = Data(hexString: "0000086bf9e7704509aa41311a66fa0a1b479c")!;
        result = BravoSigner.sign(input: badInput);
        XCTAssertFalse(result.success, "Expected error but signing suceeded!")
    }
}
