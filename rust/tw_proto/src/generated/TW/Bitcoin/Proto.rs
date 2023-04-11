// Automatically generated rust module for 'Bitcoin.proto' file

#![allow(non_snake_case)]
#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(unused_imports)]
#![allow(unknown_lints)]
#![allow(clippy::all)]
#![cfg_attr(rustfmt, rustfmt_skip)]


use std::borrow::Cow;
use std::collections::HashMap;
type KVMap<K, V> = HashMap<K, V>;
use quick_protobuf::{MessageInfo, MessageRead, MessageWrite, BytesReader, Writer, WriterBackend, Result};
use quick_protobuf::sizeofs::*;
use super::super::super::*;

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Transaction<'a> {
    pub version: i32,
    pub lockTime: u32,
    pub inputs: Vec<TW::Bitcoin::Proto::TransactionInput<'a>>,
    pub outputs: Vec<TW::Bitcoin::Proto::TransactionOutput<'a>>,
}

impl<'a> MessageRead<'a> for Transaction<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.version = r.read_sint32(bytes)?,
                Ok(16) => msg.lockTime = r.read_uint32(bytes)?,
                Ok(26) => msg.inputs.push(r.read_message::<TW::Bitcoin::Proto::TransactionInput>(bytes)?),
                Ok(34) => msg.outputs.push(r.read_message::<TW::Bitcoin::Proto::TransactionOutput>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Transaction<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.version == 0i32 { 0 } else { 1 + sizeof_sint32(*(&self.version)) }
        + if self.lockTime == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.lockTime) as u64) }
        + self.inputs.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + self.outputs.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.version != 0i32 { w.write_with_tag(8, |w| w.write_sint32(*&self.version))?; }
        if self.lockTime != 0u32 { w.write_with_tag(16, |w| w.write_uint32(*&self.lockTime))?; }
        for s in &self.inputs { w.write_with_tag(26, |w| w.write_message(s))?; }
        for s in &self.outputs { w.write_with_tag(34, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TransactionInput<'a> {
    pub previousOutput: Option<TW::Bitcoin::Proto::OutPoint<'a>>,
    pub sequence: u32,
    pub script: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for TransactionInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.previousOutput = Some(r.read_message::<TW::Bitcoin::Proto::OutPoint>(bytes)?),
                Ok(16) => msg.sequence = r.read_uint32(bytes)?,
                Ok(26) => msg.script = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TransactionInput<'a> {
    fn get_size(&self) -> usize {
        0
        + self.previousOutput.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.sequence == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.sequence) as u64) }
        + if self.script == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.script).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.previousOutput { w.write_with_tag(10, |w| w.write_message(s))?; }
        if self.sequence != 0u32 { w.write_with_tag(16, |w| w.write_uint32(*&self.sequence))?; }
        if self.script != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.script))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct OutPoint<'a> {
    pub hash: Cow<'a, [u8]>,
    pub index: u32,
    pub sequence: u32,
}

impl<'a> MessageRead<'a> for OutPoint<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.hash = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.index = r.read_uint32(bytes)?,
                Ok(24) => msg.sequence = r.read_uint32(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for OutPoint<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.hash == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.hash).len()) }
        + if self.index == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.index) as u64) }
        + if self.sequence == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.sequence) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.hash != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.hash))?; }
        if self.index != 0u32 { w.write_with_tag(16, |w| w.write_uint32(*&self.index))?; }
        if self.sequence != 0u32 { w.write_with_tag(24, |w| w.write_uint32(*&self.sequence))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TransactionOutput<'a> {
    pub value: i64,
    pub script: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for TransactionOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.value = r.read_int64(bytes)?,
                Ok(18) => msg.script = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TransactionOutput<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.value == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.value) as u64) }
        + if self.script == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.script).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.value != 0i64 { w.write_with_tag(8, |w| w.write_int64(*&self.value))?; }
        if self.script != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.script))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct UnspentTransaction<'a> {
    pub out_point: Option<TW::Bitcoin::Proto::OutPoint<'a>>,
    pub script: Cow<'a, [u8]>,
    pub amount: i64,
}

impl<'a> MessageRead<'a> for UnspentTransaction<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.out_point = Some(r.read_message::<TW::Bitcoin::Proto::OutPoint>(bytes)?),
                Ok(18) => msg.script = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.amount = r.read_int64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for UnspentTransaction<'a> {
    fn get_size(&self) -> usize {
        0
        + self.out_point.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.script == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.script).len()) }
        + if self.amount == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.out_point { w.write_with_tag(10, |w| w.write_message(s))?; }
        if self.script != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.script))?; }
        if self.amount != 0i64 { w.write_with_tag(24, |w| w.write_int64(*&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningInput<'a> {
    pub hash_type: u32,
    pub amount: i64,
    pub byte_fee: i64,
    pub to_address: Cow<'a, str>,
    pub change_address: Cow<'a, str>,
    pub private_key: Vec<Cow<'a, [u8]>>,
    pub scripts: KVMap<Cow<'a, str>, Cow<'a, [u8]>>,
    pub utxo: Vec<TW::Bitcoin::Proto::UnspentTransaction<'a>>,
    pub use_max_amount: bool,
    pub coin_type: u32,
    pub plan: Option<TW::Bitcoin::Proto::TransactionPlan<'a>>,
    pub lock_time: u32,
    pub output_op_return: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.hash_type = r.read_uint32(bytes)?,
                Ok(16) => msg.amount = r.read_int64(bytes)?,
                Ok(24) => msg.byte_fee = r.read_int64(bytes)?,
                Ok(34) => msg.to_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.change_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(50) => msg.private_key.push(r.read_bytes(bytes).map(Cow::Borrowed)?),
                Ok(58) => {
                    let (key, value) = r.read_map(bytes, |r, bytes| Ok(r.read_string(bytes).map(Cow::Borrowed)?), |r, bytes| Ok(r.read_bytes(bytes).map(Cow::Borrowed)?))?;
                    msg.scripts.insert(key, value);
                }
                Ok(66) => msg.utxo.push(r.read_message::<TW::Bitcoin::Proto::UnspentTransaction>(bytes)?),
                Ok(72) => msg.use_max_amount = r.read_bool(bytes)?,
                Ok(80) => msg.coin_type = r.read_uint32(bytes)?,
                Ok(90) => msg.plan = Some(r.read_message::<TW::Bitcoin::Proto::TransactionPlan>(bytes)?),
                Ok(96) => msg.lock_time = r.read_uint32(bytes)?,
                Ok(106) => msg.output_op_return = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for SigningInput<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.hash_type == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.hash_type) as u64) }
        + if self.amount == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
        + if self.byte_fee == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.byte_fee) as u64) }
        + if self.to_address == "" { 0 } else { 1 + sizeof_len((&self.to_address).len()) }
        + if self.change_address == "" { 0 } else { 1 + sizeof_len((&self.change_address).len()) }
        + self.private_key.iter().map(|s| 1 + sizeof_len((s).len())).sum::<usize>()
        + self.scripts.iter().map(|(k, v)| 1 + sizeof_len(2 + sizeof_len((k).len()) + sizeof_len((v).len()))).sum::<usize>()
        + self.utxo.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + if self.use_max_amount == false { 0 } else { 1 + sizeof_varint(*(&self.use_max_amount) as u64) }
        + if self.coin_type == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.coin_type) as u64) }
        + self.plan.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.lock_time == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.lock_time) as u64) }
        + if self.output_op_return == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.output_op_return).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.hash_type != 0u32 { w.write_with_tag(8, |w| w.write_uint32(*&self.hash_type))?; }
        if self.amount != 0i64 { w.write_with_tag(16, |w| w.write_int64(*&self.amount))?; }
        if self.byte_fee != 0i64 { w.write_with_tag(24, |w| w.write_int64(*&self.byte_fee))?; }
        if self.to_address != "" { w.write_with_tag(34, |w| w.write_string(&**&self.to_address))?; }
        if self.change_address != "" { w.write_with_tag(42, |w| w.write_string(&**&self.change_address))?; }
        for s in &self.private_key { w.write_with_tag(50, |w| w.write_bytes(&**s))?; }
        for (k, v) in self.scripts.iter() { w.write_with_tag(58, |w| w.write_map(2 + sizeof_len((k).len()) + sizeof_len((v).len()), 10, |w| w.write_string(&**k), 18, |w| w.write_bytes(&**v)))?; }
        for s in &self.utxo { w.write_with_tag(66, |w| w.write_message(s))?; }
        if self.use_max_amount != false { w.write_with_tag(72, |w| w.write_bool(*&self.use_max_amount))?; }
        if self.coin_type != 0u32 { w.write_with_tag(80, |w| w.write_uint32(*&self.coin_type))?; }
        if let Some(ref s) = self.plan { w.write_with_tag(90, |w| w.write_message(s))?; }
        if self.lock_time != 0u32 { w.write_with_tag(96, |w| w.write_uint32(*&self.lock_time))?; }
        if self.output_op_return != Cow::Borrowed(b"") { w.write_with_tag(106, |w| w.write_bytes(&**&self.output_op_return))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TransactionPlan<'a> {
    pub amount: i64,
    pub available_amount: i64,
    pub fee: i64,
    pub change: i64,
    pub utxos: Vec<TW::Bitcoin::Proto::UnspentTransaction<'a>>,
    pub branch_id: Cow<'a, [u8]>,
    pub error: TW::Common::Proto::SigningError,
    pub output_op_return: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for TransactionPlan<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.amount = r.read_int64(bytes)?,
                Ok(16) => msg.available_amount = r.read_int64(bytes)?,
                Ok(24) => msg.fee = r.read_int64(bytes)?,
                Ok(32) => msg.change = r.read_int64(bytes)?,
                Ok(42) => msg.utxos.push(r.read_message::<TW::Bitcoin::Proto::UnspentTransaction>(bytes)?),
                Ok(50) => msg.branch_id = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(56) => msg.error = r.read_enum(bytes)?,
                Ok(66) => msg.output_op_return = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TransactionPlan<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.amount == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
        + if self.available_amount == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.available_amount) as u64) }
        + if self.fee == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.fee) as u64) }
        + if self.change == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.change) as u64) }
        + self.utxos.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + if self.branch_id == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.branch_id).len()) }
        + if self.error == TW::Common::Proto::SigningError::OK { 0 } else { 1 + sizeof_varint(*(&self.error) as u64) }
        + if self.output_op_return == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.output_op_return).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.amount != 0i64 { w.write_with_tag(8, |w| w.write_int64(*&self.amount))?; }
        if self.available_amount != 0i64 { w.write_with_tag(16, |w| w.write_int64(*&self.available_amount))?; }
        if self.fee != 0i64 { w.write_with_tag(24, |w| w.write_int64(*&self.fee))?; }
        if self.change != 0i64 { w.write_with_tag(32, |w| w.write_int64(*&self.change))?; }
        for s in &self.utxos { w.write_with_tag(42, |w| w.write_message(s))?; }
        if self.branch_id != Cow::Borrowed(b"") { w.write_with_tag(50, |w| w.write_bytes(&**&self.branch_id))?; }
        if self.error != TW::Common::Proto::SigningError::OK { w.write_with_tag(56, |w| w.write_enum(*&self.error as i32))?; }
        if self.output_op_return != Cow::Borrowed(b"") { w.write_with_tag(66, |w| w.write_bytes(&**&self.output_op_return))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningOutput<'a> {
    pub transaction: Option<TW::Bitcoin::Proto::Transaction<'a>>,
    pub encoded: Cow<'a, [u8]>,
    pub transaction_id: Cow<'a, str>,
    pub error: TW::Common::Proto::SigningError,
    pub error_message: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.transaction = Some(r.read_message::<TW::Bitcoin::Proto::Transaction>(bytes)?),
                Ok(18) => msg.encoded = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.transaction_id = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(32) => msg.error = r.read_enum(bytes)?,
                Ok(42) => msg.error_message = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for SigningOutput<'a> {
    fn get_size(&self) -> usize {
        0
        + self.transaction.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.encoded == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.encoded).len()) }
        + if self.transaction_id == "" { 0 } else { 1 + sizeof_len((&self.transaction_id).len()) }
        + if self.error == TW::Common::Proto::SigningError::OK { 0 } else { 1 + sizeof_varint(*(&self.error) as u64) }
        + if self.error_message == "" { 0 } else { 1 + sizeof_len((&self.error_message).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.transaction { w.write_with_tag(10, |w| w.write_message(s))?; }
        if self.encoded != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.encoded))?; }
        if self.transaction_id != "" { w.write_with_tag(26, |w| w.write_string(&**&self.transaction_id))?; }
        if self.error != TW::Common::Proto::SigningError::OK { w.write_with_tag(32, |w| w.write_enum(*&self.error as i32))?; }
        if self.error_message != "" { w.write_with_tag(42, |w| w.write_string(&**&self.error_message))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct HashPublicKey<'a> {
    pub data_hash: Cow<'a, [u8]>,
    pub public_key_hash: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for HashPublicKey<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.data_hash = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.public_key_hash = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for HashPublicKey<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.data_hash == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.data_hash).len()) }
        + if self.public_key_hash == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.public_key_hash).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.data_hash != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.data_hash))?; }
        if self.public_key_hash != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.public_key_hash))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct PreSigningOutput<'a> {
    pub hash_public_keys: Vec<TW::Bitcoin::Proto::HashPublicKey<'a>>,
    pub error: TW::Common::Proto::SigningError,
    pub error_message: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for PreSigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.hash_public_keys.push(r.read_message::<TW::Bitcoin::Proto::HashPublicKey>(bytes)?),
                Ok(16) => msg.error = r.read_enum(bytes)?,
                Ok(26) => msg.error_message = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for PreSigningOutput<'a> {
    fn get_size(&self) -> usize {
        0
        + self.hash_public_keys.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + if self.error == TW::Common::Proto::SigningError::OK { 0 } else { 1 + sizeof_varint(*(&self.error) as u64) }
        + if self.error_message == "" { 0 } else { 1 + sizeof_len((&self.error_message).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        for s in &self.hash_public_keys { w.write_with_tag(10, |w| w.write_message(s))?; }
        if self.error != TW::Common::Proto::SigningError::OK { w.write_with_tag(16, |w| w.write_enum(*&self.error as i32))?; }
        if self.error_message != "" { w.write_with_tag(26, |w| w.write_string(&**&self.error_message))?; }
        Ok(())
    }
}

