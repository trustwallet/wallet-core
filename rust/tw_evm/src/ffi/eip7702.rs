// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use crate::modules::eip7702::{get_authorization_hash, sign_authorization};
use tw_macros::tw_ffi;
use tw_memory::ffi::tw_data::TWData;
use tw_memory::ffi::tw_string::TWString;
use tw_memory::ffi::{Nonnull, NullableMut, RawPtrTrait};
use tw_misc::try_or_else;

/// Signs an Authorization hash in [EIP-7702 format](https://eips.ethereum.org/EIPS/eip-7702)
///
/// \param chain_id The chain ID of the user.
/// \param contract_address The address of the smart contract wallet.
/// \param nonce The nonce of the user.
/// \param private_key The private key of the user.
/// \return The signed authorization.
#[tw_ffi(ty = static_function, class = TWEip7702, name = SignAuthorization)]
#[no_mangle]
pub unsafe extern "C" fn tw_eip7702_sign_authorization(
    chain_id: Nonnull<TWData>,
    contract_address: Nonnull<TWString>,
    nonce: Nonnull<TWData>,
    private_key: Nonnull<TWString>,
) -> NullableMut<TWString> {
    let chain_id = try_or_else!(TWData::from_ptr_as_ref(chain_id), std::ptr::null_mut);
    let contract_address = try_or_else!(
        TWString::from_ptr_as_ref(contract_address),
        std::ptr::null_mut
    );
    let contract_address = try_or_else!(contract_address.as_str(), std::ptr::null_mut);
    let nonce = try_or_else!(TWData::from_ptr_as_ref(nonce), std::ptr::null_mut);
    let private_key = try_or_else!(TWString::from_ptr_as_ref(private_key), std::ptr::null_mut);
    let private_key = try_or_else!(private_key.as_str(), std::ptr::null_mut);
    let signed_authorization = try_or_else!(
        sign_authorization(
            chain_id.as_slice(),
            contract_address,
            nonce.as_slice(),
            private_key
        ),
        std::ptr::null_mut
    );
    TWString::from(signed_authorization).into_ptr()
}

/// Computes an Authorization hash in [EIP-7702 format](https://eips.ethereum.org/EIPS/eip-7702)
/// `keccak256('0x05' || rlp([chain_id, address, nonce]))`.
///
/// \param chain_id The chain ID of the user.
/// \param contract_address The address of the smart contract wallet.
/// \param nonce The nonce of the user.
/// \return The authorization hash.
#[tw_ffi(ty = static_function, class = TWEip7702, name = GetAuthorizationHash)]
#[no_mangle]
pub unsafe extern "C" fn tw_eip7702_get_authorization_hash(
    chain_id: Nonnull<TWData>,
    contract_address: Nonnull<TWString>,
    nonce: Nonnull<TWData>,
) -> NullableMut<TWData> {
    let chain_id = try_or_else!(TWData::from_ptr_as_ref(chain_id), std::ptr::null_mut);
    let contract_address = try_or_else!(
        TWString::from_ptr_as_ref(contract_address),
        std::ptr::null_mut
    );
    let contract_address = try_or_else!(contract_address.as_str(), std::ptr::null_mut);
    let nonce = try_or_else!(TWData::from_ptr_as_ref(nonce), std::ptr::null_mut);
    let authorization_hash = try_or_else!(
        get_authorization_hash(chain_id.as_slice(), contract_address, nonce.as_slice()),
        std::ptr::null_mut
    );
    TWData::from(authorization_hash).into_ptr()
}
