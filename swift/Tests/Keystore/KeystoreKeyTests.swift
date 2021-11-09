// Copyright © 2017-2018 Trust.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class KeystoreKeyTests: XCTestCase {
    func testReadWallet() {
        let url = Bundle(for: type(of: self)).url(forResource: "key", withExtension: "json")!
        let key = StoredKey.load(path: url.path)!

        XCTAssertEqual(key.identifier, "e13b209c-3b2f-4327-bab0-3bef2e51630d")
    }

    func testReadMyEtherWallet() {
        let url = Bundle(for: type(of: self)).url(forResource: "myetherwallet", withExtension: "uu")!

        XCTAssertNotNil(StoredKey.load(path: url.path))
    }

    func testInvalidPassword() {
        let url = Bundle(for: type(of: self)).url(forResource: "key", withExtension: "json")!
        let key = StoredKey.load(path: url.path)!

        XCTAssertNil(key.decryptPrivateKey(password: Data("password".utf8)))
    }

    func testDecrypt() {
        let url = Bundle(for: type(of: self)).url(forResource: "key", withExtension: "json")!
        let key = StoredKey.load(path: url.path)!
        let privateKey = key.decryptPrivateKey(password: Data("testpassword".utf8))!

        XCTAssertEqual(privateKey.hexString, "7a28b5ba57c53603b0b07b56bba752f7784bf506fa95edc395f5cf6c7514fe9d")
    }

    func testCreateWallet() {
        let privateKeyData = Data(hexString: "3a1076bf45ab87712ad64ccb3b10217737f7faacbf2872e88fdd9a537d8fe266")!
        let key = StoredKey.importPrivateKey(privateKey: privateKeyData, name: "name", password: Data("password".utf8), coin: .ethereum)!
        let decrypted = key.decryptPrivateKey(password: Data("password".utf8))!

        XCTAssertEqual(decrypted.hexString, privateKeyData.hexString)
    }

    func testDecodingEthereumAddress() {
        let url = Bundle(for: type(of: self)).url(forResource: "key", withExtension: "json")!
        let key = StoredKey.load(path: url.path)!
        let account = key.accountForCoin(coin: .ethereum, wallet: nil)!

        XCTAssertEqual(account.address.description, "0x008AeEda4D805471dF9b2A5B0f38A0C3bCBA786b")
    }

    func testDecodingBitcoinAddress() {
        let url = Bundle(for: type(of: self)).url(forResource: "key_bitcoin", withExtension: "json")!
        let key = StoredKey.load(path: url.path)!
        let account = key.accountForCoin(coin: .bitcoin, wallet: nil)!

        XCTAssertEqual(account.address.description, "3PWazDi9n1Hfyq9gXFxDxzADNL8RNYyK2y")
    }

    func testLongHexPassword() {
        let json = """
        {
            "address": "34bae2218c254ed190c0f5b1dd4323aee8e7da09",
            "id": "86066d8c-8dba-4d81-afd4-934e2a2b72a2",
            "version": 3,
            "crypto": {
                "cipher": "aes-128-ctr",
                "cipherparams": {
                    "iv": "a4976ad73057007ad788d1f792db851d"
                },
                "ciphertext": "5e4458d69964172c492616b751d6589b4ad7da4217dcfccecc3f4e515a934bb8",
                "kdf": "scrypt",
                "kdfparams": {
                    "dklen": 32,
                    "n": 4096,
                    "p": 6,
                    "r": 8,
                    "salt": "24c72d92bf88a4f7c7b3f5e3cb3620714d71fceabbb0bc6099f50c6d5d898e7c"
                },
                "mac": "c15e3035ddcaca766dfc56648978d33e94d3c57d4a5e13fcf8b5f8dbb0902900"
            }
        }
        """.data(using: .utf8)!
        let keystore = StoredKey.importJSON(json: json)!
        let password = Data(hexString: "2d6eefbfbd4622efbfbdefbfbd516718efbfbdefbfbdefbfbdefbfbd59efbfbd30efbfbdefbfbd3a4348efbfbd2aefbfbdefbfbd49efbfbd27efbfbd0638efbfbdefbfbdefbfbd4cefbfbd6befbfbdefbfbd6defbfbdefbfbd63efbfbd5aefbfbd61262b70efbfbdefbfbdefbfbdefbfbdefbfbdc7aa373163417cefbfbdefbfbdefbfbd44efbfbdefbfbd1d10efbfbdefbfbdefbfbd61dc9e5b124befbfbd11efbfbdefbfbd2fefbfbdefbfbd3d7c574868efbfbdefbfbdefbfbd37043b7b5c1a436471592f02efbfbd18efbfbdefbfbd2befbfbdefbfbd7218efbfbd6a68efbfbdcb8e5f3328773ec48174efbfbd67efbfbdefbfbdefbfbdefbfbdefbfbd2a31efbfbd7f60efbfbdd884efbfbd57efbfbd25efbfbd590459efbfbd37efbfbd2bdca20fefbfbdefbfbdefbfbdefbfbd39450113efbfbdefbfbdefbfbd454671efbfbdefbfbdd49fefbfbd47efbfbdefbfbdefbfbdefbfbd00efbfbdefbfbdefbfbdefbfbd05203f4c17712defbfbd7bd1bbdc967902efbfbdc98a77efbfbd707a36efbfbd12efbfbdefbfbd57c78cefbfbdefbfbdefbfbd10efbfbdefbfbdefbfbde1a1bb08efbfbdefbfbd26efbfbdefbfbd58efbfbdefbfbdc4b1efbfbd295fefbfbd0eefbfbdefbfbdefbfbd0e6eefbfbd")!
        let data = keystore.decryptPrivateKey(password: password)
        XCTAssertEqual(data?.hexString, "043c5429c7872502531708ec0d821c711691402caf37ef7ba78a8c506f10653b")
    }

    func testLongPassword() {
        let json = """
        {
            "address": "b92bcd5b14d2d70651d483c8f03bae79223b88ec",
            "id": "480fc670-e5e6-4b39-ba33-61f54ce792f9",
            "version": 3,
            "crypto": {
                "cipher": "aes-128-ctr",
                "cipherparams": {
                    "iv": "5d88369e4306bcc98e1fe70e7710f3a0"
                },
                "ciphertext": "de9bac5d1ca94cdd067a51275a56f0766ed3631de108609ee240c42a0994f97e",
                "kdf": "scrypt",
                "kdfparams": {
                    "dklen": 32,
                    "n": 4096,
                    "p": 6,
                    "r": 8,
                    "salt": "3849479dd4880793cfd92ebfeb30c60dccf04f3f76a2778fe89bead4237ddad4"
                },
                "mac": "361df97ff456009849ab1ffd4e71b61d7e66c9d2071c5a7563d1bbbdb0b2653b"
            }
        }
        """.data(using: .utf8)!
        let keystore = StoredKey.importJSON(json: json)!
        let password = "2d6eefbfbd4622efbfbdefbfbd516718efbfbdefbfbdefbfbdefbfbd59efbfbd30efbfbdefbfbd3a4348efbfbd2aefbfbdefbfbd49efbfbd27efbfbd0638efbfbdefbfbdefbfbd4cefbfbd6befbfbdefbfbd6defbfbdefbfbd63efbfbd5aefbfbd61262b70efbfbdefbfbdefbfbdefbfbdefbfbdc7aa373163417cefbfbdefbfbdefbfbd44efbfbdefbfbd1d10efbfbdefbfbdefbfbd61dc9e5b124befbfbd11efbfbdefbfbd2fefbfbdefbfbd3d7c574868efbfbdefbfbdefbfbd37043b7b5c1a436471592f02efbfbd18efbfbdefbfbd2befbfbdefbfbd7218efbfbd6a68efbfbdcb8e5f3328773ec48174efbfbd67efbfbdefbfbdefbfbdefbfbdefbfbd2a31efbfbd7f60efbfbdd884efbfbd57efbfbd25efbfbd590459efbfbd37efbfbd2bdca20fefbfbdefbfbdefbfbdefbfbd39450113efbfbdefbfbdefbfbd454671efbfbdefbfbdd49fefbfbd47efbfbdefbfbdefbfbdefbfbd00efbfbdefbfbdefbfbdefbfbd05203f4c17712defbfbd7bd1bbdc967902efbfbdc98a77efbfbd707a36efbfbd12efbfbdefbfbd57c78cefbfbdefbfbdefbfbd10efbfbdefbfbdefbfbde1a1bb08efbfbdefbfbd26efbfbdefbfbd58efbfbdefbfbdc4b1efbfbd295fefbfbd0eefbfbdefbfbdefbfbd0e6eefbfbd".data(using: .utf8)!
        let data = keystore.decryptPrivateKey(password: password)
        XCTAssertEqual(data?.hexString, "4357b2f9a6150ba969bc52f01c98cce5313595fe49f2d08303759c73e5c7a46c")
    }

    struct KdfParams: Decodable {
        let dklen: Int
        let n: Int
    }

    struct EncryptionParameters: Decodable {
        let kdf: String
        let kdfparams: KdfParams
    }

    func testEncryptionParameters() {
        let url = Bundle(for: type(of: self)).url(forResource: "key", withExtension: "json")!
        let key = StoredKey.load(path: url.path)!

        let paramsData = key.encryptionParameters!.data(using: .utf8)!
        let params = try! JSONDecoder().decode(EncryptionParameters.self, from: paramsData)

        XCTAssertEqual(params.kdf, "scrypt");
        XCTAssertEqual(params.kdfparams.n, 262144);
    }
}
