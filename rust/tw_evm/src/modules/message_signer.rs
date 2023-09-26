// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::signature::{eip155_replay_protection, legacy_replay_protection};
use std::borrow::Cow;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_coin_entry::modules::message_signer::MessageSigner;
use tw_coin_entry::signing_output_error;
use tw_encoding::hex::ToHex;
use tw_hash::sha3::keccak256;
use tw_hash::{H256, H520};
use tw_keypair::ecdsa::secp256k1;
use tw_keypair::traits::SigningKeyTrait;
use tw_number::U256;
use tw_proto::Ethereum::Proto;
use tw_proto::TxCompiler::Proto as CompilerProto;

const ETHEREUM_PREFIX: u8 = 0x19;
const ETHEREUM_MESSAGE_PREFIX: &str = "Ethereum Signed Message:\n";

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

    fn verify_message(&self, _input: Self::MessageVerifyingInput<'_>) -> bool {
        todo!()
    }
}

pub enum SignatureType {
    Standard,
    Legacy,
    Eip155 { chain_id: u32 },
}

impl SignatureType {
    fn with_msg_type(msg_type: Proto::MessageType, chain_id: u32) -> SignatureType {
        match msg_type {
            Proto::MessageType::MessageType_immutable_x => SignatureType::Standard,
            Proto::MessageType::MessageType_legacy | Proto::MessageType::MessageType_typed => {
                SignatureType::Legacy
            },
            Proto::MessageType::MessageType_eip155
            | Proto::MessageType::MessageType_typed_eip155 => SignatureType::Eip155 { chain_id },
        }
    }
}

pub struct MessageSignature {
    r: H256,
    s: H256,
    v: u8,
}

impl MessageSignature {
    pub fn prepared(sign: secp256k1::Signature, sign_type: SignatureType) -> SigningResult<Self> {
        let v = match sign_type {
            SignatureType::Standard => U256::from(sign.v()),
            SignatureType::Legacy => legacy_replay_protection(sign.v()),
            SignatureType::Eip155 { chain_id } => {
                eip155_replay_protection(U256::from(chain_id), sign.v())
            },
        };
        // `v` should fit `u8` integer.
        let v = u8::try_from(v)?;
        Ok(MessageSignature {
            r: sign.r(),
            s: sign.s(),
            v,
        })
    }

    pub fn to_bytes(&self) -> H520 {
        let mut bytes = Vec::with_capacity(H520::LEN);
        bytes.extend_from_slice(self.r.as_slice());
        bytes.extend_from_slice(self.s.as_slice());
        bytes.push(self.v);

        H520::try_from(bytes.as_slice()).expect("'bytes' is 130 byte array")
    }
}

impl EthMessageSigner {
    pub fn preimage_hashes_eip191(msg: &str) -> CompilerProto::PreSigningOutput<'static> {
        let mut data = Vec::with_capacity(msg.len() * 2);

        data.push(ETHEREUM_PREFIX);
        data.extend_from_slice(ETHEREUM_MESSAGE_PREFIX.as_bytes());
        data.extend_from_slice(msg.len().to_string().as_bytes());
        data.extend_from_slice(msg.as_bytes());

        let hash = keccak256(&data);

        CompilerProto::PreSigningOutput {
            data_hash: Cow::Owned(hash),
            data: Cow::Owned(data),
            ..CompilerProto::PreSigningOutput::default()
        }
    }

    fn message_preimage_hashes_impl(
        input: Proto::MessageSigningInput<'_>,
    ) -> SigningResult<CompilerProto::PreSigningOutput<'static>> {
        match input.message_type {
            Proto::MessageType::MessageType_legacy
            | Proto::MessageType::MessageType_eip155
            | Proto::MessageType::MessageType_immutable_x => {
                Ok(Self::preimage_hashes_eip191(&input.message))
            },
            Proto::MessageType::MessageType_typed
            | Proto::MessageType::MessageType_typed_eip155 => {
                // TODO
                Err(SigningError(SigningErrorType::Error_not_supported))
            },
        }
    }

    fn sign_message_impl(
        input: Proto::MessageSigningInput<'_>,
    ) -> SigningResult<Proto::MessageSigningOutput<'static>> {
        let private_key = secp256k1::PrivateKey::try_from(input.private_key.as_ref())?;
        let signature_type = SignatureType::with_msg_type(input.message_type, input.chain_id);

        let preimage = Self::message_preimage_hashes_impl(input)?;
        let hash_to_sign = H256::try_from(preimage.data_hash.as_ref())
            .map_err(|_| SigningError(SigningErrorType::Error_internal))?;

        let secp_sign = private_key.sign(hash_to_sign)?;
        let prepared_sign = MessageSignature::prepared(secp_sign, signature_type)?;

        Ok(Proto::MessageSigningOutput {
            signature: Cow::Owned(prepared_sign.to_bytes().to_hex()),
            ..Proto::MessageSigningOutput::default()
        })
    }
}
