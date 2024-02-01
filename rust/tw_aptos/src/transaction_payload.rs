// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::serde_helper::vec_bytes;
use move_core_types::identifier::Identifier;
use move_core_types::language_storage::{ModuleId, StructTag, TypeTag};
use move_core_types::parser::parse_transaction_argument;
use move_core_types::transaction_argument::TransactionArgument;
use serde::{Deserialize, Serialize};
use serde_json::{json, Value};
use std::default::Default;
use std::str::FromStr;
use tw_coin_entry::error::{SigningError, SigningErrorType, SigningResult};
use tw_encoding::{bcs, EncodingError, EncodingResult};
use tw_memory::Data;
use tw_proto::Aptos;

pub type EntryFunctionResult<T> = Result<T, EntryFunctionError>;

#[derive(Debug)]
pub enum EntryFunctionError {
    MissingFunctionName,
    InvalidFunctionName,
    MissingArguments,
    InvalidArguments,
    EncodingError,
    MissingTypeArguments,
    InvalidTypeArguments,
}

impl From<EncodingError> for EntryFunctionError {
    fn from(_error: EncodingError) -> Self {
        EntryFunctionError::EncodingError
    }
}

impl From<EntryFunctionError> for SigningError {
    fn from(_: EntryFunctionError) -> Self {
        SigningError(SigningErrorType::Error_invalid_params)
    }
}

#[derive(Clone, Debug, Eq, PartialEq, Serialize, Deserialize)]
pub struct EntryFunction {
    module: ModuleId,
    function: Identifier,
    ty_args: Vec<TypeTag>,
    #[serde(with = "vec_bytes")]
    args: Vec<Vec<u8>>,
    #[serde(skip_serializing)]
    json_args: Value,
}

impl TryFrom<Value> for EntryFunction {
    type Error = EntryFunctionError;

    fn try_from(value: Value) -> EntryFunctionResult<Self> {
        let function_str = value["function"]
            .as_str()
            .ok_or(EntryFunctionError::MissingFunctionName)?;
        let tag = StructTag::from_str(function_str)
            .map_err(|_| EntryFunctionError::InvalidFunctionName)?;

        let args = value["arguments"]
            .as_array()
            .ok_or(EntryFunctionError::MissingArguments)?
            .iter()
            .map(|element| {
                let arg_str = element.to_string();
                let arg = parse_transaction_argument(
                    arg_str.trim_start_matches('"').trim_end_matches('"'),
                )
                .map_err(|_| EntryFunctionError::InvalidArguments)?;
                serialize_argument(&arg).map_err(EntryFunctionError::from)
            })
            .collect::<EntryFunctionResult<Vec<Data>>>()?;

        let ty_args = value["type_arguments"]
            .as_array()
            .ok_or(EntryFunctionError::MissingTypeArguments)?
            .iter()
            .map(|element| {
                let ty_arg_str = element
                    .as_str()
                    .ok_or(EntryFunctionError::InvalidTypeArguments)?;
                TypeTag::from_str(ty_arg_str).map_err(|_| EntryFunctionError::InvalidTypeArguments)
            })
            .collect::<EntryFunctionResult<Vec<TypeTag>>>()?;

        Ok(EntryFunction {
            module: tag.module_id(),
            function: tag.name,
            ty_args,
            args,
            json_args: value["arguments"].clone(),
        })
    }
}

fn serialize_argument(arg: &TransactionArgument) -> EncodingResult<Data> {
    match arg {
        TransactionArgument::U8(v) => bcs::encode(v),
        TransactionArgument::U16(v) => bcs::encode(v),
        TransactionArgument::U32(v) => bcs::encode(v),
        TransactionArgument::U64(v) => bcs::encode(v),
        TransactionArgument::U128(v) => bcs::encode(v),
        TransactionArgument::U256(v) => bcs::encode(v),
        TransactionArgument::U8Vector(v) => bcs::encode(v),
        TransactionArgument::Bool(v) => bcs::encode(v),
        TransactionArgument::Address(v) => {
            let serialized_v = bcs::encode(v)?;
            bcs::encode(&serialized_v)
        },
    }
}

pub fn convert_proto_struct_tag_to_type_tag(
    struct_tag: Aptos::Proto::StructTag,
) -> SigningResult<TypeTag> {
    TypeTag::from_str(&format!(
        "{}::{}::{}",
        struct_tag.account_address, struct_tag.module, struct_tag.name
    ))
    .map_err(|_| SigningError(SigningErrorType::Error_invalid_params))
}

pub fn convert_type_tag_to_struct_tag(type_tag: TypeTag) -> Aptos::Proto::StructTag<'static> {
    if let TypeTag::Struct(st) = type_tag {
        Aptos::Proto::StructTag {
            account_address: st.address.to_hex_literal().into(),
            module: st.module.to_string().into(),
            name: st.name.to_string().into(),
        }
    } else {
        Aptos::Proto::StructTag::default()
    }
}

impl EntryFunction {
    fn to_json(&self) -> Value {
        // Create a JSON array from the `ty_args` field by filtering and mapping
        // the items that match `TypeTag::Struct` to their string representation.
        let type_arguments: Value = self
            .ty_args
            .iter()
            .map(|item| Some(json!(item.to_string())))
            .collect();

        // Construct the final JSON value
        json!({
            "type": "entry_function_payload",
            "function": format!("{}::{}", self.module.short_str_lossless(), self.function.clone().into_string()),
            "arguments": self.json_args,
            "type_arguments": type_arguments
        })
    }
}

#[derive(Clone, Debug, Eq, PartialEq, Serialize, Deserialize)]
pub enum TransactionPayload {
    Script,
    ModuleBundle,
    /// A transaction that executes an existing entry function published on-chain.
    EntryFunction(EntryFunction),
}

impl TransactionPayload {
    pub fn to_json(&self) -> Value {
        match self {
            TransactionPayload::Script => Value::default(),
            TransactionPayload::ModuleBundle => Value::default(),
            TransactionPayload::EntryFunction(entry) => entry.to_json(),
        }
    }
}

impl EntryFunction {
    pub fn new(
        module: ModuleId,
        function: Identifier,
        ty_args: Vec<TypeTag>,
        args: Vec<Vec<u8>>,
        json_args: Value,
    ) -> Self {
        EntryFunction {
            module,
            function,
            ty_args,
            args,
            json_args,
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::address::Address;
    use move_core_types::account_address::AccountAddress;
    use move_core_types::identifier::Identifier;
    use move_core_types::language_storage::{ModuleId, TypeTag};
    use serde_json::{json, Value};
    use std::str::FromStr;
    use tw_encoding::hex;

    #[test]
    fn test_payload_from_json() {
        let payload_value: Value = json!({
            "arguments": ["0xc95db29a67a848940829b3df6119b5e67b788ff0248676e4484c7c6f29c0f5e6"],
            "function": "0xc23c3b70956ce8d88fb18ad9ed3b463fe873cb045db3f6d2e2fb15b9aab71d50::IFO::release",
            "type": "entry_function_payload",
            "type_arguments": [
              "0x48e0e3958d42b8d452c9199d4a221d0d1b15d14655787453dbe77208ced90517::coins::BUSD",
              "0x48e0e3958d42b8d452c9199d4a221d0d1b15d14655787453dbe77208ced90517::coins::DAI",
              "0x9936836587ca33240d3d3f91844651b16cb07802faf5e34514ed6f78580deb0a::uints::U1"
            ]
        });

        let v = EntryFunction::try_from(payload_value.clone()).unwrap();
        assert_eq!(payload_value, v.to_json());
    }

    #[test]
    fn test_payload_from_json_with_arg_non_str() {
        let payload_value: Value = json!({
            "type":"entry_function_payload",
            "function":"0xd11107bdf0d6d7040c6c0bfbdecb6545191fdf13e8d8d259952f53e1713f61b5::ditto_staking::stake_aptos",
            "type_arguments":[],
            "arguments": [1000000]
        });

        let v = EntryFunction::try_from(payload_value.clone()).unwrap();
        assert_eq!(payload_value, v.to_json());
    }

    #[test]
    fn test_basic_payload() {
        let addr =
            Address::from_str("0xeeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b")
                .unwrap()
                .inner();
        let amount: i64 = 1000;
        let args = vec![bcs::encode(&addr).unwrap(), bcs::encode(&amount).unwrap()];
        let module = ModuleId::new(AccountAddress::ONE, Identifier::from_str("coin").unwrap());
        let function = Identifier::from_str("transfer").unwrap();
        let type_tag = vec![TypeTag::from_str("0x1::aptos_coin::AptosCoin").unwrap()];
        let entry = EntryFunction::new(
            module,
            function,
            type_tag,
            args,
            json!([addr.to_hex_literal(), amount.to_string()]),
        );
        let tp = TransactionPayload::EntryFunction(entry);
        let serialized = bcs::encode(&tp).unwrap();
        assert_eq!(hex::encode(serialized, false), "02000000000000000000000000000000000000000000000000000000000000000104636f696e087472616e73666572010700000000000000000000000000000000000000000000000000000000000000010a6170746f735f636f696e094170746f73436f696e000220eeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b08e803000000000000");
        let payload_value: Value = json!({
                "function": "0x1::coin::transfer",
                "type": "entry_function_payload",
                "arguments": ["0xeeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b", "1000"],
                "type_arguments": ["0x1::aptos_coin::AptosCoin"]
        });
        assert_eq!(tp.to_json(), payload_value);
    }
}
