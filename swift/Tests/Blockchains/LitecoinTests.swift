// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class LitecoinTests: XCTestCase {
    func testAddress() {
        let privateKey1 = PrivateKey(data: Data(hexString: "a22ddec5c567b4488bb00f69b6146c50da2ee883e2c096db098726394d585730")!)!
        let publicKey1 = privateKey1.getPublicKeySecp256k1(compressed: true)

        let legacyAddress = BitcoinAddress(publicKey: publicKey1, prefix: CoinType.litecoin.p2pkhPrefix)!
        XCTAssertEqual(BitcoinAddress(string: "LV7LV7Z4bWDEjYkfx9dQo6k6RjGbXsg6hS")!.description, legacyAddress.description)

        let privateKey2 = PrivateKey(data: Data(hexString: "f6ee7e6c9bd2f4dc8f0db0dc4679de06c998afc42d825edf7966dd4488b0aa1f")!)!
        let publicKey2 = privateKey2.getPublicKeySecp256k1(compressed: true)
        let compatibleAddress = BitcoinAddress.compatibleAddress(publicKey: publicKey2, prefix: CoinType.litecoin.p2shPrefix)
        XCTAssertEqual(BitcoinAddress(string: "M8eTgzhoFTErAjkGa6cyBomcHfxAprbDgD")!.description, compatibleAddress.description)

        let privateKey3 = PrivateKey(data: Data(hexString: "55f9cbb0376c422946fa28397c1219933ac60b312ede41bfacaf701ecd546625")!)!
        let publicKey3 = privateKey3.getPublicKeySecp256k1(compressed: true)
        let bech32Address = SegwitAddress(hrp: .litecoin, publicKey: publicKey3)
        XCTAssertEqual(SegwitAddress(string: "ltc1qytnqzjknvv03jwfgrsmzt0ycmwqgl0asjnaxwu")!.description, bech32Address.description)
    }

    func testLitecoinBlockchain() {
        let chain = CoinType.litecoin
        XCTAssertTrue(chain.validate(address: "ltc1qytnqzjknvv03jwfgrsmzt0ycmwqgl0asjnaxwu"))
        XCTAssertTrue(chain.validate(address: "LWPvWpD86JNrBVGiju377iuShKGPq5AmoV"))
        XCTAssertTrue(chain.validate(address: "MX9ZMoqAQ7ATem77pCPTfeCP1n93YaLekd"))
        XCTAssertFalse(chain.validate(address: "Xm1iDLBP5tdxTxc6t7uJBCVjC4L2A5vB2J"))
        XCTAssertFalse(chain.validate(address: "bitcoincash:qq07l6rr5lsdm3m80qxw80ku2ex0tj76vvsxpvmgme"))
        XCTAssertFalse(chain.validate(address: "bc1qvtvte5tzlqlfhcdmph94lxk8jcz54q6psyvgla"))
    }

    func testExtendedKeys() {
        let wallet = HDWallet.test

        // .bip44
        let lptv = wallet.getExtendedPrivateKey(purpose: .bip44, coin: .litecoin, version: .ltpv)
        let ltub = wallet.getExtendedPubKey(purpose: .bip44, coin: .litecoin, version: .ltub)

        XCTAssertEqual(lptv, "Ltpv77Tkf73NsKY3NQWVr6sHXeDQHWV6EVBXStcyxfnwEVebFuz1URxi16SBzj4v7n3mSYh8PQXhSFM2aXNFdx8bvWBLZGXeik3UQXeXn5vudGj")
        XCTAssertEqual(ltub, "Ltub2Ye6FtTv7U4zzHDL6iMfcE3cj5BHJjkBXQj1deZEAgSBrHB5oM191hYTF8BC34r7vRDGng59yfP6FH4m3nttc3TLDg944G8QK7d5NnygCRu")

        // .bip49
        let mtpv = wallet.getExtendedPrivateKey(purpose: .bip49, coin: .litecoin, version: .mtpv)
        let mtub = wallet.getExtendedPubKey(purpose: .bip49, coin: .litecoin, version: .mtub)
        XCTAssertEqual(mtpv, "Mtpv7SPQ3PnRFU5yMidTBbXKxb6pgrE1Ny1yVssVvTz8VLDppPrhdydSaoMp6fm58VbtBTrVZVacMrSUim44RccBLu8NFAqj7ZaB5JBzb8cgQHp")
        XCTAssertEqual(mtub, "Mtub2sZjeBCxVccvybLHSD1i3Aw38QvCTDadaPyXbSkRRX1RQm3mxtfsbQU5M3PdCSP4xAFHCceEQ3FmQF69Du2wbcmebt3CaWAGALBSe8c4Gvw")

        // .bip84
        let zprv = wallet.getExtendedPrivateKey(purpose: .bip84, coin: .litecoin, version: .zprv)
        let zpub = wallet.getExtendedPubKey(purpose: .bip84, coin: .litecoin, version: .zpub)
        XCTAssertEqual(zprv, "zprvAeCuQd5okFvvK1oeAQEPtgtPd5feXtcmszyCDK8HUPob28R79tUgtpCga79KgtDkUBn72AMig5NNzUCFY1JeRsZcEitDVEYuB48uHt2YEDB")
        XCTAssertEqual(zpub, "zpub6sCFp8chadVDXVt7GRmQFpq8B7W8wMLdFDto1hXu2jLZtvkFhRnwScXARNfrGSeyhR8DBLJnaUUkBbkmB2GwUYkecEAMUcbUpFQV4v7PXcs")
    }

    func testDeriveFromLtub() {
        let xpub = "Ltub2Ye6FtTv7U4zzHDL6iMfcE3cj5BHJjkBXQj1deZEAgSBrHB5oM191hYTF8BC34r7vRDGng59yfP6FH4m3nttc3TLDg944G8QK7d5NnygCRu"
        let litecoin = CoinType.litecoin
        let xpubAddr2 = HDWallet.derive(from: xpub, at: DerivationPath(purpose: .bip44, coinType: litecoin, account: 0, change: 0, address: 2))!
        let xpubAddr9 = HDWallet.derive(from: xpub, at: DerivationPath(purpose: .bip44, coinType: litecoin, account: 0, change: 0, address: 9))!

        XCTAssertEqual(BitcoinAddress(publicKey: xpubAddr2, prefix: CoinType.litecoin.p2pkhPrefix)!.description, "LdJvSS8gcRSN1WbSEj6srV8dKzGcybHGKt")
        XCTAssertEqual(BitcoinAddress(publicKey: xpubAddr9, prefix: CoinType.litecoin.p2pkhPrefix)!.description, "Laj4byUKgW3wuou4G3XCAPWqzVc3SdEpQk")
    }

    func testDeriveFromMtub() {
        let ypub = "Mtub2sZjeBCxVccvybLHSD1i3Aw38QvCTDadaPyXbSkRRX1RQm3mxtfsbQU5M3PdCSP4xAFHCceEQ3FmQF69Du2wbcmebt3CaWAGALBSe8c4Gvw"

        let litecoin = CoinType.litecoin
        let ypubAddr3 = HDWallet.derive(from: ypub, at: DerivationPath(purpose: .bip49, coinType: litecoin, account: 0, change: 0, address: 3))!
        let ypubAddr10 = HDWallet.derive(from: ypub, at: DerivationPath(purpose: .bip49, coinType: litecoin, account: 0, change: 0, address: 10))!

        XCTAssertEqual(BitcoinAddress.compatibleAddress(publicKey: ypubAddr3, prefix: CoinType.litecoin.p2shPrefix).description, "MVr2vvjyaTzmfX3LFZcg5KZ7Cc36pgAWcy")
        XCTAssertEqual(BitcoinAddress.compatibleAddress(publicKey: ypubAddr10, prefix: CoinType.litecoin.p2shPrefix).description, "MTgkF6T5h92QDmpFsBk4fJeYt3dx5ERQtD")
    }

    func testDeriveFromZPub() {
        let zpub = "zpub6sCFp8chadVDXVt7GRmQFpq8B7W8wMLdFDto1hXu2jLZtvkFhRnwScXARNfrGSeyhR8DBLJnaUUkBbkmB2GwUYkecEAMUcbUpFQV4v7PXcs"
        let litecoin = CoinType.litecoin
        let zpubAddr4 = HDWallet.derive(from: zpub, at: DerivationPath(purpose: litecoin.purpose, coinType: litecoin, account: 0, change: 0, address: 4))!
        let zpubAddr11 = HDWallet.derive(from: zpub, at: DerivationPath(purpose: litecoin.purpose, coinType: litecoin, account: 0, change: 0, address: 11))!

        XCTAssertEqual(SegwitAddress(hrp: .litecoin, publicKey: zpubAddr4).description, "ltc1qcgnevr9rp7aazy62m4gen0tfzlssa52axwytt6")
        XCTAssertEqual(SegwitAddress(hrp: .litecoin, publicKey: zpubAddr11).description, "ltc1qy072y8968nzp6mz3j292h8lp72d678fcmms6vl")
    }
}
