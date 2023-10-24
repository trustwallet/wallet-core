// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use move_core_types::identifier::Identifier;
use move_core_types::language_storage::{ModuleId, TypeTag};
use serde::{Deserialize, Serialize};
use crate::{serde_helper::vec_bytes};

#[derive(Clone, Debug, Hash, Eq, PartialEq, Serialize, Deserialize)]
pub struct EntryFunction {
    module: ModuleId,
    function: Identifier,
    ty_args: Vec<TypeTag>,
    #[serde(with = "vec_bytes")]
    args: Vec<Vec<u8>>,
}

#[derive(Clone, Debug, Hash, Eq, PartialEq, Serialize, Deserialize)]
pub enum TransactionPayload {
    Script,
    ModuleBundle,
    /// A transaction that executes an existing entry function published on-chain.
    EntryFunction(EntryFunction),
}

impl EntryFunction {
    pub fn new(
        module: ModuleId,
        function: Identifier,
        ty_args: Vec<TypeTag>,
        args: Vec<Vec<u8>>,
    ) -> Self {
        EntryFunction {
            module,
            function,
            ty_args,
            args,
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
        let entry = EntryFunction::new(module, function, type_tag, args);
        let tp = TransactionPayload::EntryFunction(entry);
        let serialized = bcs::to_bytes(&tp).unwrap();
        assert_eq!(hex::encode(serialized, false), "02000000000000000000000000000000000000000000000000000000000000000104636f696e087472616e73666572010700000000000000000000000000000000000000000000000000000000000000010a6170746f735f636f696e094170746f73436f696e000220eeff357ea5c1a4e7bc11b2b17ff2dc2dcca69750bfef1e1ebcaccf8c8018175b08e803000000000000");
    }
}