// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SuiAddress;
use crate::constants::{
    ADD_STAKE_MUL_COIN_FUN_NAME, SUI_SYSTEM_MODULE_NAME, SUI_SYSTEM_PACKAGE_ID,
    WITHDRAW_STAKE_FUN_NAME,
};
use crate::transaction::command::Command;
use crate::transaction::programmable_transaction::ProgrammableTransactionBuilder;
use crate::transaction::sui_types::{CallArg, ObjectArg, ObjectRef};
use crate::transaction::transaction_data::TransactionData;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_encoding::bcs;

pub struct TransactionBuilder;

impl TransactionBuilder {
    pub fn request_add_stake(
        signer: SuiAddress,
        // TODO ObjectRef as a struct
        coins: Vec<ObjectRef>,
        amount: Option<u64>,
        validator: SuiAddress,
        // TODO it was optional
        gas: ObjectRef,
        gas_budget: u64,
        gas_price: u64,
    ) -> SigningResult<TransactionData> {
        // let gas_price = self.0.get_reference_gas_price().await?;
        // let gas = self
        //     .select_gas(signer, gas, gas_budget, coins.clone(), gas_price)
        //     .await?;

        let obj_vec: Vec<_> = coins.into_iter().map(ObjectArg::ImmOrOwnedObject).collect();

        // let mut obj_vec = vec![];
        // let oref = coins
        //     .pop()
        //     .ok_or_else(|| anyhow!("Coins input should contain at lease one coin object."))?;
        // let (oref, coin_type) = self.get_object_ref_and_type(coin).await?;
        //
        // let ObjectType::Struct(type_) = &coin_type else {
        //     return Err(anyhow!("Provided object [{coin}] is not a move object."));
        // };
        // ensure!(
        //     type_.is_coin(),
        //     "Expecting either Coin<T> input coin objects. Received [{type_}]"
        // );
        // for coin_ref in coins {
        //     let (oref, type_) = self.get_object_ref_and_type(coin).await?;
        //     ensure!(
        //         type_ == coin_type,
        //         "All coins should be the same type, expecting {coin_type}, got {type_}."
        //     );
        //     obj_vec.push(ObjectArg::ImmOrOwnedObject(coin_ref))
        // }

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
        &self,
        signer: SuiAddress,
        staked_sui: ObjectRef,
        gas: ObjectRef,
        gas_budget: u64,
        gas_price: u64,
    ) -> SigningResult<TransactionData> {
        // let staked_sui = self.get_object_ref(staked_sui).await?;
        // let gas = self
        //     .select_gas(signer, gas, gas_budget, vec![], gas_price)
        //     .await?;
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
    /// The gas object can not appear in input_coins. If the gas object is not specified, the RPC server will auto-select one.
    #[allow(clippy::too_many_arguments)]
    pub fn pay(
        &self,
        signer: SuiAddress,
        input_coins: Vec<ObjectRef>,
        recipients: Vec<SuiAddress>,
        amounts: Vec<u64>,
        // TODO was Optional
        gas: ObjectRef,
        gas_budget: u64,
        gas_price: u64,
    ) -> SigningResult<TransactionData> {
        if input_coins.iter().any(|coin| coin.0 == gas.0) {
            // Gas coin is in input coins of Pay transaction, use PaySui transaction instead!.
            return Err(SigningError(SigningErrorType::Error_invalid_params));
        }

        // let handles: Vec<_> = input_coins
        //     .iter()
        //     .map(|id| self.get_object_ref(*id))
        //     .collect();
        // let coin_refs = join_all(handles)
        //     .await
        //     .into_iter()
        //     .collect::<anyhow::Result<Vec<ObjectRef>>>()?;
        // let gas_price = self.0.get_reference_gas_price().await?;
        // let gas = self
        //     .select_gas(signer, gas, gas_budget, input_coins, gas_price)
        //     .await?;

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

    pub fn pay_sui(
        &self,
        signer: SuiAddress,
        mut input_coins: Vec<ObjectRef>,
        recipients: Vec<SuiAddress>,
        amounts: Vec<u64>,
        gas_budget: u64,
        gas_price: u64,
    ) -> SigningResult<TransactionData> {
        if input_coins.is_empty() {
            // "Empty input coins for Pay related transaction"
            return Err(SigningError(SigningErrorType::Error_invalid_params));
        }

        // let handles: Vec<_> = input_coins
        //     .into_iter()
        //     .map(|id| self.get_object_ref(id))
        //     .collect();
        // let mut coin_refs = join_all(handles)
        //     .await
        //     .into_iter()
        //     .collect::<anyhow::Result<Vec<ObjectRef>>>()?;

        // [0] is safe because input_coins is non-empty and coins are of same length as input_coins.
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

    pub fn pay_all_sui(
        &self,
        signer: SuiAddress,
        mut input_coins: Vec<ObjectRef>,
        recipient: SuiAddress,
        gas_budget: u64,
        gas_price: u64,
    ) -> SigningResult<TransactionData> {
        if input_coins.is_empty() {
            // "Empty input coins for Pay related transaction"
            return Err(SigningError(SigningErrorType::Error_invalid_params));
        }

        // let handles: Vec<_> = input_coins
        //     .into_iter()
        //     .map(|id| self.get_object_ref(id))
        //     .collect();
        //
        // let mut coin_refs = join_all(handles)
        //     .await
        //     .into_iter()
        //     .collect::<anyhow::Result<Vec<ObjectRef>>>()?;

        // [0] is safe because input_coins is non-empty and coins are of same length as input_coins.
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
}
