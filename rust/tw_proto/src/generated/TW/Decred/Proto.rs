// Automatically generated rust module for 'Decred.proto' file

#![allow(non_snake_case)]
#![allow(non_upper_case_globals)]
#![allow(non_camel_case_types)]
#![allow(unused_imports)]
#![allow(unknown_lints)]
#![allow(clippy::all)]
#![cfg_attr(rustfmt, rustfmt_skip)]


use std::borrow::Cow;
use quick_protobuf::{MessageInfo, MessageRead, MessageWrite, BytesReader, Writer, WriterBackend, Result};
use quick_protobuf::sizeofs::*;
use super::super::super::*;

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Transaction<'a> {
    pub serializeType: u32,
    pub version: u32,
    pub inputs: Vec<TW::Decred::Proto::TransactionInput<'a>>,
    pub outputs: Vec<TW::Decred::Proto::TransactionOutput<'a>>,
    pub lockTime: u32,
    pub expiry: u32,
}

impl<'a> MessageRead<'a> for Transaction<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.serializeType = r.read_uint32(bytes)?,
                Ok(16) => msg.version = r.read_uint32(bytes)?,
                Ok(26) => msg.inputs.push(r.read_message::<TW::Decred::Proto::TransactionInput>(bytes)?),
                Ok(34) => msg.outputs.push(r.read_message::<TW::Decred::Proto::TransactionOutput>(bytes)?),
                Ok(40) => msg.lockTime = r.read_uint32(bytes)?,
                Ok(48) => msg.expiry = r.read_uint32(bytes)?,
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
        + if self.serializeType == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.serializeType) as u64) }
        + if self.version == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.version) as u64) }
        + self.inputs.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + self.outputs.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + if self.lockTime == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.lockTime) as u64) }
        + if self.expiry == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.expiry) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.serializeType != 0u32 { w.write_with_tag(8, |w| w.write_uint32(*&self.serializeType))?; }
        if self.version != 0u32 { w.write_with_tag(16, |w| w.write_uint32(*&self.version))?; }
        for s in &self.inputs { w.write_with_tag(26, |w| w.write_message(s))?; }
        for s in &self.outputs { w.write_with_tag(34, |w| w.write_message(s))?; }
        if self.lockTime != 0u32 { w.write_with_tag(40, |w| w.write_uint32(*&self.lockTime))?; }
        if self.expiry != 0u32 { w.write_with_tag(48, |w| w.write_uint32(*&self.expiry))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TransactionInput<'a> {
    pub previousOutput: Option<TW::Bitcoin::Proto::OutPoint<'a>>,
    pub sequence: u32,
    pub valueIn: i64,
    pub blockHeight: u32,
    pub blockIndex: u32,
    pub script: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for TransactionInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.previousOutput = Some(r.read_message::<TW::Bitcoin::Proto::OutPoint>(bytes)?),
                Ok(16) => msg.sequence = r.read_uint32(bytes)?,
                Ok(24) => msg.valueIn = r.read_int64(bytes)?,
                Ok(32) => msg.blockHeight = r.read_uint32(bytes)?,
                Ok(40) => msg.blockIndex = r.read_uint32(bytes)?,
                Ok(50) => msg.script = r.read_bytes(bytes).map(Cow::Borrowed)?,
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
        + if self.valueIn == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.valueIn) as u64) }
        + if self.blockHeight == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.blockHeight) as u64) }
        + if self.blockIndex == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.blockIndex) as u64) }
        + if self.script == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.script).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.previousOutput { w.write_with_tag(10, |w| w.write_message(s))?; }
        if self.sequence != 0u32 { w.write_with_tag(16, |w| w.write_uint32(*&self.sequence))?; }
        if self.valueIn != 0i64 { w.write_with_tag(24, |w| w.write_int64(*&self.valueIn))?; }
        if self.blockHeight != 0u32 { w.write_with_tag(32, |w| w.write_uint32(*&self.blockHeight))?; }
        if self.blockIndex != 0u32 { w.write_with_tag(40, |w| w.write_uint32(*&self.blockIndex))?; }
        if self.script != Cow::Borrowed(b"") { w.write_with_tag(50, |w| w.write_bytes(&**&self.script))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TransactionOutput<'a> {
    pub value: i64,
    pub version: u32,
    pub script: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for TransactionOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.value = r.read_int64(bytes)?,
                Ok(16) => msg.version = r.read_uint32(bytes)?,
                Ok(26) => msg.script = r.read_bytes(bytes).map(Cow::Borrowed)?,
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
        + if self.version == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.version) as u64) }
        + if self.script == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.script).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.value != 0i64 { w.write_with_tag(8, |w| w.write_int64(*&self.value))?; }
        if self.version != 0u32 { w.write_with_tag(16, |w| w.write_uint32(*&self.version))?; }
        if self.script != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.script))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningOutput<'a> {
    pub transaction: Option<TW::Decred::Proto::Transaction<'a>>,
    pub encoded: Cow<'a, [u8]>,
    pub transaction_id: Cow<'a, str>,
    pub error: TW::Common::Proto::SigningError,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.transaction = Some(r.read_message::<TW::Decred::Proto::Transaction>(bytes)?),
                Ok(18) => msg.encoded = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.transaction_id = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(32) => msg.error = r.read_enum(bytes)?,
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
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.transaction { w.write_with_tag(10, |w| w.write_message(s))?; }
        if self.encoded != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.encoded))?; }
        if self.transaction_id != "" { w.write_with_tag(26, |w| w.write_string(&**&self.transaction_id))?; }
        if self.error != TW::Common::Proto::SigningError::OK { w.write_with_tag(32, |w| w.write_enum(*&self.error as i32))?; }
        Ok(())
    }
}

