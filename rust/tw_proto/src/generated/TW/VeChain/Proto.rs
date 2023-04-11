// Automatically generated rust module for 'VeChain.proto' file

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
pub struct Clause<'a> {
    pub to: Cow<'a, str>,
    pub value: Cow<'a, [u8]>,
    pub data: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for Clause<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.to = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.value = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.data = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Clause<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.to == "" { 0 } else { 1 + sizeof_len((&self.to).len()) }
        + if self.value == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.value).len()) }
        + if self.data == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.data).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.to != "" { w.write_with_tag(10, |w| w.write_string(&**&self.to))?; }
        if self.value != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.value))?; }
        if self.data != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.data))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningInput<'a> {
    pub chain_tag: u32,
    pub block_ref: u64,
    pub expiration: u32,
    pub clauses: Vec<TW::VeChain::Proto::Clause<'a>>,
    pub gas_price_coef: u32,
    pub gas: u64,
    pub depends_on: Cow<'a, [u8]>,
    pub nonce: u64,
    pub private_key: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.chain_tag = r.read_uint32(bytes)?,
                Ok(16) => msg.block_ref = r.read_uint64(bytes)?,
                Ok(24) => msg.expiration = r.read_uint32(bytes)?,
                Ok(34) => msg.clauses.push(r.read_message::<TW::VeChain::Proto::Clause>(bytes)?),
                Ok(40) => msg.gas_price_coef = r.read_uint32(bytes)?,
                Ok(48) => msg.gas = r.read_uint64(bytes)?,
                Ok(58) => msg.depends_on = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(64) => msg.nonce = r.read_uint64(bytes)?,
                Ok(74) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
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
        + if self.chain_tag == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.chain_tag) as u64) }
        + if self.block_ref == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.block_ref) as u64) }
        + if self.expiration == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.expiration) as u64) }
        + self.clauses.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + if self.gas_price_coef == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.gas_price_coef) as u64) }
        + if self.gas == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.gas) as u64) }
        + if self.depends_on == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.depends_on).len()) }
        + if self.nonce == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.nonce) as u64) }
        + if self.private_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.private_key).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.chain_tag != 0u32 { w.write_with_tag(8, |w| w.write_uint32(*&self.chain_tag))?; }
        if self.block_ref != 0u64 { w.write_with_tag(16, |w| w.write_uint64(*&self.block_ref))?; }
        if self.expiration != 0u32 { w.write_with_tag(24, |w| w.write_uint32(*&self.expiration))?; }
        for s in &self.clauses { w.write_with_tag(34, |w| w.write_message(s))?; }
        if self.gas_price_coef != 0u32 { w.write_with_tag(40, |w| w.write_uint32(*&self.gas_price_coef))?; }
        if self.gas != 0u64 { w.write_with_tag(48, |w| w.write_uint64(*&self.gas))?; }
        if self.depends_on != Cow::Borrowed(b"") { w.write_with_tag(58, |w| w.write_bytes(&**&self.depends_on))?; }
        if self.nonce != 0u64 { w.write_with_tag(64, |w| w.write_uint64(*&self.nonce))?; }
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(74, |w| w.write_bytes(&**&self.private_key))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningOutput<'a> {
    pub encoded: Cow<'a, [u8]>,
    pub signature: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.encoded = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.signature = r.read_bytes(bytes).map(Cow::Borrowed)?,
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
        + if self.encoded == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.encoded).len()) }
        + if self.signature == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.signature).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.encoded != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.encoded))?; }
        if self.signature != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.signature))?; }
        Ok(())
    }
}

