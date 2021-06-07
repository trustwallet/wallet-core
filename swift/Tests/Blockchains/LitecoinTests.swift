// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
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
        let ltub = wallet.getExtendedPublicKey(purpose: .bip44, coin: .litecoin, version: .ltub)

        XCTAssertEqual(lptv, "Ltpv77Tkf73NsKY3NQWVr6sHXeDQHWV6EVBXStcyxfnwEVebFuz1URxi16SBzj4v7n3mSYh8PQXhSFM2aXNFdx8bvWBLZGXeik3UQXeXn5vudGj")
        XCTAssertEqual(ltub, "Ltub2Ye6FtTv7U4zzHDL6iMfcE3cj5BHJjkBXQj1deZEAgSBrHB5oM191hYTF8BC34r7vRDGng59yfP6FH4m3nttc3TLDg944G8QK7d5NnygCRu")

        // .bip49
        let mtpv = wallet.getExtendedPrivateKey(purpose: .bip49, coin: .litecoin, version: .mtpv)
        let mtub = wallet.getExtendedPublicKey(purpose: .bip49, coin: .litecoin, version: .mtub)
        XCTAssertEqual(mtpv, "Mtpv7SPQ3PnRFU5yMidTBbXKxb6pgrE1Ny1yVssVvTz8VLDppPrhdydSaoMp6fm58VbtBTrVZVacMrSUim44RccBLu8NFAqj7ZaB5JBzb8cgQHp")
        XCTAssertEqual(mtub, "Mtub2sZjeBCxVccvybLHSD1i3Aw38QvCTDadaPyXbSkRRX1RQm3mxtfsbQU5M3PdCSP4xAFHCceEQ3FmQF69Du2wbcmebt3CaWAGALBSe8c4Gvw")

        // .bip84
        let zprv = wallet.getExtendedPrivateKey(purpose: .bip84, coin: .litecoin, version: .zprv)
        let zpub = wallet.getExtendedPublicKey(purpose: .bip84, coin: .litecoin, version: .zpub)
        XCTAssertEqual(zprv, "zprvAeCuQd5okFvvK1oeAQEPtgtPd5feXtcmszyCDK8HUPob28R79tUgtpCga79KgtDkUBn72AMig5NNzUCFY1JeRsZcEitDVEYuB48uHt2YEDB")
        XCTAssertEqual(zpub, "zpub6sCFp8chadVDXVt7GRmQFpq8B7W8wMLdFDto1hXu2jLZtvkFhRnwScXARNfrGSeyhR8DBLJnaUUkBbkmB2GwUYkecEAMUcbUpFQV4v7PXcs")
    }

    func testDeriveFromLtub() {
        let xpub = "Ltub2Ye6FtTv7U4zzHDL6iMfcE3cj5BHJjkBXQj1deZEAgSBrHB5oM191hYTF8BC34r7vRDGng59yfP6FH4m3nttc3TLDg944G8QK7d5NnygCRu"
        let litecoin = CoinType.litecoin
        let xpubAddr2 = HDWallet.getPublicKeyFromExtended(
            extended: xpub,
            coin: litecoin,
            derivationPath: DerivationPath(purpose: .bip44, coin: litecoin.slip44Id, account: 0, change: 0, address: 2).description
        )!
        let xpubAddr9 = HDWallet.getPublicKeyFromExtended(
            extended: xpub,
            coin: litecoin,
            derivationPath: DerivationPath(purpose: .bip44, coin: litecoin.slip44Id, account: 0, change: 0, address: 9).description
        )!

        XCTAssertEqual(BitcoinAddress(publicKey: xpubAddr2, prefix: CoinType.litecoin.p2pkhPrefix)!.description, "LdJvSS8gcRSN1WbSEj6srV8dKzGcybHGKt")
        XCTAssertEqual(BitcoinAddress(publicKey: xpubAddr9, prefix: CoinType.litecoin.p2pkhPrefix)!.description, "Laj4byUKgW3wuou4G3XCAPWqzVc3SdEpQk")
    }

    func testDeriveFromMtub() {
        let ypub = "Mtub2sZjeBCxVccvybLHSD1i3Aw38QvCTDadaPyXbSkRRX1RQm3mxtfsbQU5M3PdCSP4xAFHCceEQ3FmQF69Du2wbcmebt3CaWAGALBSe8c4Gvw"

        let litecoin = CoinType.litecoin
        let ypubAddr3 = HDWallet.getPublicKeyFromExtended(
            extended: ypub,
            coin: litecoin,
            derivationPath: DerivationPath(purpose: .bip49, coin: litecoin.slip44Id, account: 0, change: 0, address: 3).description
        )!
        let ypubAddr10 = HDWallet.getPublicKeyFromExtended(
            extended: ypub,
            coin: litecoin,
            derivationPath: DerivationPath(purpose: .bip49, coin: litecoin.slip44Id, account: 0, change: 0, address: 10).description
        )!

        XCTAssertEqual(BitcoinAddress.compatibleAddress(publicKey: ypubAddr3, prefix: CoinType.litecoin.p2shPrefix).description, "MVr2vvjyaTzmfX3LFZcg5KZ7Cc36pgAWcy")
        XCTAssertEqual(BitcoinAddress.compatibleAddress(publicKey: ypubAddr10, prefix: CoinType.litecoin.p2shPrefix).description, "MTgkF6T5h92QDmpFsBk4fJeYt3dx5ERQtD")
    }

    func testDeriveFromZPub() {
        let zpub = "zpub6sCFp8chadVDXVt7GRmQFpq8B7W8wMLdFDto1hXu2jLZtvkFhRnwScXARNfrGSeyhR8DBLJnaUUkBbkmB2GwUYkecEAMUcbUpFQV4v7PXcs"
        let litecoin = CoinType.litecoin
        let zpubAddr4 = HDWallet.getPublicKeyFromExtended(
            extended: zpub,
            coin: litecoin,
            derivationPath: DerivationPath(purpose: .bip49, coin: litecoin.slip44Id, account: 0, change: 0, address: 4).description
        )!
        let zpubAddr11 = HDWallet.getPublicKeyFromExtended(
            extended: zpub,
            coin: litecoin,
            derivationPath: DerivationPath(purpose: .bip49, coin: litecoin.slip44Id, account: 0, change: 0, address: 11).description
        )!

        XCTAssertEqual(SegwitAddress(hrp: .litecoin, publicKey: zpubAddr4).description, "ltc1qcgnevr9rp7aazy62m4gen0tfzlssa52axwytt6")
        XCTAssertEqual(SegwitAddress(hrp: .litecoin, publicKey: zpubAddr11).description, "ltc1qy072y8968nzp6mz3j292h8lp72d678fcmms6vl")
    }

    func testPlanAndSign_8435() throws {
        let address = "ltc1q0dvup9kzplv6yulzgzzxkge8d35axkq4n45hum"
        let lockScript = BitcoinScript.lockScriptForAddress(address: address, coin: .litecoin)
        let utxos = [
            BitcoinUnspentTransaction.with {
                $0.outPoint.hash = Data.reverse(hexString: "a85fd6a9a7f2f54cacb57e83dfd408e51c0a5fc82885e3fa06be8692962bc407")
                $0.outPoint.index = 0
                $0.outPoint.sequence = UINT32_MAX
                $0.script = lockScript.data
                $0.amount = 3899774
            }
        ]

        // redeem p2pwkh
        let scriptHash = lockScript.matchPayToWitnessPublicKeyHash()!
        var input = BitcoinSigningInput.with {
            $0.toAddress = "ltc1qt36tu30tgk35tyzsve6jjq3dnhu2rm8l8v5q00"
            $0.changeAddress = address
            $0.hashType = BitcoinScript.hashTypeForCoin(coinType: .litecoin)
            $0.amount = 1200000
            $0.coinType = CoinType.litecoin.rawValue
            $0.byteFee = 1
            $0.utxo = utxos
            $0.useMaxAmount = false
            $0.scripts = [
                scriptHash.hexString: BitcoinScript.buildPayToPublicKeyHash(hash: scriptHash).data
            ]
        }

        // Plan
        let plan: BitcoinTransactionPlan = AnySigner.plan(input: input, coin: .litecoin)

        XCTAssertEqual(plan.amount, 1200000)
        XCTAssertEqual(plan.fee, 141)
        XCTAssertEqual(plan.change, 2699633)

        // Extend input with private key
        input.privateKey = [Data(hexString: "690b34763f34e0226ad2a4d47098269322e0402f847c97166e8f39959fcaff5a")!]
        input.plan = plan

        // Sign
        let output: BitcoinSigningOutput = AnySigner.sign(input: input, coin: .litecoin)
        XCTAssertEqual(output.error, TW_Common_Proto_SigningError.ok)

        // https://blockchair.com/litecoin/transaction/8435d205614ee70066060734adf03af4194d0c3bc66dd01bb124ab7fd25e2ef8
        let txId = output.transactionID
        XCTAssertEqual(txId, "8435d205614ee70066060734adf03af4194d0c3bc66dd01bb124ab7fd25e2ef8")

        let encoded = output.encoded
        XCTAssertEqual(encoded.hexString,
            "01000000" + // version
            "0001" + // marker & flag
            "01" + // inputs
                "07c42b969286be06fae38528c85f0a1ce508d4df837eb5ac4cf5f2a7a9d65fa8" + "00000000" + "00" + "ffffffff" +
            "02" + // outputs
                "804f120000000000" + "16" + "00145c74be45eb45a3459050667529022d9df8a1ecff" +
                "7131290000000000" + "16" + "00147b59c096c20fd9a273e240846b23276c69d35815" +
            // witness
                "02" +
                    "47" + "304402204139b82927dd80445f27a5d2c29fa4881dbd2911714452a4a706145bc43cc4bf022016fbdf4b09bc5a9c43e79edb1c1061759779a20c35535082bdc469a61ed0771f01" +
                    "21" + "02499e327a05cc8bb4b3c34c8347ecfcb152517c9927c092fa273be5379fde3226" +
            "00000000" // nLockTime
        )
    }
}
