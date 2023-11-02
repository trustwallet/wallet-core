// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

pub mod hasher;
pub mod modules;
pub mod private_key;
pub mod public_key;
pub mod transaction;

pub type Address = tw_bech32_address::Bech32Address;

#[allow(non_snake_case)]
#[rustfmt::skip]
pub mod proto {
    use tw_proto::google;

    include!(concat!(env!("OUT_DIR"), "/proto/mod.rs"));
}
