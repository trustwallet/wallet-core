// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

extension BitcoinCashAddress: Equatable {

    /// Version bytes
    ///
    /// https://github.com/bitcoincashorg/bitcoincash.org/blob/master/spec/cashaddr.md
    public static let p2khVersion: UInt8 = 0x00
    public static let p2shVersion: UInt8 = 0x08

    public static func validate(hrp: String) -> Bool {
        return HRP.bitcoinCash.description == hrp
    }
}
