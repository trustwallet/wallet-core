// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::message::eip191::Eip191Message;
use crate::message::eip712::eip712_message::Eip712Message;
use crate::message::signature::{MessageSignature, SignatureType};
use crate::message::{to_signing, EthMessage, EthMessageBoxed};
use std::borrow::Cow;
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::modules::message_signer::MessageSigner;
use tw_coin_entry::signing_output_error;
use tw_encoding::hex::ToHex;
use tw_keypair::ecdsa::secp256k1;
use tw_keypair::ecdsa::signature::VerifySignature;
use tw_keypair::traits::{SigningKeyTrait, VerifyingKeyTrait};
use tw_number::U256;
use tw_proto::Ethereum::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;

#[derive(Default)]
pub struct EthMessageSigner;

impl MessageSigner for EthMessageSigner {
    type MessageSigningInput<'a> = Proto::MessageSigningInput<'a>;
    type MessagePreSigningOutput = CompilerProto::PreSigningOutput<'static>;
    type MessageSigningOutput = Proto::MessageSigningOutput<'static>;
    type MessageVerifyingInput<'a> = Proto::MessageVerifyingInput<'a>;

    fn message_preimage_hashes(
        &self,
        _coin: &dyn CoinContext,
        input: Self::MessageSigningInput<'_>,
    ) -> Self::MessagePreSigningOutput {
        Self::message_preimage_hashes_impl(input)
            .unwrap_or_else(|e| signing_output_error!(CompilerProto::PreSigningOutput, e))
    }

    fn sign_message(
        &self,
        _coin: &dyn CoinContext,
        input: Self::MessageSigningInput<'_>,
    ) -> Self::MessageSigningOutput {
        Self::sign_message_impl(input)
            .unwrap_or_else(|e| signing_output_error!(Proto::MessageSigningOutput, e))
    }

    fn verify_message(
        &self,
        _coin: &dyn CoinContext,
        input: Self::MessageVerifyingInput<'_>,
    ) -> bool {
        Self::verify_message_impl(input).unwrap_or_default()
    }
}

impl EthMessageSigner {
    fn message_preimage_hashes_impl(
        input: Proto::MessageSigningInput<'_>,
    ) -> SigningResult<CompilerProto::PreSigningOutput<'static>> {
        let msg = Self::message_from_proto(input)?;
        let hash = msg.hash().map_err(to_signing)?.to_vec();
        Ok(CompilerProto::PreSigningOutput {
            data: Cow::Owned(hash.clone()),
            data_hash: Cow::Owned(hash),
            ..CompilerProto::PreSigningOutput::default()
        })
    }

    fn sign_message_impl(
        input: Proto::MessageSigningInput<'_>,
    ) -> SigningResult<Proto::MessageSigningOutput<'static>> {
        let private_key = secp256k1::PrivateKey::try_from(input.private_key.as_ref())?;
        let signature_type =
            Self::signature_type_from_proto(input.message_type, input.chain_id.clone());

        let msg = Self::message_from_proto(input)?;

        let hash_to_sign = msg.hash().map_err(to_signing)?;
        let secp_sign = private_key.sign(hash_to_sign)?;
        let prepared_sign = MessageSignature::prepared(secp_sign, signature_type)?;

        Ok(Proto::MessageSigningOutput {
            signature: Cow::Owned(prepared_sign.to_bytes().to_hex()),
            ..Proto::MessageSigningOutput::default()
        })
    }

    fn verify_message_impl(input: Proto::MessageVerifyingInput<'_>) -> SigningResult<bool> {
        let public_key = secp256k1::PublicKey::try_from(input.public_key.as_ref())?;

        let msg_hash = Self::message_from_str(&input.message)?
            .hash()
            .map_err(to_signing)?;
        let secp_signature =
            MessageSignature::from_str(&input.signature)?.to_secp256k1_signature()?;

        let actual_public_key = secp256k1::PublicKey::recover(secp_signature.clone(), msg_hash)?;
        let valid = actual_public_key == public_key
            && public_key.verify(VerifySignature::from(secp_signature), msg_hash);

        Ok(valid)
    }

    fn message_from_proto(input: Proto::MessageSigningInput<'_>) -> SigningResult<EthMessageBoxed> {
        match input.message_type {
            Proto::MessageType::MessageType_legacy
            | Proto::MessageType::MessageType_eip155
            | Proto::MessageType::MessageType_immutable_x => {
                Ok(Eip191Message::new(input.message).into_boxed())
            },
            Proto::MessageType::MessageType_typed
            | Proto::MessageType::MessageType_typed_eip155 => match input.chain_id {
                Some(expected_chain_id) => {
                    let expected_chain_id = U256::from(expected_chain_id.chain_id);
                    Ok(Eip712Message::new_checked(input.message, expected_chain_id)
                        .map_err(to_signing)?
                        .into_boxed())
                },
                None => Ok(Eip712Message::new(input.message)
                    .map_err(to_signing)?
                    .into_boxed()),
            },
        }
    }

    fn message_from_str(user_message: &str) -> SigningResult<EthMessageBoxed> {
        match Eip712Message::new(user_message) {
            Ok(typed_data) => Ok(typed_data.into_boxed()),
            Err(_) => Ok(Eip191Message::new(user_message).into_boxed()),
        }
    }

    fn signature_type_from_proto(
        msg_type: Proto::MessageType,
        maybe_chain_id: Option<Proto::MaybeChainId>,
    ) -> SignatureType {
        match msg_type {
            Proto::MessageType::MessageType_immutable_x => SignatureType::Standard,
            Proto::MessageType::MessageType_legacy | Proto::MessageType::MessageType_typed => {
                SignatureType::Legacy
            },
            Proto::MessageType::MessageType_eip155
            | Proto::MessageType::MessageType_typed_eip155 => {
                let chain_id = U256::from(maybe_chain_id.unwrap_or_default().chain_id);
                SignatureType::Eip155 { chain_id }
            },
        }
    }
}
