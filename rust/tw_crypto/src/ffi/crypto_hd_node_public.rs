// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![allow(clippy::missing_safety_doc)]

use crate::crypto_hd_node::hd_node_public::HDNodePublic;
use tw_hash::hasher::Hasher;
use tw_keypair::tw::Curve;
use tw_macros::tw_ffi;
use tw_memory::ffi::{
    tw_data::TWData, tw_string::TWString, Nonnull, NonnullMut, NullableMut, RawPtrTrait,
};
use tw_misc::try_or_else;

pub struct TWHDNodePublic(pub(crate) HDNodePublic);

impl RawPtrTrait for TWHDNodePublic {}

impl AsRef<HDNodePublic> for TWHDNodePublic {
    fn as_ref(&self) -> &HDNodePublic {
        &self.0
    }
}

/// Create a HDNodePublic from an extended public key.
///
/// \param extended_public_key *non-null* string.
/// \param curve the curve to use.
/// \param hasher the hasher to use.
/// \return Nullable pointer to HDNodePublic.
#[tw_ffi(ty = constructor, class = TWHDNodePublic, name = CreateWithExtendedPublicKey)]
#[no_mangle]
pub unsafe extern "C" fn tw_hd_node_public_create_with_extended_public_key(
    extended_public_key: Nonnull<TWString>,
    curve: u32,
    hasher: u32,
) -> NullableMut<TWHDNodePublic> {
    let extended_public_key_ref = try_or_else!(
        TWString::from_ptr_as_ref(extended_public_key),
        std::ptr::null_mut
    );
    let extended_public_key_str =
        try_or_else!(extended_public_key_ref.as_str(), std::ptr::null_mut);
    let curve = try_or_else!(Curve::from_raw(curve), std::ptr::null_mut);
    let hasher = try_or_else!(Hasher::from_repr(hasher), std::ptr::null_mut);
    HDNodePublic::try_from(extended_public_key_str, curve, hasher)
        .map(|hd_node| TWHDNodePublic(hd_node).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Delete the given HDNode.
///
/// \param key *non-null* pointer to HDNode.
#[tw_ffi(ty = destructor, class = TWHDNodePublic, name = Delete)]
#[no_mangle]
pub unsafe extern "C" fn tw_hd_node_public_delete(key: NonnullMut<TWHDNodePublic>) {
    // Take the ownership back to rust and drop the owner.
    let _ = TWHDNodePublic::from_ptr(key);
}

/// Derive a child key for a particular path.
///
/// \param hd_node *non-null* pointer to HDNodePublic.
/// \param path *non-null* string.
/// \return Nullable pointer to HDNodePublic.
#[tw_ffi(ty = static_function, class = TWHDNodePublic, name = DeriveFromPath)]
#[no_mangle]
pub unsafe extern "C" fn tw_hd_node_public_derive_from_path(
    hd_node: Nonnull<TWHDNodePublic>,
    path: Nonnull<TWString>,
    hasher: u32,
) -> NullableMut<TWHDNodePublic> {
    let hd_node_ref = try_or_else!(TWHDNodePublic::from_ptr_as_ref(hd_node), std::ptr::null_mut);
    let path_ref = try_or_else!(TWString::from_ptr_as_ref(path), std::ptr::null_mut);
    let path_str = try_or_else!(path_ref.as_str(), std::ptr::null_mut);
    let hasher = try_or_else!(Hasher::from_repr(hasher), std::ptr::null_mut);
    hd_node_ref
        .0
        .derive_from_path(path_str, hasher)
        .map(|hd_node| TWHDNodePublic(hd_node).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Get the public key data.
///
/// \param hd_node *non-null* pointer to HDNodePublic.
/// \return *non-null* pointer to TWData.
#[tw_ffi(ty = property, class = TWHDNodePublic, name = PublicKeyData)]
#[no_mangle]
pub unsafe extern "C" fn tw_hd_node_public_public_key_data(
    hd_node: Nonnull<TWHDNodePublic>,
) -> NonnullMut<TWData> {
    let hd_node_ref = try_or_else!(TWHDNodePublic::from_ptr_as_ref(hd_node), std::ptr::null_mut);
    hd_node_ref
        .0
        .public_key_data()
        .map(|data| TWData::from(data).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}
