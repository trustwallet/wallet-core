// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

pub mod blockchain_type;
pub mod coin_context;
pub mod dispatcher;
pub mod error;
pub mod registry;

pub mod coin_type {
    include!(concat!(env!("OUT_DIR"), "/coin_type.rs"));
}
