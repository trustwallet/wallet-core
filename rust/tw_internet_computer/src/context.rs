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
        Principal::from_slice(&[0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0x01])
    }
}

#[derive(Default)]
pub struct StandardInternetComputerContext;

impl InternetComputerContext for StandardInternetComputerContext {
    type Address = AccountIdentifier;
    type Canister = StandardInternetComputerLedgerCanister;
}
