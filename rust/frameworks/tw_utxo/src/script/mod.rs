// SPDX-License-Identifier: Apache-2.0
//
// Copyright © 2017 Trust Wallet.

use crate::encode::compact_integer::CompactInteger;
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
    pub fn new() -> Self {
        Self::default()
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
    pub fn as_items(&self) -> &[Script] {
        &self.items
    }
    pub fn is_empty(&self) -> bool {
        self.items.is_empty()
    }
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
        Witness { items: items.into_iter().map(Script::from).collect() }
    }
}
