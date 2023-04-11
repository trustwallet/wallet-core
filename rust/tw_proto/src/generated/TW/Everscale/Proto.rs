// Automatically generated rust module for 'Everscale.proto' file

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
pub enum MessageBehavior {
    SimpleTransfer = 0,
    SendAllBalance = 1,
}

impl Default for MessageBehavior {
    fn default() -> Self {
        MessageBehavior::SimpleTransfer
    }
}

impl From<i32> for MessageBehavior {
    fn from(i: i32) -> Self {
        match i {
            0 => MessageBehavior::SimpleTransfer,
            1 => MessageBehavior::SendAllBalance,
            _ => Self::default(),
        }
    }
}

impl<'a> From<&'a str> for MessageBehavior {
    fn from(s: &'a str) -> Self {
        match s {
            "SimpleTransfer" => MessageBehavior::SimpleTransfer,
            "SendAllBalance" => MessageBehavior::SendAllBalance,
            _ => Self::default(),
        }
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Transfer<'a> {
    pub bounce: bool,
    pub behavior: TW::Everscale::Proto::MessageBehavior,
    pub amount: u64,
    pub expired_at: u32,
    pub to: Cow<'a, str>,
    pub account_state_oneof: TW::Everscale::Proto::mod_Transfer::OneOfaccount_state_oneof<'a>,
}

impl<'a> MessageRead<'a> for Transfer<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.bounce = r.read_bool(bytes)?,
                Ok(16) => msg.behavior = r.read_enum(bytes)?,
                Ok(24) => msg.amount = r.read_uint64(bytes)?,
                Ok(32) => msg.expired_at = r.read_uint32(bytes)?,
                Ok(42) => msg.to = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(50) => msg.account_state_oneof = TW::Everscale::Proto::mod_Transfer::OneOfaccount_state_oneof::encoded_contract_data(r.read_string(bytes).map(Cow::Borrowed)?),
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
        + if self.bounce == false { 0 } else { 1 + sizeof_varint(*(&self.bounce) as u64) }
        + if self.behavior == TW::Everscale::Proto::MessageBehavior::SimpleTransfer { 0 } else { 1 + sizeof_varint(*(&self.behavior) as u64) }
        + if self.amount == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
        + if self.expired_at == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.expired_at) as u64) }
        + if self.to == "" { 0 } else { 1 + sizeof_len((&self.to).len()) }
        + match self.account_state_oneof {
            TW::Everscale::Proto::mod_Transfer::OneOfaccount_state_oneof::encoded_contract_data(ref m) => 1 + sizeof_len((m).len()),
            TW::Everscale::Proto::mod_Transfer::OneOfaccount_state_oneof::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.bounce != false { w.write_with_tag(8, |w| w.write_bool(*&self.bounce))?; }
        if self.behavior != TW::Everscale::Proto::MessageBehavior::SimpleTransfer { w.write_with_tag(16, |w| w.write_enum(*&self.behavior as i32))?; }
        if self.amount != 0u64 { w.write_with_tag(24, |w| w.write_uint64(*&self.amount))?; }
        if self.expired_at != 0u32 { w.write_with_tag(32, |w| w.write_uint32(*&self.expired_at))?; }
        if self.to != "" { w.write_with_tag(42, |w| w.write_string(&**&self.to))?; }
        match self.account_state_oneof {            TW::Everscale::Proto::mod_Transfer::OneOfaccount_state_oneof::encoded_contract_data(ref m) => { w.write_with_tag(50, |w| w.write_string(&**m))? },
            TW::Everscale::Proto::mod_Transfer::OneOfaccount_state_oneof::None => {},
    }        Ok(())
    }
}

pub mod mod_Transfer {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfaccount_state_oneof<'a> {
    encoded_contract_data(Cow<'a, str>),
    None,
}

impl<'a> Default for OneOfaccount_state_oneof<'a> {
    fn default() -> Self {
        OneOfaccount_state_oneof::None
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningInput<'a> {
    pub private_key: Cow<'a, [u8]>,
    pub action_oneof: TW::Everscale::Proto::mod_SigningInput::OneOfaction_oneof<'a>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(18) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(10) => msg.action_oneof = TW::Everscale::Proto::mod_SigningInput::OneOfaction_oneof::transfer(r.read_message::<TW::Everscale::Proto::Transfer>(bytes)?),
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
        + match self.action_oneof {
            TW::Everscale::Proto::mod_SigningInput::OneOfaction_oneof::transfer(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Everscale::Proto::mod_SigningInput::OneOfaction_oneof::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.private_key))?; }
        match self.action_oneof {            TW::Everscale::Proto::mod_SigningInput::OneOfaction_oneof::transfer(ref m) => { w.write_with_tag(10, |w| w.write_message(m))? },
            TW::Everscale::Proto::mod_SigningInput::OneOfaction_oneof::None => {},
    }        Ok(())
    }
}

pub mod mod_SigningInput {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfaction_oneof<'a> {
    transfer(TW::Everscale::Proto::Transfer<'a>),
    None,
}

impl<'a> Default for OneOfaction_oneof<'a> {
    fn default() -> Self {
        OneOfaction_oneof::None
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningOutput<'a> {
    pub encoded: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.encoded = r.read_string(bytes).map(Cow::Borrowed)?,
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
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.encoded != "" { w.write_with_tag(10, |w| w.write_string(&**&self.encoded))?; }
        Ok(())
    }
}

