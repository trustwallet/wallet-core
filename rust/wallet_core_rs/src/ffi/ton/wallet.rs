// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use tw_keypair::ffi::pubkey::TWPublicKey;
use tw_memory::ffi::tw_string::TWString;
use tw_memory::ffi::RawPtrTrait;
use tw_misc::try_or_else;
use tw_ton::modules::wallet_provider::WalletProvider;

/// Constructs a TON Wallet V4R2 stateInit encoded as BoC (BagOfCells) for the given `public_key`.
///
/// \param public_key wallet's public key.
/// \param workchain TON workchain to which the wallet belongs. Usually, base chain is used (0).
/// \param wallet_id wallet's ID allows to create multiple wallets for the same private key.
/// \return Pointer to a base64 encoded Bag Of Cells (BoC) StateInit. Null if invalid public key provided.
#[no_mangle]
pub unsafe extern "C" fn tw_ton_wallet_build_v4_r2_state_init(
    public_key: *const TWPublicKey,
    workchain: i32,
    wallet_id: i32,
) -> *mut TWString {
    let public_key = try_or_else!(TWPublicKey::from_ptr_as_ref(public_key), std::ptr::null_mut);
    let ed_pubkey = try_or_else!(public_key.as_ref().to_ed25519(), std::ptr::null_mut).clone();

    let state_init = try_or_else!(
        WalletProvider::v4r2_state_init(ed_pubkey, workchain, wallet_id),
        std::ptr::null_mut
    );
    TWString::from(state_init).into_ptr()
}

// Constructs a TON Wallet V5R1 stateInit encoded as BoC (BagOfCells) for the given `public_key`.
///
/// \param public_key wallet's public key.
/// \param workchain TON workchain to which the wallet belongs. Usually, base chain is used (0).
/// \param wallet_id wallet's ID allows to create multiple wallets for the same private key.
/// \return Pointer to a base64 encoded Bag Of Cells (BoC) StateInit. Null if invalid public key provided.
#[no_mangle]
pub unsafe extern "C" fn tw_ton_wallet_build_v5_r1_state_init(
    public_key: *const TWPublicKey,
    workchain: i32,
    wallet_id: i32,
) -> *mut TWString {
    let public_key = try_or_else!(TWPublicKey::from_ptr_as_ref(public_key), std::ptr::null_mut);
    let ed_pubkey = try_or_else!(public_key.as_ref().to_ed25519(), std::ptr::null_mut).clone();

    let state_init = try_or_else!(
        WalletProvider::v5r1_state_init(ed_pubkey, workchain, wallet_id),
        std::ptr::null_mut
    );
    TWString::from(state_init).into_ptr()
}
