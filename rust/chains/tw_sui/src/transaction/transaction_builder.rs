// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use std::str::FromStr;

use crate::address::SuiAddress;
use crate::constants::{
    ADD_STAKE_MUL_COIN_FUN_NAME, SUI_SYSTEM_MODULE_NAME, SUI_SYSTEM_PACKAGE_ID,
    WITHDRAW_STAKE_FUN_NAME,
};
use crate::transaction::command::Command;
use crate::transaction::programmable_transaction::{
    ProgrammableTransaction, ProgrammableTransactionBuilder,
};
use crate::transaction::raw_types::{InputArg, RawTransaction, Transaction};
use crate::transaction::sui_types::{
    CallArg, ObjectArg, ObjectDigest, ObjectID, ObjectRef, SequenceNumber,
};
use crate::transaction::transaction_data::{TransactionData, TransactionKind};
use move_core_types::identifier::Identifier;
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
    /// https://docs.sui.io/sui-api-ref#unsafe_pay
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
    /// https://docs.sui.io/sui-api-ref#unsafe_paysui
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
    /// https://docs.sui.io/sui-api-ref#unsafe_payallsui
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
            gas,
            gas_budget,
            gas_price,
        ))
    }

    pub fn raw_json(
        raw_json: &str,
        gas_budget: u64,
        gas_price: u64,
    ) -> SigningResult<TransactionData> {
        let raw_transaction: RawTransaction = serde_json::from_str(raw_json)
            .map_err(|e| SigningError::from(e).context("Failed to parse raw JSON"))?;

        let inputs = raw_transaction
            .inputs
            .into_iter()
            .map(|input| -> SigningResult<CallArg> {
                match input.value {
                    InputArg::Pure(data) => Ok(CallArg::Pure(data)),
                    InputArg::Object(object) => Ok(CallArg::Object(object.try_into()?)),
                }
            })
            .collect::<SigningResult<Vec<_>>>()?;

        let commands = raw_transaction
            .transactions
            .into_iter()
            .map(|transaction| match transaction {
                Transaction::SplitCoins { coin, amounts } => Ok(Command::SplitCoins(
                    coin.into(),
                    amounts.into_iter().map(|amount| amount.into()).collect(),
                )),
                Transaction::MoveCall {
                    target,
                    type_arguments,
                    arguments,
                } => {
                    let parts: Vec<&str> = target.split("::").collect();
                    if parts.len() != 3 {
                        return SigningError::err(SigningErrorType::Error_invalid_params)
                            .context("Invalid target format for MoveCall command");
                    }
                    let package =
                        ObjectID::from_str(parts[0]).context("Failed to parse package ID")?;
                    let module = Identifier::from_str(parts[1])
                        .tw_err(|_| SigningErrorType::Error_invalid_params)
                        .context("Failed to parse module")?;
                    let function = Identifier::from_str(parts[2])
                        .tw_err(|_| SigningErrorType::Error_invalid_params)
                        .context("Failed to parse function")?;
                    Ok(Command::move_call(
                        package,
                        module,
                        function,
                        type_arguments.into_iter().map(|tag| tag.into()).collect(),
                        arguments
                            .into_iter()
                            .map(|argument| argument.into())
                            .collect(),
                    ))
                },
                Transaction::TransferObjects { objects, address } => Ok(Command::TransferObjects(
                    objects.into_iter().map(|object| object.into()).collect(),
                    address.into(),
                )),
            })
            .collect::<SigningResult<Vec<_>>>()?;

        let pt = ProgrammableTransaction { inputs, commands };
        let gas_payments = raw_transaction
            .gas_config
            .payment
            .into_iter()
            .map(|payment| {
                Ok((
                    ObjectID::from_str(&payment.object_id).context("Failed to parse object ID")?,
                    SequenceNumber(payment.version),
                    ObjectDigest::from_str(&payment.digest)
                        .context("Failed to parse object digest")?,
                ))
            })
            .collect::<SigningResult<Vec<_>>>()?;
        let Some(gas_payment) = gas_payments.first() else {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context("Gas payment is missing from the transaction");
        };

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
            *gas_payment,
            gas_budget,
            gas_price,
        ))
    }
}
