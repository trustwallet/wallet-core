// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::abi::function::Function;
use crate::abi::non_empty_array::NonEmptyBytes;
use crate::abi::param::Param;
use crate::abi::param_type::ParamType;
use crate::abi::uint::UintBits;
use crate::abi::{encode, token::Token};
use crate::address::Address;
use crate::message::EthMessage;
use crate::transaction::authorization_list::{Authorization, SignedAuthorization};
use std::str::FromStr;
use tw_encoding::{base64, hex};
use tw_hash::sha3::keccak256;
use tw_hash::H256;
use tw_keypair::ecdsa::der;
use tw_keypair::ecdsa::secp256k1::PrivateKey;
use tw_keypair::traits::SigningKeyTrait;
use tw_misc::traits::ToBytesVec;
use tw_number::U256;
use tw_proto::Barz::Proto::{ContractAddressInput, DiamondCutInput};

use super::error::BarzResult;

const BARZ_DOMAIN_SEPARATOR_TYPE_HASH: &str =
    "47e79534a245952e8b16893a336b85a3d9ea9fa8c573f3d803afb92a79469218";
const BARZ_MSG_HASH_DATA: &str = "b1bcb804a4a3a1af3ee7920d949bdfd417ea1b736c3552c8d6563a229a619100";
const BARZ_SIGNED_DATA_PREFIX: &str = "1901";

const BARZ_DIAMOND_CUT_SELECTOR: &str = "1f931c1c";
const BARZ_DATA_LOCATION_CHUNK: &str = "60";

pub fn get_counterfactual_address(input: &ContractAddressInput) -> BarzResult<String> {
    let encoded_data = encode::encode_tokens(&[
        Token::Address(Address::from_str(&input.account_facet)?),
        Token::Address(Address::from_str(&input.verification_facet)?),
        Token::Address(Address::from_str(&input.entry_point)?),
        Token::Address(Address::from_str(&input.facet_registry)?),
        Token::Address(Address::from_str(&input.default_fallback)?),
        Token::Bytes(hex::decode(&input.public_key)?),
    ]);

    let mut init_code = hex::decode(&input.bytecode)?;
    init_code.extend_from_slice(&encoded_data);
    let init_code_hash = keccak256(&init_code);
    let init_code_hash = H256::try_from(init_code_hash.as_slice())?;

    let salt = input.salt.to_be_bytes();
    let mut salt_bytes = [0u8; 32];
    salt_bytes[32 - salt.len()..].copy_from_slice(&salt);
    let salt_bytes = H256::try_from(salt_bytes.as_slice())?;

    let address = Address::eip1014_create2_address(&input.factory, &salt_bytes, &init_code_hash)?;
    Ok(address.into_checksum_address())
}

pub fn get_init_code(
    factory_address: &str,
    public_key: &[u8],
    verification_facet: &str,
    salt: u32,
) -> BarzResult<Vec<u8>> {
    let tokens = [
        Token::Address(Address::from_str(verification_facet)?),
        Token::Bytes(public_key.to_vec()),
        Token::Uint {
            uint: salt.into(),
            bits: UintBits::default(),
        },
    ];

    let function = Function {
        name: "createAccount".to_string(),
        inputs: vec![
            Param::with_type(ParamType::Address),
            Param::with_type(ParamType::Bytes),
            Param::with_type(ParamType::Uint {
                bits: UintBits::default(),
            }),
        ],
        outputs: vec![],
    };
    let encoded = function.encode_input(&tokens)?;

    let mut envelope = Vec::new();
    envelope.extend_from_slice(&hex::decode(factory_address)?);
    envelope.extend_from_slice(&encoded);
    Ok(envelope)
}

pub fn get_formatted_signature(
    signature: &[u8],
    challenge: &[u8],
    authenticator_data: &[u8],
    client_data_json: &str,
) -> BarzResult<Vec<u8>> {
    let challenge_base64 = base64::encode(challenge, base64::URL_SAFE);

    let challenge_base64 = challenge_base64.trim_end_matches('=');

    let Some(challenge_pos) = client_data_json.find(challenge_base64) else {
        return Ok(Vec::new());
    };

    let client_data_json_pre = &client_data_json[..challenge_pos];
    let client_data_json_post = &client_data_json[challenge_pos + challenge_base64.len()..];

    let Ok(signature) = der::Signature::from_bytes(signature) else {
        return Ok(Vec::new());
    };

    Ok(encode::encode_tokens(&[
        Token::Uint {
            uint: U256::from_big_endian(*signature.r()),
            bits: UintBits::default(),
        },
        Token::Uint {
            uint: U256::from_big_endian(*signature.s()),
            bits: UintBits::default(),
        },
        Token::Bytes(authenticator_data.to_vec()),
        Token::String(client_data_json_pre.to_string()),
        Token::String(client_data_json_post.to_string()),
    ]))
}

pub fn get_prefixed_msg_hash(
    msg_hash: &[u8],
    barz_address: &str,
    chain_id: u32,
) -> BarzResult<Vec<u8>> {
    // keccak256("EIP712Domain(uint256 chainId,address verifyingContract)")
    let domain_separator_type_hash = hex::decode(BARZ_DOMAIN_SEPARATOR_TYPE_HASH)?;
    // keccak256("BarzMessage(bytes message)")
    let barz_msg_hash_data = hex::decode(BARZ_MSG_HASH_DATA)?;
    let signed_data_prefix = hex::decode(BARZ_SIGNED_DATA_PREFIX)?;

    let domain_separator_tokens = vec![
        Token::FixedBytes(NonEmptyBytes::new(domain_separator_type_hash)?),
        Token::Uint {
            uint: U256::from(chain_id),
            bits: UintBits::default(),
        },
        Token::Address(Address::from_str(barz_address)?),
    ];

    let domain_separator = encode::encode_tokens(&domain_separator_tokens);
    let domain_separator_hash = keccak256(&domain_separator);

    let raw_message_tokens = vec![
        Token::FixedBytes(NonEmptyBytes::new(barz_msg_hash_data)?),
        Token::FixedBytes(NonEmptyBytes::new(keccak256(msg_hash).to_vec())?),
    ];

    let raw_message_data = encode::encode_tokens(&raw_message_tokens);

    let encoded_msg_tokens = vec![
        Token::FixedBytes(NonEmptyBytes::new(domain_separator_hash.to_vec())?),
        Token::FixedBytes(NonEmptyBytes::new(keccak256(&raw_message_data).to_vec())?),
    ];

    let encoded_msg = encode::encode_tokens(&encoded_msg_tokens);

    let mut final_encoded_msg = signed_data_prefix;
    final_encoded_msg.extend_from_slice(&encoded_msg);

    let encoded_msg_hash = keccak256(&final_encoded_msg);
    Ok(encoded_msg_hash.to_vec())
}

// Function to encode the diamondCut function call using protobuf message as input
pub fn get_diamond_cut_code(input: &DiamondCutInput) -> BarzResult<Vec<u8>> {
    let diamond_cut_selector = hex::decode(BARZ_DIAMOND_CUT_SELECTOR)?;
    let data_location_chunk = hex::decode(BARZ_DATA_LOCATION_CHUNK)?;
    let mut encoded = Vec::new();

    // diamondCut() function selector
    encoded.extend_from_slice(&diamond_cut_selector);

    // First argument Data Location `diamondCut`
    let mut data_location = data_location_chunk.clone();
    while data_location.len() < 32 {
        data_location.insert(0, 0);
    }
    encoded.extend_from_slice(&data_location);

    // Encode second Parameter `init`
    let mut init_address = hex::decode(&input.init_address)?;
    while init_address.len() < 32 {
        init_address.insert(0, 0);
    }
    encoded.extend_from_slice(&init_address);

    // Third Argument Data location `_calldata`
    let call_data_data_location = encoded.len();

    // Encode number of facet cuts
    let facet_cuts_size = U256::from(input.facet_cuts.len());
    let facet_cuts_size_bytes = facet_cuts_size.to_big_endian();
    encoded.extend_from_slice(&facet_cuts_size_bytes[..]);

    let encoding_chunk = 32;
    let bytes_chunk_line = 5;
    let mut total_instruct_chunk = 0;
    let mut chunk_location = 0;
    let mut prev_data_position = 0;
    let mut instruct_chunk = 0;

    // Encode each FacetCut
    for facet_cut in input.facet_cuts.iter() {
        if instruct_chunk == 0 {
            prev_data_position = input.facet_cuts.len() * encoding_chunk;
            let position_bytes = U256::from(prev_data_position).to_big_endian();
            encoded.extend_from_slice(&position_bytes[..]);
            chunk_location = encoded.len();
        } else {
            prev_data_position += instruct_chunk * encoding_chunk;
            let position_bytes = U256::from(prev_data_position).to_big_endian();
            instruct_chunk = 0;

            // encoded.extend_from_slice(&position_bytes[..]);
            encoded.splice(chunk_location..chunk_location, position_bytes.to_vec());
            instruct_chunk += 1;
        }

        let mut tokens = vec![];

        // Encode facet address
        let token = Token::Address(Address::from_str(&facet_cut.facet_address)?);
        tokens.push(token);
        instruct_chunk += 1;

        // Encode FacetAction enum
        let token = Token::Uint {
            uint: U256::from(facet_cut.action as u32),
            bits: UintBits::default(),
        };
        tokens.push(token);
        instruct_chunk += 1;
        // Add data storage position
        let token = Token::FixedBytes(NonEmptyBytes::new(data_location.clone())?);
        tokens.push(token);
        instruct_chunk += 1;

        // Encode number of function selectors
        let token = Token::Uint {
            uint: U256::from(facet_cut.function_selectors.len()),
            bits: UintBits::default(),
        };
        tokens.push(token);
        instruct_chunk += 1;

        // Encode function selectors
        for selector in &facet_cut.function_selectors {
            let token = Token::FixedBytes(NonEmptyBytes::new(selector.to_vec())?);
            tokens.push(token);
            instruct_chunk += 1;
        }
        let encoded_token = encode::encode_tokens(&tokens);
        encoded.extend_from_slice(&encoded_token);
        total_instruct_chunk += instruct_chunk;
    }

    // Insert calldata length
    let calldata_length =
        (total_instruct_chunk * encoding_chunk) + (bytes_chunk_line * encoding_chunk);
    let calldata_length_bytes = U256::from(calldata_length).to_big_endian();
    encoded.splice(
        call_data_data_location..call_data_data_location,
        calldata_length_bytes.to_vec(),
    );

    // Handle init data
    let init_data_length = input.init_data.len();
    if init_data_length == 0 {
        return Ok(Vec::new());
    }
    let init_data_length_bytes = U256::from(init_data_length).to_big_endian();
    encoded.extend_from_slice(&init_data_length_bytes[..]);
    encoded.extend_from_slice(&input.init_data);

    // Add padding
    let padding_length = (encoding_chunk) - ((init_data_length) % (encoding_chunk * 2));
    let padding = vec![0u8; padding_length];
    encoded.extend_from_slice(&padding);
    Ok(encoded)
}

#[allow(clippy::too_many_arguments)]
pub fn get_encoded_hash(
    chain_id: &[u8],
    code_address: &str,
    code_name: &str,
    code_version: &str,
    type_hash: &str,
    domain_separator_hash: &str,
    sender: &str,
    user_op_hash: &str,
) -> BarzResult<Vec<u8>> {
    // Create code_address_bytes32 by padding with zeros
    let mut code_address_bytes32 = vec![0u8; 12];
    code_address_bytes32.extend_from_slice(&hex::decode(code_address.trim_start_matches("0x"))?);

    // Create domain separator
    let tokens = vec![
        Token::FixedBytes(NonEmptyBytes::new(hex::decode(
            domain_separator_hash.trim_start_matches("0x"),
        )?)?),
        Token::FixedBytes(NonEmptyBytes::new(
            keccak256(code_name.as_bytes()).to_vec(),
        )?),
        Token::FixedBytes(NonEmptyBytes::new(
            keccak256(code_version.as_bytes()).to_vec(),
        )?),
        Token::Uint {
            uint: U256::from_big_endian_slice(chain_id)?,
            bits: UintBits::default(),
        },
        Token::Address(Address::from_str(sender)?),
        Token::FixedBytes(NonEmptyBytes::new(code_address_bytes32)?),
    ];

    let domain_separator = encode::encode_tokens(&tokens);
    let domain_separator_encoded_hash = keccak256(&domain_separator);

    // Create message hash
    let mut message_to_hash = Vec::new();
    message_to_hash.extend_from_slice(&hex::decode(type_hash)?);
    message_to_hash.extend_from_slice(&hex::decode(user_op_hash)?);
    let message_hash = keccak256(&message_to_hash);

    // Create final encoded hash
    let mut encoded = Vec::new();
    encoded.extend_from_slice(&hex::decode(BARZ_SIGNED_DATA_PREFIX)?);
    encoded.extend_from_slice(&domain_separator_encoded_hash);
    encoded.extend_from_slice(&message_hash);

    Ok(keccak256(&encoded))
}

pub fn sign_user_op_hash(hash: &str, private_key: &str) -> BarzResult<Vec<u8>> {
    let private_key = PrivateKey::try_from(private_key)?;
    let message = H256::from_str(hash)?;
    let signature = private_key.sign(message)?;

    let mut result = signature.to_vec();
    // v value (last byte, should be 0 or 1, add 27 to make it 27 or 28)
    let v_value = result[64] + 27;
    result[64] = v_value;

    Ok(result)
}

pub fn get_authorization_hash(
    chain_id: &[u8],
    contract_address: &str,
    nonce: &[u8],
) -> BarzResult<Vec<u8>> {
    let authorization = Authorization {
        chain_id: U256::from_big_endian_slice(chain_id)?,
        address: Address::from_str(contract_address)?,
        nonce: U256::from_big_endian_slice(nonce)?,
    };

    Ok(authorization.hash()?.to_vec())
}

pub fn sign_authorization(
    chain_id: &[u8],
    contract_address: &str,
    nonce: &[u8],
    private_key: &str,
) -> BarzResult<String> {
    let authorization = Authorization {
        chain_id: U256::from_big_endian_slice(chain_id)?,
        address: Address::from_str(contract_address)?,
        nonce: U256::from_big_endian_slice(nonce)?,
    };
    let authorization_hash = authorization.hash()?;
    let private_key = PrivateKey::try_from(private_key)?;
    let signature = private_key.sign(authorization_hash)?;

    let signed_authorization = SignedAuthorization {
        authorization,
        y_parity: signature.v(),
        r: U256::from_big_endian(signature.r()),
        s: U256::from_big_endian(signature.s()),
    };

    Ok(serde_json::to_string(&signed_authorization)?)
}
