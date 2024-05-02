// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_coin_entry::error::prelude::*;
use tw_hash::hasher::sha256_ripemd;
use tw_hash::Hash;
use tw_proto::BitcoinV3::Proto;
use tw_utxo::transaction::standard_transaction::builder::TransactionBuilder;
use tw_utxo::transaction::standard_transaction::unsigned_transaction::UnsignedTransaction;

pub mod utxo_protobuf;

/// The module is dedicated to convert [`Proto::SigningInput`] to [`UnsignedTransaction`].
pub struct TransactionProtobuf;

impl TransactionProtobuf {
    pub fn unsigned_tx_from_proto(
        _input: &Proto::SigningInput,
    ) -> SigningResult<UnsignedTransaction> {
        let _transaction_builder = TransactionBuilder::default();
        todo!()
    }
}

pub fn pubkey_hash_from_proto<const N: usize>(
    input: &Proto::PublicKeyOrHash,
) -> SigningResult<Hash<N>> {
    use Proto::mod_PublicKeyOrHash::OneOfvariant as PublicKeyOrHashType;

    let hash_data = match input.variant {
        PublicKeyOrHashType::pubkey(ref pubkey) => sha256_ripemd(pubkey.as_ref()),
        PublicKeyOrHashType::hash(ref hash) => hash.to_vec(),
        PublicKeyOrHashType::None => {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context("Expected a public key or its hash")
        },
    };
    Hash::<N>::try_from(hash_data.as_slice())
        .tw_err(|_| SigningErrorType::Error_invalid_params)
        .with_context(|| format!("Expected exactly {N} bytes public key hash"))
}
