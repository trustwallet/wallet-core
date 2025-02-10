// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::aptos_move_types::MoveType;
use crate::constants::{OBJECT_MODULE, OBJECT_STRUCT};
use crate::serde_helper::vec_bytes;
use move_core_types::account_address::AccountAddress;
use move_core_types::identifier::Identifier;
use move_core_types::language_storage::{ModuleId, StructTag, TypeTag, CORE_CODE_ADDRESS};
use move_core_types::parser::parse_transaction_argument;
use move_core_types::transaction_argument::TransactionArgument;
use move_core_types::u256;
use move_core_types::value::{MoveStruct, MoveStructLayout, MoveTypeLayout, MoveValue};
use serde::{Deserialize, Serialize};
use serde_json::{json, Value};
use std::default::Default;
use std::str::FromStr;
use tw_coin_entry::error::prelude::*;
use tw_encoding::hex::DecodeHex;
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
    fn from(e: EntryFunctionError) -> Self {
        SigningError::new(SigningErrorType::Error_invalid_params)
            .context(format!("Error decoding EntryFunction: {e:?}"))
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
        Self::parse_with_abi(value, json!([]))
    }
}

impl EntryFunction {
    pub fn parse_with_abi(value: Value, abi: Value) -> EntryFunctionResult<Self> {
        let function_str = value["function"]
            .as_str()
            .ok_or(EntryFunctionError::MissingFunctionName)?;
        let tag = StructTag::from_str(function_str)
            .map_err(|_| EntryFunctionError::InvalidFunctionName)?;

        let abi = abi
            .as_array()
            .ok_or(EntryFunctionError::MissingTypeArguments)?;
        let get_abi_str =
            |index: usize| -> Option<String> { abi.get(index)?.as_str().map(|s| s.to_string()) };

        let args = value["arguments"]
            .as_array()
            .ok_or(EntryFunctionError::MissingArguments)?
            .iter()
            .enumerate()
            .map(|(index, element)| {
                let arg_str = element.to_string();
                let arg_str = arg_str.trim_start_matches('"').trim_end_matches('"');

                if let Some(abi_str) = get_abi_str(index) {
                    let arg = convert_to_move_value(&abi_str, element.clone())?;
                    bcs::encode(&arg).map_err(EntryFunctionError::from)
                } else {
                    let arg = parse_transaction_argument(arg_str)
                        .map_err(|_| EntryFunctionError::InvalidArguments)?;
                    serialize_argument(&arg).map_err(EntryFunctionError::from)
                }
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

fn convert_to_move_value(abi_str: &str, element: Value) -> EntryFunctionResult<MoveValue> {
    let move_type: MoveType = abi_str
        .parse()
        .map_err(|_| EntryFunctionError::InvalidTypeArguments)?;
    let type_tag: TypeTag = move_type
        .try_into()
        .map_err(|_| EntryFunctionError::InvalidTypeArguments)?;
    // Taken from: https://github.com/aptos-labs/aptos-core/blob/aaa3514c8ee4e5d38b89d916eadff7286a42e040/api/types/src/convert.rs#L845-L872
    let layout = match type_tag {
        TypeTag::Struct(ref boxed_struct) => {
            // The current framework can't handle generics, so we handle this here
            if boxed_struct.address == AccountAddress::ONE
                && boxed_struct.module.as_ident_str() == OBJECT_MODULE
                && boxed_struct.name.as_ident_str() == OBJECT_STRUCT
            {
                // Objects are just laid out as an address
                MoveTypeLayout::Address
            } else {
                // For all other structs, use their set layout
                build_type_layout(&type_tag)?
            }
        },
        _ => build_type_layout(&type_tag)?,
    };
    parse_argument(&layout, element).map_err(|_| EntryFunctionError::InvalidArguments)
}

fn build_type_layout(t: &TypeTag) -> EncodingResult<MoveTypeLayout> {
    use TypeTag::*;
    Ok(match t {
        Bool => MoveTypeLayout::Bool,
        U8 => MoveTypeLayout::U8,
        U64 => MoveTypeLayout::U64,
        U128 => MoveTypeLayout::U128,
        Address => MoveTypeLayout::Address,
        Vector(elem_t) => MoveTypeLayout::Vector(Box::new(build_type_layout(elem_t)?)),
        Struct(s) => MoveTypeLayout::Struct(build_struct_layout(s)?),
        U16 => MoveTypeLayout::U16,
        U32 => MoveTypeLayout::U32,
        U256 => MoveTypeLayout::U256,
        Signer => Err(EncodingError::InvalidInput)?,
    })
}

fn build_struct_layout(s: &StructTag) -> EncodingResult<MoveStructLayout> {
    let type_arguments = s
        .type_params
        .iter()
        .map(build_type_layout)
        .collect::<EncodingResult<Vec<MoveTypeLayout>>>()?;
    if type_arguments.is_empty() {
        Ok(MoveStructLayout::WithTypes {
            type_: s.clone(),
            fields: vec![],
        })
    } else {
        Ok(MoveStructLayout::Runtime(type_arguments))
    }
}

fn parse_argument(layout: &MoveTypeLayout, val: Value) -> EncodingResult<MoveValue> {
    let val_str = val
        .to_string()
        .trim_start_matches('"')
        .trim_end_matches('"')
        .to_string();
    Ok(match layout {
        MoveTypeLayout::Bool => MoveValue::Bool(
            val_str
                .parse::<bool>()
                .map_err(|_| EncodingError::InvalidInput)?,
        ),
        MoveTypeLayout::U8 => MoveValue::U8(
            val_str
                .parse::<u8>()
                .map_err(|_| EncodingError::InvalidInput)?,
        ),
        MoveTypeLayout::U16 => MoveValue::U16(
            val_str
                .parse::<u16>()
                .map_err(|_| EncodingError::InvalidInput)?,
        ),
        MoveTypeLayout::U32 => MoveValue::U32(
            val_str
                .parse::<u32>()
                .map_err(|_| EncodingError::InvalidInput)?,
        ),
        MoveTypeLayout::U64 => MoveValue::U64(
            val_str
                .parse::<u64>()
                .map_err(|_| EncodingError::InvalidInput)?,
        ),
        MoveTypeLayout::U128 => MoveValue::U128(
            val_str
                .parse::<u128>()
                .map_err(|_| EncodingError::InvalidInput)?,
        ),
        MoveTypeLayout::U256 => MoveValue::U256(
            val_str
                .parse::<u256::U256>()
                .map_err(|_| EncodingError::InvalidInput)?,
        ),
        MoveTypeLayout::Address => MoveValue::Address(
            val_str
                .parse::<AccountAddress>()
                .map_err(|_| EncodingError::InvalidInput)?,
        ),
        MoveTypeLayout::Vector(item_layout) => parse_vector_argument(item_layout.as_ref(), val)?,
        MoveTypeLayout::Struct(struct_layout) => parse_struct_argument(struct_layout, val)?,
        // Some values, e.g., signer or ones with custom serialization
        // (native), are not stored to storage and so we do not expect
        // to see them here.
        MoveTypeLayout::Signer => {
            return Err(EncodingError::InvalidInput);
        },
    })
}

fn parse_vector_argument(layout: &MoveTypeLayout, val: Value) -> EncodingResult<MoveValue> {
    if matches!(layout, MoveTypeLayout::U8) {
        Ok(MoveValue::Vector(
            val.as_str()
                .ok_or(EncodingError::InvalidInput)?
                .decode_hex()
                .map_err(|_| EncodingError::InvalidInput)?
                .into_iter()
                .map(MoveValue::U8)
                .collect::<Vec<_>>(),
        ))
    } else {
        let val = trim_if_needed(val)?;
        if let Value::Array(list) = val {
            let vals = list
                .into_iter()
                .map(|v| parse_argument(layout, v).map_err(|_| EncodingError::InvalidInput))
                .collect::<EncodingResult<_>>()?;
            Ok(MoveValue::Vector(vals))
        } else {
            Err(EncodingError::InvalidInput)
        }
    }
}

// Inspired from: https://github.com/aptos-labs/aptos-core/blob/aaa3514c8ee4e5d38b89d916eadff7286a42e040/api/types/src/convert.rs#L924
// However, we expect struct with strings and unnamed fields while the original code expects struct with named fields.
// This is because the original code uses a module resolver internally to obtain the struct types and we don't have that here.
// In order to be able to accept that as an API, we need to change the code to accept struct with unnamed fields.
fn parse_struct_argument(layout: &MoveStructLayout, val: Value) -> EncodingResult<MoveValue> {
    let field_layouts = match layout {
        MoveStructLayout::Runtime(fields) => fields,
        MoveStructLayout::WithTypes { type_, .. } => {
            if is_utf8_string(type_) {
                let string = val.as_str().ok_or(EncodingError::InvalidInput)?;
                return Ok(new_vm_utf8_string(string));
            } else {
                return Err(EncodingError::InvalidInput);
            }
        },
        _ => return Err(EncodingError::InvalidInput),
    };
    let val = trim_if_needed(val)?;
    let field_values = if let Value::Array(fields) = val {
        fields
    } else {
        return Err(EncodingError::InvalidInput);
    };
    let fields = field_layouts
        .iter()
        .zip(field_values.into_iter())
        .map(|(field_layout, value)| {
            let move_value = parse_argument(field_layout, value)?;
            Ok(move_value)
        })
        .collect::<EncodingResult<_>>()?;

    Ok(MoveValue::Struct(MoveStruct::Runtime(fields)))
}

fn trim_if_needed(val: Value) -> EncodingResult<Value> {
    if val.is_string() {
        let val_str = val.as_str().ok_or(EncodingError::InvalidInput)?;
        let val_str = val_str
            .trim_start_matches('"')
            .trim_end_matches('"')
            .to_string();
        let val: Value = serde_json::from_str(&val_str).map_err(|_| EncodingError::InvalidInput)?;
        Ok(val)
    } else {
        Ok(val)
    }
}

fn is_utf8_string(st: &StructTag) -> bool {
    st.address == CORE_CODE_ADDRESS
        && st.name.to_string() == "String"
        && st.module.to_string() == "string"
}

fn new_vm_utf8_string(string: &str) -> MoveValue {
    let byte_vector = MoveValue::Vector(
        string
            .as_bytes()
            .iter()
            .map(|byte| MoveValue::U8(*byte))
            .collect(),
    );
    MoveValue::Struct(MoveStruct::Runtime(vec![byte_vector]))
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
        TransactionArgument::Address(v) => bcs::encode(v),
    }
}

pub fn convert_proto_struct_tag_to_type_tag(
    struct_tag: Aptos::Proto::StructTag,
) -> SigningResult<TypeTag> {
    TypeTag::from_str(&format!(
        "{}::{}::{}",
        struct_tag.account_address, struct_tag.module, struct_tag.name
    ))
    .tw_err(SigningErrorType::Error_invalid_params)
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

        let tp = TransactionPayload::EntryFunction(v);
        let serialized = bcs::encode(&tp).unwrap();
        assert_eq!(hex::encode(serialized, false), "02c23c3b70956ce8d88fb18ad9ed3b463fe873cb045db3f6d2e2fb15b9aab71d500349464f0772656c65617365030748e0e3958d42b8d452c9199d4a221d0d1b15d14655787453dbe77208ced9051705636f696e730442555344000748e0e3958d42b8d452c9199d4a221d0d1b15d14655787453dbe77208ced9051705636f696e730344414900079936836587ca33240d3d3f91844651b16cb07802faf5e34514ed6f78580deb0a0575696e7473025531000120c95db29a67a848940829b3df6119b5e67b788ff0248676e4484c7c6f29c0f5e6");
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
        let expected_serialized = "02000000000000000000000000000000000000000000000000000000000000000104636f696e087472616e73666572010700000000000000000000000000000000000000000000000000000000000000010a6170746f735f636f696e094170746f73436f696e000220eeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b08e803000000000000";
        assert_eq!(hex::encode(serialized, false), expected_serialized);
        let payload_value: Value = json!({
                "function": "0x1::coin::transfer",
                "type": "entry_function_payload",
                "arguments": ["0xeeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b", "1000"],
                "type_arguments": ["0x1::aptos_coin::AptosCoin"]
        });
        assert_eq!(tp.to_json(), payload_value);

        // Rebuild a new EntryFunction object from the JSON above
        let v = EntryFunction::try_from(payload_value.clone()).unwrap();
        let tp = TransactionPayload::EntryFunction(v);
        // Serialize the new EntryFunction object and compare with the expected serialized value
        let serialized = bcs::encode(&tp).unwrap();
        assert_eq!(hex::encode(serialized, false), expected_serialized);
    }

    #[test]
    fn test_payload_with_vector_of_u8() {
        let payload_value: Value = json!({
            "type":"entry_function_payload",
            "function":"0x9770fa9c725cbd97eb50b2be5f7416efdfd1f1554beb0750d4dae4c64e860da3::controller::deposit",
            "type_arguments":["0x1::aptos_coin::AptosCoin"],
            "arguments":[
                "0x010302"
            ]
        });
        let abi = r#"[
            "vector<u8>"
        ]"#;
        let abi_value: Value = serde_json::from_str(abi).unwrap();
        let v = EntryFunction::parse_with_abi(payload_value.clone(), abi_value).unwrap();
        let v = bcs::decode::<Vec<u8>>(&v.args[0]).unwrap();
        assert_eq!(v, vec![1u8, 3u8, 2u8]);
    }

    #[test]
    fn test_payload_with_vector_of_u64() {
        let payload_value: Value = json!({
            "type":"entry_function_payload",
            "function":"0x9770fa9c725cbd97eb50b2be5f7416efdfd1f1554beb0750d4dae4c64e860da3::controller::deposit",
            "type_arguments":["0x1::aptos_coin::AptosCoin"],
            "arguments":[
                "[\"1\", \"2\", \"3\"]"
            ]
        });
        let abi = r#"[
            "vector<u64>"
        ]"#;
        let abi_value: Value = serde_json::from_str(abi).unwrap();
        let v = EntryFunction::parse_with_abi(payload_value.clone(), abi_value).unwrap();
        let v = bcs::decode::<Vec<u64>>(&v.args[0]).unwrap();
        assert_eq!(v, vec![1u64, 2u64, 3u64]);
    }

    #[test]
    fn test_payload_with_vector_of_vector() {
        let payload_value: Value = json!({
            "type":"entry_function_payload",
            "function":"0x9770fa9c725cbd97eb50b2be5f7416efdfd1f1554beb0750d4dae4c64e860da3::controller::deposit",
            "type_arguments":["0x1::aptos_coin::AptosCoin"],
            "arguments":[
                "[\"0x4d61696e204163636f756e74\",\"0x6112\"]"
            ]
        });
        let abi = r#"[
            "vector<vector<u8>>"
        ]"#;
        let abi_value: Value = serde_json::from_str(abi).unwrap();
        let v = EntryFunction::parse_with_abi(payload_value.clone(), abi_value).unwrap();
        let v = bcs::decode::<Vec<Vec<u8>>>(&v.args[0]).unwrap();
        assert_eq!(
            hex::encode(v[0].clone(), true),
            "0x4d61696e204163636f756e74"
        );
        assert_eq!(hex::encode(v[1].clone(), true), "0x6112");
    }

    #[test]
    fn test_payload_with_struct_string() {
        let payload_value: Value = json!({
            "type":"entry_function_payload",
            "function":"0x9770fa9c725cbd97eb50b2be5f7416efdfd1f1554beb0750d4dae4c64e860da3::controller::deposit",
            "type_arguments":["0x1::aptos_coin::AptosCoin"],
            "arguments":[
                "123"
            ]
        });
        let abi = r#"[
            "0x1::string::String"
        ]"#;
        let abi_value: Value = serde_json::from_str(abi).unwrap();
        let v = EntryFunction::parse_with_abi(payload_value.clone(), abi_value).unwrap();
        let v = bcs::decode::<String>(&v.args[0]).unwrap();
        assert_eq!(v, "123");
    }

    #[test]
    fn test_payload_with_struct() {
        let payload_value: Value = json!({
            "type":"entry_function_payload",
            "function":"0x9770fa9c725cbd97eb50b2be5f7416efdfd1f1554beb0750d4dae4c64e860da3::controller::deposit",
            "type_arguments":["0x1::aptos_coin::AptosCoin"],
            "arguments":[
                "[10]"
            ]
        });
        let abi = r#"[
            "0x1::coin::Coin<u64>"
        ]"#;
        let abi_value: Value = serde_json::from_str(abi).unwrap();
        let v = EntryFunction::parse_with_abi(payload_value.clone(), abi_value).unwrap();
        let v = bcs::decode::<u64>(&v.args[0]).unwrap();
        assert_eq!(v, 10u64);
    }

    fn assert_value_conversion<V: Serialize>(abi_str: &str, v: V, expected: MoveValue) {
        let vm_value = convert_to_move_value(abi_str, json!(v)).unwrap();
        assert_eq!(vm_value, expected);
    }

    #[test]
    fn test_value_conversion() {
        assert_value_conversion("u8", 1i32, MoveValue::U8(1));
        assert_value_conversion("u64", "1", MoveValue::U64(1));
        assert_value_conversion("u128", "1", MoveValue::U128(1));
        assert_value_conversion("bool", true, MoveValue::Bool(true));

        let address = AccountAddress::from_hex_literal("0x1").unwrap();
        assert_value_conversion("address", "0x1", MoveValue::Address(address));

        assert_value_conversion("0x1::string::String", "hello", new_vm_utf8_string("hello"));

        assert_value_conversion(
            "vector<u8>",
            "0x0102",
            MoveValue::Vector(vec![MoveValue::U8(1), MoveValue::U8(2)]),
        );
        assert_value_conversion(
            "vector<u64>",
            ["1", "2"],
            MoveValue::Vector(vec![MoveValue::U64(1), MoveValue::U64(2)]),
        );

        assert_value_conversion(
            "0x1::guid::ID<u64, address>", // As we do not have access to the module resolver, the types of the struct should be provided as params
            ["1", "0x1"],
            MoveValue::Struct(MoveStruct::Runtime(vec![
                MoveValue::U64(1),
                MoveValue::Address(address),
            ])),
        );
    }
}
