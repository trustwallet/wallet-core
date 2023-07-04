// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_coin_entry::{CoinEntry, ProtoError};
use tw_coin_registry::{coin_dispatcher, CoinRegistryContext, CoinType, RegistryError};

pub struct AnySigner;

pub type SigningResult<T> = Result<T, SigningError>;

pub enum SigningError {
    InvalidInput,
    Unsupported,
}

impl From<RegistryError> for SigningError {
    fn from(err: RegistryError) -> Self {
        match err {
            RegistryError::UnknownCoinType | RegistryError::Unsupported => {
                SigningError::Unsupported
            },
        }
    }
}

impl From<ProtoError> for SigningError {
    fn from(_err: ProtoError) -> Self {
        SigningError::InvalidInput
    }
}

impl AnySigner {
    pub fn sign(input: &[u8], coin: CoinType) -> SigningResult<Vec<u8>> {
        let ctx = CoinRegistryContext::new(coin)?;
        let entry = coin_dispatcher(coin)?;
        entry.sign_raw(&ctx, input).map_err(SigningError::from)
    }
}
