// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

use crate::{
    address::{AccountIdentifier, IcpAddress},
    protocol::principal::Principal,
};

pub trait CanisterId {
    fn principal_id() -> Principal;
}

pub trait InternetComputerContext {
    type Address: IcpAddress;
    type Canister: CanisterId;
}

pub struct StandardInternetComputerLedgerCanister;

impl CanisterId for StandardInternetComputerLedgerCanister {
    fn principal_id() -> Principal {
        // ICP Ledger Canister: ryjl3-tyaaa-aaaaa-aaaba-cai
        Principal::from_slice(&[0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x01])
    }
}

#[derive(Default)]
pub struct StandardInternetComputerContext;

impl InternetComputerContext for StandardInternetComputerContext {
    type Address = AccountIdentifier;
    type Canister = StandardInternetComputerLedgerCanister;
}

#[cfg(test)]
mod test {

    use std::marker::PhantomData;

    use tw_coin_entry::error::AddressResult;

    use super::*;

    const TEST_OWNER_PRINCIPAL_ID: &str =
        "t4u4z-y3dur-j63pk-nw4rv-yxdbt-agtt6-nygn7-ywh6y-zm2f4-sdzle-3qe";
    const TEST_TEXTUAL_ICP_ADDRESS: &str =
        "943d12e762f43806782f524b8f90297298a6d79e4749b41b585ec427409c826a";
    const TEXTUAL_ICP_LEDGER_CANISTER_ID: &str = "ryjl3-tyaaa-aaaaa-aaaba-cai";

    pub struct ContextTest<Context: InternetComputerContext> {
        _phantom: PhantomData<Context>,
    }

    impl<Context: InternetComputerContext> ContextTest<Context> {
        fn canister_id() -> Principal {
            Context::Canister::principal_id()
        }

        fn account_identifier_optional(s: &str) -> AddressResult<Option<Context::Address>> {
            Context::Address::from_str_optional(s)
        }
    }

    #[test]
    fn standard_internet_computer_context_canister_address() {
        let owner = Principal::from_text(TEST_OWNER_PRINCIPAL_ID).unwrap();
        let expected_account_identifier = AccountIdentifier::new(&owner);
        let account_identifier =
            ContextTest::<StandardInternetComputerContext>::account_identifier_optional(
                TEST_TEXTUAL_ICP_ADDRESS,
            )
            .unwrap()
            .unwrap();
        assert_eq!(expected_account_identifier, account_identifier);
    }

    #[test]
    fn standard_internet_computer_context_canister_type() {
        let ledger_canister_id = ContextTest::<StandardInternetComputerContext>::canister_id();
        assert_eq!(ledger_canister_id.to_text(), TEXTUAL_ICP_LEDGER_CANISTER_ID);
    }

    #[test]
    fn standard_internet_computer_ledger_canister() {
        let ledger_canister_id = StandardInternetComputerLedgerCanister::principal_id();
        assert_eq!(ledger_canister_id.to_text(), TEXTUAL_ICP_LEDGER_CANISTER_ID);
    }
}
