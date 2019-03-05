// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class LitecoinTests: XCTestCase {
    func testAddress() {
        let litcoin = Litecoin()
        let privateKey1 = PrivateKey(wif: "T8VERgAiBcUnRXmWxgVzp6AaH1hKwPQQQeghi3n9ZY6nF59GuTJf")!
        let publicKey1 = privateKey1.getPublicKey(compressed: true)

        let legacyAddress = litcoin.legacyAddress(for: publicKey1)
        XCTAssertEqual(LitecoinAddress(string: "LV7LV7Z4bWDEjYkfx9dQo6k6RjGbXsg6hS")!.description, legacyAddress.description)

        let privateKey2 = PrivateKey(wif: "TBKynom8diHvD7TzpURLY2EHP6MbR7iYiaD6fGPiTB5pHbxSNXgH")!
        let publicKey2 = privateKey2.getPublicKey(compressed: true)
        let compatibleAddress = litcoin.compatibleAddress(for: publicKey2)
        XCTAssertEqual(LitecoinAddress(string: "M8eTgzhoFTErAjkGa6cyBomcHfxAprbDgD")!.description, compatibleAddress.description)

        let privateKey3 = PrivateKey(wif: "T5w6v6RpidKc8JMMcRDi6f6xTaEVV52LG2W73mCVesA4ZGWef2xA")!
        let publicKey3 = privateKey3.getPublicKey(compressed: true)
        let bech32Address = litcoin.address(for: publicKey3)
        XCTAssertEqual(LitecoinBech32Address(string: "ltc1qytnqzjknvv03jwfgrsmzt0ycmwqgl0asjnaxwu")!.description, bech32Address.description)
    }

    func testLitecoinBlockchain() {
        let chain = Litecoin()
        XCTAssertTrue(chain.validate(address: "ltc1qytnqzjknvv03jwfgrsmzt0ycmwqgl0asjnaxwu"))
        XCTAssertTrue(chain.validate(address: "LWPvWpD86JNrBVGiju377iuShKGPq5AmoV"))
        XCTAssertTrue(chain.validate(address: "MX9ZMoqAQ7ATem77pCPTfeCP1n93YaLekd"))
        XCTAssertFalse(chain.validate(address: "Xm1iDLBP5tdxTxc6t7uJBCVjC4L2A5vB2J"))
        XCTAssertFalse(chain.validate(address: "bitcoincash:qq07l6rr5lsdm3m80qxw80ku2ex0tj76vvsxpvmgme"))
        XCTAssertFalse(chain.validate(address: "bc1qvtvte5tzlqlfhcdmph94lxk8jcz54q6psyvgla"))
    }

    func testExtendedKeys() {
        let wallet = testWallet

        // .bip44
        let lptv = wallet.getExtendedPrivateKey(curve: .secp256k1, purpose: .bip44, coin: .litecoin, version: .ltpv)
        let ltub = wallet.getExtendedPubKey(curve: .secp256k1, purpose: .bip44, coin: .litecoin, version: .ltub)

        XCTAssertEqual(lptv, "Ltpv77Tkf73NsKY3NQWVr6sHXeDQHWV6EVBXStcyxfnwEVebFuz1URxi16SBzj4v7n3mSYh8PQXhSFM2aXNFdx8bvWBLZGXeik3UQXeXn5vudGj")
        XCTAssertEqual(ltub, "Ltub2Ye6FtTv7U4zzHDL6iMfcE3cj5BHJjkBXQj1deZEAgSBrHB5oM191hYTF8BC34r7vRDGng59yfP6FH4m3nttc3TLDg944G8QK7d5NnygCRu")

        // .bip49
        let mtpv = wallet.getExtendedPrivateKey(curve: .secp256k1, purpose: .bip49, coin: .litecoin, version: .mtpv)
        let mtub = wallet.getExtendedPubKey(curve: .secp256k1, purpose: .bip49, coin: .litecoin, version: .mtub)
        XCTAssertEqual(mtpv, "Mtpv7SPQ3PnRFU5yMidTBbXKxb6pgrE1Ny1yVssVvTz8VLDppPrhdydSaoMp6fm58VbtBTrVZVacMrSUim44RccBLu8NFAqj7ZaB5JBzb8cgQHp")
        XCTAssertEqual(mtub, "Mtub2sZjeBCxVccvybLHSD1i3Aw38QvCTDadaPyXbSkRRX1RQm3mxtfsbQU5M3PdCSP4xAFHCceEQ3FmQF69Du2wbcmebt3CaWAGALBSe8c4Gvw")

        // .bip84
        let zprv = wallet.getExtendedPrivateKey(curve: .secp256k1, purpose: .bip84, coin: .litecoin, version: .zprv)
        let zpub = wallet.getExtendedPubKey(curve: .secp256k1, purpose: .bip84, coin: .litecoin, version: .zpub)
        XCTAssertEqual(zprv, "zprvAeCuQd5okFvvK1oeAQEPtgtPd5feXtcmszyCDK8HUPob28R79tUgtpCga79KgtDkUBn72AMig5NNzUCFY1JeRsZcEitDVEYuB48uHt2YEDB")
        XCTAssertEqual(zpub, "zpub6sCFp8chadVDXVt7GRmQFpq8B7W8wMLdFDto1hXu2jLZtvkFhRnwScXARNfrGSeyhR8DBLJnaUUkBbkmB2GwUYkecEAMUcbUpFQV4v7PXcs")
    }

    func testDeriveFromLtub() {
        let xpub = "Ltub2Ye6FtTv7U4zzHDL6iMfcE3cj5BHJjkBXQj1deZEAgSBrHB5oM191hYTF8BC34r7vRDGng59yfP6FH4m3nttc3TLDg944G8QK7d5NnygCRu"
        let bc = Litecoin()
        let xpubAddr2 = bc.derive(from: xpub, at: bc.derivationPath(at: 2))!
        let xpubAddr9 = bc.derive(from: xpub, at: bc.derivationPath(at: 9))!

        XCTAssertEqual(xpubAddr2.description, "ltc1qce3dynjdqw2sjk65fnnfymzq0tagnhygjtnmhp")
        XCTAssertEqual(xpubAddr9.description, "ltc1q4g9nuxacjljnjudglk5m6d866s0vpds32rgyv6")
    }

    func testDeriveFromMtub() {
        let ypub = "Mtub2sZjeBCxVccvybLHSD1i3Aw38QvCTDadaPyXbSkRRX1RQm3mxtfsbQU5M3PdCSP4xAFHCceEQ3FmQF69Du2wbcmebt3CaWAGALBSe8c4Gvw"
        let bc = Litecoin()
        let ypubAddr3 = bc.derive(from: ypub, at: bc.derivationPath(at: 3))!
        let ypubAddr10 = bc.derive(from: ypub, at: bc.derivationPath(at: 10))!

        XCTAssertEqual(ypubAddr3.description, "ltc1qwdrtc0vn70f2pggaqln8qse245kwnedwsfuhy6")
        XCTAssertEqual(ypubAddr10.description, "ltc1qlp99dfc57evpuce4at7nupgg2vat3av7zek7ls")
    }

    func testDeriveFromZPub() {
        let zpub = "zpub6sCFp8chadVDXVt7GRmQFpq8B7W8wMLdFDto1hXu2jLZtvkFhRnwScXARNfrGSeyhR8DBLJnaUUkBbkmB2GwUYkecEAMUcbUpFQV4v7PXcs"
        let bc = Litecoin()
        let zpubAddr4 = bc.derive(from: zpub, at: bc.derivationPath(at: 4))!
        let zpubAddr11 = bc.derive(from: zpub, at: bc.derivationPath(at: 11))!

        XCTAssertEqual(zpubAddr4.description, "ltc1qcgnevr9rp7aazy62m4gen0tfzlssa52axwytt6")
        XCTAssertEqual(zpubAddr11.description, "ltc1qy072y8968nzp6mz3j292h8lp72d678fcmms6vl")
    }
}
