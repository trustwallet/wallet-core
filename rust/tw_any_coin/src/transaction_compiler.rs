// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use tw_coin_entry::coin_entry::{PublicKeyBytes, SignatureBytes};
use tw_coin_entry::coin_entry_ext::CoinEntryExt;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_coin_entry::modules::input_builder::BuildSigningInputArgs;
use tw_coin_registry::{coin_dispatcher, CoinType};

pub struct TransactionCompiler;

impl TransactionCompiler {
    pub fn build_input(coin: CoinType, args: BuildSigningInputArgs) -> SigningResult<Vec<u8>> {
        let (ctx, entry) = coin_dispatcher(coin)?;
        match entry.build_signing_input(&ctx, args) {
            Ok(Some(result)) => Ok(result),
            // The chain doesn't support `build_signing_input`.
            Ok(None) => Err(SigningError(SigningErrorType::Error_internal)),
            Err(e) => Err(e),
        }
    }

    pub fn preimage_hashes(coin: CoinType, input: &[u8]) -> SigningResult<Vec<u8>> {
        let (ctx, entry) = coin_dispatcher(coin)?;
        entry
            .preimage_hashes(&ctx, input)
            .map_err(SigningError::from)
    }

    pub fn compile(
        coin: CoinType,
        input: &[u8],
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> SigningResult<Vec<u8>> {
        let (ctx, entry) = coin_dispatcher(coin)?;
        entry
            .compile(&ctx, input, signatures, public_keys)
            .map_err(SigningError::from)
    }
}
