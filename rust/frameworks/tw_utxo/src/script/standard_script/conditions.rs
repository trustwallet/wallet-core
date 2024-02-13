use tw_hash::H160;
use tw_hash::H256;

use super::opcodes::*;
use super::Script;
use super::{SEGWIT_VERSION, TAPROOT_VERSION};

pub fn new_p2sh(script_hash: &H160) -> Script {
    let mut s = Script::with_capacity(23);
    s.push(OP_HASH160);
    s.push_slice(script_hash.as_slice());
    s.push(OP_EQUAL);
    s
}

// TODO: Use the tw:: PublicKey instead?
pub fn new_p2pk(pubkey: &tw_keypair::ecdsa::secp256k1::PublicKey) -> Script {
    let mut s = Script::with_capacity(35);
    s.push_slice(pubkey.compressed().as_slice());
    s.push(OP_CHECKSIG);
    s
}

pub fn new_p2pkh(pubkey_hash: &H160) -> Script {
    let mut s = Script::with_capacity(25);
    s.push(OP_DUP);
    s.push(OP_HASH160);
    s.push_slice(pubkey_hash.as_slice());
    s.push(OP_EQUALVERIFY);
    s.push(OP_CHECKSIG);
    s
}

pub fn new_p2wsh(script_hash: &H256) -> Script {
    let mut s = Script::with_capacity(34);
    s.push(SEGWIT_VERSION);
    s.push_slice(script_hash.as_slice());
    s
}

// TODO: Note that the witness pubkey hash must be serialized in a special way.
pub fn new_p2wpkh(pubkey_hash: &H160) -> Script {
    let mut s = Script::with_capacity(22);
    s.push(SEGWIT_VERSION);
    s.push_slice(pubkey_hash.as_slice());
    s
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
