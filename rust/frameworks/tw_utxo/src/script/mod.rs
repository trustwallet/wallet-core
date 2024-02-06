// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

const OP_HASH160: u8 = 0xa9;
const OP_PUSHBYTES_20: u8 = 0x14;
const OP_EQUAL: u8 = 0x87;
const OP_DUP: u8 = 0x76;
const OP_EQUALVERIFY: u8 = 0x88;
const OP_CHECKSIG: u8 = 0xac;
const OP_PUSHBYTES_65: u8 = 0x41;
const OP_PUSHBYTES_33: u8 = 0x21;
const OP_PUSHBYTES_32: u8 = 0x20;

const SEGWIT_VERSION: u8 = 0;
const TAPROOT_VERSION: u8 = 1;

use crate::encode::stream::Stream;
use crate::encode::Encodable;
use tw_memory::Data;

#[derive(Clone, Debug, Default)]
pub struct Script {
    bytes: Data,
}

impl Encodable for Script {
    fn encode(&self, stream: &mut Stream) {
        stream.append(&self.bytes);
    }
}

impl Script {
    pub fn is_p2sh(&self) -> bool {
        let b = &self.bytes;
        b.len() == 23 && b[0] == OP_HASH160 && b[1] == OP_PUSHBYTES_20 && b[22] == OP_EQUAL
    }
    pub fn is_p2pkh(&self) -> bool {
        let b = &self.bytes;
        b.len() == 25
            && b[0] == OP_DUP
            && b[1] == OP_HASH160
            && b[2] == OP_PUSHBYTES_20
            && b[23] == OP_EQUALVERIFY
            && b[24] == OP_CHECKSIG
    }
    pub fn is_p2pk(&self) -> bool {
        let b = &self.bytes;
        match b.len() {
            67 if b[0] == OP_PUSHBYTES_65 && b[66] == OP_CHECKSIG => true,
            35 if b[0] == OP_PUSHBYTES_33 && b[34] == OP_CHECKSIG => true,
            _ => false,
        }
    }
    pub fn is_p2wsh(&self) -> bool {
        let b = &self.bytes;
        b.len() == 34 && b[0] == SEGWIT_VERSION && b[1] == OP_PUSHBYTES_32
    }
    pub fn is_p2wpkh(&self) -> bool {
        let b = &self.bytes;
        b.len() == 22 && b[0] == SEGWIT_VERSION && b[1] == OP_PUSHBYTES_20
    }
    pub fn is_p2tr(&self) -> bool {
        let b = &self.bytes;
        b.len() == 34 && b[0] == TAPROOT_VERSION && b[1] == OP_PUSHBYTES_32
    }
}
