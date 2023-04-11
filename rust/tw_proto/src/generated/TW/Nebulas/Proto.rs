// Automatically generated rust module for 'Nebulas.proto' file

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
pub struct SigningInput<'a> {
    pub from_address: Cow<'a, str>,
    pub chain_id: Cow<'a, [u8]>,
    pub nonce: Cow<'a, [u8]>,
    pub gas_price: Cow<'a, [u8]>,
    pub gas_limit: Cow<'a, [u8]>,
    pub to_address: Cow<'a, str>,
    pub amount: Cow<'a, [u8]>,
    pub timestamp: Cow<'a, [u8]>,
    pub payload: Cow<'a, str>,
    pub private_key: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.from_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.chain_id = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.nonce = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.gas_price = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.gas_limit = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(50) => msg.to_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(58) => msg.amount = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(66) => msg.timestamp = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(74) => msg.payload = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(82) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
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
        + if self.from_address == "" { 0 } else { 1 + sizeof_len((&self.from_address).len()) }
        + if self.chain_id == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.chain_id).len()) }
        + if self.nonce == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.nonce).len()) }
        + if self.gas_price == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.gas_price).len()) }
        + if self.gas_limit == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.gas_limit).len()) }
        + if self.to_address == "" { 0 } else { 1 + sizeof_len((&self.to_address).len()) }
        + if self.amount == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.amount).len()) }
        + if self.timestamp == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.timestamp).len()) }
        + if self.payload == "" { 0 } else { 1 + sizeof_len((&self.payload).len()) }
        + if self.private_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.private_key).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.from_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.from_address))?; }
        if self.chain_id != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.chain_id))?; }
        if self.nonce != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.nonce))?; }
        if self.gas_price != Cow::Borrowed(b"") { w.write_with_tag(34, |w| w.write_bytes(&**&self.gas_price))?; }
        if self.gas_limit != Cow::Borrowed(b"") { w.write_with_tag(42, |w| w.write_bytes(&**&self.gas_limit))?; }
        if self.to_address != "" { w.write_with_tag(50, |w| w.write_string(&**&self.to_address))?; }
        if self.amount != Cow::Borrowed(b"") { w.write_with_tag(58, |w| w.write_bytes(&**&self.amount))?; }
        if self.timestamp != Cow::Borrowed(b"") { w.write_with_tag(66, |w| w.write_bytes(&**&self.timestamp))?; }
        if self.payload != "" { w.write_with_tag(74, |w| w.write_string(&**&self.payload))?; }
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(82, |w| w.write_bytes(&**&self.private_key))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningOutput<'a> {
    pub algorithm: u32,
    pub signature: Cow<'a, [u8]>,
    pub raw: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.algorithm = r.read_uint32(bytes)?,
                Ok(18) => msg.signature = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.raw = r.read_string(bytes).map(Cow::Borrowed)?,
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
        + if self.algorithm == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.algorithm) as u64) }
        + if self.signature == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.signature).len()) }
        + if self.raw == "" { 0 } else { 1 + sizeof_len((&self.raw).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.algorithm != 0u32 { w.write_with_tag(8, |w| w.write_uint32(*&self.algorithm))?; }
        if self.signature != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.signature))?; }
        if self.raw != "" { w.write_with_tag(26, |w| w.write_string(&**&self.raw))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Data<'a> {
    pub type_pb: Cow<'a, str>,
    pub payload: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for Data<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.type_pb = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.payload = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Data<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.type_pb == "" { 0 } else { 1 + sizeof_len((&self.type_pb).len()) }
        + if self.payload == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.payload).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.type_pb != "" { w.write_with_tag(10, |w| w.write_string(&**&self.type_pb))?; }
        if self.payload != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.payload))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct RawTransaction<'a> {
    pub hash: Cow<'a, [u8]>,
    pub from: Cow<'a, [u8]>,
    pub to: Cow<'a, [u8]>,
    pub value: Cow<'a, [u8]>,
    pub nonce: u64,
    pub timestamp: i64,
    pub data: Option<TW::Nebulas::Proto::Data<'a>>,
    pub chain_id: u32,
    pub gas_price: Cow<'a, [u8]>,
    pub gas_limit: Cow<'a, [u8]>,
    pub alg: u32,
    pub sign: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for RawTransaction<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.hash = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.from = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.to = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.value = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(40) => msg.nonce = r.read_uint64(bytes)?,
                Ok(48) => msg.timestamp = r.read_int64(bytes)?,
                Ok(58) => msg.data = Some(r.read_message::<TW::Nebulas::Proto::Data>(bytes)?),
                Ok(64) => msg.chain_id = r.read_uint32(bytes)?,
                Ok(74) => msg.gas_price = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(82) => msg.gas_limit = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(88) => msg.alg = r.read_uint32(bytes)?,
                Ok(98) => msg.sign = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for RawTransaction<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.hash == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.hash).len()) }
        + if self.from == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.from).len()) }
        + if self.to == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.to).len()) }
        + if self.value == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.value).len()) }
        + if self.nonce == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.nonce) as u64) }
        + if self.timestamp == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.timestamp) as u64) }
        + self.data.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.chain_id == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.chain_id) as u64) }
        + if self.gas_price == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.gas_price).len()) }
        + if self.gas_limit == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.gas_limit).len()) }
        + if self.alg == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.alg) as u64) }
        + if self.sign == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.sign).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.hash != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.hash))?; }
        if self.from != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.from))?; }
        if self.to != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.to))?; }
        if self.value != Cow::Borrowed(b"") { w.write_with_tag(34, |w| w.write_bytes(&**&self.value))?; }
        if self.nonce != 0u64 { w.write_with_tag(40, |w| w.write_uint64(*&self.nonce))?; }
        if self.timestamp != 0i64 { w.write_with_tag(48, |w| w.write_int64(*&self.timestamp))?; }
        if let Some(ref s) = self.data { w.write_with_tag(58, |w| w.write_message(s))?; }
        if self.chain_id != 0u32 { w.write_with_tag(64, |w| w.write_uint32(*&self.chain_id))?; }
        if self.gas_price != Cow::Borrowed(b"") { w.write_with_tag(74, |w| w.write_bytes(&**&self.gas_price))?; }
        if self.gas_limit != Cow::Borrowed(b"") { w.write_with_tag(82, |w| w.write_bytes(&**&self.gas_limit))?; }
        if self.alg != 0u32 { w.write_with_tag(88, |w| w.write_uint32(*&self.alg))?; }
        if self.sign != Cow::Borrowed(b"") { w.write_with_tag(98, |w| w.write_bytes(&**&self.sign))?; }
        Ok(())
    }
}

