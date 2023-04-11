// Automatically generated rust module for 'Nano.proto' file

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
    pub private_key: Cow<'a, [u8]>,
    pub parent_block: Cow<'a, [u8]>,
    pub representative: Cow<'a, str>,
    pub balance: Cow<'a, str>,
    pub work: Cow<'a, str>,
    pub link_oneof: TW::Nano::Proto::mod_SigningInput::OneOflink_oneof<'a>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.parent_block = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.representative = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(50) => msg.balance = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(58) => msg.work = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.link_oneof = TW::Nano::Proto::mod_SigningInput::OneOflink_oneof::link_block(r.read_bytes(bytes).map(Cow::Borrowed)?),
                Ok(34) => msg.link_oneof = TW::Nano::Proto::mod_SigningInput::OneOflink_oneof::link_recipient(r.read_string(bytes).map(Cow::Borrowed)?),
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
        + if self.parent_block == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.parent_block).len()) }
        + if self.representative == "" { 0 } else { 1 + sizeof_len((&self.representative).len()) }
        + if self.balance == "" { 0 } else { 1 + sizeof_len((&self.balance).len()) }
        + if self.work == "" { 0 } else { 1 + sizeof_len((&self.work).len()) }
        + match self.link_oneof {
            TW::Nano::Proto::mod_SigningInput::OneOflink_oneof::link_block(ref m) => 1 + sizeof_len((m).len()),
            TW::Nano::Proto::mod_SigningInput::OneOflink_oneof::link_recipient(ref m) => 1 + sizeof_len((m).len()),
            TW::Nano::Proto::mod_SigningInput::OneOflink_oneof::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.private_key))?; }
        if self.parent_block != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.parent_block))?; }
        if self.representative != "" { w.write_with_tag(42, |w| w.write_string(&**&self.representative))?; }
        if self.balance != "" { w.write_with_tag(50, |w| w.write_string(&**&self.balance))?; }
        if self.work != "" { w.write_with_tag(58, |w| w.write_string(&**&self.work))?; }
        match self.link_oneof {            TW::Nano::Proto::mod_SigningInput::OneOflink_oneof::link_block(ref m) => { w.write_with_tag(26, |w| w.write_bytes(&**m))? },
            TW::Nano::Proto::mod_SigningInput::OneOflink_oneof::link_recipient(ref m) => { w.write_with_tag(34, |w| w.write_string(&**m))? },
            TW::Nano::Proto::mod_SigningInput::OneOflink_oneof::None => {},
    }        Ok(())
    }
}

pub mod mod_SigningInput {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOflink_oneof<'a> {
    link_block(Cow<'a, [u8]>),
    link_recipient(Cow<'a, str>),
    None,
}

impl<'a> Default for OneOflink_oneof<'a> {
    fn default() -> Self {
        OneOflink_oneof::None
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningOutput<'a> {
    pub signature: Cow<'a, [u8]>,
    pub block_hash: Cow<'a, [u8]>,
    pub json: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.signature = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.block_hash = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.json = r.read_string(bytes).map(Cow::Borrowed)?,
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
        + if self.block_hash == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.block_hash).len()) }
        + if self.json == "" { 0 } else { 1 + sizeof_len((&self.json).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.signature != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.signature))?; }
        if self.block_hash != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.block_hash))?; }
        if self.json != "" { w.write_with_tag(26, |w| w.write_string(&**&self.json))?; }
        Ok(())
    }
}

