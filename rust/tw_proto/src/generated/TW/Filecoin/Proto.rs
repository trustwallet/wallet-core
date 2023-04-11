// Automatically generated rust module for 'Filecoin.proto' file

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

#[derive(Debug, PartialEq, Eq, Clone, Copy)]
pub enum DerivationType {
    SECP256K1 = 0,
    DELEGATED = 1,
}

impl Default for DerivationType {
    fn default() -> Self {
        DerivationType::SECP256K1
    }
}

impl From<i32> for DerivationType {
    fn from(i: i32) -> Self {
        match i {
            0 => DerivationType::SECP256K1,
            1 => DerivationType::DELEGATED,
            _ => Self::default(),
        }
    }
}

impl<'a> From<&'a str> for DerivationType {
    fn from(s: &'a str) -> Self {
        match s {
            "SECP256K1" => DerivationType::SECP256K1,
            "DELEGATED" => DerivationType::DELEGATED,
            _ => Self::default(),
        }
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningInput<'a> {
    pub private_key: Cow<'a, [u8]>,
    pub to: Cow<'a, str>,
    pub nonce: u64,
    pub value: Cow<'a, [u8]>,
    pub gas_limit: i64,
    pub gas_fee_cap: Cow<'a, [u8]>,
    pub gas_premium: Cow<'a, [u8]>,
    pub params: Cow<'a, [u8]>,
    pub derivation: TW::Filecoin::Proto::DerivationType,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.to = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.nonce = r.read_uint64(bytes)?,
                Ok(34) => msg.value = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(40) => msg.gas_limit = r.read_int64(bytes)?,
                Ok(50) => msg.gas_fee_cap = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(58) => msg.gas_premium = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(66) => msg.params = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(72) => msg.derivation = r.read_enum(bytes)?,
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
        + if self.private_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.private_key).len()) }
        + if self.to == "" { 0 } else { 1 + sizeof_len((&self.to).len()) }
        + if self.nonce == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.nonce) as u64) }
        + if self.value == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.value).len()) }
        + if self.gas_limit == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.gas_limit) as u64) }
        + if self.gas_fee_cap == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.gas_fee_cap).len()) }
        + if self.gas_premium == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.gas_premium).len()) }
        + if self.params == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.params).len()) }
        + if self.derivation == TW::Filecoin::Proto::DerivationType::SECP256K1 { 0 } else { 1 + sizeof_varint(*(&self.derivation) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.private_key))?; }
        if self.to != "" { w.write_with_tag(18, |w| w.write_string(&**&self.to))?; }
        if self.nonce != 0u64 { w.write_with_tag(24, |w| w.write_uint64(*&self.nonce))?; }
        if self.value != Cow::Borrowed(b"") { w.write_with_tag(34, |w| w.write_bytes(&**&self.value))?; }
        if self.gas_limit != 0i64 { w.write_with_tag(40, |w| w.write_int64(*&self.gas_limit))?; }
        if self.gas_fee_cap != Cow::Borrowed(b"") { w.write_with_tag(50, |w| w.write_bytes(&**&self.gas_fee_cap))?; }
        if self.gas_premium != Cow::Borrowed(b"") { w.write_with_tag(58, |w| w.write_bytes(&**&self.gas_premium))?; }
        if self.params != Cow::Borrowed(b"") { w.write_with_tag(66, |w| w.write_bytes(&**&self.params))?; }
        if self.derivation != TW::Filecoin::Proto::DerivationType::SECP256K1 { w.write_with_tag(72, |w| w.write_enum(*&self.derivation as i32))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningOutput<'a> {
    pub json: Cow<'a, str>,
    pub error_message: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.json = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.error_message = r.read_string(bytes).map(Cow::Borrowed)?,
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
        + if self.json == "" { 0 } else { 1 + sizeof_len((&self.json).len()) }
        + if self.error_message == "" { 0 } else { 1 + sizeof_len((&self.error_message).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.json != "" { w.write_with_tag(10, |w| w.write_string(&**&self.json))?; }
        if self.error_message != "" { w.write_with_tag(18, |w| w.write_string(&**&self.error_message))?; }
        Ok(())
    }
}

