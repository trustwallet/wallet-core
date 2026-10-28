// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SuiAddress;
use crate::constants::{
    ADD_STAKE_MUL_COIN_FUN_NAME, SUI_SYSTEM_MODULE_NAME, SUI_SYSTEM_PACKAGE_ID,
    WITHDRAW_STAKE_FUN_NAME,
};
use crate::transaction::command::{Argument, Command};
use crate::transaction::programmable_transaction::{
    ProgrammableTransaction, ProgrammableTransactionBuilder,
};
use crate::transaction::raw_types::RawTransaction;
use crate::transaction::sui_types::{
    sui_coin_type_tag, CallArg, ObjectArg, ObjectRef, TransactionExpiration,
};
use crate::transaction::transaction_data::{TransactionData, TransactionKind};
use move_core_types::language_storage::TypeTag;
use tw_coin_entry::error::prelude::*;
use tw_encoding::bcs;

/// How a transaction pays for gas.
#[derive(Debug)]
pub enum GasPayment {
    /// A gas coin object.
    Coin(ObjectRef),
    /// SIP-58: `gas_budget` is withdrawn from the sender's SUI address balance and no gas object
    /// is referenced. The transaction then needs a [`TransactionExpiration::ValidDuring`] window.
    AddressBalance(TransactionExpiration),
}

impl GasPayment {
    /// The `gas_data.payment` objects and the transaction expiration this gas payment implies.
    fn into_parts(self) -> (Vec<ObjectRef>, Option<TransactionExpiration>) {
        match self {
            GasPayment::Coin(gas) => (vec![gas], None),
            GasPayment::AddressBalance(expiration) => (vec![], Some(expiration)),
        }
    }
}

/// SIP-58: `amount` of `coin_type` that is withdrawn from the sender's address balance.
#[derive(Debug)]
pub struct AddressBalanceWithdrawal {
    pub amount: u64,
    pub coin_type: TypeTag,
}

pub struct TransactionBuilder;

impl TransactionBuilder {
    /// `address_balance_amount` (SIP-58) is withdrawn from the sender's SUI address balance and staked
    /// together with `coins`; 0 means none.
    #[allow(clippy::too_many_arguments)]
    pub fn request_add_stake(
        signer: SuiAddress,
        coins: Vec<ObjectRef>,
        address_balance_amount: u64,
        amount: Option<u64>,
        validator: SuiAddress,
        gas: GasPayment,
        gas_budget: u64,
        gas_price: u64,
    ) -> SigningResult<TransactionData> {
        let obj_vec: Vec<_> = coins.into_iter().map(ObjectArg::ImmOrOwnedObject).collect();

        let pt = {
            let mut builder = ProgrammableTransactionBuilder::default();
            let system_state = builder.input(CallArg::SUI_SYSTEM_MUT).unwrap();
            let mut coin_args = obj_vec
                .into_iter()
                .map(|obj| builder.obj(obj))
                .collect::<SigningResult<Vec<_>>>()?;
            if address_balance_amount > 0 {
                coin_args.push(builder.redeem_funds(address_balance_amount, &sui_coin_type_tag())?);
            }
            let coins_vec = builder.command(Command::MakeMoveVec(None, coin_args));
            let arguments = vec![
                system_state,
                coins_vec,
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
        let (gas_payment, expiration) = gas.into_parts();
        Ok(TransactionData::new(
            TransactionKind::ProgrammableTransaction(pt),
            signer,
            gas_payment,
            gas_budget,
            gas_price,
            expiration,
        ))
    }

    pub fn request_withdraw_stake(
        signer: SuiAddress,
        staked_sui: ObjectRef,
        gas: GasPayment,
        gas_budget: u64,
        gas_price: u64,
    ) -> SigningResult<TransactionData> {
        let pt = {
            let mut builder = ProgrammableTransactionBuilder::default();
            builder.move_call(
                SUI_SYSTEM_PACKAGE_ID,
                SUI_SYSTEM_MODULE_NAME.to_owned(),
                WITHDRAW_STAKE_FUN_NAME.to_owned(),
                vec![],
                vec![
                    CallArg::SUI_SYSTEM_MUT,
                    CallArg::Object(ObjectArg::ImmOrOwnedObject(staked_sui)),
                ],
            )?;
            builder.finish()
        };
        let (gas_payment, expiration) = gas.into_parts();
        Ok(TransactionData::new(
            TransactionKind::ProgrammableTransaction(pt),
            signer,
            gas_payment,
            gas_budget,
            gas_price,
            expiration,
        ))
    }

    /// Send `Coin<T>` to a list of addresses, where T can be any coin type, following a list of amounts.
    /// The object specified in the gas field will be used to pay the gas fee for the transaction.
    /// The gas object can not appear in input_coins.
    /// https://docs.sui.io/sui-api-ref#unsafe_pay
    #[allow(clippy::too_many_arguments)]
    pub fn pay(
        signer: SuiAddress,
        input_coins: Vec<ObjectRef>,
        withdrawal: Option<AddressBalanceWithdrawal>,
        recipients: Vec<SuiAddress>,
        amounts: Vec<u64>,
        gas: GasPayment,
        gas_budget: u64,
        gas_price: u64,
    ) -> SigningResult<TransactionData> {
        if let GasPayment::Coin(gas_coin) = &gas {
            if input_coins.iter().any(|coin| coin.0 == gas_coin.0) {
                return SigningError::err(SigningErrorType::Error_invalid_params).context(
                    "Gas coin is in input coins of Pay transaction, use PaySui transaction instead!",
                );
            }
        }

        let pt = {
            let mut builder = ProgrammableTransactionBuilder::default();
            match withdrawal {
                Some(w) => builder.pay_from_funds(
                    input_coins,
                    w.amount,
                    &w.coin_type,
                    recipients,
                    amounts,
                )?,
                None => builder.pay(input_coins, recipients, amounts)?,
            }
            builder.finish()
        };
        let (gas_payment, expiration) = gas.into_parts();
        Ok(TransactionData::new(
            TransactionKind::ProgrammableTransaction(pt),
            signer,
            gas_payment,
            gas_budget,
            gas_price,
            expiration,
        ))
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

    /// SIP-58 variant of [`Self::pay_sui`]: besides `input_coins`, `address_balance_amount` MIST is
    /// withdrawn from the sender's SUI address balance, and gas is paid either with the first input
    /// coin (`expiration` is `None`, as before) or from the address balance (`expiration` is the
    /// `ValidDuring` window; then no input coin is needed or reserved for gas).
    ///
    /// With neither a withdrawal nor an expiration this is exactly [`Self::pay_sui`].
    #[allow(clippy::too_many_arguments)]
    pub fn pay_sui_funds(
        signer: SuiAddress,
        input_coins: Vec<ObjectRef>,
        address_balance_amount: u64,
        recipients: Vec<SuiAddress>,
        amounts: Vec<u64>,
        gas_budget: u64,
        gas_price: u64,
        expiration: Option<TransactionExpiration>,
    ) -> SigningResult<TransactionData> {
        let Some(expiration) = expiration else {
            if address_balance_amount == 0 {
                return Self::pay_sui(
                    signer,
                    input_coins,
                    recipients,
                    amounts,
                    gas_budget,
                    gas_price,
                );
            }
            if input_coins.is_empty() {
                return SigningError::err(SigningErrorType::Error_invalid_params).context(
                    "An address balance withdrawal needs a gas coin, or a `valid_during` window to pay gas from the address balance",
                );
            }
            // Gas is paid with the input coins (smashed into the gas coin). The withdrawal joins them.
            let pt = {
                let mut builder = ProgrammableTransactionBuilder::default();
                let redeemed =
                    builder.redeem_funds(address_balance_amount, &sui_coin_type_tag())?;
                builder.merge_coins(Argument::GasCoin, vec![redeemed]);
                builder.pay_sui(recipients, amounts)?;
                builder.finish()
            };
            return Ok(TransactionData::new(
                TransactionKind::ProgrammableTransaction(pt),
                signer,
                input_coins,
                gas_budget,
                gas_price,
                None,
            ));
        };

        let pt = {
            let mut builder = ProgrammableTransactionBuilder::default();
            builder.pay_from_funds(
                input_coins,
                address_balance_amount,
                &sui_coin_type_tag(),
                recipients,
                amounts,
            )?;
            builder.finish()
        };
        Ok(TransactionData::new(
            TransactionKind::ProgrammableTransaction(pt),
            signer,
            vec![],
            gas_budget,
            gas_price,
            Some(expiration),
        ))
    }

    /// SIP-58 variant of [`Self::pay_all_sui`], see [`Self::pay_sui_funds`] for the parameters.
    ///
    /// With neither a withdrawal nor an expiration this is exactly [`Self::pay_all_sui`].
    pub fn pay_all_sui_funds(
        signer: SuiAddress,
        input_coins: Vec<ObjectRef>,
        address_balance_amount: u64,
        recipient: SuiAddress,
        gas_budget: u64,
        gas_price: u64,
        expiration: Option<TransactionExpiration>,
    ) -> SigningResult<TransactionData> {
        let Some(expiration) = expiration else {
            if address_balance_amount == 0 {
                return Self::pay_all_sui(signer, input_coins, recipient, gas_budget, gas_price);
            }
            if input_coins.is_empty() {
                return SigningError::err(SigningErrorType::Error_invalid_params).context(
                    "An address balance withdrawal needs a gas coin, or a `valid_during` window to pay gas from the address balance",
                );
            }
            let pt = {
                let mut builder = ProgrammableTransactionBuilder::default();
                let redeemed =
                    builder.redeem_funds(address_balance_amount, &sui_coin_type_tag())?;
                builder.merge_coins(Argument::GasCoin, vec![redeemed]);
                builder.pay_all_sui(recipient);
                builder.finish()
            };
            return Ok(TransactionData::new(
                TransactionKind::ProgrammableTransaction(pt),
                signer,
                input_coins,
                gas_budget,
                gas_price,
                None,
            ));
        };

        let pt = {
            let mut builder = ProgrammableTransactionBuilder::default();
            builder.pay_all_sui_from_funds(input_coins, address_balance_amount, recipient)?;
            builder.finish()
        };
        Ok(TransactionData::new(
            TransactionKind::ProgrammableTransaction(pt),
            signer,
            vec![],
            gas_budget,
            gas_price,
            Some(expiration),
        ))
    }

    pub fn transfer_object(
        signer: SuiAddress,
        object: ObjectRef,
        recipient: SuiAddress,
        gas: GasPayment,
        gas_budget: u64,
        gas_price: u64,
    ) -> SigningResult<TransactionData> {
        let mut builder = ProgrammableTransactionBuilder::default();
        builder.transfer_object(recipient, object)?;

        let (gas_payment, expiration) = gas.into_parts();
        Ok(TransactionData::new(
            TransactionKind::ProgrammableTransaction(builder.finish()),
            signer,
            gas_payment,
            gas_budget,
            gas_price,
            expiration,
        ))
    }

    pub fn raw_json(
        raw_json: &str,
        gas_budget: u64,
        gas_price: u64,
    ) -> SigningResult<TransactionData> {
        let raw_transaction: RawTransaction = serde_json::from_str(raw_json)
            .map_err(|e| SigningError::from(e).context("Failed to parse raw JSON"))?;

        if raw_transaction.version != 1 {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context("Invalid transaction version. Only version 1 is supported.");
        }

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
}
