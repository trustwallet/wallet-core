// Automatically generated rust module for 'EOS.proto' file

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
pub enum KeyType {
    LEGACY = 0,
    MODERNK1 = 1,
    MODERNR1 = 2,
}

impl Default for KeyType {
    fn default() -> Self {
        KeyType::LEGACY
    }
}

impl From<i32> for KeyType {
    fn from(i: i32) -> Self {
        match i {
            0 => KeyType::LEGACY,
            1 => KeyType::MODERNK1,
            2 => KeyType::MODERNR1,
            _ => Self::default(),
        }
    }
}

impl<'a> From<&'a str> for KeyType {
    fn from(s: &'a str) -> Self {
        match s {
            "LEGACY" => KeyType::LEGACY,
            "MODERNK1" => KeyType::MODERNK1,
            "MODERNR1" => KeyType::MODERNR1,
            _ => Self::default(),
        }
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Asset<'a> {
    pub amount: i64,
    pub decimals: u32,
    pub symbol: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for Asset<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.amount = r.read_int64(bytes)?,
                Ok(16) => msg.decimals = r.read_uint32(bytes)?,
                Ok(26) => msg.symbol = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Asset<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.amount == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
        + if self.decimals == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.decimals) as u64) }
        + if self.symbol == "" { 0 } else { 1 + sizeof_len((&self.symbol).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.amount != 0i64 { w.write_with_tag(8, |w| w.write_int64(*&self.amount))?; }
        if self.decimals != 0u32 { w.write_with_tag(16, |w| w.write_uint32(*&self.decimals))?; }
        if self.symbol != "" { w.write_with_tag(26, |w| w.write_string(&**&self.symbol))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningInput<'a> {
    pub chain_id: Cow<'a, [u8]>,
    pub reference_block_id: Cow<'a, [u8]>,
    pub reference_block_time: i32,
    pub currency: Cow<'a, str>,
    pub sender: Cow<'a, str>,
    pub recipient: Cow<'a, str>,
    pub memo: Cow<'a, str>,
    pub asset: Option<TW::EOS::Proto::Asset<'a>>,
    pub private_key: Cow<'a, [u8]>,
    pub private_key_type: TW::EOS::Proto::KeyType,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.chain_id = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.reference_block_id = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(29) => msg.reference_block_time = r.read_sfixed32(bytes)?,
                Ok(34) => msg.currency = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.sender = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(50) => msg.recipient = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(58) => msg.memo = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(66) => msg.asset = Some(r.read_message::<TW::EOS::Proto::Asset>(bytes)?),
                Ok(74) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(80) => msg.private_key_type = r.read_enum(bytes)?,
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
        + if self.chain_id == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.chain_id).len()) }
        + if self.reference_block_id == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.reference_block_id).len()) }
        + if self.reference_block_time == 0i32 { 0 } else { 1 + 4 }
        + if self.currency == "" { 0 } else { 1 + sizeof_len((&self.currency).len()) }
        + if self.sender == "" { 0 } else { 1 + sizeof_len((&self.sender).len()) }
        + if self.recipient == "" { 0 } else { 1 + sizeof_len((&self.recipient).len()) }
        + if self.memo == "" { 0 } else { 1 + sizeof_len((&self.memo).len()) }
        + self.asset.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.private_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.private_key).len()) }
        + if self.private_key_type == TW::EOS::Proto::KeyType::LEGACY { 0 } else { 1 + sizeof_varint(*(&self.private_key_type) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.chain_id != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.chain_id))?; }
        if self.reference_block_id != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.reference_block_id))?; }
        if self.reference_block_time != 0i32 { w.write_with_tag(29, |w| w.write_sfixed32(*&self.reference_block_time))?; }
        if self.currency != "" { w.write_with_tag(34, |w| w.write_string(&**&self.currency))?; }
        if self.sender != "" { w.write_with_tag(42, |w| w.write_string(&**&self.sender))?; }
        if self.recipient != "" { w.write_with_tag(50, |w| w.write_string(&**&self.recipient))?; }
        if self.memo != "" { w.write_with_tag(58, |w| w.write_string(&**&self.memo))?; }
        if let Some(ref s) = self.asset { w.write_with_tag(66, |w| w.write_message(s))?; }
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(74, |w| w.write_bytes(&**&self.private_key))?; }
        if self.private_key_type != TW::EOS::Proto::KeyType::LEGACY { w.write_with_tag(80, |w| w.write_enum(*&self.private_key_type as i32))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningOutput<'a> {
    pub json_encoded: Cow<'a, str>,
    pub error: TW::Common::Proto::SigningError,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.json_encoded = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.error = r.read_enum(bytes)?,
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
        + if self.json_encoded == "" { 0 } else { 1 + sizeof_len((&self.json_encoded).len()) }
        + if self.error == TW::Common::Proto::SigningError::OK { 0 } else { 1 + sizeof_varint(*(&self.error) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.json_encoded != "" { w.write_with_tag(10, |w| w.write_string(&**&self.json_encoded))?; }
        if self.error != TW::Common::Proto::SigningError::OK { w.write_with_tag(16, |w| w.write_enum(*&self.error as i32))?; }
        Ok(())
    }
}

