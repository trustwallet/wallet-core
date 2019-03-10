// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public typealias TWAionSigningInput = TW_Aion_Proto_SigningInput
public typealias TWAionSigningOutput = TW_Aion_Proto_SigningOutput

public final class Aion: Blockchain {
    
    public override var coinType: CoinType {
        return .aion
    }
    
    public override func address(for publicKey: PublicKey) -> Address {
        return AionAddress(publicKey: publicKey)
    }

    public override func address(string: String) -> Address? {
        return AionAddress(string: string)
    }

    public override func address(data: Data) -> Address? {
        return AionAddress(data: data)
    }
    
}

extension AionAddress: Address {
    
    public static func isValid(data: Data) -> Bool {
        return AionAddress(data: data) != nil
    }
    
}
