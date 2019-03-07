// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public class Tezos: Blockchain {
    public override var coinType: CoinType {
        return .tezos
    }

    public override func address(for publicKey: PublicKey) -> Address {
        return TezosAddress(publicKey: publicKey)
    }

    public override func address(string: String) -> Address? {
        return TezosAddress(string: string)
    }

    public override func address(data: Data) -> Address? {
        guard let stringRepresentation = String(data: data, encoding: .utf8) else {
            return nil
        }
        return TezosAddress(string: stringRepresentation)
    }
}

extension TezosAddress: Address {
    public static func isValid(data: Data) -> Bool {
        guard let stringRepresentation = String(data: data, encoding: .utf8) else {
            return false
        }
        return TWTezosAddressIsValidString(stringRepresentation)
    }

    public var data: Data {
        return keyHash
    }
}
