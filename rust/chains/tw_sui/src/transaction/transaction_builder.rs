// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SuiAddress;
use crate::constants::{
    ADD_STAKE_MUL_COIN_FUN_NAME, SUI_SYSTEM_MODULE_NAME, SUI_SYSTEM_PACKAGE_ID,
    WITHDRAW_STAKE_FUN_NAME,
};
use crate::transaction::command::Command;
use crate::transaction::programmable_transaction::{
    ProgrammableTransaction, ProgrammableTransactionBuilder,
};
use crate::transaction::raw_types::RawTransaction;
use crate::transaction::raw_types_v2::RawTransactionV2;
use crate::transaction::sui_types::{CallArg, ObjectArg, ObjectRef, TransactionExpiration};
use crate::transaction::transaction_data::{TransactionData, TransactionKind};
use serde::Deserialize;
use tw_coin_entry::error::prelude::*;
use tw_encoding::bcs;

pub struct TransactionBuilder;

impl TransactionBuilder {
    pub fn request_add_stake(
        signer: SuiAddress,
        coins: Vec<ObjectRef>,
        amount: Option<u64>,
        validator: SuiAddress,
        gas: ObjectRef,
        gas_budget: u64,
        gas_price: u64,
    ) -> SigningResult<TransactionData> {
        let obj_vec: Vec<_> = coins.into_iter().map(ObjectArg::ImmOrOwnedObject).collect();

        let pt = {
            let mut builder = ProgrammableTransactionBuilder::default();
            let arguments = vec![
                builder.input(CallArg::SUI_SYSTEM_MUT).unwrap(),
                builder.make_obj_vec(obj_vec)?,
                builder.input(CallArg::Pure(bcs::encode(&amount)?)).unwrap(),
                builder
                    .input(CallArg::Pure(bcs::encode(&validator)?))
                    .unwrap(),
            ];
            builder.command(Command::move_call(
                SUI_SYSTEM_PACKAGE_ID,
                SUI_SYSTEM_MODULE_NAME.to_owned(),
                ADD_STAKE_MUL_COIN_FUN_NAME.to_owned(),
                vec![],
                arguments,
            ));
            builder.finish()
        };
        Ok(TransactionData::new_programmable(
            signer,
            vec![gas],
            pt,
            gas_budget,
            gas_price,
        ))
    }

    pub fn request_withdraw_stake(
        signer: SuiAddress,
        staked_sui: ObjectRef,
        gas: ObjectRef,
        gas_budget: u64,
        gas_price: u64,
    ) -> SigningResult<TransactionData> {
        TransactionData::new_move_call(
            signer,
            SUI_SYSTEM_PACKAGE_ID,
            SUI_SYSTEM_MODULE_NAME.to_owned(),
            WITHDRAW_STAKE_FUN_NAME.to_owned(),
            vec![],
            gas,
            vec![
                CallArg::SUI_SYSTEM_MUT,
                CallArg::Object(ObjectArg::ImmOrOwnedObject(staked_sui)),
            ],
            gas_budget,
            gas_price,
        )
    }

    /// Send `Coin<T>` to a list of addresses, where T can be any coin type, following a list of amounts.
    /// The object specified in the gas field will be used to pay the gas fee for the transaction.
    /// The gas object can not appear in input_coins.
    #[allow(clippy::too_many_arguments)]
    pub fn pay(
        signer: SuiAddress,
        input_coins: Vec<ObjectRef>,
        recipients: Vec<SuiAddress>,
        amounts: Vec<u64>,
        gas: ObjectRef,
        gas_budget: u64,
        gas_price: u64,
    ) -> SigningResult<TransactionData> {
        if input_coins.iter().any(|coin| coin.0 == gas.0) {
            return SigningError::err(SigningErrorType::Error_invalid_params).context(
                "Gas coin is in input coins of Pay transaction, use PaySui transaction instead!",
            );
        }

        TransactionData::new_pay(
            signer,
            input_coins,
            recipients,
            amounts,
            gas,
            gas_budget,
            gas_price,
        )
    }

    /// Send SUI coins to a list of addresses, following a list of amounts.
    /// This is for SUI coin only and does not require a separate gas coin object.
    pub fn pay_sui(
        signer: SuiAddress,
        mut input_coins: Vec<ObjectRef>,
        recipients: Vec<SuiAddress>,
        amounts: Vec<u64>,
        gas_budget: u64,
        gas_price: u64,
    ) -> SigningResult<TransactionData> {
        if input_coins.is_empty() {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context("Empty input coins for Pay related transaction");
        }

        let gas_object_ref = input_coins.remove(0);
        TransactionData::new_pay_sui(
            signer,
            input_coins,
            recipients,
            amounts,
            gas_object_ref,
            gas_budget,
            gas_price,
        )
    }

    /// Send all SUI coins to one recipient.
    /// This is for SUI coin only and does not require a separate gas coin object.
    pub fn pay_all_sui(
        signer: SuiAddress,
        mut input_coins: Vec<ObjectRef>,
        recipient: SuiAddress,
        gas_budget: u64,
        gas_price: u64,
    ) -> SigningResult<TransactionData> {
        if input_coins.is_empty() {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context("Empty input coins for Pay related transaction");
        }

        let gas_object_ref = input_coins.remove(0);
        Ok(TransactionData::new_pay_all_sui(
            signer,
            input_coins,
            recipient,
            gas_object_ref,
            gas_budget,
            gas_price,
        ))
    }

    pub fn transfer_object(
        signer: SuiAddress,
        object: ObjectRef,
        recipient: SuiAddress,
        gas: ObjectRef,
        gas_budget: u64,
        gas_price: u64,
    ) -> SigningResult<TransactionData> {
        let mut builder = ProgrammableTransactionBuilder::default();
        builder.transfer_object(recipient, object)?;

        Ok(TransactionData::new(
            TransactionKind::ProgrammableTransaction(builder.finish()),
            signer,
            vec![gas],
            gas_budget,
            gas_price,
            None,
        ))
    }

    /// Builds a transaction out of a JSON serialized by the `@mysten/sui` TypeScript SDK.
    /// Both the version 1 (`Transaction.serialize()`) and version 2 (`Transaction.toJSON()`)
    /// formats are supported.
    pub fn raw_json(
        raw_json: &str,
        gas_budget: u64,
        gas_price: u64,
    ) -> SigningResult<TransactionData> {
        /// Only the `version` is deserialized here to choose the actual schema.
        #[derive(Deserialize)]
        struct RawTransactionVersion {
            version: u32,
        }

        let RawTransactionVersion { version } = serde_json::from_str(raw_json).map_err(|e| {
            SigningError::from(e).context("Failed to parse 'version' of the raw JSON transaction")
        })?;

        match version {
            1 => Self::raw_json_v1(raw_json, gas_budget, gas_price),
            2 => Self::raw_json_v2(raw_json, gas_budget, gas_price),
            _ => SigningError::err(SigningErrorType::Error_invalid_params).context(format!(
                "Invalid transaction version '{version}'. Only versions 1 and 2 are supported."
            )),
        }
    }

    fn raw_json_v1(
        raw_json: &str,
        gas_budget: u64,
        gas_price: u64,
    ) -> SigningResult<TransactionData> {
        let raw_transaction: RawTransaction = serde_json::from_str(raw_json)
            .map_err(|e| SigningError::from(e).context("Failed to parse raw JSON"))?;

        let mut raw_inputs = raw_transaction.inputs;
        raw_inputs.sort_by_key(|input| input.index);

        for (pos, input) in raw_inputs.iter().enumerate() {
            if input.index as usize != pos {
                return SigningError::err(SigningErrorType::Error_invalid_params).context(
                    format!(
                        "Input indices must be unique and contiguous (0..{}), but found index {} at position {}",
                        raw_inputs.len() - 1,
                        input.index,
                        pos
                    ),
                );
            }
        }

        let inputs = raw_inputs
            .into_iter()
            .map(|input| input.value.try_into())
            .collect::<SigningResult<Vec<_>>>()?;

        let commands = raw_transaction
            .transactions
            .into_iter()
            .map(|transaction| transaction.try_into())
            .collect::<SigningResult<Vec<_>>>()?;

        let pt = ProgrammableTransaction { inputs, commands };
        let gas_payments = raw_transaction
            .gas_config
            .payment
            .into_iter()
            .map(|payment| payment.try_into())
            .collect::<SigningResult<Vec<_>>>()?;

        if gas_payments.is_empty() {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context("Empty gas payment in raw JSON transaction");
        }

        let gas_budget = if gas_budget != 0 {
            gas_budget
        } else {
            raw_transaction.gas_config.budget
        };

        let gas_price = if gas_price != 0 {
            gas_price
        } else {
            raw_transaction.gas_config.price
        };

        Ok(TransactionData::new(
            TransactionKind::ProgrammableTransaction(pt),
            raw_transaction.sender,
            gas_payments,
            gas_budget,
            gas_price,
            raw_transaction.expiration.map(|e| e.into()),
        ))
    }

    fn raw_json_v2(
        raw_json: &str,
        gas_budget: u64,
        gas_price: u64,
    ) -> SigningResult<TransactionData> {
        let raw_transaction: RawTransactionV2 = serde_json::from_str(raw_json)
            .map_err(|e| SigningError::from(e).context("Failed to parse raw JSON"))?;

        let sender = raw_transaction
            .sender
            .or_tw_err(SigningErrorType::Error_invalid_params)
            .context("No 'sender' specified in raw JSON transaction")?;

        // Unlike the version 1 format, inputs are positional - no index normalization is needed.
        let inputs = raw_transaction
            .inputs
            .into_iter()
            .map(CallArg::try_from)
            .collect::<SigningResult<Vec<_>>>()?;

        let commands = raw_transaction
            .commands
            .into_iter()
            .map(Command::try_from)
            .collect::<SigningResult<Vec<_>>>()?;

        let pt = ProgrammableTransaction { inputs, commands };

        let gas_data = raw_transaction.gas_data;
        let gas_payments = gas_data
            .payment
            .unwrap_or_default()
            .into_iter()
            .map(ObjectRef::try_from)
            .collect::<SigningResult<Vec<_>>>()?;

        if gas_payments.is_empty() {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context("Empty gas payment in raw JSON transaction");
        }

        // An explicitly provided `SigningInput.gas_budget` / `reference_gas_price` takes precedence
        // over the value embedded in the JSON, which is nullable in the version 2 format.
        let gas_budget = if gas_budget != 0 {
            gas_budget
        } else {
            gas_data
                .budget
                .or_tw_err(SigningErrorType::Error_invalid_params)
                .context(
                    "No gas budget specified neither in 'SigningInput' nor in raw JSON transaction",
                )?
                .0
        };

        let gas_price = if gas_price != 0 {
            gas_price
        } else {
            gas_data
                .price
                .or_tw_err(SigningErrorType::Error_invalid_params)
                .context(
                    "No gas price specified neither in 'SigningInput' nor in raw JSON transaction",
                )?
                .0
        };

        let expiration = raw_transaction
            .expiration
            .map(TransactionExpiration::try_from)
            .transpose()?;

        Ok(TransactionData::new_allow_sponsor(
            TransactionKind::ProgrammableTransaction(pt),
            sender,
            gas_payments,
            gas_budget,
            gas_price,
            // Defaults to the sender when no sponsor is set.
            gas_data.owner.unwrap_or(sender),
            expiration,
        ))
    }
}
