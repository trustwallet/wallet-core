// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public class Ripple: Blockchain {

    public override var coinType: CoinType {
        return .ripple
    }

    public override func address(data: Data) -> Address? {
        return RippleAddress(data: data)
    }

    public override func address(string: String) -> Address? {
        return RippleAddress(string: string)
    }

    public override func address(for publicKey: PublicKey) -> Address {
        return RippleAddress(publicKey: publicKey.compressed)
    }
}

extension RippleAddress: Address {
    public static func isValid(data: Data) -> Bool {
        return data.count == 21
    }

    public var data: Data {
        return keyHash
    }
}
