// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::TonAddress;
use crate::signing_request::{
    JettonTransferRequest, SigningRequest, TransferPayload, TransferRequest,
};
use crate::wallet::{wallet_v4, wallet_v5, VersionedTonWallet};
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_keypair::ed25519::sha512::{KeyPair, PublicKey};
use tw_number::U256;
use tw_proto::TheOpenNetwork::Proto;
use tw_ton_sdk::error::cell_to_signing_error;
use Proto::mod_Transfer::OneOfpayload as PayloadType;

const STATE_INIT_EXPIRE_AT: u32 = 0xffffffff;

pub struct SigningRequestBuilder;

impl SigningRequestBuilder {
    pub fn build(input: &Proto::SigningInput) -> SigningResult<SigningRequest> {
        let wallet = Self::wallet(input)?;

        let messages = input
            .messages
            .iter()
            .map(Self::transfer_request)
            .collect::<SigningResult<Vec<_>>>()?;

        let expire_at = if input.sequence_number == 0 {
            STATE_INIT_EXPIRE_AT
        } else if input.expire_at == 0 {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context("'expire_at' must be set");
        } else {
            input.expire_at
        };

        Ok(SigningRequest {
            wallet,
            messages,
            expire_at,
            seqno: input.sequence_number,
        })
    }

    /// Currently, V4R2 and V5R1 wallets supported.
    fn wallet(input: &Proto::SigningInput) -> SigningResult<VersionedTonWallet> {
        if !input.private_key.is_empty() {
            let key_pair = KeyPair::try_from(input.private_key.as_ref())
                .into_tw()
                .context("Invalid private key")?;

            return match input.wallet_version {
                Proto::WalletVersion::WALLET_V4_R2 => Ok(VersionedTonWallet::V4R2(
                    wallet_v4::WalletV4R2::std_with_key_pair(&key_pair)
                        .map_err(cell_to_signing_error)?,
                )),
                Proto::WalletVersion::WALLET_V5_R1 => Ok(VersionedTonWallet::V5R1(
                    wallet_v5::WalletV5R1::std_with_key_pair(&key_pair)
                        .map_err(cell_to_signing_error)?,
                )),
                _ => SigningError::err(SigningErrorType::Error_not_supported)
                    .context("Wallet version not supported"),
            };
        }

        let public_key = PublicKey::try_from(input.public_key.as_ref())
            .into_tw()
            .context("Expected either 'private_key' or 'public_key' to be set")?;

        match input.wallet_version {
            Proto::WalletVersion::WALLET_V4_R2 => Ok(VersionedTonWallet::V4R2(
                wallet_v4::WalletV4R2::std_with_public_key(public_key)
                    .map_err(cell_to_signing_error)?,
            )),
            Proto::WalletVersion::WALLET_V5_R1 => Ok(VersionedTonWallet::V5R1(
                wallet_v5::WalletV5R1::std_with_public_key(public_key)
                    .map_err(cell_to_signing_error)?,
            )),
            _ => SigningError::err(SigningErrorType::Error_not_supported)
                .context("Wallet version not supported"),
        }
    }

    fn transfer_request(input: &Proto::Transfer) -> SigningResult<TransferRequest> {
        let dest = TonAddress::from_str(input.dest.as_ref())
            .into_tw()
            .context("Invalid 'dest' address")?
            // Set the 'bounceable' flag explicitly as specified in the Protobuf.
            .set_bounceable(input.bounceable);

        let comment = if input.comment.is_empty() {
            None
        } else {
            Some(input.comment.to_string())
        };

        let state_init = if input.state_init.is_empty() {
            None
        } else {
            Some(input.state_init.to_string())
        };

        let mode = u8::try_from(input.mode)
            .tw_err(|_| SigningErrorType::Error_invalid_params)
            .context("'mode' must fit uint8")?;

        let payload = match input.payload {
            PayloadType::jetton_transfer(ref jetton) => {
                Some(Self::jetton_transfer_request(jetton)?)
            },
            PayloadType::custom_payload(ref custom) => Some(Self::custom_request(custom)?),
            PayloadType::None => None,
        };

        Ok(TransferRequest {
            dest,
            ton_amount: U256::from(input.amount),
            mode,
            comment,
            state_init,
            payload,
        })
    }

    fn jetton_transfer_request(input: &Proto::JettonTransfer) -> SigningResult<TransferPayload> {
        let dest = TonAddress::from_str(input.to_owner.as_ref())
            .into_tw()
            .context("Invalid 'dest' address")?;

        let response_address = TonAddress::from_str(input.response_address.as_ref())
            .into_tw()
            .context("Invalid 'response_address' address")?;

        let custom_payload = if input.custom_payload.is_empty() {
            None
        } else {
            Some(input.custom_payload.to_string())
        };

        let jetton_payload = JettonTransferRequest {
            query_id: input.query_id,
            jetton_amount: U256::from(input.jetton_amount),
            dest,
            response_address,
            custom_payload,
            forward_ton_amount: U256::from(input.forward_amount),
        };

        Ok(TransferPayload::JettonTransfer(jetton_payload))
    }

    fn custom_request(input: &str) -> SigningResult<TransferPayload> {
        Ok(TransferPayload::Custom(input.to_string()))
    }
}
