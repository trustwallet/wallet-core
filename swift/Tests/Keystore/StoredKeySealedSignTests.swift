// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

// Sealed signing (Security Layer, Phase 2) through the generated Swift bindings. Each case signs with
// `StoredKey.sign*` (the key never leaves WalletCore) and compares the result byte for byte with the
// app's current flow: export the key with `StoredKey.privateKey(coin:password:)`, then sign with the
// public API. Failures must come back as a `StoredKeySignError`, never as nil data on success.
class StoredKeySealedSignTests: XCTestCase {
    let mnemonic = "team engine square letter hero song dizzy scrub tornado fabric divert saddle"
    let password = Data("password".utf8)

    // A keystore with the default account of `coin`, its address and its exported key (the baseline).
    func makeKey(_ coin: CoinType) -> (StoredKey, String, PrivateKey) {
        let key = StoredKey.importHDWallet(mnemonic: mnemonic, name: "sealed", password: password, coin: coin)!
        let address = key.accountForCoin(coin: coin, wallet: nil)!.address
        let privateKey = key.privateKey(coin: coin, password: password)!
        return (key, address, privateKey)
    }

    // The output of a call that must succeed.
    func sealedData(_ result: StoredKeySignResult, file: StaticString = #file, line: UInt = #line) -> Data {
        XCTAssertEqual(result.error, .ok, result.errorMessage, file: file, line: line)
        XCTAssertEqual(result.errorMessage, "", file: file, line: line)
        guard let data = result.data else {
            XCTFail("no data on success", file: file, line: line)
            return Data()
        }
        return data
    }

    // A call that must fail with `expected`: no data, and a diagnostic message.
    func assertFails(_ result: StoredKeySignResult, _ expected: StoredKeySignError, file: StaticString = #file, line: UInt = #line) {
        XCTAssertEqual(result.error, expected, result.errorMessage, file: file, line: line)
        XCTAssertNil(result.data, file: file, line: line)
        XCTAssertFalse(result.errorMessage.isEmpty, file: file, line: line)
    }

    // MARK: - Transactions

    func ethereumTransferInput() -> EthereumSigningInput {
        EthereumSigningInput.with {
            $0.chainID = Data(hexString: "01")!
            $0.nonce = Data(hexString: "00")!
            $0.gasPrice = Data(hexString: "04a817c800")!
            $0.gasLimit = Data(hexString: "5208")!
            $0.toAddress = "0x3535353535353535353535353535353535353535"
            $0.transaction = EthereumTransaction.with {
                $0.transfer = EthereumTransaction.Transfer.with {
                    $0.amount = Data(hexString: "0de0b6b3a7640000")!
                }
            }
        }
    }

    func testEthereumTransactionMatchesAnySigner() throws {
        let (key, address, privateKey) = makeKey(.ethereum)
        let input = ethereumTransferInput()

        let sealed = sealedData(key.sign(coin: .ethereum, address: address, password: password, input: try input.serializedData()))

        var baselineInput = input
        baselineInput.privateKey = privateKey.data
        let baseline: EthereumSigningOutput = AnySigner.sign(input: baselineInput, coin: .ethereum)
        XCTAssertEqual(sealed.hexString, try baseline.serializedData().hexString)
        XCTAssertEqual(try EthereumSigningOutput(serializedData: sealed).error, .ok)
    }

    func testTronTransactionMatchesAnySigner() throws {
        let (key, address, privateKey) = makeKey(.tron)
        let input = TronSigningInput.with {
            $0.transaction = TronTransaction.with {
                $0.contractOneof = .transfer(TronTransferContract.with {
                    $0.ownerAddress = address
                    $0.toAddress = "THTR75o8xXAgCTQqpiot2AFRAjvW1tSbVV"
                    $0.amount = 2000000
                })
                $0.timestamp = 1539295479000
                $0.expiration = 1539295479000 + 10 * 60 * 60 * 1000
                $0.blockHeader = TronBlockHeader.with {
                    $0.timestamp = 1539295479000
                    $0.number = 3111739
                    $0.version = 3
                    $0.txTrieRoot = Data(hexString: "64288c2db0641316762a99dbb02ef7c90f968b60f9f2e410835980614332f86d")!
                    $0.parentHash = Data(hexString: "00000000002f7b3af4f5f8b9e23a30c530f719f165b742e7358536b280eead2d")!
                    $0.witnessAddress = Data(hexString: "415863f6091b8e71766da808b1dd3159790f61de7d")!
                }
            }
        }

        let sealed = sealedData(key.sign(coin: .tron, address: address, password: password, input: try input.serializedData()))

        var baselineInput = input
        baselineInput.privateKey = privateKey.data
        let baseline: TronSigningOutput = AnySigner.sign(input: baselineInput, coin: .tron)
        XCTAssertEqual(sealed.hexString, try baseline.serializedData().hexString)
        XCTAssertEqual(try TronSigningOutput(serializedData: sealed).error, .ok)
    }

    func solanaTransferInput() -> SolanaSigningInput {
        SolanaSigningInput.with {
            $0.recentBlockhash = "8oFmGuWUpsy8h8WP8AXTp3yhcLt4gQJRG5GxNcK7jfhX"
            $0.transferTransaction = SolanaTransfer.with {
                $0.recipient = "EN2sCsJ1WDV8UFqsiTXHcUPUxQ4juE71eCknHYYMifkd"
                $0.value = 42000
            }
        }
    }

    func testSolanaTransactionMatchesAnySigner() throws {
        let (key, address, privateKey) = makeKey(.solana)
        let input = solanaTransferInput()

        let sealed = sealedData(key.sign(coin: .solana, address: address, password: password, input: try input.serializedData()))

        var baselineInput = input
        baselineInput.privateKey = privateKey.data
        let baseline: SolanaSigningOutput = AnySigner.sign(input: baselineInput, coin: .solana)
        XCTAssertEqual(sealed.hexString, try baseline.serializedData().hexString)
        XCTAssertEqual(try SolanaSigningOutput(serializedData: sealed).error, .ok)
    }

    // MARK: - Messages

    func testEthereumPersonalMessageMatchesMessageSigner() {
        let (key, address, privateKey) = makeKey(.ethereum)
        let text = "Hello Trust Wallet"

        let sealed = sealedData(key.signEthereumMessage(coin: .ethereum, address: address, password: password, message: Data(text.utf8)))
        XCTAssertEqual(sealed.count, 65)
        XCTAssertEqual(sealed.hexString, EthereumMessageSigner.signMessage(privateKey: privateKey, message: text))
    }

    func testEthereumTypedDataMatchesMessageSigner() {
        let (key, address, privateKey) = makeKey(.ethereum)
        let typedData = """
        {"types":{"EIP712Domain":[{"name":"name","type":"string"},{"name":"version","type":"string"},{"name":"chainId","type":"uint256"},{"name":"verifyingContract","type":"address"}],"Person":[{"name":"name","type":"string"},{"name":"wallet","type":"address"}],"Mail":[{"name":"from","type":"Person"},{"name":"to","type":"Person"},{"name":"contents","type":"string"}]},"primaryType":"Mail","domain":{"name":"Ether Mail","version":"1","chainId":1,"verifyingContract":"0xCcCCccccCCCCcCCCCCCcCcCccCcCCCcCcccccccC"},"message":{"from":{"name":"Cow","wallet":"0xCD2a3d9F938E13CD947Ec05AbC7FE734Df8DD826"},"to":{"name":"Bob","wallet":"0xbBbBBBBbbBBBbbbBbbBbbbbBBbBbbbbBbBbbBBbB"},"contents":"Hello, Bob!"}}
        """

        let sealed = sealedData(key.signEthereumTypedData(coin: .ethereum, address: address, password: password, typedData: typedData))
        XCTAssertEqual(sealed.hexString, EthereumMessageSigner.signTypedMessage(privateKey: privateKey, messageJson: typedData))

        assertFails(key.signEthereumTypedData(coin: .ethereum, address: address, password: password, typedData: "{not json"), .invalidInput)
    }

    func testBizUserOpHashMatchesBiz() {
        let (key, address, privateKey) = makeKey(.smartChain)
        let chainId = Data(hexString: "38")! // 56
        let codeAddress = "0x2e234DAe75C793f67A35089C9d99245E1C58470b"
        let typeHash = "0x4f51e7a567f083a31264743067875fc6a7ae45c32c5bd71f6a998c4625b13867"
        let domainSeparatorHash = "0xd87cd6ef79d4e2b95e15ce8abf732db51ec771f1ca2edccf22a46c729ac56472"
        let userOpHash = "0xf177858c1c500e51f38ffe937bed7e4d3a8678725900be4682d3ce04d97071eb"

        let sealed = sealedData(key.signBizUserOpHash(coin: .smartChain, address: address, password: password, chainId: chainId,
                                                      codeAddress: codeAddress, codeName: "Biz", codeVersion: "v1.0.0", typeHash: typeHash,
                                                      domainSeparatorHash: domainSeparatorHash, userOpHash: userOpHash))

        // The app's flow: the encoded hash with sender = the account, then the signed hash.
        let encoded = Biz.getEncodedHash(chainId: chainId, codeAddress: codeAddress, codeName: "Biz", codeVersion: "v1.0.0", typeHash: typeHash,
                                         domainSeparatorHash: domainSeparatorHash, sender: address, userOpHash: userOpHash)!
        let baseline = Biz.getSignedHash(hash: "0x" + encoded.hexString, privateKey: privateKey.data.hexString)!
        XCTAssertEqual(sealed.hexString, baseline.hexString)
    }

    func testEip7702AuthorizationMatchesEip7702() {
        let (key, address, privateKey) = makeKey(.ethereum)
        let chainId = Data(hexString: "01")!
        let nonce = Data(hexString: "01")!
        let contract = "0xB91aaa96B138A1B1D94c9df4628187132c5F2bf1"

        let sealed = sealedData(key.signEip7702Authorization(coin: .ethereum, address: address, password: password,
                                                             chainId: chainId, contractAddress: contract, nonce: nonce))
        let baseline = Eip7702.signAuthorization(chainId: chainId, contractAddress: contract, nonce: nonce, privateKey: privateKey.data.hexString)!
        // The JSON comes back as UTF-8 data.
        XCTAssertEqual(String(decoding: sealed, as: UTF8.self), baseline)

        assertFails(key.signEip7702Authorization(coin: .ethereum, address: address, password: password,
                                                 chainId: chainId, contractAddress: "0x1234", nonce: nonce), .invalidInput)
    }

    func testTronMessageMatchesTronMessageSigner() {
        let (key, address, privateKey) = makeKey(.tron)
        let text = "Hello World"

        let sealed = sealedData(key.signTronMessage(coin: .tron, address: address, password: password, message: Data(text.utf8)))
        XCTAssertEqual(sealed.count, 65)
        // The public signer returns hex; the sealed result is the raw signature.
        XCTAssertEqual(sealed.hexString, TronMessageSigner.signMessage(privateKey: privateKey, message: text))
    }

    func testSolanaMessageMatchesMessageSigner() throws {
        let (key, address, privateKey) = makeKey(.solana)
        let text = "Hello world"

        let sealed = sealedData(key.signSolanaMessage(coin: .solana, address: address, password: password, message: Data(text.utf8)))
        XCTAssertEqual(sealed.count, 64)

        let input = SolanaMessageSigningInput.with {
            $0.privateKey = privateKey.data
            $0.message = text
        }
        let output = try SolanaMessageSigningOutput(serializedData: MessageSigner.sign(coin: .solana, input: try input.serializedData())!)
        XCTAssertEqual(output.error, .ok)
        XCTAssertEqual(sealed.hexString, Base58.decodeNoCheck(string: output.signature)!.hexString)
    }

    // MARK: - Errors

    func testFailuresReturnErrorCodes() throws {
        let (key, address, _) = makeKey(.ethereum)
        let input = try ethereumTransferInput().serializedData()

        assertFails(key.sign(coin: .ethereum, address: address, password: Data("wrong".utf8), input: input), .wrongPassword)
        assertFails(key.sign(coin: .ethereum, address: "0x3535353535353535353535353535353535353535", password: password, input: input), .accountNotFound)
        assertFails(key.sign(coin: .cosmos, address: address, password: password, input: input), .unsupportedCoin)
        assertFails(key.sign(coin: .ethereum, address: address, password: password, input: Data(hexString: "ffffffff")!), .invalidInput)

        var withKey = ethereumTransferInput()
        withKey.privateKey = Data(repeating: 0x46, count: 32)
        assertFails(key.sign(coin: .ethereum, address: address, password: password, input: try withKey.serializedData()), .invalidInput)

        // A message function only signs for its own coins.
        assertFails(key.signTronMessage(coin: .ethereum, address: address, password: password, message: Data("hi".utf8)), .unsupportedCoin)
    }

    // A Solana "message" that is exactly a transaction message is refused, so it can never yield a
    // transaction signature. The same bytes with one extra byte are not a transaction and are signed.
    func testSolanaMessageThatIsTransactionRefused() throws {
        let (key, address, privateKey) = makeKey(.solana)
        var input = solanaTransferInput()
        input.privateKey = privateKey.data
        let output: SolanaSigningOutput = AnySigner.sign(input: input, coin: .solana)
        let transactionMessage = Base58.decodeNoCheck(string: output.unsignedTx)!

        assertFails(key.signSolanaMessage(coin: .solana, address: address, password: password, message: transactionMessage), .transactionBytesRefused)

        let padded = transactionMessage + Data([0])
        let sealed = sealedData(key.signSolanaMessage(coin: .solana, address: address, password: password, message: padded))
        XCTAssertEqual(sealed.hexString, privateKey.sign(digest: padded, curve: .ed25519)!.hexString)
    }
}
