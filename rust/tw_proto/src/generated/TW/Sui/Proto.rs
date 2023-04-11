// Automatically generated rust module for 'Sui.proto' file

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
pub struct SignDirect<'a> {
    pub unsigned_tx_msg: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for SignDirect<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.unsigned_tx_msg = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for SignDirect<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.unsigned_tx_msg == "" { 0 } else { 1 + sizeof_len((&self.unsigned_tx_msg).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.unsigned_tx_msg != "" { w.write_with_tag(10, |w| w.write_string(&**&self.unsigned_tx_msg))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningInput<'a> {
    pub private_key: Cow<'a, [u8]>,
    pub transaction_payload: TW::Sui::Proto::mod_SigningInput::OneOftransaction_payload<'a>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.transaction_payload = TW::Sui::Proto::mod_SigningInput::OneOftransaction_payload::sign_direct_message(r.read_message::<TW::Sui::Proto::SignDirect>(bytes)?),
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
        + match self.transaction_payload {
            TW::Sui::Proto::mod_SigningInput::OneOftransaction_payload::sign_direct_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Sui::Proto::mod_SigningInput::OneOftransaction_payload::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.private_key))?; }
        match self.transaction_payload {            TW::Sui::Proto::mod_SigningInput::OneOftransaction_payload::sign_direct_message(ref m) => { w.write_with_tag(18, |w| w.write_message(m))? },
            TW::Sui::Proto::mod_SigningInput::OneOftransaction_payload::None => {},
    }        Ok(())
    }
}

pub mod mod_SigningInput {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOftransaction_payload<'a> {
    sign_direct_message(TW::Sui::Proto::SignDirect<'a>),
    None,
}

impl<'a> Default for OneOftransaction_payload<'a> {
    fn default() -> Self {
        OneOftransaction_payload::None
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningOutput<'a> {
    pub unsigned_tx: Cow<'a, str>,
    pub signature: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.unsigned_tx = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.signature = r.read_string(bytes).map(Cow::Borrowed)?,
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
        + if self.unsigned_tx == "" { 0 } else { 1 + sizeof_len((&self.unsigned_tx).len()) }
        + if self.signature == "" { 0 } else { 1 + sizeof_len((&self.signature).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.unsigned_tx != "" { w.write_with_tag(10, |w| w.write_string(&**&self.unsigned_tx))?; }
        if self.signature != "" { w.write_with_tag(18, |w| w.write_string(&**&self.signature))?; }
        Ok(())
    }
}

