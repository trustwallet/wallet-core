// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.
//
// This is a GENERATED FILE, changes made here WILL BE LOST.
//

import Foundation

public final class OntologySigner {
    
    public static func encode(input: TWOntologySigningInput) -> TWOntologySigningOutput {
        let inputData = TWDataCreateWithNSData(try! input.serializedData())
        defer {
            TWDataDelete(inputData)
        }
        let resultData = TWDataNSData(TWOntologySignerSign(inputData))
        return try! TWOntologySigningOutput(serializedData: resultData)
    }
    
    let rawValue: OpaquePointer
    
    init(rawValue: OpaquePointer) {
        self.rawValue = rawValue
    }
    
    
}
