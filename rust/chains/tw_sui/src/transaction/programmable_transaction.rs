// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SuiAddress;
use crate::constants::{COIN_MODULE_NAME, REDEEM_FUNDS_FUN_NAME, SUI_FRAMEWORK_PACKAGE_ID};
use crate::transaction::command::{Argument, Command};
use crate::transaction::sui_types::{
    sui_coin_type_tag, CallArg, FundsWithdrawalArg, ObjectArg, ObjectID, ObjectRef,
};
use indexmap::IndexMap;
use move_core_types::identifier::Identifier;
use move_core_types::language_storage::TypeTag;
use serde::{Deserialize, Serialize};
use tw_coin_entry::error::prelude::*;
use tw_encoding::bcs;

/// A series of commands where the results of one command can be used in future
/// commands
#[derive(Debug, Deserialize, Serialize)]
pub struct ProgrammableTransaction {
    /// Input objects or primitive values
    pub inputs: Vec<CallArg>,
    /// The commands to be executed sequentially. A failure in any command will
    /// result in the failure of the entire transaction.
    pub commands: Vec<Command>,
}

#[derive(Eq, Hash, PartialEq)]
enum BuilderArg {
    Object(ObjectID),
    Pure(Vec<u8>),
    ForcedNonUniquePure(usize),
}

#[derive(Default)]
pub struct ProgrammableTransactionBuilder {
    inputs: IndexMap<BuilderArg, CallArg>,
    commands: Vec<Command>,
}

impl ProgrammableTransactionBuilder {
    pub fn finish(self) -> ProgrammableTransaction {
        let Self { inputs, commands } = self;
        let inputs = inputs.into_values().collect();
        ProgrammableTransaction { inputs, commands }
    }

    /// Will fail to generate if recipients and amounts do not have the same lengths.
    /// Or if coins is empty
    pub fn pay(
        &mut self,
        coins: Vec<ObjectRef>,
        recipients: Vec<SuiAddress>,
        amounts: Vec<u64>,
    ) -> SigningResult<()> {
        let mut coins = coins.into_iter();
        let Some(coin) = coins.next() else {
            // coins vector is empty
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context("No coins provided");
        };
        let coin_arg = self.obj(ObjectArg::ImmOrOwnedObject(coin))?;
        let merge_args: Vec<_> = coins
            .map(|c| self.obj(ObjectArg::ImmOrOwnedObject(c)))
            .collect::<Result<_, _>>()?;
        if !merge_args.is_empty() {
            self.command(Command::MergeCoins(coin_arg, merge_args));
        }
        self.pay_impl(recipients, amounts, coin_arg)
    }

    /// Will fail to generate if recipients and amounts do not have the same lengths
    pub fn pay_sui(&mut self, recipients: Vec<SuiAddress>, amounts: Vec<u64>) -> SigningResult<()> {
        self.pay_impl(recipients, amounts, Argument::GasCoin)
    }

    pub fn pay_all_sui(&mut self, recipient: SuiAddress) {
        let rec_arg = self.pure(recipient).unwrap();
        self.command(Command::TransferObjects(vec![Argument::GasCoin], rec_arg));
    }

    pub fn transfer_object(
        &mut self,
        recipient: SuiAddress,
        object_ref: ObjectRef,
    ) -> SigningResult<()> {
        let rec_arg = self.pure(recipient).unwrap();
        let obj_arg = self.obj(ObjectArg::ImmOrOwnedObject(object_ref));
        self.commands
            .push(Command::TransferObjects(vec![obj_arg?], rec_arg));
        Ok(())
    }

    /// Will fail to generate if given an empty ObjVec
    pub fn move_call(
        &mut self,
        package: ObjectID,
        module: Identifier,
        function: Identifier,
        type_arguments: Vec<TypeTag>,
        call_args: Vec<CallArg>,
    ) -> SigningResult<()> {
        let arguments = call_args
            .into_iter()
            .map(|a| self.input(a))
            .collect::<SigningResult<_>>()?;
        self.command(Command::move_call(
            package,
            module,
            function,
            type_arguments,
            arguments,
        ));
        Ok(())
    }

    pub fn input(&mut self, call_arg: CallArg) -> SigningResult<Argument> {
        match call_arg {
            CallArg::Pure(bytes) => {
                let force_separate = false;
                Ok(self.pure_bytes(bytes, force_separate))
            },
            CallArg::Object(obj) => self.obj(obj),
            CallArg::FundsWithdrawal(withdrawal) => Ok(self.funds_withdrawal(withdrawal)),
        }
    }

    /// SIP-58 withdrawal input. Never deduplicated: the chain allows several withdrawals, even of
    /// the same type, and each is reserved separately.
    pub fn funds_withdrawal(&mut self, withdrawal: FundsWithdrawalArg) -> Argument {
        let key = BuilderArg::ForcedNonUniquePure(self.inputs.len());
        let (i, _) = self
            .inputs
            .insert_full(key, CallArg::FundsWithdrawal(withdrawal));
        Argument::Input(i as u16)
    }

    pub fn pure_bytes(&mut self, bytes: Vec<u8>, force_separate: bool) -> Argument {
        let arg = if force_separate {
            BuilderArg::ForcedNonUniquePure(self.inputs.len())
        } else {
            BuilderArg::Pure(bytes.clone())
        };
        let (i, _) = self.inputs.insert_full(arg, CallArg::Pure(bytes));
        Argument::Input(i as u16)
    }

    pub fn pure<T: Serialize>(&mut self, value: T) -> SigningResult<Argument> {
        let force_separate = false;
        Ok(self.pure_bytes(bcs::encode(&value)?, force_separate))
    }

    pub fn obj(&mut self, obj_arg: ObjectArg) -> SigningResult<Argument> {
        let id = obj_arg.id();
        let obj_arg = if let Some(old_value) = self.inputs.get(&BuilderArg::Object(id)) {
            let old_obj_arg = match old_value {
                CallArg::Pure(_) => {
                    return SigningError::err(SigningErrorType::Error_internal)
                        .context("Expected Object, found Pure")
                },
                CallArg::FundsWithdrawal(_) => {
                    return SigningError::err(SigningErrorType::Error_internal)
                        .context("Expected Object, found FundsWithdrawal")
                },
                CallArg::Object(arg) => arg,
            };
            match (old_obj_arg, obj_arg) {
                (
                    ObjectArg::SharedObject {
                        id: id1,
                        initial_shared_version: v1,
                        mutable: mut1,
                    },
                    ObjectArg::SharedObject {
                        id: id2,
                        initial_shared_version: v2,
                        mutable: mut2,
                    },
                ) if v1 == &v2 => {
                    if id1 != &id2 || id != id2 {
                        // "invariant violation! object has id does not match call arg"
                        return SigningError::err(SigningErrorType::Error_internal)
                            .context("invariant violation! object has id does not match call arg");
                    }
                    ObjectArg::SharedObject {
                        id,
                        initial_shared_version: v2,
                        mutable: *mut1 || mut2,
                    }
                },
                (old_obj_arg, obj_arg) => {
                    if old_obj_arg != &obj_arg {
                        return SigningError::err(SigningErrorType::Error_internal)
                            .with_context(|| format!("Mismatched Object argument kind for object {id:?}. {old_value:?} is not compatible with {obj_arg:?}"));
                    }
                    obj_arg
                },
            }
        } else {
            obj_arg
        };
        let (i, _) = self
            .inputs
            .insert_full(BuilderArg::Object(id), CallArg::Object(obj_arg));
        Ok(Argument::Input(i as u16))
    }

    pub fn make_obj_vec(
        &mut self,
        objs: impl IntoIterator<Item = ObjectArg>,
    ) -> SigningResult<Argument> {
        let make_vec_args = objs
            .into_iter()
            .map(|obj| self.obj(obj))
            .collect::<SigningResult<_>>()?;
        Ok(self.command(Command::MakeMoveVec(None, make_vec_args)))
    }

    /// SIP-58: withdraws `amount` of `coin_type` from the sender's address balance and redeems it
    /// into a `Coin<T>` (`0x2::coin::redeem_funds<T>`). The returned coin exists only inside this
    /// transaction: a `Coin` has no `drop`, so every path must consume it completely (merge it,
    /// transfer it or stake it) or the transaction aborts. Redeem exactly what is needed.
    pub fn redeem_funds(&mut self, amount: u64, coin_type: &TypeTag) -> SigningResult<Argument> {
        let withdrawal = self.input(CallArg::FundsWithdrawal(
            FundsWithdrawalArg::balance_from_sender(amount, coin_type.clone()),
        ))?;
        Ok(self.command(Command::move_call(
            SUI_FRAMEWORK_PACKAGE_ID,
            COIN_MODULE_NAME.to_owned(),
            REDEEM_FUNDS_FUN_NAME.to_owned(),
            vec![coin_type.clone()],
            vec![withdrawal],
        )))
    }

    /// Merges `sources` into `destination`. No-op for an empty list.
    pub fn merge_coins(&mut self, destination: Argument, sources: Vec<Argument>) {
        if !sources.is_empty() {
            self.command(Command::MergeCoins(destination, sources));
        }
    }

    /// One `Coin<T>` argument gathered from owned `coins` and, if `withdraw > 0`, a redeemed
    /// SIP-58 withdrawal merged into the first owned coin. `None` when there is nothing to gather.
    ///
    /// A withdrawal on its own (no owned coin) comes back as the temporary redeemed coin: the caller
    /// must consume it whole, see [`Self::pay_from_funds`] for how a payment avoids a left-over.
    pub fn gather_coin(
        &mut self,
        coins: Vec<ObjectRef>,
        withdraw: u64,
        coin_type: &TypeTag,
    ) -> SigningResult<Option<Argument>> {
        let mut gathered = None;
        let mut coins = coins.into_iter();
        if let Some(first) = coins.next() {
            let first = self.obj(ObjectArg::ImmOrOwnedObject(first))?;
            let others: Vec<_> = coins
                .map(|c| self.obj(ObjectArg::ImmOrOwnedObject(c)))
                .collect::<Result<_, _>>()?;
            self.merge_coins(first, others);
            gathered = Some(first);
        }
        if withdraw > 0 {
            let redeemed = self.redeem_funds(withdraw, coin_type)?;
            gathered = Some(match gathered {
                Some(owned) => {
                    self.merge_coins(owned, vec![redeemed]);
                    owned
                },
                None => redeemed,
            });
        }
        Ok(gathered)
    }

    /// SIP-58: pays `amounts` of `coin_type` to `recipients` from owned `coins` and/or the address
    /// balance. Not for the gas coin: `Argument::GasCoin` is never used.
    ///
    /// - With owned `coins`, the `withdraw` (if any) is merged into them and the amounts are split
    ///   off; the change stays in the first owned coin.
    /// - Without owned coins everything comes from the address balance, and `withdraw` must be
    ///   exactly the total of `amounts`: each recipient gets a coin redeemed for exactly its amount.
    ///   Nothing is split, so no empty or left-over `Coin` (which has no `drop`) has to be disposed of.
    pub fn pay_from_funds(
        &mut self,
        coins: Vec<ObjectRef>,
        withdraw: u64,
        coin_type: &TypeTag,
        recipients: Vec<SuiAddress>,
        amounts: Vec<u64>,
    ) -> SigningResult<()> {
        if coins.is_empty() {
            return self.pay_from_funds_only(withdraw, coin_type, recipients, amounts);
        }
        let Some(source) = self.gather_coin(coins, withdraw, coin_type)? else {
            return SigningError::err(SigningErrorType::Error_internal)
                .context("Owned coins were given but nothing was gathered");
        };
        self.pay_impl(recipients, amounts, source)
    }

    fn pay_from_funds_only(
        &mut self,
        withdraw: u64,
        coin_type: &TypeTag,
        recipients: Vec<SuiAddress>,
        amounts: Vec<u64>,
    ) -> SigningResult<()> {
        if recipients.len() != amounts.len() {
            return SigningError::err(SigningErrorType::Error_invalid_params).with_context(|| {
                let recipients_num = recipients.len();
                let amounts_num = amounts.len();
                format!("Recipients and amounts mismatch. Got {recipients_num} recipients but {amounts_num} amounts")
            });
        }
        // One transfer per distinct recipient, like `pay_impl`.
        let mut per_recipient: IndexMap<SuiAddress, u64> = IndexMap::new();
        let mut total = 0u64;
        for (recipient, amount) in recipients.into_iter().zip(amounts) {
            total = total
                .checked_add(amount)
                .or_tw_err(SigningErrorType::Error_invalid_params)
                .context("Total of the amounts overflows u64")?;
            let entry = per_recipient.entry(recipient).or_default();
            *entry += amount;
        }
        if total == 0 {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context("Nothing to pay: no input coins and a zero total amount");
        }
        if withdraw != total {
            return SigningError::err(SigningErrorType::Error_invalid_params).with_context(|| {
                format!(
                    "Without input coins the address balance withdrawal ({withdraw}) must equal the total amount ({total})"
                )
            });
        }
        // A recipient with nothing to receive would get a zero-amount withdrawal, which the chain does not accept.
        if per_recipient.values().any(|amount| *amount == 0) {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context("A recipient has a zero amount: nothing to withdraw for it");
        }
        for (recipient, amount) in per_recipient {
            let coin = self.redeem_funds(amount, coin_type)?;
            let recipient = self.pure(recipient)?;
            self.command(Command::TransferObjects(vec![coin], recipient));
        }
        Ok(())
    }

    /// SIP-58, gas paid from the address balance: sends everything gathered from `coins` and a
    /// `withdraw` from the address balance to `recipient`.
    pub fn pay_all_sui_from_funds(
        &mut self,
        coins: Vec<ObjectRef>,
        withdraw: u64,
        recipient: SuiAddress,
    ) -> SigningResult<()> {
        let Some(source) = self.gather_coin(coins, withdraw, &sui_coin_type_tag())? else {
            return SigningError::err(SigningErrorType::Error_invalid_params)
                .context("No input coins and no address balance withdrawal to pay from");
        };
        let recipient = self.pure(recipient)?;
        self.command(Command::TransferObjects(vec![source], recipient));
        Ok(())
    }

    pub fn command(&mut self, command: Command) -> Argument {
        let i = self.commands.len();
        self.commands.push(command);
        Argument::Result(i as u16)
    }

    fn pay_impl(
        &mut self,
        recipients: Vec<SuiAddress>,
        amounts: Vec<u64>,
        coin: Argument,
    ) -> SigningResult<()> {
        if recipients.len() != amounts.len() {
            // "Recipients and amounts mismatch. Got {} recipients but {} amounts"
            return SigningError::err(SigningErrorType::Error_invalid_params).with_context(|| {
                let recipients_num = recipients.len();
                let amounts_num = amounts.len();
                format!("Recipients and amounts mismatch. Got {recipients_num} recipients but {amounts_num} amounts")
            });
        }
        if amounts.is_empty() {
            return Ok(());
        }

        // collect recipients in the case where they are non-unique in order
        // to minimize the number of transfers that must be performed
        let mut recipient_map: IndexMap<SuiAddress, Vec<usize>> = IndexMap::new();
        let mut amt_args = vec![];
        for (i, (recipient, amount)) in recipients.into_iter().zip(amounts).enumerate() {
            recipient_map.entry(recipient).or_default().push(i);
            amt_args.push(self.pure(amount)?);
        }
        let Argument::Result(split_primary) = self.command(Command::SplitCoins(coin, amt_args))
        else {
            return SigningError::err(SigningErrorType::Error_internal)
                .context("self.command should always give an Argument::Result");
        };
        for (recipient, split_secondaries) in recipient_map {
            let rec_arg = self.pure(recipient).unwrap();
            let coins = split_secondaries
                .into_iter()
                .map(|j| Argument::NestedResult(split_primary, j as u16))
                .collect();
            self.command(Command::TransferObjects(coins, rec_arg));
        }
        Ok(())
    }
}
