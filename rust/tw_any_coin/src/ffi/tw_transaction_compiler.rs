// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use crate::transaction_compiler::TransactionCompiler;
use tw_coin_registry::coin_type::CoinType;
use tw_memory::ffi::tw_data::TWData;
use tw_memory::ffi::tw_data_vector::TWDataVector;
use tw_memory::ffi::RawPtrTrait;
use tw_misc::try_or_else;

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
    let coin = try_or_else!(CoinType::try_from(coin), std::ptr::null_mut);

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
    let coin = try_or_else!(CoinType::try_from(coin), std::ptr::null_mut);

    TransactionCompiler::compile(
        coin,
        input.as_slice(),
        signatures_ref.to_data_vec(),
        public_keys_ref.to_data_vec(),
    )
    .map(|output| TWData::from(output).into_ptr())
    .unwrap_or_else(|_| std::ptr::null_mut())
}
