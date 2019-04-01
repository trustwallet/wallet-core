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

<<<<<<< HEAD
    public static func sign(data: TW_Ontology_Proto_SigningInput) -> TW_Ontology_Proto_SigningOutput {
        let dataData = TWDataCreateWithNSData(try! data.serializedData())
=======
    public static func sign(input: TW_Ontology_Proto_SigningInput) -> TW_Ontology_Proto_SigningOutput {
        let inputData = TWDataCreateWithNSData(try! input.serializedData())
>>>>>>> upstream/master
        defer {
            TWDataDelete(dataData)
        }
<<<<<<< HEAD
        let resultData = TWDataNSData(TWOntologySignerSign(dataData))
=======
        let resultData = TWDataNSData(TWOntologySignerSign(inputData))
>>>>>>> upstream/master
        return try! TW_Ontology_Proto_SigningOutput(serializedData: resultData)
    }

    let rawValue: OpaquePointer

    init(rawValue: OpaquePointer) {
        self.rawValue = rawValue
    }


}
