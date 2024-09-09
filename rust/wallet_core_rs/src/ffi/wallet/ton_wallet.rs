// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use tw_hd_wallet::ton::mnemonic::TonMnemonic;
use tw_hd_wallet::ton::TonWallet;
use tw_keypair::ffi::privkey::TWPrivateKey;
use tw_keypair::ffi::pubkey::TWPublicKey;
use tw_keypair::traits::KeyPairTrait;
use tw_keypair::tw;
use tw_memory::ffi::tw_string::TWString;
use tw_memory::ffi::RawPtrTrait;
use tw_misc::{try_or_else, try_or_false};
use tw_ton::modules::wallet_provider::WalletProvider;

pub struct TWTONWallet(TonWallet);

impl RawPtrTrait for TWTONWallet {}

/// Determines whether the English mnemonic and passphrase are valid.
///
/// \param mnemonic Non-null english mnemonic
/// \param passphrase Nullable optional passphrase
/// \note passphrase can be null or empty string if no passphrase required
/// \return whether the mnemonic and passphrase are valid (valid checksum)
#[no_mangle]
pub unsafe extern "C" fn tw_ton_wallet_is_valid_mnemonic(
    mnemonic: *const TWString,
    passphrase: *const TWString,
) -> bool {
    let mnemonic = try_or_false!(TWString::from_ptr_as_ref(mnemonic));
    let mnemonic = try_or_false!(mnemonic.as_str());
    let mnemonic = try_or_false!(TonMnemonic::new(mnemonic));

    let passphrase = TWString::from_ptr_as_ref(passphrase)
        .and_then(TWString::as_str)
        .map(|pass| pass.to_string());

    TonWallet::new(mnemonic, passphrase).is_ok()
}

/// Creates a `TONWallet` from a valid TON mnemonic and passphrase.
///
/// \param mnemonic Non-null english mnemonic
/// \param passphrase Nullable optional passphrase
/// \note Null is returned on invalid mnemonic and passphrase
/// \note passphrase can be null or empty string if no passphrase required
/// \return Nullable TWTONWallet
#[no_mangle]
pub unsafe extern "C" fn tw_ton_wallet_create_with_mnemonic(
    mnemonic: *const TWString,
    passphrase: *const TWString,
) -> *mut TWTONWallet {
    let mnemonic = try_or_else!(TWString::from_ptr_as_ref(mnemonic), std::ptr::null_mut);
    let mnemonic = try_or_else!(mnemonic.as_str(), std::ptr::null_mut);
    let mnemonic = try_or_else!(TonMnemonic::new(mnemonic), std::ptr::null_mut);

    let passphrase = TWString::from_ptr_as_ref(passphrase)
        .and_then(TWString::as_str)
        .map(|pass| pass.to_string());

    let wallet = try_or_else!(TonWallet::new(mnemonic, passphrase), std::ptr::null_mut);
    TWTONWallet(wallet).into_ptr()
}

/// Delete the given TON mnemonic.
///
/// \param wallet *non-null* pointer to TWTONMnemonic.
#[no_mangle]
pub unsafe extern "C" fn tw_ton_wallet_delete(wallet: *mut TWTONWallet) {
    // Take the ownership back to rust and drop the owner.
    let _ = TWTONWallet::from_ptr(wallet);
}

/// Generates Ed25519 private key associated with the wallet.
///
/// \param wallet non-null TWTONWallet
/// \note Returned object needs to be deleted with \TWPrivateKeyDelete
/// \return The Ed25519 private key
#[no_mangle]
pub unsafe extern "C" fn tw_ton_wallet_get_key(wallet: *const TWTONWallet) -> *mut TWPrivateKey {
    let wallet = try_or_else!(TWTONWallet::from_ptr_as_ref(wallet), std::ptr::null_mut);
    let key = wallet.0.to_key_pair().private().clone();
    TWPrivateKey(tw::PrivateKey::from(key)).into_ptr()
}

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
