// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public typealias TWNimiqSigningInput = TW_Nimiq_Proto_SigningInput
public typealias TWNimiqSigningOutput = TW_Nimiq_Proto_SigningOutput

public final class Nimq: Blockchain {
    public override var coinType: CoinType {
        return .nimiq
    }

    public override func address(for publicKey: PublicKey) -> Address {
        return NimiqAddress(publicKey: publicKey)
    }

    public override func address(string: String) -> Address? {
        return NimiqAddress(string: string)
    }

    public override func address(data: Data) -> Address? {
        return NimiqAddress(data: data)
    }
}

extension NimiqAddress: Address {
    public static func isValid(data: Data) -> Bool {
        return data.count == 20
    }

    public var data: Data {
        return keyHash
    }
}
