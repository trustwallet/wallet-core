// Automatically generated rust module for 'Aion.proto' file

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
    pub nonce: Cow<'a, [u8]>,
    pub gas_price: Cow<'a, [u8]>,
    pub gas_limit: Cow<'a, [u8]>,
    pub to_address: Cow<'a, str>,
    pub amount: Cow<'a, [u8]>,
    pub payload: Cow<'a, [u8]>,
    pub private_key: Cow<'a, [u8]>,
    pub timestamp: u64,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.nonce = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.gas_price = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.gas_limit = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.to_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.amount = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(50) => msg.payload = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(58) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(64) => msg.timestamp = r.read_uint64(bytes)?,
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
        + if self.nonce == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.nonce).len()) }
        + if self.gas_price == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.gas_price).len()) }
        + if self.gas_limit == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.gas_limit).len()) }
        + if self.to_address == "" { 0 } else { 1 + sizeof_len((&self.to_address).len()) }
        + if self.amount == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.amount).len()) }
        + if self.payload == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.payload).len()) }
        + if self.private_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.private_key).len()) }
        + if self.timestamp == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.timestamp) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.nonce != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.nonce))?; }
        if self.gas_price != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.gas_price))?; }
        if self.gas_limit != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.gas_limit))?; }
        if self.to_address != "" { w.write_with_tag(34, |w| w.write_string(&**&self.to_address))?; }
        if self.amount != Cow::Borrowed(b"") { w.write_with_tag(42, |w| w.write_bytes(&**&self.amount))?; }
        if self.payload != Cow::Borrowed(b"") { w.write_with_tag(50, |w| w.write_bytes(&**&self.payload))?; }
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(58, |w| w.write_bytes(&**&self.private_key))?; }
        if self.timestamp != 0u64 { w.write_with_tag(64, |w| w.write_uint64(*&self.timestamp))?; }
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

