// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

import Foundation

extension BitcoinAddress: Equatable {
    public var base58String: String {
        return description
    }

    public func hash(into hasher: inout Hasher) {
        hasher.combine(description)
    }

    /// Creates a legacy Bitcoin address for segwit redeem script.
    public static func compatibleAddress(publicKey: PublicKey, prefix: UInt8) -> BitcoinAddress {
        let witnessVersion = Data([0x00, 0x14])
        let redeemScript = Hash.sha256RIPEMD(data: witnessVersion + publicKey.bitcoinKeyHash)
        let address = Base58.encode(data: [prefix] + redeemScript)
        return BitcoinAddress(string: address)!
    }
}
