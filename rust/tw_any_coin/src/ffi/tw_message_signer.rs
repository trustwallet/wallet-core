// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use crate::message_signer::MessageSigner;
use crate::TWFFICoinType;
use tw_coin_registry::coin_type::CoinType;
use tw_macros::tw_ffi;
use tw_memory::ffi::{tw_data::TWData, Nonnull};
use tw_memory::ffi::{NullableMut, RawPtrTrait};
use tw_misc::{try_or_else, try_or_false};

/// Signs an arbitrary message to prove ownership of an address for off-chain services.
///
/// \param coin The given coin type to sign the message for.
/// \param input The serialized data of a signing input (e.g. TW.Ethereum.Proto.MessageSigningInput).
/// \return The serialized data of a `SigningOutput` proto object. (e.g. TW.Ethereum.Proto.MessageSigningOutput).
#[tw_ffi(ty = static_function, class = TWMessageSigner, name = Sign)]
#[no_mangle]
pub unsafe extern "C" fn tw_message_signer_sign(
    coin: TWFFICoinType,
    input: Nonnull<TWData>,
) -> NullableMut<TWData> {
    let input = try_or_else!(TWData::from_ptr_as_ref(input), std::ptr::null_mut);
    let coin = try_or_else!(CoinType::try_from(coin), std::ptr::null_mut);

    MessageSigner::sign_message(input.as_slice(), coin)
        .map(|output| TWData::from(output).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}

/// Verifies a signature for a message.
///
/// \param coin The given coin type to sign the message for.
/// \param input The serialized data of a verifying input (e.g. TW.Ethereum.Proto.MessageVerifyingInput).
/// \return whether the signature is valid.
#[tw_ffi(ty = static_function, class = TWMessageSigner, name = Verify)]
#[no_mangle]
pub unsafe extern "C" fn tw_message_signer_verify(
    coin: TWFFICoinType,
    input: Nonnull<TWData>,
) -> bool {
    let input = try_or_false!(TWData::from_ptr_as_ref(input));
    let coin = try_or_false!(CoinType::try_from(coin));
    MessageSigner::verify_message(input.as_slice(), coin).unwrap_or_default()
}

/// Computes preimage hashes of a message.
///
/// \param coin The given coin type to sign the message for.
/// \param input The serialized data of a signing input (e.g. TW.Ethereum.Proto.MessageSigningInput).
/// \return The serialized data of TW.TxCompiler.PreSigningOutput.
#[tw_ffi(ty = static_function, class = TWMessageSigner, name = PreImageHashes)]
#[no_mangle]
pub unsafe extern "C" fn tw_message_signer_pre_image_hashes(
    coin: TWFFICoinType,
    input: Nonnull<TWData>,
) -> NullableMut<TWData> {
    let input = try_or_else!(TWData::from_ptr_as_ref(input), std::ptr::null_mut);
    let coin = try_or_else!(CoinType::try_from(coin), std::ptr::null_mut);

    MessageSigner::message_preimage_hashes(input.as_slice(), coin)
        .map(|output| TWData::from(output).into_ptr())
        .unwrap_or_else(|_| std::ptr::null_mut())
}
