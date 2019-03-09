// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public class Stellar: Blockchain {
    
    public override var coinType: CoinType {
        return .stellar
    }
    
    public override func address(data: Data) -> Address? {
        return StellarAddress(data: data)
    }
    
    public override func address(string: String) -> Address? {
        return StellarAddress(string: string)
    }
    
    public override func address(for publicKey: PublicKey) -> Address {
        return StellarAddress(publicKey: publicKey.compressed)
    }
}

extension StellarAddress: Address {
    public static func isValid(data: Data) -> Bool {
        return data.count == 32
    }
    
    public var data: Data {
        return keyHash
    }
}
