// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::coin_context::CoinContext;
use crate::derivation::DerivationWithPath;
use tw_hash::hasher::Hasher;
use tw_keypair::tw::PublicKeyType;

/// Test coin context that panics on any `CoinContext` method call.
#[derive(Default)]
pub struct TestCoinContext {
    pub public_key_type: Option<PublicKeyType>,
    pub address_hasher: Option<Hasher>,
    pub hrp: Option<String>,
    pub p2pkh: Option<u8>,
    pub p2sh: Option<u8>,
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

    fn p2pkh_prefix(&self) -> Option<u8> {
        self.p2pkh
    }

    fn p2sh_prefix(&self) -> Option<u8> {
        self.p2sh
    }

    fn derivations(&self) -> &[DerivationWithPath] {
        unimplemented!()
    }
}
