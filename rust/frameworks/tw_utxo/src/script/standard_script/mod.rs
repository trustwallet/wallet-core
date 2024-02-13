use super::Script;
use opcodes::*;

pub mod conditions;
pub mod claims;
pub mod opcodes;

const SEGWIT_VERSION: u8 = 0;
const TAPROOT_VERSION: u8 = 1;

pub fn is_condition_p2sh(s: &Script) -> bool {
    let b = s.as_data();
    b.len() == 23 && b[0] == OP_HASH160 && b[1] == OP_PUSHBYTES_20 && b[22] == OP_EQUAL
}

pub fn is_condition_p2pkh(s: &Script) -> bool {
    let b = s.as_data();
    b.len() == 25
        && b[0] == OP_DUP
        && b[1] == OP_HASH160
        && b[2] == OP_PUSHBYTES_20
        && b[23] == OP_EQUALVERIFY
        && b[24] == OP_CHECKSIG
}

pub fn is_condition_p2pk(s: &Script) -> bool {
    let b = s.as_data();
    match b.len() {
        67 if b[0] == OP_PUSHBYTES_65 && b[66] == OP_CHECKSIG => true,
        35 if b[0] == OP_PUSHBYTES_33 && b[34] == OP_CHECKSIG => true,
        _ => false,
    }
}

pub fn is_condition_p2wsh(s: &Script) -> bool {
    let b = s.as_data();
    b.len() == 34 && b[0] == SEGWIT_VERSION && b[1] == OP_PUSHBYTES_32
}

pub fn is_condition_p2wpkh(s: &Script) -> bool {
    let b = s.as_data();
    b.len() == 22 && b[0] == SEGWIT_VERSION && b[1] == OP_PUSHBYTES_20
}

pub fn is_condition_p2tr(s: &Script) -> bool {
    let b = s.as_data();
    b.len() == 34 && b[0] == TAPROOT_VERSION && b[1] == OP_PUSHBYTES_32
}