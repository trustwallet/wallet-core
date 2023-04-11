// Automatically generated rust module for 'TheOpenNetwork.proto' file

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
pub enum WalletVersion {
    WALLET_V3_R1 = 0,
    WALLET_V3_R2 = 1,
    WALLET_V4_R2 = 2,
}

impl Default for WalletVersion {
    fn default() -> Self {
        WalletVersion::WALLET_V3_R1
    }
}

impl From<i32> for WalletVersion {
    fn from(i: i32) -> Self {
        match i {
            0 => WalletVersion::WALLET_V3_R1,
            1 => WalletVersion::WALLET_V3_R2,
            2 => WalletVersion::WALLET_V4_R2,
            _ => Self::default(),
        }
    }
}

impl<'a> From<&'a str> for WalletVersion {
    fn from(s: &'a str) -> Self {
        match s {
            "WALLET_V3_R1" => WalletVersion::WALLET_V3_R1,
            "WALLET_V3_R2" => WalletVersion::WALLET_V3_R2,
            "WALLET_V4_R2" => WalletVersion::WALLET_V4_R2,
            _ => Self::default(),
        }
    }
}

#[derive(Debug, PartialEq, Eq, Clone, Copy)]
pub enum SendMode {
    DEFAULT = 0,
    PAY_FEES_SEPARATELY = 1,
    IGNORE_ACTION_PHASE_ERRORS = 2,
    DESTROY_ON_ZERO_BALANCE = 32,
    ATTACH_ALL_INBOUND_MESSAGE_VALUE = 64,
    ATTACH_ALL_CONTRACT_BALANCE = 128,
}

impl Default for SendMode {
    fn default() -> Self {
        SendMode::DEFAULT
    }
}

impl From<i32> for SendMode {
    fn from(i: i32) -> Self {
        match i {
            0 => SendMode::DEFAULT,
            1 => SendMode::PAY_FEES_SEPARATELY,
            2 => SendMode::IGNORE_ACTION_PHASE_ERRORS,
            32 => SendMode::DESTROY_ON_ZERO_BALANCE,
            64 => SendMode::ATTACH_ALL_INBOUND_MESSAGE_VALUE,
            128 => SendMode::ATTACH_ALL_CONTRACT_BALANCE,
            _ => Self::default(),
        }
    }
}

impl<'a> From<&'a str> for SendMode {
    fn from(s: &'a str) -> Self {
        match s {
            "DEFAULT" => SendMode::DEFAULT,
            "PAY_FEES_SEPARATELY" => SendMode::PAY_FEES_SEPARATELY,
            "IGNORE_ACTION_PHASE_ERRORS" => SendMode::IGNORE_ACTION_PHASE_ERRORS,
            "DESTROY_ON_ZERO_BALANCE" => SendMode::DESTROY_ON_ZERO_BALANCE,
            "ATTACH_ALL_INBOUND_MESSAGE_VALUE" => SendMode::ATTACH_ALL_INBOUND_MESSAGE_VALUE,
            "ATTACH_ALL_CONTRACT_BALANCE" => SendMode::ATTACH_ALL_CONTRACT_BALANCE,
            _ => Self::default(),
        }
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Transfer<'a> {
    pub wallet_version: TW::TheOpenNetwork::Proto::WalletVersion,
    pub dest: Cow<'a, str>,
    pub amount: u64,
    pub sequence_number: u32,
    pub mode: u32,
    pub expire_at: u32,
    pub comment: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for Transfer<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.wallet_version = r.read_enum(bytes)?,
                Ok(18) => msg.dest = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.amount = r.read_uint64(bytes)?,
                Ok(32) => msg.sequence_number = r.read_uint32(bytes)?,
                Ok(40) => msg.mode = r.read_uint32(bytes)?,
                Ok(48) => msg.expire_at = r.read_uint32(bytes)?,
                Ok(58) => msg.comment = r.read_string(bytes).map(Cow::Borrowed)?,
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
        + if self.wallet_version == TW::TheOpenNetwork::Proto::WalletVersion::WALLET_V3_R1 { 0 } else { 1 + sizeof_varint(*(&self.wallet_version) as u64) }
        + if self.dest == "" { 0 } else { 1 + sizeof_len((&self.dest).len()) }
        + if self.amount == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
        + if self.sequence_number == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.sequence_number) as u64) }
        + if self.mode == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.mode) as u64) }
        + if self.expire_at == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.expire_at) as u64) }
        + if self.comment == "" { 0 } else { 1 + sizeof_len((&self.comment).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.wallet_version != TW::TheOpenNetwork::Proto::WalletVersion::WALLET_V3_R1 { w.write_with_tag(8, |w| w.write_enum(*&self.wallet_version as i32))?; }
        if self.dest != "" { w.write_with_tag(18, |w| w.write_string(&**&self.dest))?; }
        if self.amount != 0u64 { w.write_with_tag(24, |w| w.write_uint64(*&self.amount))?; }
        if self.sequence_number != 0u32 { w.write_with_tag(32, |w| w.write_uint32(*&self.sequence_number))?; }
        if self.mode != 0u32 { w.write_with_tag(40, |w| w.write_uint32(*&self.mode))?; }
        if self.expire_at != 0u32 { w.write_with_tag(48, |w| w.write_uint32(*&self.expire_at))?; }
        if self.comment != "" { w.write_with_tag(58, |w| w.write_string(&**&self.comment))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningInput<'a> {
    pub private_key: Cow<'a, [u8]>,
    pub action_oneof: TW::TheOpenNetwork::Proto::mod_SigningInput::OneOfaction_oneof<'a>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.action_oneof = TW::TheOpenNetwork::Proto::mod_SigningInput::OneOfaction_oneof::transfer(r.read_message::<TW::TheOpenNetwork::Proto::Transfer>(bytes)?),
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
            TW::TheOpenNetwork::Proto::mod_SigningInput::OneOfaction_oneof::transfer(ref m) => 1 + sizeof_len((m).get_size()),
            TW::TheOpenNetwork::Proto::mod_SigningInput::OneOfaction_oneof::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.private_key))?; }
        match self.action_oneof {            TW::TheOpenNetwork::Proto::mod_SigningInput::OneOfaction_oneof::transfer(ref m) => { w.write_with_tag(18, |w| w.write_message(m))? },
            TW::TheOpenNetwork::Proto::mod_SigningInput::OneOfaction_oneof::None => {},
    }        Ok(())
    }
}

pub mod mod_SigningInput {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfaction_oneof<'a> {
    transfer(TW::TheOpenNetwork::Proto::Transfer<'a>),
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
    pub error: TW::Common::Proto::SigningError,
    pub error_message: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.encoded = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.error = r.read_enum(bytes)?,
                Ok(26) => msg.error_message = r.read_string(bytes).map(Cow::Borrowed)?,
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
        + if self.error == TW::Common::Proto::SigningError::OK { 0 } else { 1 + sizeof_varint(*(&self.error) as u64) }
        + if self.error_message == "" { 0 } else { 1 + sizeof_len((&self.error_message).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.encoded != "" { w.write_with_tag(10, |w| w.write_string(&**&self.encoded))?; }
        if self.error != TW::Common::Proto::SigningError::OK { w.write_with_tag(16, |w| w.write_enum(*&self.error as i32))?; }
        if self.error_message != "" { w.write_with_tag(26, |w| w.write_string(&**&self.error_message))?; }
        Ok(())
    }
}

