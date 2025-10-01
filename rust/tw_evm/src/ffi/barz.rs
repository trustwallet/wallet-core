// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use crate::modules::barz::core::{
    get_counterfactual_address, get_diamond_cut_code, get_formatted_signature, get_init_code,
    get_prefixed_msg_hash,
};
use tw_keypair::ffi::pubkey::{tw_public_key_data, TWPublicKey};
use tw_macros::tw_ffi;
use tw_memory::ffi::{
    tw_data::TWData, tw_string::TWString, Nonnull, NonnullMut, NullableMut, RawPtrTrait,
};
use tw_misc::try_or_else;
use tw_proto::{
    Barz::Proto::{ContractAddressInput, DiamondCutInput},
    BytesReader, MessageRead,
};

/// Calculate a counterfactual address for the smart contract wallet
///
/// \param input The serialized data of ContractAddressInput.
/// \return The address.
#[tw_ffi(ty = static_function, class = TWBarz, name = GetCounterfactualAddress)]
#[no_mangle]
pub unsafe extern "C" fn tw_barz_get_counterfactual_address(
    input: Nonnull<TWData>,
) -> NullableMut<TWString> {
    let input = try_or_else!(TWData::from_ptr_as_ref(input), std::ptr::null_mut);
    let mut reader = BytesReader::from_bytes(input.as_slice());
    let input = try_or_else!(
        ContractAddressInput::from_reader(&mut reader, input.as_slice()),
        std::ptr::null_mut
    );
    let address = try_or_else!(get_counterfactual_address(&input), std::ptr::null_mut);
    TWString::from(address).into_ptr()
}

/// Returns the init code parameter of ERC-4337 User Operation
///
/// \param factory The address of the factory contract
/// \param public_key Public key for the verification facet
/// \param verification_facet The address of the verification facet
/// \param salt The salt of the init code; Must be non-negative
/// \return The init code.
#[tw_ffi(ty = static_function, class = TWBarz, name = GetInitCode)]
#[no_mangle]
pub unsafe extern "C" fn tw_barz_get_init_code(
    factory: Nonnull<TWString>,
    public_key: NonnullMut<TWPublicKey>,
    verification_facet: Nonnull<TWString>,
    salt: i32,
) -> NullableMut<TWData> {
    let factory_address = try_or_else!(TWString::from_ptr_as_ref(factory), std::ptr::null_mut);
    let factory_address = try_or_else!(factory_address.as_str(), std::ptr::null_mut);
    let public_key = try_or_else!(TWPublicKey::from_ptr_as_mut(public_key), std::ptr::null_mut);
    let public_key_data = tw_public_key_data(public_key);
    let verification_facet = try_or_else!(
        TWString::from_ptr_as_ref(verification_facet),
        std::ptr::null_mut
    );
    let verification_facet = try_or_else!(verification_facet.as_str(), std::ptr::null_mut);
    let salt = try_or_else!(salt.try_into(), std::ptr::null_mut);

    let init_code = try_or_else!(
        get_init_code(
            factory_address,
            public_key_data.as_slice(),
            verification_facet,
            salt
        ),
        std::ptr::null_mut
    );
    TWData::from(init_code).into_ptr()
}

/// Converts the original ASN-encoded signature from webauthn to the format accepted by Barz
///
/// \param signature Original signature
/// \param challenge The original challenge that was signed
/// \param authenticator_data Returned from Webauthn API
/// \param client_data_json Returned from Webauthn API
/// \return Bytes of the formatted signature
#[tw_ffi(ty = static_function, class = TWBarz, name = GetFormattedSignature)]
#[no_mangle]
pub unsafe extern "C" fn tw_barz_get_formatted_signature(
    signature: Nonnull<TWData>,
    challenge: Nonnull<TWData>,
    authenticator_data: Nonnull<TWData>,
    client_data_json: Nonnull<TWString>,
) -> NullableMut<TWData> {
    let signature = try_or_else!(TWData::from_ptr_as_ref(signature), std::ptr::null_mut);
    let challenge = try_or_else!(TWData::from_ptr_as_ref(challenge), std::ptr::null_mut);
    let authenticator_data = try_or_else!(
        TWData::from_ptr_as_ref(authenticator_data),
        std::ptr::null_mut
    );
    let client_data_json = try_or_else!(
        TWString::from_ptr_as_ref(client_data_json),
        std::ptr::null_mut
    );
    let client_data_json = try_or_else!(client_data_json.as_str(), std::ptr::null_mut);
    let formatted_signature = try_or_else!(
        get_formatted_signature(
            signature.as_slice(),
            challenge.as_slice(),
            authenticator_data.as_slice(),
            client_data_json
        ),
        std::ptr::null_mut
    );
    TWData::from(formatted_signature).into_ptr()
}

/// Returns the final hash to be signed by Barz for signing messages & typed data
///
/// \param msg_hash Original msgHash
/// \param barzAddress The address of Barz wallet signing the message
/// \param chainId The chainId of the network the verification will happen; Must be non-negative
/// \return The final hash to be signed.
#[tw_ffi(ty = static_function, class = TWBarz, name = GetPrefixedMsgHash)]
#[no_mangle]
pub unsafe extern "C" fn tw_barz_get_prefixed_msg_hash(
    msg_hash: Nonnull<TWData>,
    barz_address: Nonnull<TWString>,
    chain_id: i32,
) -> NullableMut<TWData> {
    let msg_hash = try_or_else!(TWData::from_ptr_as_ref(msg_hash), std::ptr::null_mut);
    let barz_address = try_or_else!(TWString::from_ptr_as_ref(barz_address), std::ptr::null_mut);
    let barz_address = try_or_else!(barz_address.as_str(), std::ptr::null_mut);
    let chain_id = try_or_else!(chain_id.try_into(), std::ptr::null_mut);

    let prefixed_msg_hash = try_or_else!(
        get_prefixed_msg_hash(msg_hash.as_slice(), barz_address, chain_id),
        std::ptr::null_mut
    );
    TWData::from(prefixed_msg_hash).into_ptr()
}

/// Returns the encoded diamondCut function call for Barz contract upgrades
///
/// \param input The serialized data of DiamondCutInput.
/// \return The diamond cut code.
#[tw_ffi(ty = static_function, class = TWBarz, name = GetDiamondCutCode)]
#[no_mangle]
pub unsafe extern "C" fn tw_barz_get_diamond_cut_code(
    input: Nonnull<TWData>,
) -> NullableMut<TWData> {
    let input = try_or_else!(TWData::from_ptr_as_ref(input), std::ptr::null_mut);
    let mut reader = BytesReader::from_bytes(input.as_slice());
    let input = try_or_else!(
        DiamondCutInput::from_reader(&mut reader, input.as_slice()),
        std::ptr::null_mut
    );
    let diamond_cut_code = try_or_else!(get_diamond_cut_code(&input), std::ptr::null_mut);
    TWData::from(diamond_cut_code).into_ptr()
}
