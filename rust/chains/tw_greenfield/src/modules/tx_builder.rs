// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::GreenfieldAddress;
use crate::public_key::GreenfieldPublicKey;
use crate::transaction::message::GreenfieldMessageBox;
use crate::transaction::{
    GreenfieldFee, GreenfieldSignMode, GreenfieldSignerInfo, GreenfieldTxBody,
    GreenfieldUnsignedTransaction,
};
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_cosmos_sdk::public_key::CosmosPublicKey;
use tw_cosmos_sdk::transaction::{Coin, SignerInfo};
use tw_misc::traits::OptionalEmpty;
use tw_number::U256;
use tw_proto::Greenfield::Proto;

const DEFAULT_TIMEOUT_HEIGHT: u64 = 0;

/// [`TxBuilder`] is used to build `UnsignedTransaction`
/// from the `TW::Greenfield::Proto::SigningInput` protobuf message.
pub struct TxBuilder;

impl TxBuilder {
    /// Please note that [`Proto::SigningInput::public_key`] must be set.
    /// If the public key should be derived from a private key, please do it before this method is called.
    pub fn unsigned_tx_from_proto(
        coin: &dyn CoinContext,
        input: &Proto::SigningInput<'_>,
    ) -> SigningResult<GreenfieldUnsignedTransaction> {
        let signer = Self::signer_info_from_proto(coin, input)?;

        let fee = input
            .fee
            .as_ref()
            .ok_or(SigningError(SigningErrorType::Error_wrong_fee))?;
        let fee = Self::fee_from_proto(fee, &signer)?;

        let eth_chain_id = U256::from_str(&input.eth_chain_id)
            .map_err(|_| SigningError(SigningErrorType::Error_invalid_params))?;

        Ok(GreenfieldUnsignedTransaction {
            signer,
            fee,
            cosmos_chain_id: input.cosmos_chain_id.to_string(),
            eth_chain_id,
            account_number: input.account_number,
            tx_body: Self::tx_body_from_proto(input)?,
        })
    }

    pub fn signer_info_from_proto(
        coin: &dyn CoinContext,
        input: &Proto::SigningInput,
    ) -> SigningResult<GreenfieldSignerInfo> {
        let public_key_params = None;
        let public_key =
            GreenfieldPublicKey::from_bytes(coin, &input.public_key, public_key_params)?;
        let sign_mode = match input.signing_mode {
            Proto::SigningMode::Eip712 => GreenfieldSignMode::Eip712,
        };
        Ok(SignerInfo {
            public_key,
            sequence: input.sequence,
            sign_mode: sign_mode.into(),
        })
    }

    fn fee_from_proto(
        input: &Proto::Fee,
        signer: &GreenfieldSignerInfo,
    ) -> SigningResult<GreenfieldFee> {
        let payer = GreenfieldAddress::with_secp256k1_pubkey(&signer.public_key.0);

        let amounts = input
            .amounts
            .iter()
            .map(Self::coin_from_proto)
            .collect::<SigningResult<_>>()?;
        Ok(GreenfieldFee {
            amounts,
            gas_limit: input.gas,
            payer: Some(payer),
            granter: None,
        })
    }

    fn coin_from_proto(input: &Proto::Amount<'_>) -> SigningResult<Coin> {
        let amount = U256::from_str(&input.amount)?;
        Ok(Coin {
            amount,
            denom: input.denom.to_string(),
        })
    }

    fn tx_body_from_proto(input: &Proto::SigningInput<'_>) -> SigningResult<GreenfieldTxBody> {
        if input.messages.is_empty() {
            return Err(SigningError(SigningErrorType::Error_invalid_params));
        }

        let messages = input
            .messages
            .iter()
            .map(Self::tx_message_from_proto)
            .collect::<SigningResult<_>>()?;

        Ok(GreenfieldTxBody {
            messages,
            memo: input.memo.to_string(),
            timeout_height: DEFAULT_TIMEOUT_HEIGHT,
        })
    }

    pub fn tx_message_from_proto(input: &Proto::Message) -> SigningResult<GreenfieldMessageBox> {
        use Proto::mod_Message::OneOfmessage_oneof as MessageEnum;

        match input.message_oneof {
            MessageEnum::send_coins_message(ref send) => Self::send_msg_from_proto(send),
            MessageEnum::bridge_transfer_out(ref transfer_out) => {
                Self::bridge_transfer_out_from_proto(transfer_out)
            },
            MessageEnum::None => Err(SigningError(SigningErrorType::Error_invalid_params)),
        }
    }

    pub fn send_msg_from_proto(
        send: &Proto::mod_Message::Send<'_>,
    ) -> SigningResult<GreenfieldMessageBox> {
        use crate::transaction::message::send_order::GreenfieldSendMessage;
        use tw_cosmos_sdk::transaction::message::cosmos_bank_message::SendMessage;

        let amounts = send
            .amounts
            .iter()
            .map(Self::coin_from_proto)
            .collect::<SigningResult<_>>()?;
        let msg = SendMessage {
            custom_type_prefix: send.type_prefix.to_string().empty_or_some(),
            from_address: GreenfieldAddress::from_str(&send.from_address)?,
            to_address: GreenfieldAddress::from_str(&send.to_address)?,
            amount: amounts,
        };
        Ok(Box::new(GreenfieldSendMessage(msg)))
    }

    pub fn bridge_transfer_out_from_proto(
        transfer_out: &Proto::mod_Message::BridgeTransferOut<'_>,
    ) -> SigningResult<GreenfieldMessageBox> {
        use crate::transaction::message::transfer_out::GreenfieldTransferOut;

        let amount = transfer_out
            .amount
            .as_ref()
            .ok_or(SigningError(SigningErrorType::Error_wrong_fee))?;

        let msg = GreenfieldTransferOut {
            custom_type_prefix: transfer_out.type_prefix.to_string().empty_or_some(),
            amount: Self::coin_from_proto(amount)?,
            from: GreenfieldAddress::from_str(&transfer_out.from_address)?,
            to: GreenfieldAddress::from_str(&transfer_out.to_address)?,
        };
        Ok(Box::new(msg))
    }
}
