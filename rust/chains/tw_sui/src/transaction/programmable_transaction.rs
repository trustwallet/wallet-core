// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::SuiAddress;
use crate::transaction::command::{Argument, Command};
use crate::transaction::sui_types::{CallArg, ObjectArg, ObjectID, ObjectRef};
use indexmap::IndexMap;
use move_core_types::identifier::Identifier;
use move_core_types::language_storage::TypeTag;
use serde::{Deserialize, Serialize};
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
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
            return Err(SigningError(SigningErrorType::Error_invalid_params));
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
        }
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
                CallArg::Pure(_) => return Err(SigningError(SigningErrorType::Error_internal)),
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
                        return Err(SigningError(SigningErrorType::Error_internal));
                    }
                    ObjectArg::SharedObject {
                        id,
                        initial_shared_version: v2,
                        mutable: *mut1 || mut2,
                    }
                },
                (old_obj_arg, obj_arg) => {
                    if old_obj_arg != &obj_arg {
                        // "Mismatched Object argument kind for object {id}. {old_value:?} is not compatible with {obj_arg:?}"
                        return Err(SigningError(SigningErrorType::Error_internal));
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
            return Err(SigningError(SigningErrorType::Error_invalid_params));
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
            panic!("self.command should always give a Argument::Result")
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
