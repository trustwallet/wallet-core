// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public typealias TWIconSigningInput = TW_Icon_Proto_SigningInput
public typealias TWIconSigningOutput = TW_Icon_Proto_SigningOutput

public final class Icon: Ethereum {
    /// Chain identifier.
    public override var chainID: Int {
        return 1
    }

    /// SLIP-044 coin type.
    public override var coinType: CoinType {
        return .icon
    }

    public override func address(for publicKey: PublicKeySecp256k1) -> Address {
        return IconAddress(publicKey: publicKey, type: .address)
    }

    public override func address(string: String) -> Address? {
        return IconAddress(string: string)
    }

    public override func address(data: Data) -> Address? {
        return IconAddress(keyHash: data, type: .address)
    }
}
