use super::{Error, Result};
use bitcoin::blockdata::opcodes::all::{
    OP_CHECKMULTISIG, OP_CHECKMULTISIGVERIFY, OP_CHECKSIG, OP_CHECKSIGVERIFY, OP_PUSHNUM_1,
    OP_PUSHNUM_16,
};
use bitcoin::blockdata::opcodes::All as OpCode;
use bitcoin::blockdata::script::{Instruction, Script};
use bitcoin::Transaction;

pub fn calculate_fee(tx: &Transaction) {
    let weight = tx.weight();
    let s = tx.input.get(0).unwrap().script_sig.as_script();
}

fn count_sigops(script: &Script, accurate: bool) -> Result<usize> {
    let mut n = 0;
    let mut pushnum_cache = None;
    for inst in script.instructions() {
        match inst.map_err(|_| Error::Todo)? {
            Instruction::Op(opcode) => {
                match opcode {
                    OP_CHECKSIG | OP_CHECKSIGVERIFY => {
                        n += 1;
                    },
                    OP_CHECKMULTISIG | OP_CHECKMULTISIGVERIFY => {
                        match (accurate, pushnum_cache) {
                            (true, Some(pushnum)) => {
                                // Add the number of pubkeys in the multisig as sigop count
                                n += usize::from(pushnum);
                            },
                            _ => {
                                // MAX_PUBKEYS_PER_MULTISIG from Bitcoin Core
                                // https://github.com/bitcoin/bitcoin/blob/v25.0/src/script/script.h#L29-L30
                                n += 20;
                            },
                        }
                    },
                    _ => {
                        pushnum_cache = decode_pushnum(opcode);
                    },
                }
            },
            Instruction::PushBytes(_) => {
                pushnum_cache = None;
            },
        }
    }

    Ok(n)
}

const fn decode_pushnum(opcode: OpCode) -> Option<u8> {
    const START: u8 = OP_PUSHNUM_1.to_u8();
    const END: u8 = OP_PUSHNUM_16.to_u8();
    match opcode.to_u8() {
        START..=END => Some(opcode.to_u8() - START + 1),
        _ => None,
    }
}

#[test]
fn test_vsize() {}
