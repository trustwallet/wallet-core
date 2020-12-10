// Copyright © 2017-2020 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import WalletCore
import XCTest

class AeternityTests: XCTestCase {

    func testAddressFromPublicKey() {
        let publicKey = PublicKey(data: Data(hexString: "ee93a4f66f8d16b819bb9beb9ffccdfcdc1412e87fee6a324c2a99a1e0e67148")!, type: PublicKeyType.ed25519)!
        let address = AnyAddress(publicKey: publicKey, coin: .aeternity)
        XCTAssertEqual(address.description, "ak_2p5878zbFhxnrm7meL7TmqwtvBaqcBddyp5eGzZbovZ5FeVfcw")
    }

    func testAddressFromString() {
        let addressString = "ak_2p5878zbFhxnrm7meL7TmqwtvBaqcBddyp5eGzZbovZ5FeVfcw"
        let address = AnyAddress(string: addressString, coin: .aeternity)!
        XCTAssertEqual(address.description, "ak_2p5878zbFhxnrm7meL7TmqwtvBaqcBddyp5eGzZbovZ5FeVfcw")
    }

    func testTransferTransactionSign() throws {
        let input = AeternitySigningInput.with {
            $0.fromAddress = "ak_2p5878zbFhxnrm7meL7TmqwtvBaqcBddyp5eGzZbovZ5FeVfcw"
            $0.toAddress = "ak_Egp9yVdpxmvAfQ7vsXGvpnyfNq71msbdUpkMNYGTeTe8kPL3v"
            $0.fee = Data(hexString: "12309ce54000")!
            $0.ttl = 82757
            $0.nonce = 49
            $0.privateKey = Data(hexString: "4646464646464646464646464646464646464646464646464646464646464646")!
            $0.payload = AeternityPayload.with {
                $0.payloadTransfer = TW_Aeternity_Proto_Payload.Transfer.with {
                    $0.amount = Data(hexString: "0a")!
                    $0.payload = "Hello World"
                }
            }
        }

        let output: AeternitySigningOutput = AnySigner.sign(input: input, coin: .aeternity)

        XCTAssertFalse(AnySigner.supportsJSON(coin: .aeternity))
        XCTAssertEqual(output.signature, "sg_VW42qDPP3MMNFAStYaumjZz7mC7BZYpbNa15E57ejqUe7JdQFWCiX65eLNUpGMpt8tSpfgCfkYzcaFppqx7W75CrcWdC8")

        // swiftlint:disable:next line_length
        XCTAssertEqual(output.encoded, "tx_+KkLAfhCuEDZ2XDV5OuHv1iuLn66sFLBUwnzp1K8JW1Zz+fEgmuEh6HEvNu0R112M3IYkVzvTSnT0pJ3TWhVOumgJ+IWwW8HuGH4XwwBoQHuk6T2b40WuBm7m+uf/M383BQS6H/uajJMKpmh4OZxSKEBHxOjsIvwAUAGYqaLadh194A87EwIZH9u1dhMeJe9UKMKhhIwnOVAAIMBQ0Uxi0hlbGxvIFdvcmxkDZqNSg==")
    }

    func testContractTransactionSign() throws {
        let input = AeternitySigningInput.with {
            $0.fromAddress = "ak_2p5878zbFhxnrm7meL7TmqwtvBaqcBddyp5eGzZbovZ5FeVfcw"
            $0.toAddress = "ak_Egp9yVdpxmvAfQ7vsXGvpnyfNq71msbdUpkMNYGTeTe8kPL3v"
            $0.fee = Data(hexString: "12309ce54000")!
            $0.ttl = 82757
            $0.nonce = 49
            $0.privateKey = Data(hexString: "4646464646464646464646464646464646464646464646464646464646464646")!
            $0.payload = AeternityPayload.with {
                $0.payloadContractGeneric = TW_Aeternity_Proto_Payload.ContractGeneric.with {
                    $0.payload = "Hello World"
                }
            }
        }

        let output: AeternitySigningOutput = AnySigner.sign(input: input, coin: .aeternity)

        XCTAssertFalse(AnySigner.supportsJSON(coin: .aeternity))
        XCTAssertEqual(output.signature, "sg_9UV6qNYu78uck4Ds3sEruDNnJKjvqF469S4C9rAvBBZer3FPwxuSbeLtEBH9ufJiup2NyudC5Hi5ZHkAXTm6NnsGm55dd")

        // swiftlint:disable:next line_length
        XCTAssertEqual(output.encoded, "tx_+KkLAfhCuEBAxVYM3tXXtZNrpF5oTQ7JCjviPN9k0sLuE24QlLXW+xp5BUhYTBNcVE5AQPBHxEEqtzQh+K+7i1vzhTylwtQBuGH4XwwBoQHuk6T2b40WuBm7m+uf/M383BQS6H/uajJMKpmh4OZxSKEBHxOjsIvwAUAGYqaLadh194A87EwIZH9u1dhMeJe9UKMAhhIwnOVAAIMBQ0Uxi0hlbGxvIFdvcmxkooC1hg==")
    }
}
