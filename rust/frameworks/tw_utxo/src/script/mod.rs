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

    fn encoded_size(&self) -> usize
    where
        Self: Sized,
    {
        self.bytes.encoded_size()
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

    pub fn as_slice(&self) -> &[u8] {
        &self.bytes
    }

    pub fn to_vec(&self) -> Data {
        self.bytes.clone()
    }
}

impl From<Script> for Data {
    fn from(value: Script) -> Self {
        value.bytes
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
        stream.append_list(&self.items);
    }

    fn encoded_size(&self) -> usize {
        CompactInteger::from(self.items.len()).encoded_size()
            + self.items.iter().map(Script::encoded_size).sum::<usize>()
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

    pub fn clear(&mut self) {
        self.items.clear();
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use tw_encoding::hex::{DecodeHex, ToHex};

    #[test]
    fn test_script_push_long_data_1() {
        let data1 = "304402202457e78cc1b7f50d0543863c27de75d07982bde8359b9e3316adec0aec165f2f02200203fd331c4e4a4a02f48cf1c291e2c0d6b2f7078a784b5b3649fca41f8794d401";
        let data2 = "552103244e602b46755f24327142a0517288cebd159eccb6ccf41ea6edf1f601e9af952103bbbacc302d19d29dbfa62d23f37944ae19853cf260c745c2bea739c95328fcb721039227e83246bd51140fe93538b2301c9048be82ef2fb3c7fc5d78426ed6f609ad210229bf310c379b90033e2ecb07f77ecf9b8d59acb623ab7be25a0caed539e2e6472103703e2ed676936f10b3ce9149fa2d4a32060fb86fa9a70a4efe3f21d7ab90611921031e9b7c6022400a6bb0424bbcde14cff6c016b91ee3803926f3440abf5c146d05210334667f975f55a8455d515a2ef1c94fdfa3315f12319a14515d2a13d82831f62f57ae";

        let mut script = Script::new();
        script.push(OP_PUSHBYTES_0);
        script.push_slice(&data1.decode_hex().unwrap());
        script.push(OP_PUSHBYTES_0);
        script.push_slice(&data2.decode_hex().unwrap());

        // OP_0
        // OP_PUSHBYTES_71
        // 304402202457e78cc1b7f50d0543863c27de75d07982bde8359b9e3316adec0aec165f2f02200203fd331c4e4a4a02f48cf1c291e2c0d6b2f7078a784b5b3649fca41f8794d401
        // OP_0
        // OP_PUSHDATA1
        // 552103244e602b46755f24327142a0517288cebd159eccb6ccf41ea6edf1f601e9af952103bbbacc302d19d29dbfa62d23f37944ae19853cf260c745c2bea739c95328fcb721039227e83246bd51140fe93538b2301c9048be82ef2fb3c7fc5d78426ed6f609ad210229bf310c379b90033e2ecb07f77ecf9b8d59acb623ab7be25a0caed539e2e6472103703e2ed676936f10b3ce9149fa2d4a32060fb86fa9a70a4efe3f21d7ab90611921031e9b7c6022400a6bb0424bbcde14cff6c016b91ee3803926f3440abf5c146d05210334667f975f55a8455d515a2ef1c94fdfa3315f12319a14515d2a13d82831f62f57ae
        assert_eq!(
            script.bytes.to_hex(),
            "0047304402202457e78cc1b7f50d0543863c27de75d07982bde8359b9e3316adec0aec165f2f02200203fd331c4e4a4a02f48cf1c291e2c0d6b2f7078a784b5b3649fca41f8794d401004cf1552103244e602b46755f24327142a0517288cebd159eccb6ccf41ea6edf1f601e9af952103bbbacc302d19d29dbfa62d23f37944ae19853cf260c745c2bea739c95328fcb721039227e83246bd51140fe93538b2301c9048be82ef2fb3c7fc5d78426ed6f609ad210229bf310c379b90033e2ecb07f77ecf9b8d59acb623ab7be25a0caed539e2e6472103703e2ed676936f10b3ce9149fa2d4a32060fb86fa9a70a4efe3f21d7ab90611921031e9b7c6022400a6bb0424bbcde14cff6c016b91ee3803926f3440abf5c146d05210334667f975f55a8455d515a2ef1c94fdfa3315f12319a14515d2a13d82831f62f57ae"
        );
    }

    #[test]
    fn test_script_push_long_data_2() {
        // Don't use fixed (on stack) byte array
        // because it causes (signal: 10, SIGBUS: access to undefined memory) in WASM.
        let data1 = vec![1; 65_530];
        let data2 = vec![2; 70_000];

        let mut script = Script::new();
        script.push(OP_PUSHBYTES_0);
        script.push_slice(&data1);
        script.push(OP_PUSHBYTES_0);
        script.push_slice(&data2);

        let mut data_buf1 = bitcoin::script::PushBytesBuf::new();
        data_buf1.extend_from_slice(&data1).unwrap();
        let mut data_buf2 = bitcoin::script::PushBytesBuf::new();
        data_buf2.extend_from_slice(&data2).unwrap();

        let bitcoin_script = bitcoin::script::Script::builder()
            .push_opcode(bitcoin::opcodes::OP_0)
            .push_slice(data_buf1)
            .push_opcode(bitcoin::opcodes::OP_0)
            .push_slice(data_buf2)
            .into_bytes();

        assert_eq!(script.bytes.to_hex(), bitcoin_script.to_hex());
    }
}
