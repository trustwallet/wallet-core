// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::modules::intent::{Intent, IntentMessage, PersonalMessage};
use crate::signature::SuiSignatureInfo;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::modules::message_signer::MessageSigner;
use tw_coin_entry::signing_output_error;
use tw_encoding::bcs;
use tw_hash::blake2::blake2_b;
use tw_hash::H256;
use tw_keypair::ed25519;
use tw_keypair::traits::{KeyPairTrait, SigningKeyTrait, VerifyingKeyTrait};
use tw_memory::Data;
use tw_misc::traits::ToBytesVec;
use tw_misc::try_or_false;
use tw_proto::Sui::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;

pub struct SuiMessageSigner;

/// Sui personal message signer.
/// Here is an example of how to sign a message:
/// https://github.com/MystenLabs/sui/blob/a16c942b72c13f42846b3c543b6622af85a5f634/crates/sui-types/src/unit_tests/utils.rs#L201
impl SuiMessageSigner {
    pub fn sign_message_impl(
        _coin: &dyn CoinContext,
        input: Proto::MessageSigningInput,
    ) -> SigningResult<Proto::MessageSigningOutput<'static>> {
        let key_pair = ed25519::sha512::KeyPair::try_from(input.private_key.as_ref())?;

        let hash = Self::message_preimage_hashes_impl(input.message.as_bytes().into())?;

        let signature = key_pair.sign(hash.to_vec())?;
        let signature_info = SuiSignatureInfo::ed25519(&signature, key_pair.public());

        Ok(Proto::MessageSigningOutput {
            signature: signature_info.to_base64().into(),
            ..Proto::MessageSigningOutput::default()
        })
    }

    pub fn message_preimage_hashes_impl(message: Data) -> SigningResult<H256> {
        let data = PersonalMessage { message };
        let intent_msg = IntentMessage::new(Intent::personal_message(), data);

        let data_to_sign = bcs::encode(&intent_msg).tw_err(SigningErrorType::Error_internal)?;

        let data_to_sign = blake2_b(&data_to_sign, H256::LEN)
            .and_then(|hash| H256::try_from(hash.as_slice()))
            .tw_err(SigningErrorType::Error_internal)?;

        Ok(data_to_sign)
    }
}

impl MessageSigner for SuiMessageSigner {
    type MessageSigningInput<'a> = Proto::MessageSigningInput<'a>;
    type MessagePreSigningOutput = CompilerProto::PreSigningOutput<'static>;
    type MessageSigningOutput = Proto::MessageSigningOutput<'static>;
    type MessageVerifyingInput<'a> = Proto::MessageVerifyingInput<'a>;

    fn message_preimage_hashes(
        &self,
        _coin: &dyn CoinContext,
        input: Self::MessageSigningInput<'_>,
    ) -> Self::MessagePreSigningOutput {
        let hash = match Self::message_preimage_hashes_impl(input.message.as_bytes().into()) {
            Ok(hash) => hash,
            Err(e) => return signing_output_error!(CompilerProto::PreSigningOutput, e),
        };

        CompilerProto::PreSigningOutput {
            data: hash.to_vec().into(),
            data_hash: hash.to_vec().into(),
            ..CompilerProto::PreSigningOutput::default()
        }
    }

    fn sign_message(
        &self,
        coin: &dyn CoinContext,
        input: Self::MessageSigningInput<'_>,
    ) -> Self::MessageSigningOutput {
        Self::sign_message_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(Proto::MessageSigningOutput, e))
    }

    fn verify_message(
        &self,
        _coin: &dyn CoinContext,
        input: Self::MessageVerifyingInput<'_>,
    ) -> bool {
        let signature_info = try_or_false!(SuiSignatureInfo::from_base64(input.signature.as_ref()));
        let public_key = try_or_false!(ed25519::sha512::PublicKey::try_from(
            input.public_key.as_ref()
        ));

        // Check if the public key in the signature matches the public key in the input.
        if signature_info.public_key.ne(&public_key.to_bytes()) {
            return false;
        }
        let signature = try_or_false!(ed25519::Signature::try_from(
            signature_info.signature.as_slice()
        ));
        let hash = try_or_false!(Self::message_preimage_hashes_impl(
            input.message.as_bytes().to_vec()
        ));

        // Verify the signature.
        public_key.verify(signature, hash.to_vec())
    }
}
