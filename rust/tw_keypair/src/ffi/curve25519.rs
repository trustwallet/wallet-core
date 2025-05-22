// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use crate::ed25519::waves::PublicKey as Curve25519PublicKey;
use tw_macros::tw_ffi;
use tw_memory::ffi::{tw_data::TWData, Nonnull, NullableMut, RawPtrTrait};
use tw_misc::traits::ToBytesVec;
use tw_misc::try_or_else;

#[tw_ffi(ty = static_function, class = TWCurve25519, name = PubkeyToEd25519)]
#[no_mangle]
pub unsafe extern "C" fn tw_curve25519_pubkey_to_ed25519(
    pubkey: Nonnull<TWData>,
) -> NullableMut<TWData> {
    let pubkey = TWData::from_ptr_as_ref(pubkey)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    let pubkey = try_or_else!(Curve25519PublicKey::try_from(pubkey), std::ptr::null_mut);
    let ed25519_pubkey = try_or_else!(pubkey.to_standard_pubkey(), std::ptr::null_mut);
    TWData::from(ed25519_pubkey.to_vec()).into_ptr()
}
