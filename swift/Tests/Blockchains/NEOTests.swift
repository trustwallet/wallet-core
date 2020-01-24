// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class NEOTests: XCTestCase {

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "2A9EAB0FEC93CD94FA0A209AC5604602C1F0105FB02EAB398E17B4517C2FFBAB")!)!
        let pubkey = key.getPublicKeyNist256p1()
        let address = AnyAddress(publicKey: pubkey, coin: .neo)
        let addressFromString = AnyAddress(string: "AQCSMB3oSDA1dHPn6GXN6KB4NHmdo1fX41", coin: .neo)

        XCTAssertEqual(pubkey.data.hexString, "024d520691053ba53daf1692d2403c9a3ddd4955e82c513a065ec0e1a65476fc10")
        XCTAssertEqual(address.description, addressFromString?.description)
    }

    func testSign() {
        var inputTx = NEOSigningInput.with {
            $0.privateKey = Data(hexString: "F18B2F726000E86B4950EBEA7BFF151F69635951BC4A31C44F28EE6AF7AEC128")!
        }

        let input = NEOTransactionInput.with {
            $0.prevHash = Data(hexString: "de24ccbca45777a86c2ca5b272d1f1a293dae811a7f86bfd2b7e67d59cb3859c")!
            $0.prevIndex = 1
        }

        inputTx.inputs.append(input)

        let output0 = NEOTransactionOutput.with {
            $0.assetID = "9b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc5"
            $0.value = (1 * 100000000)
            $0.address = "Ad9A1xPbuA5YBFr1XPznDwBwQzdckAjCev"
        }
        let output1 = NEOTransactionOutput.with {
            $0.assetID = "9b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc5"
            $0.value = (892 * 100000000)
            $0.address = "AdtSLMBqACP4jv8tRWwyweXGpyGG46eMXV"
        }

        inputTx.outputs.append(output0)
        inputTx.outputs.append(output1)

        let signedTx = NEOSigner.sign(input: inputTx)
        let result = signedTx.encoded.hexString

        XCTAssertEqual("800000019c85b39cd5677e2bfd6bf8a711e8da93a2f1d172b2a52c6ca87757a4bccc24de0100029b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc500e1f50500000000ea610aa6db39bd8c8556c9569d94b5e5a5d0ad199b7cffdaa674beae0f930ebe6085af9093e5fe56b34a5c220ccdcf6efc336fc500fcbbc414000000f2908c7efc0c9e43ffa7e79170ba37e501e1b4ac0141405046619c8e20e1fdeec92ce95f3019f6e7cc057294eb16b2d5e55c105bf32eb27e1fc01c1858576228f1fef8c0945a8ad69688e52a4ed19f5b85f5eff7e961d7232102a41c2aea8568864b106553729d32b1317ec463aa23e7a3521455d95992e17a7aac",
                result)

    }
}
