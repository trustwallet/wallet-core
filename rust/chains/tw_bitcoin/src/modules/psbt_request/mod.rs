// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::marker::PhantomData;
use tw_coin_entry::error::prelude::*;
use tw_memory::Data;
use tw_proto::BitcoinV2::Proto;
use tw_utxo::context::UtxoContext;
use tw_utxo::transaction::unsigned_transaction::UnsignedTransaction;

pub use bitcoin::psbt::Psbt;

pub mod output_psbt;
pub mod standard_psbt_request_handler;
pub mod utxo_psbt;

pub trait PsbtRequestHandler<Context: UtxoContext> {
    /// Parses a PSBT request from Protobuf.
    fn parse_request(
        input: &Proto::SigningInput,
        psbt_input: &Proto::Psbt,
    ) -> SigningResult<PsbtRequest<Context>>;

    /// Finalizes the [Partially Signed Bitcoin Transaction](Psbt)
    /// by updating the final `script_sig` and/or `witness`.
    fn update_signed(
        psbt: &mut Context::Psbt,
        signed_tx: &Context::Transaction,
    ) -> SigningResult<()>;

    /// Serializes the PSBT into bytes.
    fn serialize_psbt(psbt: &Context::Psbt) -> SigningResult<Data>;
}

pub struct PsbtRequest<Context: UtxoContext> {
    pub psbt: Context::Psbt,
    pub unsigned_tx: UnsignedTransaction<Context::Transaction>,
    pub _phantom: PhantomData<Context>,
}

pub struct NoPsbtRequestBuilder;

impl<Context: UtxoContext> PsbtRequestHandler<Context> for NoPsbtRequestBuilder {
    fn parse_request(
        _input: &Proto::SigningInput,
        _psbt_input: &Proto::Psbt,
    ) -> SigningResult<PsbtRequest<Context>> {
        SigningError::err(SigningErrorType::Error_not_supported)
            .context("PSBT signing is not supported")
    }

    fn update_signed(
        _psbt: &mut Context::Psbt,
        _signed_tx: &Context::Transaction,
    ) -> SigningResult<()> {
        SigningError::err(SigningErrorType::Error_not_supported)
            .context("PSBT signing is not supported")
    }

    fn serialize_psbt(_psbt: &Context::Psbt) -> SigningResult<Data> {
        SigningError::err(SigningErrorType::Error_not_supported)
            .context("PSBT signing is not supported")
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::context::StandardBitcoinContext;
    use tw_encoding::hex::DecodeHex;
    use tw_utxo::transaction::standard_transaction::Transaction;

    #[test]
    fn test_no_psbt_request_builder() {
        let input = Proto::SigningInput::default();
        let psbt_input = Proto::Psbt::default();

        // Test parse_request
        let result: SigningResult<PsbtRequest<StandardBitcoinContext>> =
            NoPsbtRequestBuilder::parse_request(&input, &psbt_input);
        assert_eq!(
            *result.err().expect("Expected an error").error_type(),
            SigningErrorType::Error_not_supported
        );

        // Test update_signed
        let raw_psbt = "70736274ff01008202000000015c37bcf049b7e62dd5bfd707e0998ce86163b786e3cd45db2336cb794a8d8aa10000000000ffffffff03f82a000000000000160014bf5a13a26791a5db6406304a46952e264c2b28910000000000000000056a032b3a6291950000000000001976a9147c2c0ac72afbde13ecf52fca54368e7883b538b188ac000000000001007e0200000002714916920be4dbc87cbb8697ca9b1420d6b1e47e7d732e2d2e0e7a935087788d0000000000ffffffff326c951cd9b3dc382e2d6be88796b65d7bac90406a5f72660171ac826e414a630200000000ffffffff01efca0000000000001976a9147c2c0ac72afbde13ecf52fca54368e7883b538b188ac0000000000000000".decode_hex().unwrap();
        let mut psbt = Psbt::deserialize(&raw_psbt).unwrap();
        let signed_tx = Transaction {
            version: 1,
            inputs: vec![],
            outputs: vec![],
            locktime: 0,
        };
        let result =
            <NoPsbtRequestBuilder as PsbtRequestHandler<StandardBitcoinContext>>::update_signed(
                &mut psbt, &signed_tx,
            );
        assert_eq!(
            *result.err().expect("Expected an error").error_type(),
            SigningErrorType::Error_not_supported
        );

        // Test serialize_psbt
        let result =
            <NoPsbtRequestBuilder as PsbtRequestHandler<StandardBitcoinContext>>::serialize_psbt(
                &psbt,
            );
        assert_eq!(
            *result.err().expect("Expected an error").error_type(),
            SigningErrorType::Error_not_supported
        );
    }
}
