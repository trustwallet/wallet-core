// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::address::{Address, CosmosAddress};
use crate::context::CosmosContext;
use crate::public_key::CosmosPublicKey;
use crate::transaction::message::standard_cosmos_message::SendMessage;
use crate::transaction::message::{SerializeMessage, SerializeMessageBox};
use crate::transaction::{Coin, Fee, SignMode, SignerInfo, TxBody, UnsignedTransaction};
use std::marker::PhantomData;
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_number::U256;
use tw_proto::Cosmos::Proto;

const DEFAULT_TIMEOUT_HEIGHT: u64 = 0;

pub struct TxBuilder<Context> {
    _phantom: PhantomData<Context>,
}

impl<Context> TxBuilder<Context>
where
    Context: CosmosContext,
{
    /// Please note that [`Proto::SigningInput::public_key`] must be set.
    /// If the public key should be derived from a private key, please do it before this method is called.
    pub fn unsigned_tx_from_proto(
        coin: &dyn CoinContext,
        input: &Proto::SigningInput<'_>,
    ) -> SigningResult<UnsignedTransaction<Context>> {
        let fee = input
            .fee
            .as_ref()
            .ok_or(SigningError(SigningErrorType::Error_wrong_fee))?;
        let signer = Self::signer_info_from_proto(coin, input)?;

        Ok(UnsignedTransaction {
            signer,
            fee: Self::fee_from_proto(fee)?,
            chain_id: input.chain_id.to_string(),
            account_number: input.account_number,
            tx_body: Self::tx_body_from_proto(coin, input)?,
        })
    }

    pub fn signer_info_from_proto(
        coin: &dyn CoinContext,
        input: &Proto::SigningInput,
    ) -> SigningResult<SignerInfo<Context::PublicKey>> {
        let public_key = Context::PublicKey::from_bytes(coin, &input.public_key)?;
        Ok(SignerInfo {
            public_key,
            sequence: input.sequence,
            // At this moment, we support the Direct signing mode only.
            sign_mode: SignMode::Direct,
        })
    }

    fn fee_from_proto(input: &Proto::Fee) -> SigningResult<Fee<Context::Address>> {
        let amounts = input
            .amounts
            .iter()
            .map(Self::coin_from_proto)
            .collect::<SigningResult<_>>()?;
        Ok(Fee {
            amounts,
            gas_limit: input.gas,
            payer: None,
            granter: None,
        })
    }

    fn coin_from_proto(input: &Proto::Amount<'_>) -> SigningResult<Coin> {
        let amount = U256::from_str(&input.amount)?;
        Ok(Coin {
            denom: input.denom.to_string(),
            amount,
        })
    }

    fn tx_body_from_proto(
        coin: &dyn CoinContext,
        input: &Proto::SigningInput<'_>,
    ) -> SigningResult<TxBody> {
        let messages = input
            .messages
            .iter()
            .map(|msg| Self::tx_message(coin, msg))
            .collect::<SigningResult<_>>()?;

        Ok(TxBody {
            messages,
            memo: input.memo.to_string(),
            timeout_height: DEFAULT_TIMEOUT_HEIGHT,
        })
    }

    fn tx_message(
        coin: &dyn CoinContext,
        input: &Proto::Message,
    ) -> SigningResult<SerializeMessageBox> {
        use Proto::mod_Message::OneOfmessage_oneof as MessageEnum;

        match input.message_oneof {
            MessageEnum::send_coins_message(ref send) => Self::send_msg_from_proto(coin, send),
            _ => todo!(),
        }
    }

    pub fn send_msg_from_proto(
        coin: &dyn CoinContext,
        send: &Proto::mod_Message::Send,
    ) -> SigningResult<SerializeMessageBox> {
        let amounts = send
            .amounts
            .iter()
            .map(Self::coin_from_proto)
            .collect::<SigningResult<_>>()?;
        let msg = SendMessage {
            from: Address::from_str_with_coin(coin, &send.from_address)?,
            to: Address::from_str_with_coin(coin, &send.to_address)?,
            amounts,
        };
        Ok(msg.into_boxed())
    }
}
