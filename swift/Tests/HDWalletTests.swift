// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

let testWallet = HDWallet(mnemonic: "ripple scissors kick mammal hire column oak again sun offer wealth tomorrow wagon turn fatal", passphrase: "TREZOR")

class HDWalletTests: XCTestCase {
    func testSeed() {
        let wallet = testWallet

        XCTAssertEqual(wallet.seed.hexString, "7ae6f661157bda6492f6162701e570097fc726b6235011ea5ad09bf04986731ed4d92bc43cbdee047b60ea0dd1b1fa4274377c9bf5bd14ab1982c272d8076f29")
    }

    func testSeedNoPassword() {
        let wallet = HDWallet(mnemonic: testWallet.mnemonic, passphrase: "")

        XCTAssertEqual(wallet.seed.hexString, "354c22aedb9a37407adc61f657a6f00d10ed125efa360215f36c6919abd94d6dbc193a5f9c495e21ee74118661e327e84a5f5f11fa373ec33b80897d4697557d")
    }

    func testDerive() {
        let wallet = testWallet
        let key0 = wallet.getKey(at: Ethereum().derivationPath(at: 0))
        let key1 = wallet.getKey(at: Ethereum().derivationPath(at: 1))

        XCTAssertEqual(EthereumAddress(publicKey: key0.getPublicKey(compressed: false)).description, "0x27Ef5cDBe01777D62438AfFeb695e33fC2335979")
        XCTAssertEqual(EthereumAddress(publicKey: key1.getPublicKey(compressed: false)).description, "0x98f5438cDE3F0Ff6E11aE47236e93481899d1C47")
    }

    func testWanchain() {
        let blockchain = Wanchain()
        let wallet = testWallet
        let key0 = wallet.getKey(at: blockchain.derivationPath(at: 0))

        XCTAssertEqual(blockchain.address(for: key0.getPublicKey(compressed: false)).description, "0x4DDa26870B4b3fa3FbA32222159359038f588318")
    }

    func testDeriveBitcoin() {
        let blockchain = Bitcoin()
        let wallet = testWallet
        let key = wallet.getKey(at: blockchain.derivationPath(at: 0))
        let address = blockchain.address(for: key.getPublicKey(compressed: false))

        XCTAssertEqual("bc1qumwjg8danv2vm29lp5swdux4r60ezptzz7ce85", address.description)
        XCTAssertEqual(key.getPublicKey(compressed: true).description, key.getPublicKey(compressed: true).description)
    }

    func testDeriveLitecoin() {
        let blockchain = Litecoin()
        let wallet = testWallet
        let key = wallet.getKey(at: blockchain.derivationPath(at: 0))
        let address = blockchain.address(for: key.getPublicKey(compressed: false))

        XCTAssertEqual("ltc1qmj6hw649d7q2teduv599zv9ls9akz60gkdwnp7", address.description)
    }

    func testDeriveTron() {
        let blockchain = Tron()
        let wallet = testWallet
        let key = wallet.getKey(at: blockchain.derivationPath(at: 0))
        let address = blockchain.address(for: key.getPublicKey(compressed: false))

        XCTAssertEqual("THJrqfbBhoB1vX97da6S6nXWkafCxpyCNB", address.description)
    }

    func testDeriveIcon() {
        let blockchain = Icon()
        let wallet = testWallet
        let key0 = wallet.getKey(at: blockchain.derivationPath(at: 0))
        let key1 = wallet.getKey(at: blockchain.derivationPath(at: 1))
        let address0 = blockchain.address(for: key0.getPublicKey(compressed: false))
        let address1 = blockchain.address(for: key1.getPublicKey(compressed: false))

        XCTAssertEqual("hx78c6f744c68d48793cd64716189c181c66907b24", address0.description)
        XCTAssertEqual("hx92373c16531761b31a7124c94718da43db8c9d89", address1.description)
    }

    func testDeriveEOS() {
        let blockchain = EOS()
        let wallet = testWallet
        let key = wallet.getKey(at: blockchain.derivationPath(at: 0))
        let address = blockchain.address(for: key.getPublicKey(compressed: true))

        XCTAssertEqual("EOS5LhLzbYV9jL94MYSFFnuKUX4fSB9xRp2zmXBxZ9AjmbAwZKxoq", address.description)
    }

    func testDeriveBitcoinCash() {
        let blockchain = BitcoinCash()
        let wallet = testWallet
        let key = wallet.getKey(at: blockchain.derivationPath(at: 0))
        let address = blockchain.address(for: key.getPublicKey(compressed: true))

        XCTAssertEqual("bitcoincash:qqxktqe0pzf0yepvap9rf2g8zxq8t5mqx50dwpqlxl", address.description)
    }

    func testDeriveDash() {
        let blockchain = Dash()
        let wallet = testWallet
        let key = wallet.getKey(at: blockchain.derivationPath(at: 0))
        let address = blockchain.address(for: key.getPublicKey(compressed: false))

        XCTAssertEqual("XsJg3pJaoyKEf5jMYTb5wGf3TKp9W3KX5a", address.description)
    }

    func testDeriveZcoin() {
        let blockchain = Zcoin()
        let wallet = testWallet
        let key = wallet.getKey(at: blockchain.derivationPath(at: 0))
        let address = blockchain.address(for: key.getPublicKey(compressed: false))

        XCTAssertEqual("a5jgmKczLE7fbgBmVkDTvvAQx8pYZKL7LP", address.description)
    }

    func testDeriveBinanceChain() {
        let blockchain = BinanceChain()
        let wallet = testWallet
        let key = wallet.getKey(at: blockchain.derivationPath(at: 0))
        let address = blockchain.address(for: key.getPublicKey(compressed: false))

        XCTAssertEqual("bnb1wk7kxw0qrvxe2pj9mk6ydjx0t4j9jla8pja0td", address.description)
    }

    func testDeriveZcash() {
        let blockchain = Zcash()
        let wallet = testWallet
        let key = wallet.getKey(at: blockchain.derivationPath(at: 0))
        let address = blockchain.address(for: key.getPublicKey(compressed: false))
        XCTAssertEqual("t1RygJmrLdNGgi98gUgEJDTVaELTAYWoMBy", address.description)
    }

    func testDeriveRipple() {
        let blockchain = Ripple()
        let wallet = testWallet
        let key = wallet.getKey(at: blockchain.derivationPath(at: 0))
        let address = blockchain.address(for: key.getPublicKey(compressed: false))

        XCTAssertEqual("r36yxStAh7qgTQNHTzjZvXybCTzUFhrfav", address.description)
    }
    func testSignHash() {
        let wallet = testWallet
        let key = wallet.getKey(at: Ethereum().derivationPath(at: 0))
        let hash = Data(hexString: "3F891FDA3704F0368DAB65FA81EBE616F4AA2A0854995DA4DC0B59D2CADBD64F")!
        let result = key.sign(digest: hash)!

        let publicKey = key.getPublicKey(compressed: false)
        XCTAssertEqual(result.count, 65)
        XCTAssertTrue(publicKey.verify(signature: result, message: hash))
    }

    func testExtendedKeys() {
        let wallet = HDWallet(mnemonic: "abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon abandon about", passphrase: "")

        let xprv = wallet.getExtendedPrivateKey(curve: .secp256k1, purpose: .bip44, coin: .bitcoin, version: .xprv)
        let xpub = wallet.getExtendedPubKey(curve: .secp256k1, purpose: .bip44, coin: .bitcoin, version: .xpub)

        XCTAssertEqual(xprv, "xprv9xpXFhFpqdQK3TmytPBqXtGSwS3DLjojFhTGht8gwAAii8py5X6pxeBnQ6ehJiyJ6nDjWGJfZ95WxByFXVkDxHXrqu53WCRGypk2ttuqncb")
        XCTAssertEqual(xpub, "xpub6BosfCnifzxcFwrSzQiqu2DBVTshkCXacvNsWGYJVVhhawA7d4R5WSWGFNbi8Aw6ZRc1brxMyWMzG3DSSSSoekkudhUd9yLb6qx39T9nMdj")

        let yprv = wallet.getExtendedPrivateKey(curve: .secp256k1, purpose: .bip49, coin: .bitcoin, version: .yprv)
        let ypub = wallet.getExtendedPubKey(curve: .secp256k1, purpose: .bip49, coin: .bitcoin, version: .ypub)

        XCTAssertEqual(yprv, "yprvAHwhK6RbpuS3dgCYHM5jc2ZvEKd7Bi61u9FVhYMpgMSuZS613T1xxQeKTffhrHY79hZ5PsskBjcc6C2V7DrnsMsNaGDaWev3GLRQRgV7hxF")
        XCTAssertEqual(ypub, "ypub6Ww3ibxVfGzLrAH1PNcjyAWenMTbbAosGNB6VvmSEgytSER9azLDWCxoJwW7Ke7icmizBMXrzBx9979FfaHxHcrArf3zbeJJJUZPf663zsP")

        let zprv = wallet.getExtendedPrivateKey(curve: .secp256k1, purpose: .bip84, coin: .bitcoin, version: .zprv)
        let zpub = wallet.getExtendedPubKey(curve: .secp256k1, purpose: .bip84, coin: .bitcoin, version: .zpub)

        XCTAssertEqual(zprv, "zprvAdG4iTXWBoARxkkzNpNh8r6Qag3irQB8PzEMkAFeTRXxHpbF9z4QgEvBRmfvqWvGp42t42nvgGpNgYSJA9iefm1yYNZKEm7z6qUWCroSQnE")
        XCTAssertEqual(zpub, "zpub6rFR7y4Q2AijBEqTUquhVz398htDFrtymD9xYYfG1m4wAcvPhXNfE3EfH1r1ADqtfSdVCToUG868RvUUkgDKf31mGDtKsAYz2oz2AGutZYs")
    }

    func testDeriveFromXPub() {
        let xpub = "xpub6BosfCnifzxcFwrSzQiqu2DBVTshkCXacvNsWGYJVVhhawA7d4R5WSWGFNbi8Aw6ZRc1brxMyWMzG3DSSSSoekkudhUd9yLb6qx39T9nMdj"
        let bc = Bitcoin()
        let xpubAddr2 = bc.derive(from: xpub, at: bc.derivationPath(at: 2))!
        let xpubAddr9 = bc.derive(from: xpub, at: bc.derivationPath(at: 9))!

        XCTAssertEqual(xpubAddr2.description, "bc1qmansqj24utny54uag2ped8censfwnszplhg27m")
        XCTAssertEqual(xpubAddr9.description, "bc1q3rn9d0vknmnlk5tpjhwqvxgz0txnu4wzup72mm")
    }

    func testDeriveFromYPub() {
        let ypub = "ypub6Ww3ibxVfGzLrAH1PNcjyAWenMTbbAosGNB6VvmSEgytSER9azLDWCxoJwW7Ke7icmizBMXrzBx9979FfaHxHcrArf3zbeJJJUZPf663zsP"
        let bc = Bitcoin()
        let ypubAddr3 = bc.derive(from: ypub, at: bc.derivationPath(at: 3))!
        let ypubAddr10 = bc.derive(from: ypub, at: bc.derivationPath(at: 10))!

        XCTAssertEqual(ypubAddr3.description, "bc1qfd52at6ap7am8uc4gpk9fcmyayq4ydcenh0l35")
        XCTAssertEqual(ypubAddr10.description, "bc1q0fkafre8qg4yd2v3054eugp784zs2jgmutcgma")
    }

    func testDeriveFromZPub() {
        let zpub = "zpub6rFR7y4Q2AijBEqTUquhVz398htDFrtymD9xYYfG1m4wAcvPhXNfE3EfH1r1ADqtfSdVCToUG868RvUUkgDKf31mGDtKsAYz2oz2AGutZYs"
        let bc = Bitcoin()
        let zpubAddr4 = bc.derive(from: zpub, at: bc.derivationPath(at: 4))!
        let zpubAddr11 = bc.derive(from: zpub, at: bc.derivationPath(at: 11))!

        XCTAssertEqual(zpubAddr4.description, "bc1qm97vqzgj934vnaq9s53ynkyf9dgr05rargr04n")
        XCTAssertEqual(zpubAddr11.description, "bc1qxr4fjkvnxjqphuyaw5a08za9g6qqh65t8qwgum")
    }

    func testDeriveFromZPub2() {
        let zpub = "zpub6qeA5j9oSq8tZaYEBTp1X61ZSjeen6HbiUBSG4KLPD8d65Pi7eSMPNuxCqgbLdtnim2hgnJEzmE6jhFoJXtJdRxRKRdNFQBJ6iidx9BHGyk"

        let bc = Bitcoin()
        let pubkey = bc.derivePubkey(from: zpub, at: bc.derivationPath(at: 0))!
        let address = bc.address(for: pubkey)
        let address1 = bc.derive(from: zpub, at: bc.derivationPath(at: 0))

        XCTAssertEqual(pubkey.data.hexString, "039fdd3652495d01b6a363f8db8b3adce09f83ea5c43ff872ad0a39192340256b0")
        XCTAssertEqual(address.description, "bc1qearv5ezm3xfgy2t98denkzxwst4f35fvz608wa")
        XCTAssertEqual(address.description, address1?.description)
    }
}
