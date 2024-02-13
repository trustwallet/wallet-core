use tw_hash::H160;
use tw_hash::H256;

use super::SEGWIT_VERSION;
use super::opcodes::*;
use super::Script;

pub fn new_condition_p2pk(pubkey: &tw_keypair::ecdsa::secp256k1::PublicKey) -> Script {
    let mut s = Script::with_capacity(35);
    s.push_slice(pubkey.compressed().as_slice());
    s.push(OP_CHECKSIG);
    s
}

pub fn new_condition_p2pkh(pubkey_hash: &H160) -> Script {
    let mut s = Script::with_capacity(25);
    s.push(OP_DUP);
    s.push(OP_HASH160);
    s.push_slice(pubkey_hash.as_slice());
    s.push(OP_EQUALVERIFY);
    s.push(OP_CHECKSIG);
    s
}

pub fn new_condition_p2sh(script_hash: &H160) -> Script {
    let mut s = Script::with_capacity(23);
    s.push(OP_HASH160);
    s.push_slice(script_hash.as_slice());
    s.push(OP_EQUAL);
    s
}

// TODO: Note that the witness pubkey hash must be serialized in a special way.
pub fn new_condition_p2wpkh(pubkey_hash: &H160) -> Script {
    let mut s = Script::with_capacity(22);
    s.push(SEGWIT_VERSION);
    s.push_slice(pubkey_hash.as_slice());
    s
}

pub fn new_condition_p2wsh(script_hash: &H256) -> Script {
    let mut s = Script::with_capacity(34);
    s.push(SEGWIT_VERSION);
    s.push_slice(script_hash.as_slice());
    s
}
