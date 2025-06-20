// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

#![allow(clippy::missing_safety_doc)]

use crate::modules::barz::core::{
    get_authorization_hash, get_counterfactual_address, get_diamond_cut_code, get_encoded_hash,
    get_formatted_signature, get_init_code, get_prefixed_msg_hash, sign_authorization,
    sign_user_op_hash,
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
/// \param factory The address of the factory contract.
/// \param public_key Public key for the verification facet
/// \param verification_facet The address of the verification facet.
/// \param salt The salt of the init code.
/// \return The init code.
#[tw_ffi(ty = static_function, class = TWBarz, name = GetInitCode)]
#[no_mangle]
pub unsafe extern "C" fn tw_barz_get_init_code(
    factory: Nonnull<TWString>,
    public_key: NonnullMut<TWPublicKey>,
    verification_facet: Nonnull<TWString>,
    salt: u32,
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
/// \param chainId The chainId of the network the verification will happen
/// \return The final hash to be signed.
#[tw_ffi(ty = static_function, class = TWBarz, name = GetPrefixedMsgHash)]
#[no_mangle]
pub unsafe extern "C" fn tw_barz_get_prefixed_msg_hash(
    msg_hash: Nonnull<TWData>,
    barz_address: Nonnull<TWString>,
    chain_id: u32,
) -> NullableMut<TWData> {
    let msg_hash = try_or_else!(TWData::from_ptr_as_ref(msg_hash), std::ptr::null_mut);
    let barz_address = try_or_else!(TWString::from_ptr_as_ref(barz_address), std::ptr::null_mut);
    let barz_address = try_or_else!(barz_address.as_str(), std::ptr::null_mut);
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

/// Computes an Authorization hash in [EIP-7702 format](https://eips.ethereum.org/EIPS/eip-7702)
/// `keccak256('0x05' || rlp([chain_id, address, nonce]))`.
///
/// \param chain_id The chain ID of the user.
/// \param contract_address The address of the smart contract wallet.
/// \param nonce The nonce of the user.
/// \return The authorization hash.
#[tw_ffi(ty = static_function, class = TWBarz, name = GetAuthorizationHash)]
#[no_mangle]
pub unsafe extern "C" fn tw_barz_get_authorization_hash(
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

/// Returns the signed authorization hash
///
/// \param chain_id The chain ID of the user.
/// \param contract_address The address of the smart contract wallet.
/// \param nonce The nonce of the user.
/// \param private_key The private key of the user.
/// \return The signed authorization.
#[tw_ffi(ty = static_function, class = TWBarz, name = SignAuthorization)]
#[no_mangle]
pub unsafe extern "C" fn tw_barz_sign_authorization(
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

/// Returns the encoded hash of the user operation
///
/// \param chain_id The chain ID of the user.
/// \param code_address The address of the smart contract wallet.
/// \param code_name The name of the smart contract wallet.
/// \param code_version The version of the smart contract wallet.
/// \param type_hash The type hash of the smart contract wallet.
/// \param domain_separator_hash The domain separator hash of the smart contract wallet.
/// \param sender The sender of the smart contract wallet.
/// \param user_op_hash The user operation hash of the smart contract wallet.
/// \return The encoded hash.
#[tw_ffi(ty = static_function, class = TWBarz, name = GetEncodedHash)]
#[no_mangle]
pub unsafe extern "C" fn tw_barz_get_encoded_hash(
    chain_id: Nonnull<TWData>,
    code_address: Nonnull<TWString>,
    code_name: Nonnull<TWString>,
    code_version: Nonnull<TWString>,
    type_hash: Nonnull<TWString>,
    domain_separator_hash: Nonnull<TWString>,
    sender: Nonnull<TWString>,
    user_op_hash: Nonnull<TWString>,
) -> NullableMut<TWData> {
    let chain_id = try_or_else!(TWData::from_ptr_as_ref(chain_id), std::ptr::null_mut);
    let code_address = try_or_else!(TWString::from_ptr_as_ref(code_address), std::ptr::null_mut);
    let code_address = try_or_else!(code_address.as_str(), std::ptr::null_mut);
    let code_name = try_or_else!(TWString::from_ptr_as_ref(code_name), std::ptr::null_mut);
    let code_name = try_or_else!(code_name.as_str(), std::ptr::null_mut);
    let code_version = try_or_else!(TWString::from_ptr_as_ref(code_version), std::ptr::null_mut);
    let code_version = try_or_else!(code_version.as_str(), std::ptr::null_mut);
    let type_hash = try_or_else!(TWString::from_ptr_as_ref(type_hash), std::ptr::null_mut);
    let type_hash = try_or_else!(type_hash.as_str(), std::ptr::null_mut);
    let domain_separator_hash = try_or_else!(
        TWString::from_ptr_as_ref(domain_separator_hash),
        std::ptr::null_mut
    );
    let domain_separator_hash = try_or_else!(domain_separator_hash.as_str(), std::ptr::null_mut);
    let sender = try_or_else!(TWString::from_ptr_as_ref(sender), std::ptr::null_mut);
    let sender = try_or_else!(sender.as_str(), std::ptr::null_mut);
    let user_op_hash = try_or_else!(TWString::from_ptr_as_ref(user_op_hash), std::ptr::null_mut);
    let user_op_hash = try_or_else!(user_op_hash.as_str(), std::ptr::null_mut);
    let encoded_hash = try_or_else!(
        get_encoded_hash(
            chain_id.as_slice(),
            code_address,
            code_name,
            code_version,
            type_hash,
            domain_separator_hash,
            sender,
            user_op_hash
        ),
        std::ptr::null_mut
    );
    TWData::from(encoded_hash).into_ptr()
}

/// Signs a message using the private key
///
/// \param hash The hash of the user.
/// \param private_key The private key of the user.
/// \return The signed hash.
#[tw_ffi(ty = static_function, class = TWBarz, name = GetSignedHash)]
#[no_mangle]
pub unsafe extern "C" fn tw_barz_get_signed_hash(
    hash: Nonnull<TWString>,
    private_key: Nonnull<TWString>,
) -> NullableMut<TWData> {
    let hash = try_or_else!(TWString::from_ptr_as_ref(hash), std::ptr::null_mut);
    let hash = try_or_else!(hash.as_str(), std::ptr::null_mut);
    let private_key = try_or_else!(TWString::from_ptr_as_ref(private_key), std::ptr::null_mut);
    let private_key = try_or_else!(private_key.as_str(), std::ptr::null_mut);
    let signed_hash = try_or_else!(sign_user_op_hash(hash, private_key), std::ptr::null_mut);
    TWData::from(signed_hash).into_ptr()
}
