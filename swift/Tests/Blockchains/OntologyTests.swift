// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class OntologyTests: XCTestCase {

    func testSignOngBalanceOf() {

        let input = OntologySigningInput.with {
            $0.contract = "ONG"
            $0.method = "balanceOf"
            $0.queryAddress = "ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD"
            $0.nonce = 3486522027
        }

        let output: OntologySigningOutput = AnySigner.sign(input: input, coin: .ontology)
        let result = output.encoded.hexString
        XCTAssertEqual("00d1ab1ad0cf0000000000000000000000000000000000000000000000000000000000000000000000004d1446b1a18af6b7c9f8a4602f9f73eeb3030f0c29b70962616c616e63654f661400000000000000000000000000000000000000020068164f6e746f6c6f67792e4e61746976652e496e766f6b650000", result)
    }

    func testSignOntBalanceOf() {

        let input = OntologySigningInput.with {
            $0.contract = "ONT"
            $0.method = "balanceOf"
            $0.queryAddress = "ANDfjwrUroaVtvBguDtrWKRMyxFwvVwnZD"
            $0.nonce = 3959576200
        }

        let output: OntologySigningOutput = AnySigner.sign(input: input, coin: .ontology)
        let result = output.encoded.hexString
        XCTAssertEqual("00d1885602ec0000000000000000000000000000000000000000000000000000000000000000000000004d1446b1a18af6b7c9f8a4602f9f73eeb3030f0c29b70962616c616e63654f661400000000000000000000000000000000000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b650000", result)
    }

    func testSignOntTransfer() {

        let input = OntologySigningInput.with {
            $0.contract = "ONT"
            $0.method = "transfer"
            $0.ownerPrivateKey = Data(hexString: "4646464646464646464646464646464646464646464646464646464646464646")!
            $0.toAddress = "Af1n2cZHhMZumNqKgw9sfCNoTWu9de4NDn"
            $0.amount = 1
            $0.gasPrice = 500
            $0.gasLimit = 20000
            $0.payerPrivateKey = Data(hexString: "4646464646464646464646464646464646464646464646464646464646464652")!
            $0.nonce = 2338116610
        }

        let output: OntologySigningOutput = AnySigner.sign(input: input, coin: .ontology)
        let result = output.encoded.hexString

        XCTAssertEqual("00d102d45c8bf401000000000000204e00000000000057e9d1a61f9aafa798b6c7fbeae35639681d7df67100c66b14fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc814feec06b79ed299ea06fcb94abac41aaf3ead76586a7cc8516a7cc86c51c1087472616e736665721400000000000000000000000000000000000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b6500024140301766d925382a6ebb2ebeb18d3741954c9370dcf6d9c45b34ce7b18bc42dcdb7cff28ddaf7f1048822c0ca21a0c4926323a2497875b963f3b8cbd3717aa6e7c2321031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486ac414038466b25ac49a22ba8c301328ef049a61711b257987e85e25d63e0444a14e860305a4cd3bb6ea2fe80fd293abb3c592e679c42c546cbf3baa051a07b28b374a6232103d9fd62df332403d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1eeac", result)

        // TODO uncomment when nist256p1 Rust implementation is enabled.
        // XCTAssertEqual("00d102d45c8bf401000000000000204e00000000000057e9d1a61f9aafa798b6c7fbeae35639681d7df67100c66b14fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc814feec06b79ed299ea06fcb94abac41aaf3ead76586a7cc8516a7cc86c51c1087472616e736665721400000000000000000000000000000000000000010068164f6e746f6c6f67792e4e61746976652e496e766f6b6500024140301766d925382a6ebb2ebeb18d3741954c9370dcf6d9c45b34ce7b18bc42dcdb8300d7215080efb87dd3f35de5f3b6d98aacd6161fbc0845b82d0d8be4b8b6d52321031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486ac414038466b25ac49a22ba8c301328ef049a61711b257987e85e25d63e0444a14e860305a4cd3bb6ea2fe80fd293abb3c592e679c42c546cbf3baa051a07b28b374a6232103d9fd62df332403d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1eeac", result)
    }

    func testSignOngTransfer() {

        let input = OntologySigningInput.with {
            $0.contract = "ONG"
            $0.method = "transfer"
            $0.ownerPrivateKey = Data(hexString: "4646464646464646464646464646464646464646464646464646464646464646")!
            $0.toAddress = "Af1n2cZHhMZumNqKgw9sfCNoTWu9de4NDn"
            $0.amount = 1
            $0.gasPrice = 500
            $0.gasLimit = 20000
            $0.payerPrivateKey = Data(hexString: "4646464646464646464646464646464646464646464646464646464646464652")!
            $0.nonce = 2827104669
        }

        let output: OntologySigningOutput = AnySigner.sign(input: input, coin: .ontology)
        let result = output.encoded.hexString

        XCTAssertEqual("00d19d3182a8f401000000000000204e00000000000057e9d1a61f9aafa798b6c7fbeae35639681d7df67100c66b14fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc814feec06b79ed299ea06fcb94abac41aaf3ead76586a7cc8516a7cc86c51c1087472616e736665721400000000000000000000000000000000000000020068164f6e746f6c6f67792e4e61746976652e496e766f6b6500024140e27e935b87855efad62bb76b21c7b591f445f867eff86f888ca6ee1870ecd80f73b8ab199a4d757b4c7b9ed46c4ff8cfa8aefaa90b7fb6485e358034448cba752321031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486ac4140450047b2efb384129a16ec4c707790e9379b978cc7085170071d8d7c5c037d743b078bd4e21bb4404c0182a32ee05260e22454dffb34dacccf458dfbee6d32db232103d9fd62df332403d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1eeac", result)

        // TODO uncomment when nist256p1 Rust implementation is enabled.
        // XCTAssertEqual("00d19d3182a8f401000000000000204e00000000000057e9d1a61f9aafa798b6c7fbeae35639681d7df67100c66b14fbacc8214765d457c8e3f2b5a1d3c4981a2e9d2a6a7cc814feec06b79ed299ea06fcb94abac41aaf3ead76586a7cc8516a7cc86c51c1087472616e736665721400000000000000000000000000000000000000020068164f6e746f6c6f67792e4e61746976652e496e766f6b6500024140e27e935b87855efad62bb76b21c7b591f445f867eff86f888ca6ee1870ecd80f8c4754e565b28a85b384612b93b00730143800049b97e83c95844a8eb7d66adc2321031bec1250aa8f78275f99a6663688f31085848d0ed92f1203e447125f927b7486ac4140450047b2efb384129a16ec4c707790e9379b978cc7085170071d8d7c5c037d74c4f8742a1de44bc0b3fe7d5cd11fad9edac2a5cdabe2c3b824743cc70df5f276232103d9fd62df332403d9114f3fa3da0d5aec9dfa42948c2f50738d52470469a1a1eeac", result)
    }

}
