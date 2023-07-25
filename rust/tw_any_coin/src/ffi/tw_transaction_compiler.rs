// Copyright © 2017-2023 Trust Wallet.
//
// This file is part of Trust. The full Trust copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#![allow(clippy::missing_safety_doc)]

use crate::transaction_compiler::TransactionCompiler;
use tw_coin_entry::modules::input_builder::BuildSigningInputArgs;
use tw_memory::ffi::tw_data::TWData;
use tw_memory::ffi::tw_data_vector::TWDataVector;
use tw_memory::ffi::tw_string::TWString;
use tw_memory::ffi::RawPtrTrait;
use tw_misc::try_or_else;

/// Builds a coin-specific `SigningInput` (proto object) from a simple transaction.
///
/// \param coin coin type.
/// \param from sender of the transaction.
/// \param to receiver of the transaction.
/// \param amount transaction amount in string
/// \param asset optional asset name, like "BNB"
/// \param memo optional memo
/// \param chain_id optional chainId to override default
/// \return serialized data of the SigningInput proto object.
#[no_mangle]
pub unsafe extern "C" fn tw_transaction_compiler_build_input(
    coin: u32,
    from: *const TWString,
    to: *const TWString,
    amount: *const TWString,
    asset: *const TWString,
    memo: *const TWString,
    chain_id: *const TWString,
) -> *mut TWData {
    macro_rules! try_str {
        ($arg:expr) => {{
            // Try to cast `*const TWString` into `&TWString`.
            let tw_string = try_or_else!(TWString::from_ptr_as_ref($arg), std::ptr::null_mut);
            // Try to get `&str` from `&TWString`.
            let str = try_or_else!(tw_string.as_str(), std::ptr::null_mut);
            str.to_string()
        }};
    }

    let input = BuildSigningInputArgs {
        from: try_str!(from),
        to: try_str!(to),
        amount: try_str!(amount),
        asset: try_str!(asset),
        memo: try_str!(memo),
        chain_id: try_str!(chain_id),
    };

    TransactionCompiler::build_input(coin, input)
        .map(|output| TWData::from(output).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Obtains pre-signing hashes of a transaction.
///
/// We provide a default `PreSigningOutput` in TransactionCompiler.proto.
/// For some special coins, such as bitcoin, we will create a custom `PreSigningOutput` object in its proto file.
/// \param coin coin type.
/// \param input The serialized data of a signing input
/// \return serialized data of a proto object `PreSigningOutput` includes hash.
#[no_mangle]
pub unsafe extern "C" fn tw_transaction_compiler_pre_image_hashes(
    coin: u32,
    input: *const TWData,
) -> *mut TWData {
    let input = try_or_else!(TWData::from_ptr_as_ref(input), std::ptr::null_mut);

    TransactionCompiler::preimage_hashes(coin, input.as_slice())
        .map(|output| TWData::from(output).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Compiles a complete transaction with one or more external signatures.
///
/// Puts together from transaction input and provided public keys and signatures. The signatures must match the hashes
/// returned by `TWTransactionCompilerPreImageHashes`, in the same order. The publicKeyHash attached
/// to the hashes enable identifying the private key needed for signing the hash.
/// \param coin coin type.
/// \param input The serialized data of a signing input.
/// \param signatures signatures to compile, using `TWDataVector`.
/// \param public_keys public keys for signers to match private keys, using `TWDataVector`.
/// \return serialized data of a proto object `SigningOutput`.
#[no_mangle]
pub unsafe extern "C" fn tw_transaction_compiler_compile(
    coin: u32,
    input: *const TWData,
    signatures: *const TWDataVector,
    public_keys: *const TWDataVector,
) -> *mut TWData {
    let input = try_or_else!(TWData::from_ptr_as_ref(input), std::ptr::null_mut);
    let signatures_ref = try_or_else!(
        TWDataVector::from_ptr_as_ref(signatures),
        std::ptr::null_mut
    );
    let public_keys_ref = try_or_else!(
        TWDataVector::from_ptr_as_ref(public_keys),
        std::ptr::null_mut
    );

    TransactionCompiler::compile(
        coin,
        input.as_slice(),
        signatures_ref.to_data_vec(),
        public_keys_ref.to_data_vec(),
    )
    .map(|output| TWData::from(output).into_ptr())
    .unwrap_or_else(|_| std::ptr::null_mut())
}
