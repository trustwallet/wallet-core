// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public class BitcoinCash: Bitcoin {

    public override init(purpose: Purpose = .bip44) {
        super.init(purpose: purpose)
    }

    override public var coinType: CoinType {
        return .bitcoinCash
    }

    override public var hrp: HRP {
        return .bitcoinCash
    }

    override public var supportSegwit: Bool {
        return false
    }

    override public func address(for publicKey: PublicKeySecp256k1) -> Address {
        return BitcoinCashAddress(publicKey: publicKey)
    }

    override open func address(string: String) -> Address? {
        let value: String = {
            if string.lowercased().hasPrefix(HRP.bitcoinCash.description) {
                return string.lowercased()
            }
            return [HRP.bitcoinCash.description, string.lowercased()].joined(separator: ":")
        }()
        if let cashAddr = BitcoinCashAddress(string: value) {
            return cashAddr
        } else {
            return BitcoinAddress(string: string)
        }
    }

    override open func address(data: Data) -> Address? {
        if let cashAddr = BitcoinCashAddress(data: data) {
            return cashAddr
        } else {
            return BitcoinAddress(data: data)
        }
    }
}
