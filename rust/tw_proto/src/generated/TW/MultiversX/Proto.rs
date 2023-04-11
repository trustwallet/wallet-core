// Automatically generated rust module for 'MultiversX.proto' file

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
pub struct GenericAction<'a> {
    pub accounts: Option<TW::MultiversX::Proto::Accounts<'a>>,
    pub value: Cow<'a, str>,
    pub data: Cow<'a, str>,
    pub version: u32,
    pub options: u32,
}

impl<'a> MessageRead<'a> for GenericAction<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.accounts = Some(r.read_message::<TW::MultiversX::Proto::Accounts>(bytes)?),
                Ok(18) => msg.value = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.data = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(32) => msg.version = r.read_uint32(bytes)?,
                Ok(40) => msg.options = r.read_uint32(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for GenericAction<'a> {
    fn get_size(&self) -> usize {
        0
        + self.accounts.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.value == "" { 0 } else { 1 + sizeof_len((&self.value).len()) }
        + if self.data == "" { 0 } else { 1 + sizeof_len((&self.data).len()) }
        + if self.version == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.version) as u64) }
        + if self.options == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.options) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.accounts { w.write_with_tag(10, |w| w.write_message(s))?; }
        if self.value != "" { w.write_with_tag(18, |w| w.write_string(&**&self.value))?; }
        if self.data != "" { w.write_with_tag(26, |w| w.write_string(&**&self.data))?; }
        if self.version != 0u32 { w.write_with_tag(32, |w| w.write_uint32(*&self.version))?; }
        if self.options != 0u32 { w.write_with_tag(40, |w| w.write_uint32(*&self.options))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct EGLDTransfer<'a> {
    pub accounts: Option<TW::MultiversX::Proto::Accounts<'a>>,
    pub amount: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for EGLDTransfer<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.accounts = Some(r.read_message::<TW::MultiversX::Proto::Accounts>(bytes)?),
                Ok(18) => msg.amount = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for EGLDTransfer<'a> {
    fn get_size(&self) -> usize {
        0
        + self.accounts.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.amount == "" { 0 } else { 1 + sizeof_len((&self.amount).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.accounts { w.write_with_tag(10, |w| w.write_message(s))?; }
        if self.amount != "" { w.write_with_tag(18, |w| w.write_string(&**&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct ESDTTransfer<'a> {
    pub accounts: Option<TW::MultiversX::Proto::Accounts<'a>>,
    pub token_identifier: Cow<'a, str>,
    pub amount: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for ESDTTransfer<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.accounts = Some(r.read_message::<TW::MultiversX::Proto::Accounts>(bytes)?),
                Ok(18) => msg.token_identifier = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.amount = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for ESDTTransfer<'a> {
    fn get_size(&self) -> usize {
        0
        + self.accounts.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.token_identifier == "" { 0 } else { 1 + sizeof_len((&self.token_identifier).len()) }
        + if self.amount == "" { 0 } else { 1 + sizeof_len((&self.amount).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.accounts { w.write_with_tag(10, |w| w.write_message(s))?; }
        if self.token_identifier != "" { w.write_with_tag(18, |w| w.write_string(&**&self.token_identifier))?; }
        if self.amount != "" { w.write_with_tag(26, |w| w.write_string(&**&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct ESDTNFTTransfer<'a> {
    pub accounts: Option<TW::MultiversX::Proto::Accounts<'a>>,
    pub token_collection: Cow<'a, str>,
    pub token_nonce: u64,
    pub amount: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for ESDTNFTTransfer<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.accounts = Some(r.read_message::<TW::MultiversX::Proto::Accounts>(bytes)?),
                Ok(18) => msg.token_collection = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.token_nonce = r.read_uint64(bytes)?,
                Ok(34) => msg.amount = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for ESDTNFTTransfer<'a> {
    fn get_size(&self) -> usize {
        0
        + self.accounts.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.token_collection == "" { 0 } else { 1 + sizeof_len((&self.token_collection).len()) }
        + if self.token_nonce == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.token_nonce) as u64) }
        + if self.amount == "" { 0 } else { 1 + sizeof_len((&self.amount).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.accounts { w.write_with_tag(10, |w| w.write_message(s))?; }
        if self.token_collection != "" { w.write_with_tag(18, |w| w.write_string(&**&self.token_collection))?; }
        if self.token_nonce != 0u64 { w.write_with_tag(24, |w| w.write_uint64(*&self.token_nonce))?; }
        if self.amount != "" { w.write_with_tag(34, |w| w.write_string(&**&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Accounts<'a> {
    pub sender_nonce: u64,
    pub sender: Cow<'a, str>,
    pub sender_username: Cow<'a, str>,
    pub receiver: Cow<'a, str>,
    pub receiver_username: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for Accounts<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.sender_nonce = r.read_uint64(bytes)?,
                Ok(18) => msg.sender = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.sender_username = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.receiver = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.receiver_username = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Accounts<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.sender_nonce == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.sender_nonce) as u64) }
        + if self.sender == "" { 0 } else { 1 + sizeof_len((&self.sender).len()) }
        + if self.sender_username == "" { 0 } else { 1 + sizeof_len((&self.sender_username).len()) }
        + if self.receiver == "" { 0 } else { 1 + sizeof_len((&self.receiver).len()) }
        + if self.receiver_username == "" { 0 } else { 1 + sizeof_len((&self.receiver_username).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.sender_nonce != 0u64 { w.write_with_tag(8, |w| w.write_uint64(*&self.sender_nonce))?; }
        if self.sender != "" { w.write_with_tag(18, |w| w.write_string(&**&self.sender))?; }
        if self.sender_username != "" { w.write_with_tag(26, |w| w.write_string(&**&self.sender_username))?; }
        if self.receiver != "" { w.write_with_tag(34, |w| w.write_string(&**&self.receiver))?; }
        if self.receiver_username != "" { w.write_with_tag(42, |w| w.write_string(&**&self.receiver_username))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningInput<'a> {
    pub private_key: Cow<'a, [u8]>,
    pub chain_id: Cow<'a, str>,
    pub gas_price: u64,
    pub gas_limit: u64,
    pub message_oneof: TW::MultiversX::Proto::mod_SigningInput::OneOfmessage_oneof<'a>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.chain_id = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.gas_price = r.read_uint64(bytes)?,
                Ok(32) => msg.gas_limit = r.read_uint64(bytes)?,
                Ok(42) => msg.message_oneof = TW::MultiversX::Proto::mod_SigningInput::OneOfmessage_oneof::generic_action(r.read_message::<TW::MultiversX::Proto::GenericAction>(bytes)?),
                Ok(50) => msg.message_oneof = TW::MultiversX::Proto::mod_SigningInput::OneOfmessage_oneof::egld_transfer(r.read_message::<TW::MultiversX::Proto::EGLDTransfer>(bytes)?),
                Ok(58) => msg.message_oneof = TW::MultiversX::Proto::mod_SigningInput::OneOfmessage_oneof::esdt_transfer(r.read_message::<TW::MultiversX::Proto::ESDTTransfer>(bytes)?),
                Ok(66) => msg.message_oneof = TW::MultiversX::Proto::mod_SigningInput::OneOfmessage_oneof::esdtnft_transfer(r.read_message::<TW::MultiversX::Proto::ESDTNFTTransfer>(bytes)?),
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
        + if self.chain_id == "" { 0 } else { 1 + sizeof_len((&self.chain_id).len()) }
        + if self.gas_price == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.gas_price) as u64) }
        + if self.gas_limit == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.gas_limit) as u64) }
        + match self.message_oneof {
            TW::MultiversX::Proto::mod_SigningInput::OneOfmessage_oneof::generic_action(ref m) => 1 + sizeof_len((m).get_size()),
            TW::MultiversX::Proto::mod_SigningInput::OneOfmessage_oneof::egld_transfer(ref m) => 1 + sizeof_len((m).get_size()),
            TW::MultiversX::Proto::mod_SigningInput::OneOfmessage_oneof::esdt_transfer(ref m) => 1 + sizeof_len((m).get_size()),
            TW::MultiversX::Proto::mod_SigningInput::OneOfmessage_oneof::esdtnft_transfer(ref m) => 1 + sizeof_len((m).get_size()),
            TW::MultiversX::Proto::mod_SigningInput::OneOfmessage_oneof::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.private_key))?; }
        if self.chain_id != "" { w.write_with_tag(18, |w| w.write_string(&**&self.chain_id))?; }
        if self.gas_price != 0u64 { w.write_with_tag(24, |w| w.write_uint64(*&self.gas_price))?; }
        if self.gas_limit != 0u64 { w.write_with_tag(32, |w| w.write_uint64(*&self.gas_limit))?; }
        match self.message_oneof {            TW::MultiversX::Proto::mod_SigningInput::OneOfmessage_oneof::generic_action(ref m) => { w.write_with_tag(42, |w| w.write_message(m))? },
            TW::MultiversX::Proto::mod_SigningInput::OneOfmessage_oneof::egld_transfer(ref m) => { w.write_with_tag(50, |w| w.write_message(m))? },
            TW::MultiversX::Proto::mod_SigningInput::OneOfmessage_oneof::esdt_transfer(ref m) => { w.write_with_tag(58, |w| w.write_message(m))? },
            TW::MultiversX::Proto::mod_SigningInput::OneOfmessage_oneof::esdtnft_transfer(ref m) => { w.write_with_tag(66, |w| w.write_message(m))? },
            TW::MultiversX::Proto::mod_SigningInput::OneOfmessage_oneof::None => {},
    }        Ok(())
    }
}

pub mod mod_SigningInput {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfmessage_oneof<'a> {
    generic_action(TW::MultiversX::Proto::GenericAction<'a>),
    egld_transfer(TW::MultiversX::Proto::EGLDTransfer<'a>),
    esdt_transfer(TW::MultiversX::Proto::ESDTTransfer<'a>),
    esdtnft_transfer(TW::MultiversX::Proto::ESDTNFTTransfer<'a>),
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
pub struct SigningOutput<'a> {
    pub encoded: Cow<'a, str>,
    pub signature: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.encoded = r.read_string(bytes).map(Cow::Borrowed)?,
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
        + if self.encoded == "" { 0 } else { 1 + sizeof_len((&self.encoded).len()) }
        + if self.signature == "" { 0 } else { 1 + sizeof_len((&self.signature).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.encoded != "" { w.write_with_tag(10, |w| w.write_string(&**&self.encoded))?; }
        if self.signature != "" { w.write_with_tag(18, |w| w.write_string(&**&self.signature))?; }
        Ok(())
    }
}

