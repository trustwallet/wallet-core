// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_keypair::tw::PublicKeyType;

/// Extend the trait with methods required for blockchain additions.
pub trait CoinContext {
    fn public_key_type(&self) -> PublicKeyType;

    // Example:
    // fn ss58_prefix(&self) -> Option<String>;
}
