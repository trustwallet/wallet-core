// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::evm_context::EvmContext;
use std::borrow::Cow;
use std::marker::PhantomData;
use std::str::FromStr;
use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::error::SigningResult;
use tw_coin_entry::modules::input_builder::{BuildSigningInputArgs, InputBuilder};
use tw_number::U256;
use tw_proto::Ethereum::Proto;

#[derive(Default)]
pub struct EthInputBuilder<Context: EvmContext> {
    _phantom: PhantomData<Context>,
}

impl<Context: EvmContext> InputBuilder for EthInputBuilder<Context> {
    type SigningInput = Proto::SigningInput<'static>;

    fn build_signing_input(
        &self,
        _coin: &dyn CoinContext,
        args: BuildSigningInputArgs,
    ) -> SigningResult<Self::SigningInput> {
        let chain_id = if args.chain_id.is_empty() {
            U256::from(1_u8)
        } else {
            U256::from_str(&args.chain_id)?
        };

        // Check if the `to` argument is valid.
        Context::Address::from_str(&args.to)?;

        let amount = U256::from_str(&args.amount)?;
        let transfer = Proto::mod_Transaction::Transfer {
            amount: Cow::from(amount.to_big_endian_compact()),
            data: Cow::default(),
        };

        Ok(Proto::SigningInput {
            chain_id: Cow::from(chain_id.to_big_endian_compact()),
            to_address: Cow::from(args.to),
            transaction: Some(Proto::Transaction {
                transaction_oneof: Proto::mod_Transaction::OneOftransaction_oneof::transfer(
                    transfer,
                ),
            }),
            ..Proto::SigningInput::default()
        })
    }
}
