// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use std::default::Default;
use std::str::FromStr;
use move_core_types::identifier::Identifier;
use move_core_types::language_storage::{ModuleId, TypeTag};
use serde::{Deserialize, Serialize};
use serde_json::{json, Value};
use tw_proto::Aptos;
use crate::{serde_helper::vec_bytes};

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

pub fn convert_proto_struct_tag_to_type_tag(struct_tag: Aptos::Proto::StructTag) -> TypeTag {
    TypeTag::from_str(&format!("{}::{}::{}", struct_tag.account_address, struct_tag.module, struct_tag.name)).unwrap()
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
        let type_arguments: Value = self.ty_args.iter()
            .filter_map(|item| {
                Some(json!(item.to_string()))
            })
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
            TransactionPayload::Script => { Value::default() }
            TransactionPayload::ModuleBundle => { Value::default() }
            TransactionPayload::EntryFunction(entry) => { entry.to_json() }
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
    use std::str::FromStr;
    use crate::address::Address;
    use move_core_types::account_address::AccountAddress;
    use move_core_types::identifier::Identifier;
    use move_core_types::language_storage::{ModuleId, TypeTag};
    use serde_json::{json, Value};
    use tw_encoding::hex;
    use crate::transaction_payload::{EntryFunction, TransactionPayload};

    #[test]
    fn test_basic_payload() {
        let addr = Address::from_str("0xeeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b").unwrap().inner();
        let amount: i64 = 1000;
        let args = vec![bcs::to_bytes(&addr).unwrap(), bcs::to_bytes(&amount).unwrap()];
        let module = ModuleId::new(AccountAddress::ONE, Identifier::from_str("coin").unwrap());
        let function = Identifier::from_str("transfer").unwrap();
        let type_tag = vec![TypeTag::from_str("0x1::aptos_coin::AptosCoin").unwrap()];
        let entry = EntryFunction::new(module, function, type_tag, args, json!([addr.to_hex_literal(), amount.to_string()]));
        let tp = TransactionPayload::EntryFunction(entry);
        let serialized = bcs::to_bytes(&tp).unwrap();
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