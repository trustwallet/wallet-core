// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::context::CosmosContext;
use crate::proto::cosmos::base::v1beta1 as base_proto;
use crate::proto::cosmos::signing::v1beta1 as signing_proto;
use crate::proto::cosmos::tx::v1beta1 as tx_proto;
use crate::public_key::ProtobufPublicKey;
use crate::transaction::{
    Coin, Fee, SignMode, SignedTransaction, SignerInfo, TxBody, UnsignedTransaction,
};
use std::marker::PhantomData;
use tw_coin_entry::error::prelude::*;
use tw_memory::Data;
use tw_proto::serialize;

pub fn build_coin(coin: &Coin) -> base_proto::Coin {
    base_proto::Coin {
        amount: coin.amount.to_string(),
        denom: coin.denom.clone(),
    }
}

/// `ProtobufSerializer` serializes Cosmos specific Protobuf messages.
pub struct ProtobufSerializer<Context> {
    _phantom: PhantomData<Context>,
}

pub struct SignDirectArgs {
    pub tx_body: Data,
    pub auth_info: Data,
    pub chain_id: String,
    pub account_number: u64,
}

impl<Context: CosmosContext> ProtobufSerializer<Context> {
    /// Serializes a signed transaction into the Cosmos [`tx_proto::TxRaw`] message.
    /// [`tx_proto::TxRaw`] can be broadcasted to the network.
    pub fn build_signed_tx(signed: &SignedTransaction<Context>) -> SigningResult<tx_proto::TxRaw> {
        let tx_body = Self::build_tx_body(&signed.tx_body)?;
        let body_bytes = serialize(&tx_body).expect("Unexpected error on tx_body serialization");

        let auth_info = Self::build_auth_info(&signed.signer, &signed.fee);
        let auth_info_bytes =
            serialize(&auth_info).expect("Unexpected error on auth_info serialization");

        Ok(tx_proto::TxRaw {
            body_bytes,
            auth_info_bytes,
            signatures: vec![signed.signature.clone()],
        })
    }

    pub fn build_direct_signed_tx(args: &SignDirectArgs, signature: Data) -> tx_proto::TxRaw {
        tx_proto::TxRaw {
            body_bytes: args.tx_body.clone(),
            auth_info_bytes: args.auth_info.clone(),
            signatures: vec![signature],
        }
    }

    /// Serializes an unsigned transaction into the Cosmos [`tx_proto::SignDoc`] message.
    /// [`tx_proto::SignDoc`] is used to generate a transaction prehash and sign it.
    pub fn build_sign_doc(
        unsigned: &UnsignedTransaction<Context>,
    ) -> SigningResult<tx_proto::SignDoc> {
        let tx_body = Self::build_tx_body(&unsigned.tx_body)?;
        let body_bytes = serialize(&tx_body).expect("Unexpected error on tx_body serialization");

        let auth_info = Self::build_auth_info(&unsigned.signer, &unsigned.fee);
        let auth_info_bytes =
            serialize(&auth_info).expect("Unexpected error on auth_info serialization");

        Ok(tx_proto::SignDoc {
            body_bytes,
            auth_info_bytes,
            chain_id: unsigned.chain_id.clone(),
            account_number: unsigned.account_number,
        })
    }

    pub fn build_direct_sign_doc(args: &SignDirectArgs) -> tx_proto::SignDoc {
        tx_proto::SignDoc {
            body_bytes: args.tx_body.clone(),
            auth_info_bytes: args.auth_info.clone(),
            chain_id: args.chain_id.clone(),
            account_number: args.account_number,
        }
    }

    pub fn build_auth_info(
        signer: &SignerInfo<Context::PublicKey>,
        fee: &Fee<Context::Address>,
    ) -> tx_proto::AuthInfo {
        tx_proto::AuthInfo {
            signer_infos: vec![Self::build_signer_info(signer)],
            fee: Some(Self::build_fee(fee)),
            // At this moment, we do not support transaction tip.
            tip: None,
        }
    }

    pub fn build_tx_body(tx_body: &TxBody) -> SigningResult<tx_proto::TxBody> {
        let messages: Vec<_> = tx_body
            .messages
            .iter()
            .map(|msg| msg.to_proto())
            .collect::<SigningResult<_>>()?;

        Ok(tx_proto::TxBody {
            messages,
            memo: tx_body.memo.clone(),
            timeout_height: tx_body.timeout_height,
            extension_options: Vec::default(),
            non_critical_extension_options: Vec::default(),
        })
    }

    pub fn build_signer_info(signer: &SignerInfo<Context::PublicKey>) -> tx_proto::SignerInfo {
        use tx_proto::mod_ModeInfo::{self as mode_info, OneOfsum as SumEnum};

        // Single is the mode info for a single signer. It is structured as a message
        // to allow for additional fields such as locale for SIGN_MODE_TEXTUAL in the future.
        let mode_info = tx_proto::ModeInfo {
            sum: SumEnum::single(mode_info::Single {
                mode: Self::build_sign_mode(signer.sign_mode),
            }),
        };

        tx_proto::SignerInfo {
            public_key: Some(signer.public_key.to_proto()),
            mode_info: Some(mode_info),
            sequence: signer.sequence,
        }
    }

    fn build_fee(fee: &Fee<Context::Address>) -> tx_proto::Fee {
        tx_proto::Fee {
            amount: fee.amounts.iter().map(build_coin).collect(),
            gas_limit: fee.gas_limit,
            // Ignore `payer` and `granter` even if they set.
            payer: String::default(),
            granter: String::default(),
        }
    }

    fn build_sign_mode(sign_mode: SignMode) -> signing_proto::SignMode {
        match sign_mode {
            SignMode::Direct => signing_proto::SignMode::SIGN_MODE_DIRECT,
            SignMode::Other(other) => signing_proto::SignMode::from(other),
        }
    }
}
