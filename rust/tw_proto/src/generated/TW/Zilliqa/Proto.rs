// Automatically generated rust module for 'Zilliqa.proto' file

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
    pub message_oneof: TW::Zilliqa::Proto::mod_Transaction::OneOfmessage_oneof<'a>,
}

impl<'a> MessageRead<'a> for Transaction<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.message_oneof = TW::Zilliqa::Proto::mod_Transaction::OneOfmessage_oneof::transfer(r.read_message::<TW::Zilliqa::Proto::mod_Transaction::Transfer>(bytes)?),
                Ok(18) => msg.message_oneof = TW::Zilliqa::Proto::mod_Transaction::OneOfmessage_oneof::raw_transaction(r.read_message::<TW::Zilliqa::Proto::mod_Transaction::Raw>(bytes)?),
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
        + match self.message_oneof {
            TW::Zilliqa::Proto::mod_Transaction::OneOfmessage_oneof::transfer(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Zilliqa::Proto::mod_Transaction::OneOfmessage_oneof::raw_transaction(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Zilliqa::Proto::mod_Transaction::OneOfmessage_oneof::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        match self.message_oneof {            TW::Zilliqa::Proto::mod_Transaction::OneOfmessage_oneof::transfer(ref m) => { w.write_with_tag(10, |w| w.write_message(m))? },
            TW::Zilliqa::Proto::mod_Transaction::OneOfmessage_oneof::raw_transaction(ref m) => { w.write_with_tag(18, |w| w.write_message(m))? },
            TW::Zilliqa::Proto::mod_Transaction::OneOfmessage_oneof::None => {},
    }        Ok(())
    }
}

pub mod mod_Transaction {

use std::borrow::Cow;
use super::*;

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Transfer<'a> {
    pub amount: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for Transfer<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.amount = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Transfer<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.amount == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.amount).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.amount != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Raw<'a> {
    pub amount: Cow<'a, [u8]>,
    pub code: Cow<'a, [u8]>,
    pub data: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for Raw<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.amount = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.code = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.data = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Raw<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.amount == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.amount).len()) }
        + if self.code == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.code).len()) }
        + if self.data == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.data).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.amount != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.amount))?; }
        if self.code != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.code))?; }
        if self.data != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.data))?; }
        Ok(())
    }
}

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfmessage_oneof<'a> {
    transfer(TW::Zilliqa::Proto::mod_Transaction::Transfer<'a>),
    raw_transaction(TW::Zilliqa::Proto::mod_Transaction::Raw<'a>),
    None,
}

impl<'a> Default for OneOfmessage_oneof<'a> {
    fn default() -> Self {
        OneOfmessage_oneof::None
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningInput<'a> {
    pub version: u32,
    pub nonce: u64,
    pub to: Cow<'a, str>,
    pub gas_price: Cow<'a, [u8]>,
    pub gas_limit: u64,
    pub private_key: Cow<'a, [u8]>,
    pub transaction: Option<TW::Zilliqa::Proto::Transaction<'a>>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.version = r.read_uint32(bytes)?,
                Ok(16) => msg.nonce = r.read_uint64(bytes)?,
                Ok(26) => msg.to = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.gas_price = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(40) => msg.gas_limit = r.read_uint64(bytes)?,
                Ok(50) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(58) => msg.transaction = Some(r.read_message::<TW::Zilliqa::Proto::Transaction>(bytes)?),
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
        + if self.version == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.version) as u64) }
        + if self.nonce == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.nonce) as u64) }
        + if self.to == "" { 0 } else { 1 + sizeof_len((&self.to).len()) }
        + if self.gas_price == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.gas_price).len()) }
        + if self.gas_limit == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.gas_limit) as u64) }
        + if self.private_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.private_key).len()) }
        + self.transaction.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.version != 0u32 { w.write_with_tag(8, |w| w.write_uint32(*&self.version))?; }
        if self.nonce != 0u64 { w.write_with_tag(16, |w| w.write_uint64(*&self.nonce))?; }
        if self.to != "" { w.write_with_tag(26, |w| w.write_string(&**&self.to))?; }
        if self.gas_price != Cow::Borrowed(b"") { w.write_with_tag(34, |w| w.write_bytes(&**&self.gas_price))?; }
        if self.gas_limit != 0u64 { w.write_with_tag(40, |w| w.write_uint64(*&self.gas_limit))?; }
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(50, |w| w.write_bytes(&**&self.private_key))?; }
        if let Some(ref s) = self.transaction { w.write_with_tag(58, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningOutput<'a> {
    pub signature: Cow<'a, [u8]>,
    pub json: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.signature = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.json = r.read_string(bytes).map(Cow::Borrowed)?,
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
        + if self.signature == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.signature).len()) }
        + if self.json == "" { 0 } else { 1 + sizeof_len((&self.json).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.signature != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.signature))?; }
        if self.json != "" { w.write_with_tag(18, |w| w.write_string(&**&self.json))?; }
        Ok(())
    }
}

