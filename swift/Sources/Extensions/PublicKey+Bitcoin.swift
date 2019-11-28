// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

public extension PublicKey {
    /// Returns the ripemd160 hash of the sha2 hash of the compressed public key data.
    var bitcoinKeyHash: Data {
        return Hash.sha256RIPEMD(data: compressed.data)
    }
}
