// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class SyscoinTests: XCTestCase {
    func testAddress() {
        let privateKey1 = PrivateKey(data: Data(hexString: "a22ddec5c567b4488bb00f69b6146c50da2ee883e2c096db098726394d585730")!)!
        let publicKey1 = privateKey1.getPublicKeySecp256k1(compressed: true)

        let legacyAddress = BitcoinAddress(publicKey: publicKey1, prefix: CoinType.syscoin.p2pkhPrefix)!
        XCTAssertEqual(BitcoinAddress(string: "SXBPFk2PFDAP13qyKSdC4yptsJ8kJRAT3T")!.description, legacyAddress.description)

        let privateKey2 = PrivateKey(data: Data(hexString: "f6ee7e6c9bd2f4dc8f0db0dc4679de06c998afc42d825edf7966dd4488b0aa1f")!)!
        let publicKey2 = privateKey2.getPublicKeySecp256k1(compressed: true)
        let compatibleAddress = BitcoinAddress.compatibleAddress(publicKey: publicKey2, prefix: CoinType.syscoin.p2shPrefix)
        XCTAssertEqual(BitcoinAddress(string: "32SKP7HqJLPRNEUNUDddNAXCxyMinjbX8g")!.description, compatibleAddress.description)

        let privateKey3 = PrivateKey(data: Data(hexString: "55f9cbb0376c422946fa28397c1219933ac60b312ede41bfacaf701ecd546625")!)!
        let publicKey3 = privateKey3.getPublicKeySecp256k1(compressed: true)
        let bech32Address = SegwitAddress(hrp: .syscoin, publicKey: publicKey3)
        XCTAssertEqual(SegwitAddress(string: "sys1qytnqzjknvv03jwfgrsmzt0ycmwqgl0as7lkcf7")!.description, bech32Address.description)
    }

    func testSyscoinBlockchain() {
        let chain = CoinType.syscoin
        XCTAssertTrue(chain.validate(address: "sys1qytnqzjknvv03jwfgrsmzt0ycmwqgl0as7lkcf7"))
        XCTAssertTrue(chain.validate(address: "SXBPFk2PFDAP13qyKSdC4yptsJ8kJRAT3T"))
        XCTAssertTrue(chain.validate(address: "32SKP7HqJLPRNEUNUDddNAXCxyMinjbX8g"))
        XCTAssertFalse(chain.validate(address: "Xm1iDLBP5tdxTxc6t7uJBCVjC4L2A5vB2J"))
        XCTAssertFalse(chain.validate(address: "bitcoincash:qq07l6rr5lsdm3m80qxw80ku2ex0tj76vvsxpvmgme"))
        XCTAssertFalse(chain.validate(address: "bc1qvtvte5tzlqlfhcdmph94lxk8jcz54q6psyvgla"))
    }

    func testExtendedKeys() {
        let wallet = HDWallet.test

        // .bip44
        let xprv = wallet.getExtendedPrivateKey(purpose: .bip44, coin: .syscoin, version: .xprv)
        let xpub = wallet.getExtendedPublicKey(purpose: .bip44, coin: .syscoin, version: .xpub)

        XCTAssertEqual(xprv, "xprv9yFNgN7z81uG6QtwFt7gvbmLeDGeGfS2ar3DunwEkZcC7uLBXyy4eaaV3ir769zMLe3eHuTaGUtWVXwp6dkunLsfmA7bf3XqEFpTjHxSijx")
        XCTAssertEqual(xpub, "xpub6CEj5sesxPTZJtyQMuehHji5CF78g89sx4xpiBLrJu9AzhfL5XHKCNtxtzPzyGxqb6jMbZfmbHeSGZZArL4hLttmdC6KEMuiWy7VocTYjzR")

        // .bip49
        let yprv = wallet.getExtendedPrivateKey(purpose: .bip49, coin: .syscoin, version: .yprv)
        let ypub = wallet.getExtendedPublicKey(purpose: .bip49, coin: .syscoin, version: .ypub)
        XCTAssertEqual(yprv, "yprvAJAofBFEEQ1DLJJVMkPr4pufHLUKZ2VSbtHqPpphEgwgfvG8exgadM8vtW8AW52N7tqU4qM8JHk5xZkq3icnzoph5QA5kRVHBnhXuRMGw2b")
        XCTAssertEqual(ypub, "ypub6XAA4gn84mZWYnNxTmvrRxrPqNJoxVDHy7DSCDEJo2UfYibHCVzqB9TQjmL2TKSEZVFmTNcmdJXunEu6oV2AFQNeiszjzcnX4nbG27s4SgS")

        // .bip84
        let zprv = wallet.getExtendedPrivateKey(purpose: .bip84, coin: .syscoin, version: .zprv)
        let zpub = wallet.getExtendedPublicKey(purpose: .bip84, coin: .syscoin, version: .zpub)
        XCTAssertEqual(zprv, "zprvAcdCiLx9ooAFnC1hXh7stnobLnnu7u25rqfLeJ9v632xdCXJrc8KvgNk2eZeQQbPQHvcUpsfJzgyDkRdfnkT6vjpYqkxFv1LsPxQ7uFwLGy")
        XCTAssertEqual(zpub, "zpub6qcZ7rV3eAiYzg6AdietFvkKtpdPXMjwE4awSgZXeNZwVzrTQ9SaUUhDswmdA4A5riyimx322es7niQvJ1Fbi3mJNSVz3d3f9GBsYBb8Wky")
    }

    func testDeriveFromXpub() {
        let xpub = "xpub6CEj5sesxPTZJtyQMuehHji5CF78g89sx4xpiBLrJu9AzhfL5XHKCNtxtzPzyGxqb6jMbZfmbHeSGZZArL4hLttmdC6KEMuiWy7VocTYjzR"
        let syscoin = CoinType.syscoin
        let xpubAddr2 = HDWallet.getPublicKeyFromExtended(
            extended: xpub,
            coin: syscoin,
            derivationPath: DerivationPath(purpose: .bip44, coin: syscoin.slip44Id, account: 0, change: 0, address: 2).description
        )!
        let xpubAddr9 = HDWallet.getPublicKeyFromExtended(
            extended: xpub,
            coin: syscoin,
            derivationPath: DerivationPath(purpose: .bip44, coin: syscoin.slip44Id, account: 0, change: 0, address: 9).description
        )!

        XCTAssertEqual(BitcoinAddress(publicKey: xpubAddr2, prefix: CoinType.syscoin.p2pkhPrefix)!.description, "SkXxaA1GQu6D49qxrjSMCgsybWVsWMZb32")
        XCTAssertEqual(BitcoinAddress(publicKey: xpubAddr9, prefix: CoinType.syscoin.p2pkhPrefix)!.description, "ST8SwH6wp6qx6RnQcnUfTJCHznDPsdatzC")
    }

    func testDeriveFromYpub() {
        let ypub = "ypub6XAA4gn84mZWYnNxTmvrRxrPqNJoxVDHy7DSCDEJo2UfYibHCVzqB9TQjmL2TKSEZVFmTNcmdJXunEu6oV2AFQNeiszjzcnX4nbG27s4SgS"

        let syscoin = CoinType.syscoin
        let ypubAddr3 = HDWallet.getPublicKeyFromExtended(
            extended: ypub,
            coin: syscoin,
            derivationPath: DerivationPath(purpose: .bip49, coin: syscoin.slip44Id, account: 0, change: 0, address: 3).description
        )!
        let ypubAddr10 = HDWallet.getPublicKeyFromExtended(
            extended: ypub,
            coin: syscoin,
            derivationPath: DerivationPath(purpose: .bip49, coin: syscoin.slip44Id, account: 0, change: 0, address: 10).description
        )!

        XCTAssertEqual(BitcoinAddress.compatibleAddress(publicKey: ypubAddr3, prefix: CoinType.syscoin.p2shPrefix).description, "31hP9bQFV1iX49yGaaz2ZwoxXzqpPx2vbk")
        XCTAssertEqual(BitcoinAddress.compatibleAddress(publicKey: ypubAddr10, prefix: CoinType.syscoin.p2shPrefix).description, "3FhBf4MUNWFiMz3RTbpKb7eie4WHhErSs5")
    }

    func testDeriveFromZPub() {
        let zpub = "zpub6qcZ7rV3eAiYzg6AdietFvkKtpdPXMjwE4awSgZXeNZwVzrTQ9SaUUhDswmdA4A5riyimx322es7niQvJ1Fbi3mJNSVz3d3f9GBsYBb8Wky"
        let syscoin = CoinType.syscoin
        let zpubAddr4 = HDWallet.getPublicKeyFromExtended(
            extended: zpub,
            coin: syscoin,
            derivationPath: DerivationPath(purpose: .bip49, coin: syscoin.slip44Id, account: 0, change: 0, address: 4).description
        )!
        let zpubAddr11 = HDWallet.getPublicKeyFromExtended(
            extended: zpub,
            coin: syscoin,
            derivationPath: DerivationPath(purpose: .bip49, coin: syscoin.slip44Id, account: 0, change: 0, address: 11).description
        )!

        XCTAssertEqual(SegwitAddress(hrp: .syscoin, publicKey: zpubAddr4).description, "sys1q54ylw7uztxagxq5hz93cmdrawthhrd00knkp23")
        XCTAssertEqual(SegwitAddress(hrp: .syscoin, publicKey: zpubAddr11).description, "sys1q7yzsja5wtkswdc6rleupsvzny3gnyhye0qvdda")
    }

    func testPlanAndSign_8435() throws {
        let address = "sys1qytnqzjknvv03jwfgrsmzt0ycmwqgl0as7lkcf7"
        let lockScript = BitcoinScript.lockScriptForAddress(address: address, coin: .syscoin)
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
            $0.toAddress = "sys1q54ylw7uztxagxq5hz93cmdrawthhrd00knkp23"
            $0.changeAddress = address
            $0.hashType = BitcoinScript.hashTypeForCoin(coinType: .syscoin)
            $0.amount = 1200000
            $0.coinType = CoinType.syscoin.rawValue
            $0.byteFee = 1
            $0.utxo = utxos
            $0.useMaxAmount = false
            $0.scripts = [
                scriptHash.hexString: BitcoinScript.buildPayToPublicKeyHash(hash: scriptHash).data
            ]
        }

        // Plan
        let plan: BitcoinTransactionPlan = AnySigner.plan(input: input, coin: .syscoin)

        XCTAssertEqual(plan.amount, 1200000)
        XCTAssertEqual(plan.fee, 141)
        XCTAssertEqual(plan.change, 2699633)

        // Extend input with private key
        input.privateKey = [Data(hexString: "55f9cbb0376c422946fa28397c1219933ac60b312ede41bfacaf701ecd546625")!]
        input.plan = plan

        // Sign
        let output: BitcoinSigningOutput = AnySigner.sign(input: input, coin: .syscoin)
        XCTAssertEqual(output.error, TW_Common_Proto_SigningError.ok)

        let txId = output.transactionID
        XCTAssertEqual(txId, "cb92bae012ebdd88b720198e40d470746d1af2e8b9b875bb763c831341cb2ded")

        let encoded = output.encoded
        XCTAssertEqual(encoded.hexString,
            "0100000000010107c42b969286be06fae38528c85f0a1ce508d4df837eb5ac4cf5f2a7a9d65fa80000000000ffffffff02804f120000000000160014a549f77b8259ba83029711638db47d72ef71b5ef713129000000000016001422e6014ad3631f1939281c3625bc98db808fbfb00247304402201fff942424755a4ecc84c916c3045c73efab03f9e13e55b27a6ecf6d2027d88602205e54d2fd728e0cfedeecb987dcb346e6e14c5b24ffb26d3db543d90f6571f7080121025cf26d221b01ca4d6040893b96f1dabfd2a108d449b3fa62854421f98a42562b00000000"
        )
    }
}
