// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

pub mod blockchain_type;
pub mod coin_context;
pub mod dispatcher;
pub mod error;
pub mod registry;

pub mod coin_type {
    include!(concat!(env!("OUT_DIR"), "/coin_type.rs"));
}
