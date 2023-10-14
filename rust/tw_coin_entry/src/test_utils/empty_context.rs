// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::coin_context::CoinContext;
use tw_keypair::tw::PublicKeyType;

/// Test coin context that panics on any `CoinContext` method call.
pub struct EmptyCoinContext;

impl CoinContext for EmptyCoinContext {
    fn public_key_type(&self) -> PublicKeyType {
        panic!()
    }
}
