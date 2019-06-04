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
        let data = Data(hexString: "f7b5f7a8090c5c93cd2d6d01383c9286b221ea78d8bef3e482f0c5cdde653e68")!
        let privateKey = PrivateKey(data: data)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)
        let address = BitcoinAddress.compatibleAddress(publicKey: publicKey, prefix: P2SHPrefix.bitcoin.rawValue)

        XCTAssertEqual(address.description, "3Hv6oV8BYCoocW4eqZaEXsaR5tHhCxiMSk")
    }

    func testFromPrivateKeyUncompressed() {
        let data = Data(hexString: "f7b5f7a8090c5c93cd2d6d01383c9286b221ea78d8bef3e482f0c5cdde653e68")!
        let privateKey = PrivateKey(data: data)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: false)
        let address = BitcoinAddress.compatibleAddress(publicKey: publicKey, prefix: P2SHPrefix.bitcoin.rawValue)

        XCTAssertEqual(address.description, "3Hv6oV8BYCoocW4eqZaEXsaR5tHhCxiMSk")
    }

    func testFromPrivateKeySegwitAddress() {
        let data = Data(hexString: "28071bf4e2b0340db41b807ed8a5514139e5d6427ff9d58dbd22b7ed187103a4")!
        let privateKey = PrivateKey(data: data)!
        let publicKey = privateKey.getPublicKeySecp256k1(compressed: true)

        let address = BitcoinAddress(publicKey: publicKey, prefix: P2PKHPrefix.bitcoin.rawValue)!

        XCTAssertEqual(address.description, BitcoinAddress(string: "1PeUvjuxyf31aJKX6kCXuaqxhmG78ZUdL1")!.description)
    }

    func testIsValid() {
        XCTAssertFalse(BitcoinAddress.isValidString(string: "abc"))
        XCTAssertFalse(BitcoinAddress.isValidString(string: "0x5aAeb6053F3E94C9b9A09f33669435E7Ef1BeAed"))
        XCTAssertFalse(BitcoinAddress.isValidString(string: "175tWpb8K1S7NmH4Zx6rewF9WQrcZv245W"))
        XCTAssertTrue(BitcoinAddress.isValidString(string: "1AC4gh14wwZPULVPCdxUkgqbtPvC92PQPN"))
    }

    func testCompressedPublicKey() {
        // compressed public key starting with 0x03 (greater than midpoint of curve)
        let compressedPK = PublicKey(data: Data(hexString: "030589ee559348bd6a7325994f9c8eff12bd5d73cc683142bd0dd1a17abc99b0dc")!, type: .secp256k1)!
        XCTAssertTrue(compressedPK.isCompressed)
        XCTAssertEqual(BitcoinAddress(publicKey: compressedPK, prefix: 0)!.description, "1KbUJ4x8epz6QqxkmZbTc4f79JbWWz6g37")
    }

    func testPublicKeyToSegwitAddress() {
        let publicKey = PublicKey(data: Data(hexString: "0279BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798")!, type: .secp256k1)!
        let expect = "bc1qw508d6qejxtdg4y5r3zarvary0c5xw7kv8f3t4"
        XCTAssertTrue(SegwitAddress.isValidString(string: expect))

        let address = SegwitAddress(hrp: .bitcoin, publicKey: publicKey)
        XCTAssertEqual(address.description, expect)

        let addressFromString = SegwitAddress(string: expect)
        XCTAssertEqual(address.description, addressFromString?.description)
    }

    func testInvalidSegwitAddresses() {
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
            XCTAssertFalse(SegwitAddress.isValidString(string: invalid), "'\(invalid)' should not be a valid Bech32 address")
        }
    }

    func testValidDigiByteAddress() {
        let addressString = "DTPQ92zp96TwpG2pRuUB3oEA3kWGRZPGhg"

        XCTAssertEqual(P2PKHPrefix.d.rawValue, BitcoinAddress(string: addressString)?.prefix)
        XCTAssertTrue(BitcoinAddress.isValidString(string: addressString),
                      "'\(addressString)' should be a valid DigiByte address")

        let addressString2 = "SUngTA1vaC2E62mbnc81Mdos3TcvZHwsVo"

        XCTAssertEqual(P2SHPrefix.s.rawValue, BitcoinAddress(string: addressString2)?.prefix)
        XCTAssertTrue(BitcoinAddress.isValidString(string: addressString2),
                      "'\(addressString2)' should be a valid DigiByte address")

        let addressString3 = "dgb1qtjgmerfqwdffyf8ghcrkgy52cghsqptynmyswu"

        XCTAssertEqual(HRP.digiByte, SegwitAddress(string: addressString3)?.hrp)
        XCTAssertTrue(SegwitAddress.isValidString(string: addressString3),
                      "'\(addressString3)' should be a valid DigiByte Bech32 address")
    }

    func testInvalidDigiByteAddress() {
        let addressString = "DTPQ92zp96TwpG2pRuUB3oEA3kWGRZPGhX"

        XCTAssertNil(BitcoinAddress(string: addressString)?.prefix)
        XCTAssertFalse(BitcoinAddress.isValidString(string: addressString),
                      "'\(addressString)' should be an invalid DigiByte address")

        let addressString2 = "SUngTA1vaC2E62mbnc81Mdos3TcvZHwsVX"

        XCTAssertNil(BitcoinAddress(string: addressString2)?.prefix)
        XCTAssertFalse(BitcoinAddress.isValidString(string: addressString2),
                      "'\(addressString2)' should be an invalid DigiByte address")

        let addressString3 = "xgb1qtjgmerfqwdffyf8ghcrkgy52cghsqptynmyswu"

        XCTAssertNil(SegwitAddress(string: addressString3)?.hrp)
        XCTAssertFalse(SegwitAddress.isValidString(string: addressString3),
                      "'\(addressString3)' should be an invalid DigiByte Bech32 address")
    }

    func testValidRavencoinAddress() {
        let addressString = "RHoCwPc2FCQqwToYnSiAb3SrCET4zEHsbS"

        XCTAssertEqual(P2PKHPrefix.ravencoin.rawValue, BitcoinAddress(string: addressString)?.prefix)
        XCTAssertTrue(BitcoinAddress.isValidString(string: addressString),
                      "'\(addressString)' should be a valid Ravencoin address")

        let addressString2 = "rPWwn5h4QFZNaz1XmY39rc73sdYGGDdmq1"

        XCTAssertEqual(P2SHPrefix.ravencoin.rawValue, BitcoinAddress(string: addressString2)?.prefix)
        XCTAssertTrue(BitcoinAddress.isValidString(string: addressString2),
                      "'\(addressString2)' should be a valid Ravencoin address")

        // testnet address
        let addressString3 = "mwJAu1BWcRSQhepZ71wiGoSwsD6hnB5B7G"

        XCTAssertTrue(BitcoinAddress.isValidString(string: addressString3),
                       "'\(addressString3)' should be a valid Ravencoin testnet address")
    }

    func testInvalidRavencoinAddress() {
        // bad address
        let addressString = "XHoCwPc2FCQqwToYnSiAb3SrCET4zEHsbS"

        XCTAssertNil(BitcoinAddress(string: addressString)?.prefix)
        XCTAssertFalse(BitcoinAddress.isValidString(string: addressString),
                      "'\(addressString)' should be an invalid Ravencoin address")

        // testnet address
        let addressString2 = "mwJAu1BWcRSQhepZ71wiGoSwsD6hnB5B7G"

        XCTAssertFalse(CoinType.ravencoin.validate(address: addressString2),
                      "'\(addressString2)' should be an invalid Ravencoin address")

    }
}
