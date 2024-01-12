// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import WalletCore
import XCTest

class InternetComputerTests: XCTestCase {
    // TODO: Check and finalize implementation

    func testAddress() {
        // TODO: Check and finalize implementation

        let key = PrivateKey(data: Data(hexString: "ee42eaada903e20ef6e5069f0428d552475c1ea7ed940842da6448f6ef9d48e7")!)!
        let pubkey = key.getPublicKeySecp256k1(compressed: false)
        let address = AnyAddress(publicKey: pubkey, coin: .internetComputer)
        let addressFromString = AnyAddress(string: "2f25874478d06cf68b9833524a6390d0ba69c566b02f46626979a3d6a4153211", coin: .internetComputer)!

        XCTAssertEqual(pubkey.data.hexString, "048542e6fb4b17d6dfcac3948fe412c00d626728815ee7cc70509603f1bc92128a6e7548f3432d6248bc49ff44a1e50f6389238468d17f7d7024de5be9b181dbc8")
        XCTAssertEqual(address.description, addressFromString.description)
    }

    func testSign() {
        let key = PrivateKey(data: Data(hexString: "227102911bb99ce7285a55f952800912b7d22ebeeeee59d77fc33a5d7c7080be")!)!
        let input = InternetComputerSigningInput.with {
            $0.privateKey = key.data
            $0.transaction = InternetComputerTransaction.with {
                $0.transfer = InternetComputerTransaction.Transfer.with {
                    $0.toAccountIdentifier = "943d12e762f43806782f524b8f90297298a6d79e4749b41b585ec427409c826a"
                    $0.amount = 100000000
                    $0.memo = 0
                    $0.currentTimestampNanos = 1691709940000000000
                }
            }
                
        }
        let output: InternetComputerSigningOutput = AnySigner.sign(input: input, coin: .internetComputer)
        XCTAssertEqual(output.signedTransaction.hexString, "81826b5452414e53414354494f4e81a266757064617465a367636f6e74656e74a66c726571756573745f747970656463616c6c6e696e67726573735f6578706972791b177a297215cfe8006673656e646572581d971cd2ddeecd1cf1b28be914d7a5c43441f6296f1f9966a7c8aff68d026b63616e69737465725f69644a000000000000000201016b6d6574686f645f6e616d656773656e645f706263617267583b0a0012070a050880c2d72f2a220a20943d12e762f43806782f524b8f90297298a6d79e4749b41b585ec427409c826a3a0a088090caa5a3a78abd176d73656e6465725f7075626b65799858183018561830100607182a1886184818ce183d02010605182b188104000a0318420004183d18ab183a182118a81838184d184c187e1852188a187e18dc18d8184418ea18cd18c5189518ac188518b518bc181d188515186318bc18e618ab18d2184318d3187c184f18cd18f018de189b18b5181918dd18ef1889187218e71518c40418d4189718881843187218c611182e18cc18e6186b182118630218356a73656e6465725f736967984013186f18b9181c189818b318a8186518b2186118d418971618b1187d18eb185818e01826182f1873183b185018cb185d18ef18d81839186418b3183218da1824182f184e18a01880182718c0189018c918a018fd18c418d9189e189818b318ef1874183b185118e118a51864185918e718ed18c71889186c1822182318ca6a726561645f7374617465a367636f6e74656e74a46c726571756573745f747970656a726561645f73746174656e696e67726573735f6578706972791b177a297215cfe8006673656e646572581d971cd2ddeecd1cf1b28be914d7a5c43441f6296f1f9966a7c8aff68d0265706174687381824e726571756573745f7374617475735820e8fbc2d5b0bf837b3a369249143e50d4476faafb2dd620e4e982586a51ebcf1e6d73656e6465725f7075626b65799858183018561830100607182a1886184818ce183d02010605182b188104000a0318420004183d18ab183a182118a81838184d184c187e1852188a187e18dc18d8184418ea18cd18c5189518ac188518b518bc181d188515186318bc18e618ab18d2184318d3187c184f18cd18f018de189b18b5181918dd18ef1889187218e71518c40418d4189718881843187218c611182e18cc18e6186b182118630218356a73656e6465725f7369679840182d182718201888188618ce187f0c182a187a18d718e818df18fb18d318d41118a5186a184b18341842185318d718e618e8187a1828186c186a183618461418f3183318bd18a618a718bc18d918c818b7189d186e1865188418ff18fd18e418e9187f181b18d705184818b21872187818d6181c161833184318a2")
    }
    
    func testSignWithInvalidToAccountIdentifier() {
        let key = PrivateKey(data: Data(hexString: "227102911bb99ce7285a55f952800912b7d22ebeeeee59d77fc33a5d7c7080be")!)!
        let input = InternetComputerSigningInput.with {
            $0.privateKey = key.data
            $0.transaction = InternetComputerTransaction.with {
                $0.transfer = InternetComputerTransaction.Transfer.with {
                    $0.toAccountIdentifier = "643d12e762f43806782f524b8f90297298a6d79e4749b41b585ec427409c826b"
                    $0.amount = 100000000
                    $0.memo = 0
                    $0.currentTimestampNanos = 1691709940000000000
                }
            }
                
        }
        let output: InternetComputerSigningOutput = AnySigner.sign(input: input, coin: .internetComputer)
        XCTAssertEqual(output.error.rawValue, 16)
    }
    
    func testSignWithInvalidAmount() {
        let key = PrivateKey(data: Data(hexString: "227102911bb99ce7285a55f952800912b7d22ebeeeee59d77fc33a5d7c7080be")!)!
        let input = InternetComputerSigningInput.with {
            $0.privateKey = key.data
            $0.transaction = InternetComputerTransaction.with {
                $0.transfer = InternetComputerTransaction.Transfer.with {
                    $0.toAccountIdentifier = "943d12e762f43806782f524b8f90297298a6d79e4749b41b585ec427409c826a"
                    $0.amount = 0
                    $0.memo = 0
                    $0.currentTimestampNanos = 1691709940000000000
                }
            }
                
        }
        let output: InternetComputerSigningOutput = AnySigner.sign(input: input, coin: .internetComputer)
        XCTAssertEqual(output.error.rawValue, 23)
    }
}
