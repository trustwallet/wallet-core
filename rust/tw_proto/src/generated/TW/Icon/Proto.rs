// Automatically generated rust module for 'Icon.proto' file

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
    pub to_address: Cow<'a, str>,
    pub value: Cow<'a, [u8]>,
    pub step_limit: Cow<'a, [u8]>,
    pub timestamp: i64,
    pub nonce: Cow<'a, [u8]>,
    pub network_id: Cow<'a, [u8]>,
    pub private_key: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.from_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.to_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.value = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.step_limit = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(40) => msg.timestamp = r.read_int64(bytes)?,
                Ok(50) => msg.nonce = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(58) => msg.network_id = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(66) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
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
        + if self.to_address == "" { 0 } else { 1 + sizeof_len((&self.to_address).len()) }
        + if self.value == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.value).len()) }
        + if self.step_limit == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.step_limit).len()) }
        + if self.timestamp == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.timestamp) as u64) }
        + if self.nonce == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.nonce).len()) }
        + if self.network_id == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.network_id).len()) }
        + if self.private_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.private_key).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.from_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.from_address))?; }
        if self.to_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.to_address))?; }
        if self.value != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.value))?; }
        if self.step_limit != Cow::Borrowed(b"") { w.write_with_tag(34, |w| w.write_bytes(&**&self.step_limit))?; }
        if self.timestamp != 0i64 { w.write_with_tag(40, |w| w.write_int64(*&self.timestamp))?; }
        if self.nonce != Cow::Borrowed(b"") { w.write_with_tag(50, |w| w.write_bytes(&**&self.nonce))?; }
        if self.network_id != Cow::Borrowed(b"") { w.write_with_tag(58, |w| w.write_bytes(&**&self.network_id))?; }
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(66, |w| w.write_bytes(&**&self.private_key))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningOutput<'a> {
    pub encoded: Cow<'a, str>,
    pub signature: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.encoded = r.read_string(bytes).map(Cow::Borrowed)?,
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
        + if self.encoded == "" { 0 } else { 1 + sizeof_len((&self.encoded).len()) }
        + if self.signature == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.signature).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.encoded != "" { w.write_with_tag(10, |w| w.write_string(&**&self.encoded))?; }
        if self.signature != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.signature))?; }
        Ok(())
    }
}

