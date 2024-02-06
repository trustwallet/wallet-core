// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::coin_context::CoinContext;
use tw_hash::hasher::Hasher;
use tw_keypair::tw::PublicKeyType;

/// Test coin context that panics on any `CoinContext` method call.
#[derive(Default)]
pub struct TestCoinContext {
    pub public_key_type: Option<PublicKeyType>,
    pub address_hasher: Option<Hasher>,
    pub hrp: Option<String>,
}

impl TestCoinContext {
    pub fn with_public_key_type(mut self, public_key_type: PublicKeyType) -> TestCoinContext {
        self.public_key_type = Some(public_key_type);
        self
    }

    pub fn with_hrp(mut self, hrp: &str) -> TestCoinContext {
        self.hrp = Some(hrp.to_string());
        self
    }
}

impl CoinContext for TestCoinContext {
    fn public_key_type(&self) -> PublicKeyType {
        self.public_key_type
            .expect("EmptyCoinContext::public_key_type was not set")
    }

    fn address_hasher(&self) -> Option<Hasher> {
        self.address_hasher
    }

    fn hrp(&self) -> Option<String> {
        self.hrp.clone()
    }
}
