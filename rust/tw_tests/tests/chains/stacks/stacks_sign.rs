// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use hex::decode;
use ripemd::{Ripemd160, Digest as RipemdDigest};
use sha2::{Sha256, Sha512};
use secp256k1::{Message, PublicKey, RecoverableSignature, RecoveryId, Secp256k1, SecretKey};

#[test]
fn test_stacks_sign() {
    // Configuration
    let private_key_hex = "a1b2c3d4e5f60000000000000000000000000000000000000000000000000001";
    let recipient_version: u8 = 0x16; // Mainnet P2PKH
    let recipient_hash160_hex = "a46ff88886c2ef9762d970b4d2c63678835bd39d"; // From SP2J6ZY48GV1EZ5V2V5RB9MP66SW86PYKKNRV9EJ7
    let amount: u64 = 100_000; // microSTX
    let fee: u64 = 10_000; // microSTX
    let nonce: u64 = 5;
    let memo_bytes: [u8; 34] = {
        let mut b = [0u8; 34];
        let msg = b"hello";
        b[0..msg.len()].copy_from_slice(msg);
        b
    };

    let version: u8 = 0x00; // Mainnet
    let chain_id: u32 = 0x00000001; // Mainnet
    let anchor_mode: u8 = 0x03; // Any
    let post_condition_mode: u8 = 0x01; // Allow
    let hash_mode: u8 = 0x00; // P2PKH single-sig

    // Parse private key
    let private_key_bytes = decode(private_key_hex)?;
    let secret_key = SecretKey::from_slice(&private_key_bytes)?;

    // Compute public key (compressed)
    let secp = Secp256k1::new();
    let public_key = PublicKey::from_secret_key(&secp, &secret_key);
    let pubkey_bytes = public_key.serialize(); // 33 bytes compressed

    // Compute signer hash160: ripemd160(sha256(pubkey_bytes))
    let mut sha_hasher = Sha256::new();
    sha_hasher.update(&pubkey_bytes);
    let sha_hash = sha_hasher.finalize();
    let mut ripemd_hasher = Ripemd160::new();
    ripemd_hasher.update(&sha_hash);
    let signer_hash160: [u8; 20] = ripemd_hasher.finalize().into();

    // Recipient hash160
    let recipient_hash160 = decode(recipient_hash160_hex)?[..].try_into()?;

    // Build payload: 0x00 + recipient principal (version + hash160) + amount BE + memo
    let mut payload = vec![0x00];
    payload.push(recipient_version);
    payload.extend_from_slice(&recipient_hash160);
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
    let initial_sighash = sha512_256(&unsigned_tx);

    // Auth flag for standard origin: 0x04
    let auth_flag: u8 = 0x04;

    // Compute pre_sign_hash = SHA512/256(initial_sighash || auth_flag || fee BE || nonce BE)
    let mut pre_sign_data = vec![];
    pre_sign_data.extend_from_slice(&initial_sighash);
    pre_sign_data.push(auth_flag);
    pre_sign_data.extend_from_slice(&fee.to_be_bytes());
    pre_sign_data.extend_from_slice(&nonce.to_be_bytes());
    let pre_sign_hash = sha512_256(&pre_sign_data);

    // Sign the pre_sign_hash with ECDSA secp256k1
    let message = Message::from_digest(pre_sign_hash);
    let recoverable_sig: RecoverableSignature = secp.sign_ecdsa_recoverable(&message, &secret_key);

    // Serialize signature to 65 bytes Bitcoin-style
    let (rec_id, compact) = recoverable_sig.serialize_compact();
    let compressed = true; // since we used compressed pubkey
    let header_byte = rec_id.to_i32() as u8 + 27 + if compressed { 0 } else { 4 };
    let mut signature = [0u8; 65];
    signature[0] = header_byte;
    signature[1..].copy_from_slice(&compact);

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

    // Hex encode
    let serialized_hex = hex::encode(&signed_tx);

    println!("Signed transaction (hex): {}", serialized_hex);
}

fn sha512_256(data: &[u8]) -> [u8; 32] {
    let mut hasher = Sha512::new();
    hasher.update(data);
    let full_hash = hasher.finalize();
    let mut trunc = [0u8; 32];
    trunc.copy_from_slice(&full_hash[0..32]);
    trunc
}

