// Automatically generated rust module for 'Oasis.proto' file

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
pub struct TransferMessage<'a> {
    pub to: Cow<'a, str>,
    pub gas_price: u64,
    pub gas_amount: Cow<'a, str>,
    pub amount: Cow<'a, str>,
    pub nonce: u64,
    pub context: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for TransferMessage<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.to = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.gas_price = r.read_uint64(bytes)?,
                Ok(26) => msg.gas_amount = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.amount = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(40) => msg.nonce = r.read_uint64(bytes)?,
                Ok(50) => msg.context = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TransferMessage<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.to == "" { 0 } else { 1 + sizeof_len((&self.to).len()) }
        + if self.gas_price == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.gas_price) as u64) }
        + if self.gas_amount == "" { 0 } else { 1 + sizeof_len((&self.gas_amount).len()) }
        + if self.amount == "" { 0 } else { 1 + sizeof_len((&self.amount).len()) }
        + if self.nonce == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.nonce) as u64) }
        + if self.context == "" { 0 } else { 1 + sizeof_len((&self.context).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.to != "" { w.write_with_tag(10, |w| w.write_string(&**&self.to))?; }
        if self.gas_price != 0u64 { w.write_with_tag(16, |w| w.write_uint64(*&self.gas_price))?; }
        if self.gas_amount != "" { w.write_with_tag(26, |w| w.write_string(&**&self.gas_amount))?; }
        if self.amount != "" { w.write_with_tag(34, |w| w.write_string(&**&self.amount))?; }
        if self.nonce != 0u64 { w.write_with_tag(40, |w| w.write_uint64(*&self.nonce))?; }
        if self.context != "" { w.write_with_tag(50, |w| w.write_string(&**&self.context))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningInput<'a> {
    pub private_key: Cow<'a, [u8]>,
    pub message: TW::Oasis::Proto::mod_SigningInput::OneOfmessage<'a>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.message = TW::Oasis::Proto::mod_SigningInput::OneOfmessage::transfer(r.read_message::<TW::Oasis::Proto::TransferMessage>(bytes)?),
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
        + match self.message {
            TW::Oasis::Proto::mod_SigningInput::OneOfmessage::transfer(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Oasis::Proto::mod_SigningInput::OneOfmessage::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.private_key))?; }
        match self.message {            TW::Oasis::Proto::mod_SigningInput::OneOfmessage::transfer(ref m) => { w.write_with_tag(18, |w| w.write_message(m))? },
            TW::Oasis::Proto::mod_SigningInput::OneOfmessage::None => {},
    }        Ok(())
    }
}

pub mod mod_SigningInput {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfmessage<'a> {
    transfer(TW::Oasis::Proto::TransferMessage<'a>),
    None,
}

impl<'a> Default for OneOfmessage<'a> {
    fn default() -> Self {
        OneOfmessage::None
    }
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

