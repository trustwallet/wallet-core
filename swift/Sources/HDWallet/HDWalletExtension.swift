// Copyright © 2017-2019 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

/// A hierarchical deterministic wallet.
public extension HDWallet {
    public func getExtendedPubKey(curve: Curve = .secp256k1, purpose: Purpose, coin: CoinType, version: HDVersion) -> String {
        return getExtendedPublicKey(curve: curve, purpose: purpose, coin: coin, version: version)
    }

    public func getKey(at path: DerivationPath, curve: Curve = .secp256k1) -> PrivateKey {
        return getKey(curve: curve, purpose: path.purpose, coin: path.coinType, account: path.account, change: path.change, address: path.address)
    }
}
