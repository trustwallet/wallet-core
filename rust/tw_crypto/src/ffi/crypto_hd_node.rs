// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![allow(clippy::missing_safety_doc)]

use crate::crypto_hd_node::hd_node::HDNode;
use tw_hash::hasher::Hasher;
use tw_keypair::tw::Curve;
use tw_macros::tw_ffi;
use tw_memory::ffi::{
    tw_data::TWData, tw_string::TWString, Nonnull, NonnullMut, NullableMut, RawPtrTrait,
};
use tw_misc::try_or_else;

pub struct TWHDNode(pub(crate) HDNode);

impl RawPtrTrait for TWHDNode {}

impl AsRef<HDNode> for TWHDNode {
    fn as_ref(&self) -> &HDNode {
        &self.0
    }
}

/// Create a HDNode with the given seed and curve.
///
/// \param seed *non-null* byte array.
/// \param curve the curve to use.
/// \note Should be deleted with \tw_hd_node_delete.
/// \return Nullable pointer to HDNode.
#[tw_ffi(ty = constructor, class = TWHDNode, name = CreateWithSeed)]
#[no_mangle]
pub unsafe extern "C" fn tw_hd_node_create_with_seed(
    seed: Nonnull<TWData>,
    curve: u32,
) -> NullableMut<TWHDNode> {
    let data = TWData::from_ptr_as_ref(seed)
        .map(|data| data.as_slice())
        .unwrap_or_default();
    let curve = try_or_else!(Curve::from_raw(curve), std::ptr::null_mut);

    HDNode::new(data, curve)
        .map(|hd_node| TWHDNode(hd_node).into_ptr())
        // Return null if the private key is invalid.
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Create a HDNode from an extended private key.
///
/// \param extended_private_key *non-null* string.
/// \param curve the curve to use.
/// \param hasher the hasher to use.
/// \return Nullable pointer to HDNode.
#[tw_ffi(ty = constructor, class = TWHDNode, name = CreateWithExtendedPrivateKey)]
#[no_mangle]
pub unsafe extern "C" fn tw_hd_node_create_with_extended_private_key(
    extended_private_key: Nonnull<TWString>,
    curve: u32,
    hasher: u32,
) -> NullableMut<TWHDNode> {
    let extended_private_key_ref = try_or_else!(
        TWString::from_ptr_as_ref(extended_private_key),
        std::ptr::null_mut
    );
    let extended_private_key_str =
        try_or_else!(extended_private_key_ref.as_str(), std::ptr::null_mut);
    let curve = try_or_else!(Curve::from_raw(curve), std::ptr::null_mut);
    let hasher = try_or_else!(Hasher::try_from(hasher), std::ptr::null_mut);
    HDNode::try_from(extended_private_key_str, curve, hasher)
        .map(|hd_node| TWHDNode(hd_node).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Delete the given HDNode.
///
/// \param key *non-null* pointer to HDNode.
#[tw_ffi(ty = destructor, class = TWHDNode, name = Delete)]
#[no_mangle]
pub unsafe extern "C" fn tw_hd_node_delete(key: NonnullMut<TWHDNode>) {
    // Take the ownership back to rust and drop the owner.
    let _ = TWHDNode::from_ptr(key);
}

/// Derive a child node from a given hd_node.
///
/// \param hd_node *non-null* pointer to a hd_node.
/// \param path the path to derive.
/// \return Nullable pointer to HDNode.
#[tw_ffi(ty = static_function, class = TWHDNode, name = DeriveFromPath)]
#[no_mangle]
pub unsafe extern "C" fn tw_hd_node_derive_from_path(
    hd_node: Nonnull<TWHDNode>,
    path: Nonnull<TWString>,
    hasher: u32,
) -> NullableMut<TWHDNode> {
    let hd_node_ref = try_or_else!(TWHDNode::from_ptr_as_ref(hd_node), std::ptr::null_mut);
    let path_ref = try_or_else!(TWString::from_ptr_as_ref(path), std::ptr::null_mut);
    let path_str = try_or_else!(path_ref.as_str(), std::ptr::null_mut);
    let hasher = try_or_else!(Hasher::try_from(hasher), std::ptr::null_mut);
    hd_node_ref
        .0
        .derive_from_path(path_str, hasher)
        .map(|hd_node| TWHDNode(hd_node).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Returns the raw private-key data of a given hd_node.
///
/// \param hd_node *non-null* pointer to a hd_node.
/// \return byte array.
#[tw_ffi(ty = property, class = TWHDNode, name = PrivateKeyData)]
#[no_mangle]
pub unsafe extern "C" fn tw_hd_node_private_key_data(
    hd_node: Nonnull<TWHDNode>,
) -> NonnullMut<TWData> {
    let hd_node_ref = try_or_else!(TWHDNode::from_ptr_as_ref(hd_node), std::ptr::null_mut);
    hd_node_ref
        .0
        .private_key_data()
        .map(|data| TWData::from(data).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Returns the raw public-key data of a given hd_node.
///
/// \param hd_node *non-null* pointer to a hd_node.
/// \return byte array.
#[tw_ffi(ty = property, class = TWHDNode, name = PublicKeyData)]
#[no_mangle]
pub unsafe extern "C" fn tw_hd_node_public_key_data(
    hd_node: Nonnull<TWHDNode>,
) -> NonnullMut<TWData> {
    let hd_node_ref = try_or_else!(TWHDNode::from_ptr_as_ref(hd_node), std::ptr::null_mut);
    hd_node_ref
        .0
        .public_key_data()
        .map(|data| TWData::from(data).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Returns the chain code of a given hd_node.
///
/// \param hd_node *non-null* pointer to a hd_node.
/// \return byte array.
#[tw_ffi(ty = property, class = TWHDNode, name = ChainCode)]
#[no_mangle]
pub unsafe extern "C" fn tw_hd_node_chain_code(hd_node: Nonnull<TWHDNode>) -> NonnullMut<TWData> {
    let hd_node_ref = try_or_else!(TWHDNode::from_ptr_as_ref(hd_node), std::ptr::null_mut);
    hd_node_ref
        .0
        .chain_code()
        .map(|data| TWData::from(data).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Returns the depth of a given hd_node.
///
/// \param hd_node *non-null* pointer to a hd_node.
/// \return depth.
#[tw_ffi(ty = property, class = TWHDNode, name = Depth)]
#[no_mangle]
pub unsafe extern "C" fn tw_hd_node_depth(hd_node: Nonnull<TWHDNode>) -> u8 {
    let hd_node_ref = try_or_else!(TWHDNode::from_ptr_as_ref(hd_node), || 0);
    hd_node_ref.0.depth().unwrap_or_default()
}

/// Returns the child number of a given hd_node.
///
/// \param hd_node *non-null* pointer to a hd_node.
/// \return child number.
#[tw_ffi(ty = property, class = TWHDNode, name = ChildNumber)]
#[no_mangle]
pub unsafe extern "C" fn tw_hd_node_child_number(hd_node: Nonnull<TWHDNode>) -> u32 {
    let hd_node_ref = try_or_else!(TWHDNode::from_ptr_as_ref(hd_node), || 0);
    hd_node_ref.0.child_number().unwrap_or_default()
}

/// Returns the extended private key of a given hd_node.
///
/// \param hd_node *non-null* pointer to a hd_node.
/// \return extended private key.
#[tw_ffi(ty = static_function, class = TWHDNode, name = ExtendedPrivateKey)]
#[no_mangle]
pub unsafe extern "C" fn tw_hd_node_extended_private_key(
    hd_node: Nonnull<TWHDNode>,
    version: u32,
    hasher: u32,
) -> NonnullMut<TWString> {
    let hasher = try_or_else!(Hasher::try_from(hasher), std::ptr::null_mut);
    let hd_node_ref = try_or_else!(TWHDNode::from_ptr_as_ref(hd_node), std::ptr::null_mut);
    hd_node_ref
        .0
        .extended_private_key(version, hasher)
        .map(|key| TWString::from(key).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Returns the extended public key of a given hd_node.
///
/// \param hd_node *non-null* pointer to a hd_node.
/// \return extended public key.
#[tw_ffi(ty = static_function, class = TWHDNode, name = ExtendedPublicKey)]
#[no_mangle]
pub unsafe extern "C" fn tw_hd_node_extended_public_key(
    hd_node: Nonnull<TWHDNode>,
    version: u32,
    hasher: u32,
) -> NonnullMut<TWString> {
    let hasher = try_or_else!(Hasher::try_from(hasher), std::ptr::null_mut);
    let hd_node_ref = try_or_else!(TWHDNode::from_ptr_as_ref(hd_node), std::ptr::null_mut);
    hd_node_ref
        .0
        .extended_public_key(version, hasher)
        .map(|key| TWString::from(key).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}
