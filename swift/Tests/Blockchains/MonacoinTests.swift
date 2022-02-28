// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class MonacoinTests: XCTestCase {
    func testAddress() {
        let privateKey1 = PrivateKey(data: Data(hexString: "a22ddec5c567b4488bb00f69b6146c50da2ee883e2c096db098726394d585730")!)!
        let publicKey1 = privateKey1.getPublicKeySecp256k1(compressed: true)

        let legacyAddress = BitcoinAddress(publicKey: publicKey1, prefix: CoinType.monacoin.p2pkhPrefix)!
        XCTAssertEqual(BitcoinAddress(string: "MHnYTL9e1s8zNR2qzzJ3mMHfgjnUzyMscd")!.description, legacyAddress.description)

        let privateKey2 = PrivateKey(data: Data(hexString: "f6ee7e6c9bd2f4dc8f0db0dc4679de06c998afc42d825edf7966dd4488b0aa1f")!)!
        let publicKey2 = privateKey2.getPublicKeySecp256k1(compressed: true)
        let compatibleAddress = BitcoinAddress.compatibleAddress(publicKey: publicKey2, prefix: CoinType.monacoin.p2shPrefix)
        XCTAssertEqual(BitcoinAddress(string: "P9LUcYCEoMZEFuShhCHZcS8YSCEtGsMQ7u")!.description, compatibleAddress.description)

        let privateKey3 = PrivateKey(data: Data(hexString: "55f9cbb0376c422946fa28397c1219933ac60b312ede41bfacaf701ecd546625")!)!
        let publicKey3 = privateKey3.getPublicKeySecp256k1(compressed: true)
        let bech32Address = SegwitAddress(hrp: .monacoin, publicKey: publicKey3)
        XCTAssertEqual(SegwitAddress(string: "mona1qytnqzjknvv03jwfgrsmzt0ycmwqgl0asju3qmd")!.description, bech32Address.description)
    }

    func testMonacoinBlockchain() {
        let chain = CoinType.monacoin
        XCTAssertTrue(chain.validate(address: "mona1qytnqzjknvv03jwfgrsmzt0ycmwqgl0asju3qmd"))
        XCTAssertTrue(chain.validate(address: "MHnYTL9e1s8zNR2qzzJ3mMHfgjnUzyMscd"))
        XCTAssertTrue(chain.validate(address: "P9LUcYCEoMZEFuShhCHZcS8YSCEtGsMQ7u"))
        XCTAssertFalse(chain.validate(address: "Xm1iDLBP5tdxTxc6t7uJBCVjC4L2A5vB2J"))
        XCTAssertFalse(chain.validate(address: "bitcoincash:qq07l6rr5lsdm3m80qxw80ku2ex0tj76vvsxpvmgme"))
        XCTAssertFalse(chain.validate(address: "bc1qvtvte5tzlqlfhcdmph94lxk8jcz54q6psyvgla"))
    }

    func testExtendedKeys() {
        let wallet = HDWallet.test

        // .bip44
        let xprv = wallet.getExtendedPrivateKey(purpose: .bip44, coin: .monacoin, version: .xprv)
        let xpub = wallet.getExtendedPublicKey(purpose: .bip44, coin: .monacoin, version: .xpub)

        XCTAssertEqual(xprv, "xprv9ySV9MzJnFMm7bi5qVTcqva3oDJojNsMYuASBcj9yC4QRe4kehWYeGfUVYFiJQipELCHmiutHJVxosR7eSovWHrWCUTdhf49D1N7MGKVcUZ")
        XCTAssertEqual(xpub, "xpub6CRqYsXCccv4L5nYwWzdD4WnMF9J8qbCv862z18mXXbPJSPuCEpoC4yxLn5N81V5GWNuBsJ8g6tYsBb1V9hCipGn4WR6bc9FLUtyyRvDFse")

        // .bip49
        let yprv = wallet.getExtendedPrivateKey(purpose: .bip49, coin: .monacoin, version: .yprv)
        let ypub = wallet.getExtendedPublicKey(purpose: .bip49, coin: .monacoin, version: .ypub)
        XCTAssertEqual(yprv, "yprvAJL1swHSWeFvfKdupcrsAvTCrtAiKUhLrUjteTj8JGCzD53YrJgiVbsChMPi5h119cn5tVVk1QAFSJtcnMSSxjGCNDXg8YaWbe4Hhc5bUfL")
        XCTAssertEqual(ypub, "ypub6XKNHSpLM1pDsoiNvePsY4PwQv1CiwRCDhfVSr8jrbjy5sNhPqzy3QBgYdCayJhq5st63XZTWrea8So84QYbPgP2EvVR5dhSrW18ud4GZaT")

        // .bip84
        let zprv = wallet.getExtendedPrivateKey(purpose: .bip84, coin: .monacoin, version: .zprv)
        let zpub = wallet.getExtendedPublicKey(purpose: .bip84, coin: .monacoin, version: .zpub)
        XCTAssertEqual(zprv, "zprvAdi4KQxm5ofVJqh1Y5KxSTuC1CSEibkz3Ei4dnNMzyLt6FiW3rMPTgKUCiwv1cw4rVeDW9ju82ChFz27UNG2kxvtFsngJCYtHjDYURsefzX")
        XCTAssertEqual(zpub, "zpub6rhQivVevBDnXKmUe6rxobqvZEGj84UqQTdfSAmyZJsry43ebPfe1Udx3zUNDB3cHu2ozNCDhsy8BuNCjCvStNmodzdR2E2wWAsLyNFu5i1")
    }

    func testDeriveFromXpub() {
        let xpub = "xpub6CYWFE1BgTCW2vtbDm1RRT81i3hBkQrXCfGs5hYp211fpgLZV5xCEwXMWPAL3LgaBA9koXpLZSUo7rTyJ8q1JwqKhvzVpdzBKRGyyGb31KF"
        let monacoin = CoinType.monacoin
        let xpubAddr2 = HDWallet.getPublicKeyFromExtended(
            extended: xpub,
            coin: monacoin,
            derivationPath: DerivationPath(purpose: .bip44, coin: monacoin.slip44Id, account: 0, change: 0, address: 2).description
        )!
        let xpubAddr9 = HDWallet.getPublicKeyFromExtended(
            extended: xpub,
            coin: monacoin,
            derivationPath: DerivationPath(purpose: .bip44, coin: monacoin.slip44Id, account: 0, change: 0, address: 9).description
        )!

        XCTAssertEqual(BitcoinAddress(publicKey: xpubAddr2, prefix: CoinType.monacoin.p2pkhPrefix)!.description, "MCoYzbqdsMYTBbjr7rd2zJsSF32QMgZCSj")
        XCTAssertEqual(BitcoinAddress(publicKey: xpubAddr9, prefix: CoinType.monacoin.p2pkhPrefix)!.description, "MAtduu1Fvtv1Frx6vbg5tZDZwirCA3y8qq")
    }

    func testDeriveFromYpub() {
        let ypub = "ypub6YKchgn8hmHJ9a1c2wy1ydge6ez5AcWBVSwURTnC93yj6MT1tCUN3qvuZZPsA1CwZVh5qEGhMWhDZEK43jQqWtHBzME91ws9KD6WU9n8Nau"

        let monacoin = CoinType.monacoin
        let ypubAddr3 = HDWallet.getPublicKeyFromExtended(
            extended: ypub,
            coin: monacoin,
            derivationPath: DerivationPath(purpose: .bip49, coin: monacoin.slip44Id, account: 0, change: 0, address: 3).description
        )!
        let ypubAddr10 = HDWallet.getPublicKeyFromExtended(
            extended: ypub,
            coin: monacoin,
            derivationPath: DerivationPath(purpose: .bip49, coin: monacoin.slip44Id, account: 0, change: 0, address: 10).description
        )!

        XCTAssertEqual(BitcoinAddress.compatibleAddress(publicKey: ypubAddr3, prefix: CoinType.monacoin.p2shPrefix).description, "PRAnwctxh9UWFdjCcrQy2Ym1SxMgcjTpRx")
        XCTAssertEqual(BitcoinAddress.compatibleAddress(publicKey: ypubAddr10, prefix: CoinType.monacoin.p2shPrefix).description, "PNA4qYzxsVfFXQ3bBSfMhVqumZHAJVZAaQ")
    }

    func testDeriveFromZPub() {
        let zpub = "zpub6rPmNCEpXnLtvBTZyCWnJDr6QVyBaELfVX6kQeAXtZEAFLRCzWEBc2V35UHUQKJh1SpSNCtAtCx8KhRg5AWFnKrMCsxX4J2Zee21FQ5YS4n"
        let monacoin = CoinType.monacoin
        let zpubAddr4 = HDWallet.getPublicKeyFromExtended(
            extended: zpub,
            coin: monacoin,
            derivationPath: DerivationPath(purpose: .bip84, coin: monacoin.slip44Id, account: 0, change: 0, address: 4).description
        )!
        let zpubAddr11 = HDWallet.getPublicKeyFromExtended(
            extended: zpub,
            coin: monacoin,
            derivationPath: DerivationPath(purpose: .bip84, coin: monacoin.slip44Id, account: 0, change: 0, address: 11).description
        )!

        XCTAssertEqual(SegwitAddress(hrp: .monacoin, publicKey: zpubAddr4).description, "mona1qkrylcw85ulyrar9wt35huvpu6hlqwfg2gxf523")
        XCTAssertEqual(SegwitAddress(hrp: .monacoin, publicKey: zpubAddr11).description, "mona1qulanqvye6gmsf03m0cahr8dwtmj8gy53y8rc6n")
    }
}
