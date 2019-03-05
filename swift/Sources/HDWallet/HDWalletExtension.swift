// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

/// A hierarchical deterministic wallet.
public extension HDWallet {
    public func getExtendedPrivateKey(for purpose: Purpose, coin: CoinType, version: HDVersion) -> String {
        return getExtendedPrivateKey(purpose: purpose, coin: coin, version: version)
    }

    public func getExtendedPubKey(for purpose: Purpose, coin: CoinType, version: HDVersion) -> String {
        return getExtendedPublicKey(purpose: purpose, coin: coin, version: version)
    }

    public func getKey(at path: DerivationPath) -> PrivateKey {
        return getKey(purpose: path.purpose, coin: path.coinType, account: path.account, change: path.change, address: path.address)
    }
}
