// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

/// A hierarchical deterministic wallet.
public extension HDWallet {
    func getExtendedPubKey(purpose: Purpose, coin: CoinType, version: HDVersion) -> String {
        return getExtendedPublicKey(purpose: purpose, coin: coin, version: version)
    }

    func getKey(at path: DerivationPath) -> PrivateKey {
        return getKey(derivationPath: path.description)
    }
}
