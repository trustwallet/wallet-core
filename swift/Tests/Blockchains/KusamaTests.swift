// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import TrustWalletCore
import XCTest

class KusamaTests: XCTestCase {

    func testAddressValidation() {
        let kusama = CoinType.kusama
        // polkadot sr25519
        XCTAssertFalse(kusama.validate(address: "14PhJGbzPxhQbiq7k9uFjDQx3MNiYxnjFRSiVBvBBBfnkAoM"))
        // cosmos
        XCTAssertFalse(kusama.validate(address: "cosmos1l4f4max9w06gqrvsxf74hhyzuqhu2l3zyf0480"))
        // Bitcoin p2sh
        XCTAssertFalse(kusama.validate(address: "3317oFJC9FvxU2fwrKVsvgnMGCDzTZ5nyf"))

        XCTAssertTrue(kusama.validate(address: "ELmaX1aPkyEF7TSmYbbyCjmSgrBpGHv9EtpwR2tk1kmpwvG"))
    }

    func testAddress() {
        let key = PrivateKey(data: Data(hexString: "0x85fca134b3fe3fd523d8b528608d803890e26c93c86dc3d97b8d59c7b3540c97")!)!
        let pubkey = key.getPublicKeyEd25519()
        let address = KusamaAddress(publicKey: pubkey)
        let addressFromString = KusamaAddress(string: "HewiDTQv92L2bVtkziZC8ASxrFUxr6ajQ62RXAnwQ8FDVmg")!

        XCTAssertEqual(pubkey.data.hexString, "e0b3fcccfe0283cc0f8c105c68b5690aab8c5c1692a868e55eaca836c8779085")
        XCTAssertEqual(address.description, addressFromString.description)
        XCTAssertEqual(address.publicKey.hexString, pubkey.data.hexString)
    }

    func testStorageKey() {
        func generateStorageKey(module: String, function: String, publicKey: Data) -> Data {
            var data = Data()
            data.append(Hash.twoXXHash64Concat(data: module.data(using: .utf8)!))
            data.append(Hash.twoXXHash64Concat(data: function.data(using: .utf8)!))
            data.append(Hash.blake2b(data: publicKey, size: 32))
            return data
        }

        let address = KusamaAddress(string: "HKtMPUSoTC8Hts2uqcQVzPAuPRpecBt4XJ5Q1AT1GM3tp2r")!
        let key = generateStorageKey(module: "Balances", function: "FreeBalance", publicKey: address.publicKey)
        XCTAssertEqual(key.hexString, "c2261276cc9d1f8598ea4b6a74b15c2f6482b9ade7bc6657aaca787ba1add3b4c801483fa04e8fd48dc5c5675891cfaab709696db6de3184d95d26a1c894f1f8")
    }

    func testSigningTransfer() {
        let key = PrivateKey(data: Data(hexString: "0xabf8e5bdbe30c65656c0a3cbd181ff8a56294a69dfedd27982aace4a76909115")!)!
        let address = CoinType.kusama.deriveAddress(privateKey: key)

        XCTAssertEqual(address.description, "FfmSiZNJP72xtSaXiP2iUhBwWeMEvmjPrxY2ViVkWaeChDC")

        let genesisHash = Data(hexString: "0xb0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe")!
        let input = PolkadotSigningInput.with {
            $0.genesisHash = genesisHash
            $0.blockHash = genesisHash
            $0.nonce = 0
            $0.specVersion = 1031
            $0.balanceCall = PolkadotBalance.with {
                $0.transfer = PolkadotBalance.Transfer.with {
                    $0.toAddress = "FoQJpPyadYccjavVdTWxpxU7rUEaYhfLCPwXgkfD6Zat9QP"
                    $0.value = Data(hexString: "3039")! // 12345
                }
            }
            $0.network = .kusama
            $0.extrinsicVersion = 4
            $0.privateKey = key.data
        }
        let output = PolkadotSigner.sign(input: input)

        XCTAssertEqual(output.encoded.hexString, "2d0284ff88dc3417d5058ec4b4503e0c12ea1a0a89be200fe98922423d4334014fa6b0ee0034a113577b56545c45e18969471eebe11ed434f3b2f06e2e3dc8dc137ba804caf60757787ebdeb298327e2f29d68c5520965405ef5582db0445c06e1c11a8a0e0000000400ff8eaf04151687736326c9fea17e25fc5287613693c912909cb226aa4794f26a48e5c0")
    }

    func testSigningTransfer2() {
        // https://kusama.subscan.io/extrinsic/0x20cfbba19817e4b7a61e718d269de47e7067a24860fa978c2a8ead4c96a827c4
        // 1p test wallet
        let wallet = HDWallet.test
        let key = wallet.getKeyForCoin(coin: .kusama)

        let genesisHash = Data(hexString: "0xb0a8d493285c2df73290dfb7e61f870f17b41801197a149ca93654499ea3dafe")!
        let input = PolkadotSigningInput.with {
            $0.genesisHash = genesisHash
            $0.blockHash = genesisHash
            $0.nonce = 0
            $0.specVersion = 1031
            $0.balanceCall = PolkadotBalance.with {
                $0.transfer = PolkadotBalance.Transfer.with {
                    $0.toAddress = "CtwdfrhECFs3FpvCGoiE4hwRC4UsSiM8WL899HjRdQbfYZY"
                    // 10000000000 ~ 0.01 KSM
                    $0.value = Data(hexString: "0x02540be400")!
                }
            }
            $0.network = .kusama
            $0.extrinsicVersion = 4
            $0.privateKey = key.data
        }
        let output = PolkadotSigner.sign(input: input)

        XCTAssertEqual(output.encoded.hexString, "3d0284fff41296779fd61a5bed6c2f506cc6c9ea93d6aeb357b9c69717193f434ba24ae70043e0fe7497f1d11ca6635b7860ef9551d395172b18af22c16e375326326f524cd32ffafb3e1e73112f016a8028c50eebb608df29523751a11147e36a49f2d40a0000000400ff0e33fdfb980e4499e5c3576e742a563b6a4fc0f6f598b1917fd7a6fe393ffc720700e40b5402")
    }
}
