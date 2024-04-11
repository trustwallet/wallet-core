use crate::address::Address;
use crate::transaction_builder;
use std::str::FromStr;
use tw_coin_entry::coin_entry::{PublicKeyBytes, SignatureBytes};
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_coin_entry::signing_output_error;
use tw_proto::Aptos::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;

pub struct Compiler;

impl Compiler {
    #[inline]
    pub fn preimage_hashes(
        input: Proto::SigningInput<'_>,
    ) -> CompilerProto::PreSigningOutput<'static> {
        Self::preimage_hashes_impl(input)
            .unwrap_or_else(|e| signing_output_error!(CompilerProto::PreSigningOutput, e))
    }

    fn preimage_hashes_impl(
        input: Proto::SigningInput<'_>,
    ) -> SigningResult<CompilerProto::PreSigningOutput<'static>> {
        let builder = transaction_builder::TransactionFactory::new_from_protobuf(input.clone())?;
        let sender = Address::from_str(&input.sender)?;
        let signed_tx = builder
            .sender(sender.inner())
            .sequence_number(input.sequence_number as u64)
            .build()?
            .pre_image()?;
        Ok(CompilerProto::PreSigningOutput {
            data: signed_tx.into(),
            ..CompilerProto::PreSigningOutput::default()
        })
    }

    #[inline]
    pub fn compile(
        input: Proto::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> Proto::SigningOutput<'static> {
        Self::compile_impl(input, signatures, public_keys)
            .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    fn compile_impl(
        input: Proto::SigningInput<'_>,
        signatures: Vec<SignatureBytes>,
        public_keys: Vec<PublicKeyBytes>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let builder = transaction_builder::TransactionFactory::new_from_protobuf(input.clone())?;
        let sender = Address::from_str(&input.sender)?;
        let signature = signatures
            .first()
            .ok_or(SigningError(SigningErrorType::Error_signatures_count))?;
        let public_key = public_keys
            .first()
            .ok_or(SigningError(SigningErrorType::Error_signatures_count))?;

        let signed_tx = builder
            .sender(sender.inner())
            .sequence_number(input.sequence_number as u64)
            .build()?
            .compile(signature.to_vec(), public_key.to_vec())?;
        Ok(Proto::SigningOutput {
            raw_txn: signed_tx.raw_txn_bytes().clone().into(),
            encoded: signed_tx.encoded().clone().into(),
            authenticator: Some((*signed_tx.authenticator()).clone().into()),
            json: signed_tx.to_json().to_string().into(),
            ..Proto::SigningOutput::default()
        })
    }
}
