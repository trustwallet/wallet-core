// Automatically generated rust module for 'Ontology.proto' file

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
    pub contract: Cow<'a, str>,
    pub method: Cow<'a, str>,
    pub owner_private_key: Cow<'a, [u8]>,
    pub to_address: Cow<'a, str>,
    pub amount: u64,
    pub payer_private_key: Cow<'a, [u8]>,
    pub gas_price: u64,
    pub gas_limit: u64,
    pub query_address: Cow<'a, str>,
    pub nonce: u32,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.contract = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.method = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.owner_private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.to_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(40) => msg.amount = r.read_uint64(bytes)?,
                Ok(50) => msg.payer_private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(56) => msg.gas_price = r.read_uint64(bytes)?,
                Ok(64) => msg.gas_limit = r.read_uint64(bytes)?,
                Ok(74) => msg.query_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(80) => msg.nonce = r.read_uint32(bytes)?,
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
        + if self.contract == "" { 0 } else { 1 + sizeof_len((&self.contract).len()) }
        + if self.method == "" { 0 } else { 1 + sizeof_len((&self.method).len()) }
        + if self.owner_private_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.owner_private_key).len()) }
        + if self.to_address == "" { 0 } else { 1 + sizeof_len((&self.to_address).len()) }
        + if self.amount == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
        + if self.payer_private_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.payer_private_key).len()) }
        + if self.gas_price == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.gas_price) as u64) }
        + if self.gas_limit == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.gas_limit) as u64) }
        + if self.query_address == "" { 0 } else { 1 + sizeof_len((&self.query_address).len()) }
        + if self.nonce == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.nonce) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.contract != "" { w.write_with_tag(10, |w| w.write_string(&**&self.contract))?; }
        if self.method != "" { w.write_with_tag(18, |w| w.write_string(&**&self.method))?; }
        if self.owner_private_key != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.owner_private_key))?; }
        if self.to_address != "" { w.write_with_tag(34, |w| w.write_string(&**&self.to_address))?; }
        if self.amount != 0u64 { w.write_with_tag(40, |w| w.write_uint64(*&self.amount))?; }
        if self.payer_private_key != Cow::Borrowed(b"") { w.write_with_tag(50, |w| w.write_bytes(&**&self.payer_private_key))?; }
        if self.gas_price != 0u64 { w.write_with_tag(56, |w| w.write_uint64(*&self.gas_price))?; }
        if self.gas_limit != 0u64 { w.write_with_tag(64, |w| w.write_uint64(*&self.gas_limit))?; }
        if self.query_address != "" { w.write_with_tag(74, |w| w.write_string(&**&self.query_address))?; }
        if self.nonce != 0u32 { w.write_with_tag(80, |w| w.write_uint32(*&self.nonce))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningOutput<'a> {
    pub encoded: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.encoded = r.read_bytes(bytes).map(Cow::Borrowed)?,
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
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.encoded != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.encoded))?; }
        Ok(())
    }
}

