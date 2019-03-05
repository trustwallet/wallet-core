// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

import Foundation

public final class EOS: Blockchain {
    public var chainIDString: String {
        return "aca376f206b8fc25a6ed44dbdc66547c36c6c33e3a119ffbeaef943642f0e906"
    }

    public override var coinType: CoinType {
        return .eos
    }

    public override func address(for publicKey: PublicKey) -> Address {
        let compressed = publicKey.compressed
        let check = Hash.ripemd(data: compressed.data)[0..<4]
        return EOSAddress(data: compressed.data + check)!
    }

    public override func address(string: String) -> Address? {
        return EOSAddress(string: string)
    }

    public override func address(data: Data) -> Address? {
        return EOSAddress(data: data)
    }
}
