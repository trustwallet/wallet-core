// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::address::StacksAddress;

use tw_coin_entry::coin_context::CoinContext;
use tw_coin_entry::coin_entry::CoinAddress;
use tw_coin_entry::error::prelude::*;
use tw_coin_entry::signing_output_error;
use tw_encoding::hex;
use tw_hash::{hasher::StatefulHasher, ripemd::Sha256Ripemd, sha2, H160, H256};
use tw_keypair::{ecdsa::secp256k1::PrivateKey, traits::SigningKeyTrait};
use tw_misc::traits::ToBytesVec;
use tw_proto::Stacks::Proto;
use tw_proto::Stacks::Proto::mod_SigningInput::OneOfmessage_oneof as SigningInputMessage;

use std::str::FromStr;

pub struct StacksSigner;

impl StacksSigner {
    pub fn sign(
        coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> Proto::SigningOutput<'static> {
        Self::sign_impl(coin, input)
            .unwrap_or_else(|e| signing_output_error!(Proto::SigningOutput, e))
    }

    fn sign_impl(
        _coin: &dyn CoinContext,
        input: Proto::SigningInput<'_>,
    ) -> SigningResult<Proto::SigningOutput<'static>> {
        let signed_tx = match input.message_oneof {
            SigningInputMessage::transfer(xfer) => {
                let rcpt_version: u8 = 0x16; // Mainnet P2PKH
                let rcpt_addr = StacksAddress::from_str(&xfer.to).unwrap();
                let amount: u64 = xfer.amount.try_into().unwrap(); // microSTX
                let fee: u64 = xfer.fee.try_into().unwrap(); // microSTX
                let nonce: u64 = xfer.nonce.try_into().unwrap();
                let memo_bytes: [u8; 34] = {
                    let msg = xfer.memo.as_bytes();
                    let mut b = [0u8; 34];
                    b[0..msg.len()].copy_from_slice(msg);
                    b
                };

                let version: u8 = 0x00; // Mainnet
                let chain_id: u32 = 0x00000001; // Mainnet
                let anchor_mode: u8 = 0x03; // Any
                let post_condition_mode: u8 = 0x01; // Allow
                let hash_mode: u8 = 0x00; // P2PKH single-sig

                // Parse private key
                let private_key_bytes = input.private_key;
                let secret_key = PrivateKey::try_from(&private_key_bytes[..]).unwrap();

                // Compute public key (compressed)
                let public_key = secret_key.public();
                let pubkey_bytes = public_key.to_vec();

                // Compute signer hash160: ripemd160(sha256(pubkey_bytes))
                let hasher = Sha256Ripemd;
                let signer_hash160: [u8; 20] = hasher.hash(&pubkey_bytes).try_into().unwrap();

                // Rcpt hash160
                let rcpt_hash160_bytes = rcpt_addr.data();
                let rcpt_hash160 = H160::try_from(&rcpt_hash160_bytes[..]).unwrap();

                // Build payload: 0x00 + rcpt principal (version + hash160) + amount BE + memo
                let mut payload = vec![0x00];
                payload.push(rcpt_version);
                payload.extend_from_slice(&rcpt_hash160.as_slice());
                payload.extend_from_slice(&amount.to_be_bytes());
                payload.extend_from_slice(&memo_bytes);

                // Build unsigned auth: 0x04 + hash_mode + signer + nonce0 + fee0
                let mut auth_unsigned = vec![0x04, hash_mode];
                auth_unsigned.extend_from_slice(&signer_hash160);
                auth_unsigned.extend_from_slice(&[0u8; 8]); // nonce 0
                auth_unsigned.extend_from_slice(&[0u8; 8]); // fee 0

                // Build post conditions: u32 0
                let post_conditions = [0u8; 4];

                // Serialize unsigned tx for initial sighash
                let mut unsigned_tx = vec![version];
                unsigned_tx.extend_from_slice(&chain_id.to_be_bytes());
                unsigned_tx.extend_from_slice(&auth_unsigned);
                unsigned_tx.push(anchor_mode);
                unsigned_tx.push(post_condition_mode);
                unsigned_tx.extend_from_slice(&post_conditions);
                unsigned_tx.extend_from_slice(&payload);

                // Compute initial sighash = SHA512/256(unsigned_tx)
                let initial_sighash = sha2::sha512_256(&unsigned_tx);

                // Auth flag for standard origin: 0x04
                let auth_flag: u8 = 0x04;

                // Compute pre_sign_hash = SHA512/256(initial_sighash || auth_flag || fee BE || nonce BE)
                let mut pre_sign_data = vec![];
                pre_sign_data.extend_from_slice(&initial_sighash);
                pre_sign_data.push(auth_flag);
                pre_sign_data.extend_from_slice(&fee.to_be_bytes());
                pre_sign_data.extend_from_slice(&nonce.to_be_bytes());
                let pre_sign_hash = sha2::sha512_256(&pre_sign_data);

                // Sign the pre_sign_hash with ECDSA secp256k1
                let message = H256::try_from(&pre_sign_hash[..]).unwrap();
                let rsig = secret_key.sign(message).unwrap();

                // Serialize signature to 65 bytes Bitcoin-style
                let compressed = true; // since we used compressed pubkey
                let header_byte = rsig.v() + 27 + if compressed { 0 } else { 4 };
                let mut signature = [0u8; 65];
                signature[0] = header_byte;
                signature[1..33].copy_from_slice(&rsig.r().as_slice());
                signature[33..].copy_from_slice(&rsig.s().as_slice());

                // Key encoding: 0x00 compressed, 0x01 uncompressed
                let key_encoding: u8 = if compressed { 0x00 } else { 0x01 };

                // Now build the signed auth: 0x04 + hash_mode + signer + nonce BE + fee BE + key_encoding + signature
                let mut auth_signed = vec![0x04, hash_mode];
                auth_signed.extend_from_slice(&signer_hash160);
                auth_signed.extend_from_slice(&nonce.to_be_bytes());
                auth_signed.extend_from_slice(&fee.to_be_bytes());
                auth_signed.push(key_encoding);
                auth_signed.extend_from_slice(&signature);

                // Serialize signed tx
                let mut signed_tx = vec![version];
                signed_tx.extend_from_slice(&chain_id.to_be_bytes());
                signed_tx.extend_from_slice(&auth_signed);
                signed_tx.push(anchor_mode);
                signed_tx.push(post_condition_mode);
                signed_tx.extend_from_slice(&post_conditions);
                signed_tx.extend_from_slice(&payload);

                signed_tx
            },
            _ => todo!(),
        };

        let serialized_hex = hex::encode(&signed_tx, false);
        println!("Signed transaction (hex): {}", serialized_hex);

        Ok(Proto::SigningOutput {
            encoded: signed_tx.into(),
            ..Proto::SigningOutput::default()
        })
    }
}
