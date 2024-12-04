// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use tw_proto::Polymesh::Proto;

mod polymesh_address;
mod polymesh_compile;
mod polymesh_sign;

pub const GENESIS_HASH: &str = "0x6fbd74e5e1d0a61d52ccfe9d4adaed16dd3a7caa37c6bc4d0c2fa12e8b2f4063";
/// Private key for testing.  DO NOT USE, since this is public.
pub const PRIVATE_KEY_1: &str =
    "0x790a0a01ec2e7c7db4abcaffc92ce70a960ef9ad3021dbe3bf327c1c6343aee4";
pub const PUBLIC_KEY_1: &str = "2EANwBfNsFu9KV8JsW5sbhF6ft8bzvw5EW1LCrgHhrqtK6Ys";
pub const PUBLIC_KEY_HEX_1: &str =
    "0x4bdb9ef424035e1621e228bd11c5917d7d1dac5965d244c4c72fc91170244f0c";

pub const PUBLIC_KEY_2: &str = "2CpqFh8VnwJAjenw4xSUWCaaJ2QwGdhnCikoSEczMhjgqyj7";

pub fn balance_call(
    call: Proto::mod_Balance::OneOfmessage_oneof,
) -> Option<Proto::RuntimeCall<'_>> {
    Some(Proto::RuntimeCall {
        pallet_oneof: Proto::mod_RuntimeCall::OneOfpallet_oneof::balance_call(Proto::Balance {
            message_oneof: call,
        }),
    })
}

pub fn identity_call(
    call: Proto::mod_Identity::OneOfmessage_oneof,
) -> Option<Proto::RuntimeCall<'_>> {
    Some(Proto::RuntimeCall {
        pallet_oneof: Proto::mod_RuntimeCall::OneOfpallet_oneof::identity_call(Proto::Identity {
            message_oneof: call,
        }),
    })
}
