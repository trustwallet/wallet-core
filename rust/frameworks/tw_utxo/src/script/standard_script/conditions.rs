use bitcoin::hashes::Hash;
use secp256k1::XOnlyPublicKey;
use tw_hash::H160;
use tw_hash::H256;
use tw_hash::H264;
use tw_misc::traits::ToBytesVec;

use super::opcodes::*;
use super::Script;
use super::{SEGWIT_VERSION, TAPROOT_VERSION};

/// Creates a P2SH spending condition (_scriptPubkey_).
///
/// ```txt
/// OP_HASH160 <push><script_hash> OP_EQUAL
/// ```
pub fn new_p2sh(script_hash: &H160) -> Script {
    let mut s = Script::with_capacity(23);
    s.push(OP_HASH160);
    s.push_slice(script_hash.as_slice());
    s.push(OP_EQUAL);
    s
}

/// Creates a P2PK spending condition (_scriptPubkey_).
///
/// ```txt
/// <push><pubkey> OP_CHECKSIG
/// ```
pub fn new_p2pk(pubkey: &H264) -> Script {
    let mut s = Script::with_capacity(35);
    s.push_slice(pubkey.as_slice());
    s.push(OP_CHECKSIG);
    s
}

/// Creates a P2PKH spending condition (_scriptPubkey_).
///
/// ```txt
/// OP_DUP OP_HASH160 <push><pubkey_hash> OP_EQUALVERIFY OP_CHECKSIG
/// ```
pub fn new_p2pkh(pubkey_hash: &H160) -> Script {
    let mut s = Script::with_capacity(25);
    s.push(OP_DUP);
    s.push(OP_HASH160);
    s.push_slice(pubkey_hash.as_slice());
    s.push(OP_EQUALVERIFY);
    s.push(OP_CHECKSIG);
    s
}

/// Creates a P2WSH spending condition (_scriptPubkey_).
///
/// ```txt
/// 0 <push><script_hash>
/// ```
pub fn new_p2wsh(script_hash: &H256) -> Script {
    let mut s = Script::with_capacity(34);
    s.push(SEGWIT_VERSION);
    s.push_slice(script_hash.as_slice());
    s
}

/// Creates a P2WPKH spending condition (_scriptPubkey_). Do note that you must
/// use [`new_p2wpkh_script_code`](Self::new_p2wpkh_script_code) when generating
/// the _scriptPubkey_ for **claiming** a P2WPKH spending condition.
///
/// ```txt
/// 0 <push><pubkey_hash>
/// ```
pub fn new_p2wpkh(pubkey_hash: &H160) -> Script {
    let mut s = Script::with_capacity(22);
    s.push(SEGWIT_VERSION);
    s.push_slice(pubkey_hash.as_slice());
    s
}

/// Creates a special _scriptPubkey_ when **claiming** a P2WPKH spending
/// condition, which is used for Sighash calculation.
///
/// ```txt
/// OP_DUP OP_HASH160 <push><pubkey_hash> OP_EQUALVERIFY OP_CHECKSIG
/// ```
pub fn new_p2wpkh_script_code(pubkey_hash: &H160) -> Script {
    // We're just wrapping over the legacy P2PKH script builder.
    new_p2pkh(pubkey_hash)
}

pub fn new_p2tr_key_path(pubkey: &H264) -> Script {
    // We're relying on the `bitcoin` crate to generate anything Taproot related.
    let pubkey = bitcoin::PublicKey::from_slice(pubkey.as_slice()).unwrap();
    let internal_key = XOnlyPublicKey::from(pubkey.inner);

    let script = bitcoin::ScriptBuf::new_v1_p2tr(&secp256k1::Secp256k1::new(), internal_key, None);
    Script::from(script.to_vec())
}

pub fn new_p2tr_script_path(pubkey: &H264, merkle_root: &H256) -> Script {
    // We're relying on the `bitcoin` crate to generate anything Taproot related.
    let pubkey = bitcoin::PublicKey::from_slice(pubkey.as_slice()).unwrap();
    let internal_key = XOnlyPublicKey::from(pubkey.inner);

    let hash = bitcoin::hashes::sha256t::Hash::<_>::from_slice(merkle_root.as_slice()).unwrap();
    let merkle_root = bitcoin::taproot::TapNodeHash::from_raw_hash(hash);

    let script = bitcoin::ScriptBuf::new_v1_p2tr(
        &secp256k1::Secp256k1::new(),
        internal_key,
        Some(merkle_root),
    );
    Script::from(script.to_vec())
}

pub fn is_p2sh(s: &Script) -> bool {
    let b = s.as_data();
    b.len() == 23 && b[0] == OP_HASH160 && b[1] == OP_PUSHBYTES_20 && b[22] == OP_EQUAL
}

pub fn is_p2pk(s: &Script) -> bool {
    let b = s.as_data();
    match b.len() {
        67 if b[0] == OP_PUSHBYTES_65 && b[66] == OP_CHECKSIG => true,
        35 if b[0] == OP_PUSHBYTES_33 && b[34] == OP_CHECKSIG => true,
        _ => false,
    }
}

pub fn is_p2pkh(s: &Script) -> bool {
    let b = s.as_data();
    b.len() == 25
        && b[0] == OP_DUP
        && b[1] == OP_HASH160
        && b[2] == OP_PUSHBYTES_20
        && b[23] == OP_EQUALVERIFY
        && b[24] == OP_CHECKSIG
}

pub fn is_p2wsh(s: &Script) -> bool {
    let b = s.as_data();
    b.len() == 34 && b[0] == SEGWIT_VERSION && b[1] == OP_PUSHBYTES_32
}

pub fn is_p2wpkh(s: &Script) -> bool {
    let b = s.as_data();
    b.len() == 22 && b[0] == SEGWIT_VERSION && b[1] == OP_PUSHBYTES_20
}

pub fn is_p2tr(s: &Script) -> bool {
    let b = s.as_data();
    b.len() == 34 && b[0] == TAPROOT_VERSION && b[1] == OP_PUSHBYTES_32
}
