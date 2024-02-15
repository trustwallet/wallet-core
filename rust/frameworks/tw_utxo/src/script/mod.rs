// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::compact_integer::CompactInteger;
use crate::encode::stream::Stream;
use crate::encode::Encodable;
use standard_script::opcodes::*;
use tw_memory::Data;

pub mod standard_script;

#[derive(Clone, Debug, Default, PartialEq, Eq)]
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
    pub fn len(&self) -> usize {
        self.bytes.len()
    }
    pub fn is_empty(&self) -> bool {
        self.bytes.is_empty()
    }
    pub fn serialized_len(&self) -> usize {
        if self.bytes.is_empty() {
            return 0;
        }

        CompactInteger::from(self.bytes.len()).serialized_len() + self.bytes.len()
    }
    /// Pushes the given opcode to the end of the script.
    pub fn push(&mut self, code: u8) {
        self.bytes.push(code);
    }
    /// Pushes the given data with an OP_PUSHDATA opcode and a length indicator
    /// as prefix to the end of the script.
    ///
    /// # Panics
    ///
    /// Panics if `data.len() >= 2^32`
    pub fn push_slice(&mut self, data: &[u8]) {
        // Push the length indicator of the data.
        match data.len() {
            n if n <= 75 => {
                // Interpreted as OP_PUSHBYTES_0..=OP_PUSHBYTES_75
                self.push(n as u8);
            },
            n if n <= 255 => {
                self.push(OP_PUSHDATA1);
                self.push(n as u8);
            },
            n if n <= 65535 => {
                self.push(OP_PUSHDATA2);
                self.push((n % 0x100) as u8);
                self.push((n / 0x100) as u8);
            },
            n if n <= 4294967295 => {
                self.push(OP_PUSHDATA4);
                self.push((n % 0x100) as u8);
                self.push(((n / 0x100) % 0x100) as u8);
                self.push(((n / 0x10000) % 0x100) as u8);
                self.push((n / 0x1000000) as u8);
            },
            // Not expected to ever happen in practice
            _ => panic!("unsupported data length"),
        }

        // Finally, push the data itself.
        self.bytes.extend_from_slice(data);
    }
    /// Appends the given data to the end of the script as-is.
    pub fn append(&mut self, data: &[u8]) {
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

#[derive(Clone, Debug, Default, PartialEq, Eq)]
pub struct Witness {
    items: Vec<Script>,
}

impl Encodable for Witness {
    fn encode(&self, stream: &mut Stream) {
        // TODO: What if the witness is empty?
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
    pub fn serialized_len(&self) -> usize {
        if self.is_empty() {
            return 0;
        }

        CompactInteger::from(self.items.len()).serialized_len()
            + self
                .items
                .iter()
                .map(|item| CompactInteger::from(item.len()).serialized_len() + item.len())
                .sum::<usize>()
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
