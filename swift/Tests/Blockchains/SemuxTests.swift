// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import XCTest
import TrustWalletCore

class SemuxTests: XCTestCase {

    func testAddress() {
        let address = SemuxAddress(string: "0x0680a919c78faa59b127014b6181979ae0a62dbd")!
        XCTAssertEqual(address.description, "0x0680a919c78faa59b127014b6181979ae0a62dbd")
    }
    
    func testSigner() {
        let input = SemuxSigningInput.with {
            $0.privateKey = Data(hexString: "7ea3e3e2ce1e2c4e7696f09a252a1b9d58948bc942c0b42092080a896c43649f")!
            $0.destination = "0xdb7cadb25fdcdd546fb0268524107582c3f8999c"
            $0.value = 123456789
            $0.fee = 5000000
            $0.nonce = 123
            $0.timestamp = 1523028482000
        }

        let output = SemuxSigner.sign(input: input)

        XCTAssertEqual(output.encoded.hexString, "20e3b076d3d634b9c88b4b2ab281ffd7c440e0eeccf157f2a7cc09c3b7885958c738000114db7cadb25fdcdd546fb0268524107582c3f8999c00000000075bcd1500000000004c4b40000000000000007b000001629b9257d0006064f22068b0e9d103aaae81c099d1d59a44c7ec022550ab8dcccd28104a2a79d27c9dc9a277da765bd5bde2667af78a67a99aa33bf6e352e36546d0285526210e057f987e38f88037e8019cbb774dda106fc051fc4a6320a00294fe1866d08442")
    }
    
}
