// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public final class NEO: Blockchain {
    
    public override var coinType: CoinType {
        return .neo
    }
    
    public override func address(for publicKey: PublicKey) -> Address {
        return NEOAddress(publicKey: publicKey)
    }

    public override func address(string: String) -> Address? {
        return NEOAddress(string: string)
    }

    public override func address(data: Data) -> Address? {
        return NEOAddress(data: data)
    }
}

extension NEOAddress: Address {
    public var data: Data {
       return keyHash
    }

    public static func isValid(data: Data) -> Bool {
        return NEOAddress(data: data) != nil
    }
}
