// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::stream::Stream;
use crate::encode::Encodable;
use standard_script::opcodes::*;
use tw_memory::Data;

pub mod standard_script;

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
    pub fn new() -> Self {
        Self::default()
    }
    pub fn with_capacity(capacity: usize) -> Self {
        Script {
            bytes: Data::with_capacity(capacity),
        }
    }
    pub fn push(&mut self, code: u8) {
        self.bytes.push(code);
    }
    pub fn push_slice(&mut self, data: &[u8]) {
        // Push the length indicator of the data.
        match data.len() {
            n if n < 0x4c => {
                // n < 76
                self.push(n as u8);
            },
            n if n < 0x100 => {
                // n < 256
                self.push(OP_PUSHDATA1);
                self.push(n as u8);
            },
            n if n < 0x10000 => {
                // n < 65536
                self.push(OP_PUSHDATA2);
                self.push((n % 0x100) as u8);
                self.push((n / 0x100) as u8);
            },
            n if n < 0x100000000 => {
                // n < 4294967296
                self.push(OP_PUSHDATA4);
                self.push((n % 0x100) as u8);
                self.push(((n / 0x100) % 0x100) as u8);
                self.push(((n / 0x10000) % 0x100) as u8);
                self.push((n / 0x1000000) as u8);
            },
            // Not expected to ever happen in practice
            _ => panic!("unspupported data length"),
        }

        // Finally, push the data itself.
        self.bytes.extend_from_slice(data);
    }
    pub fn push_slice_no_len(&mut self, data: &[u8]) {
        self.bytes.extend_from_slice(data);
    }
    pub fn as_data(&self) -> &Data {
        &self.bytes
    }
}

impl From<Data> for Script {
    fn from(bytes: Data) -> Self {
        Script { bytes }
    }
}

#[derive(Clone, Debug, Default)]
pub struct Witness {
    items: Vec<Script>,
}

impl Encodable for Witness {
    fn encode(&self, stream: &mut Stream) {
        stream.append_list(&self.items);
    }
}

impl Witness {
    pub fn new() -> Self {
        Self::default()
    }
    pub fn push_item(&mut self, item: Script) {
        self.items.push(item);
    }
    pub fn as_items(&self) -> &[Script] {
        &self.items
    }
    pub fn is_empty(&self) -> bool {
        self.items.is_empty()
    }
    // TODO: This needed?
    pub fn clear(&mut self) {
        self.items.clear();
    }
}

impl From<Vec<Script>> for Witness {
    fn from(items: Vec<Script>) -> Self {
        Witness { items }
    }
}

impl From<Vec<Data>> for Witness {
    fn from(items: Vec<Data>) -> Self {
        Witness {
            items: items.into_iter().map(Script::from).collect(),
        }
    }
}
