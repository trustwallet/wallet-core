// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class BitcoinAddressTests: XCTestCase {
    func testInvalid() {
        XCTAssertNil(BitcoinAddress(string: "abc"))
        XCTAssertNil(BitcoinAddress(string: "0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed"))
        XCTAssertNil(BitcoinAddress(string: "175tWpb8K1S7NmH4Zx6rewF9WQrcZv245W"))
    }

    func testInitWithString() {
        let address = BitcoinAddress(string: "1AC4gh14wwZPULVPCdxUkgqbtPvC92PQPN")

        XCTAssertNotNil(address)
        XCTAssertEqual(address!.description, "1AC4gh14wwZPULVPCdxUkgqbtPvC92PQPN")
    }

    func testFromPrivateKey() {
        let privateKey = PrivateKey(wif: "L5XECLxq1MDvBeYXjZwz5tTYsFZRWmaYziY3Wvc2bqSRAuRcBqhg")!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
        let address = Bitcoin().compatibleAddress(for: publicKey)

        XCTAssertEqual(address.description, "3Hv6oV8BYCoocW4eqZaEXsaR5tHhCxiMSk")
    }

    func testFromPrivateKeyUncompressed() {
        let privateKey = PrivateKey(wif: "L5XECLxq1MDvBeYXjZwz5tTYsFZRWmaYziY3Wvc2bqSRAuRcBqhg")!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: false)
        let address = Bitcoin().compatibleAddress(for: publicKey)

        XCTAssertEqual(address.description, "3Hv6oV8BYCoocW4eqZaEXsaR5tHhCxiMSk")
    }

    func testFromPrivateKeySegwitAddress() {
        let privateKey = PrivateKey(wif: "KxZX6Jv3to6RWnhsffTcLLryRnNyyc8Ng2G8P9LFkbCdzGDEhNy1")!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
        let address = Bitcoin().legacyAddress(for: publicKey)

        XCTAssertEqual(address.description, Bitcoin().address(string: "1PeUvjuxyf31aJKX6kCXuaqxhmG78ZUdL1")!.description)
    }

    func testFromSewgitPrivateKey() {
        let privateKey = PrivateKey(wif: "L5XECLxq1MDvBeYXjZwz5tTYsFZRWmaYziY3Wvc2bqSRAuRcBqhg")!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
        let address = Bitcoin().compatibleAddress(for: publicKey)

        XCTAssertEqual(address.description, Bitcoin().address(string: "3Hv6oV8BYCoocW4eqZaEXsaR5tHhCxiMSk")!.description)
    }

    func testIsValid() {
        XCTAssertFalse(BitcoinAddress.isValidString(string: "abc"))
        XCTAssertFalse(BitcoinAddress.isValidString(string: "0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed"))
        XCTAssertFalse(BitcoinAddress.isValidString(string: "175tWpb8K1S7NmH4Zx6rewF9WQrcZv245W"))
        XCTAssertTrue(BitcoinAddress.isValidString(string: "1AC4gh14wwZPULVPCdxUkgqbtPvC92PQPN"))
    }

    func testCompressedPublicKey() {
        // compressed public key starting with 0x03 (greater than midpoint of curve)
        let compressedPK = PublicKey(data: Data(hexString: "030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc")!)!
        XCTAssertTrue(compressedPK.isCompressed)
        XCTAssertEqual(BitcoinAddress(publicKey: compressedPK, prefix: 0).description, "1KbUJ4x8epz6QqxkmZbTc4f79JbWWz6g37")
    }

    func testPublicKeyToBech32Address() {
        let publicKey = PublicKey(data: Data(hexString: "0279BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798")!)!
        let bitcoin = Bitcoin()
        let expect = "bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t4"
        XCTAssertTrue(Bech32Address.isValidString(string: expect))

        let address = bitcoin.address(for: publicKey)
        XCTAssertEqual(address.description, expect)

        let addressFromString = Bech32Address(string: expect)
        XCTAssertEqual(address as? Bech32Address, addressFromString)
    }

    func testInvalidBech32Addresses() {
        let addresses = [
            "bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t5",
            "bc1rw5uspcuh",
            "bc10w508d6qejxtdg4y5r3zarvary0c5xw7kw508d6qejxtdg4y5r3zarvary0c5xw7kw5rljs90",
            "tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3q0sL5k7",
            "bc1zw508d6qejxtdg4y5r3zarvaryvqyzf3du",
            "tb1qrp33g0q5c5txsp9arysrx4k6zdkfs4nce4xj0gdcccefvpysxf3pjxtptv",
            "bc1gmk9yu",
        ]

        for invalid in addresses {
            XCTAssertFalse(Bech32Address.isValidString(string: invalid), "'\(invalid)' should not be a valid Bech32 address")
        }
    }
}
