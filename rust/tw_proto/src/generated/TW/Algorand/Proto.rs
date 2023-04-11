// Automatically generated rust module for 'Algorand.proto' file

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
pub struct Transfer<'a> {
    pub to_address: Cow<'a, str>,
    pub amount: u64,
}

impl<'a> MessageRead<'a> for Transfer<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.to_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.amount = r.read_uint64(bytes)?,
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
        + if self.to_address == "" { 0 } else { 1 + sizeof_len((&self.to_address).len()) }
        + if self.amount == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.to_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.to_address))?; }
        if self.amount != 0u64 { w.write_with_tag(16, |w| w.write_uint64(*&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct AssetTransfer<'a> {
    pub to_address: Cow<'a, str>,
    pub amount: u64,
    pub asset_id: u64,
}

impl<'a> MessageRead<'a> for AssetTransfer<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.to_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.amount = r.read_uint64(bytes)?,
                Ok(24) => msg.asset_id = r.read_uint64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for AssetTransfer<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.to_address == "" { 0 } else { 1 + sizeof_len((&self.to_address).len()) }
        + if self.amount == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
        + if self.asset_id == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.asset_id) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.to_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.to_address))?; }
        if self.amount != 0u64 { w.write_with_tag(16, |w| w.write_uint64(*&self.amount))?; }
        if self.asset_id != 0u64 { w.write_with_tag(24, |w| w.write_uint64(*&self.asset_id))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct AssetOptIn {
    pub asset_id: u64,
}

impl<'a> MessageRead<'a> for AssetOptIn {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.asset_id = r.read_uint64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl MessageWrite for AssetOptIn {
    fn get_size(&self) -> usize {
        0
        + if self.asset_id == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.asset_id) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.asset_id != 0u64 { w.write_with_tag(8, |w| w.write_uint64(*&self.asset_id))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningInput<'a> {
    pub genesis_id: Cow<'a, str>,
    pub genesis_hash: Cow<'a, [u8]>,
    pub note: Cow<'a, [u8]>,
    pub private_key: Cow<'a, [u8]>,
    pub first_round: u64,
    pub last_round: u64,
    pub fee: u64,
    pub message_oneof: TW::Algorand::Proto::mod_SigningInput::OneOfmessage_oneof<'a>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.genesis_id = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.genesis_hash = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.note = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(40) => msg.first_round = r.read_uint64(bytes)?,
                Ok(48) => msg.last_round = r.read_uint64(bytes)?,
                Ok(56) => msg.fee = r.read_uint64(bytes)?,
                Ok(82) => msg.message_oneof = TW::Algorand::Proto::mod_SigningInput::OneOfmessage_oneof::transfer(r.read_message::<TW::Algorand::Proto::Transfer>(bytes)?),
                Ok(90) => msg.message_oneof = TW::Algorand::Proto::mod_SigningInput::OneOfmessage_oneof::asset_transfer(r.read_message::<TW::Algorand::Proto::AssetTransfer>(bytes)?),
                Ok(98) => msg.message_oneof = TW::Algorand::Proto::mod_SigningInput::OneOfmessage_oneof::asset_opt_in(r.read_message::<TW::Algorand::Proto::AssetOptIn>(bytes)?),
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
        + if self.genesis_id == "" { 0 } else { 1 + sizeof_len((&self.genesis_id).len()) }
        + if self.genesis_hash == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.genesis_hash).len()) }
        + if self.note == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.note).len()) }
        + if self.private_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.private_key).len()) }
        + if self.first_round == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.first_round) as u64) }
        + if self.last_round == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.last_round) as u64) }
        + if self.fee == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.fee) as u64) }
        + match self.message_oneof {
            TW::Algorand::Proto::mod_SigningInput::OneOfmessage_oneof::transfer(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Algorand::Proto::mod_SigningInput::OneOfmessage_oneof::asset_transfer(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Algorand::Proto::mod_SigningInput::OneOfmessage_oneof::asset_opt_in(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Algorand::Proto::mod_SigningInput::OneOfmessage_oneof::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.genesis_id != "" { w.write_with_tag(10, |w| w.write_string(&**&self.genesis_id))?; }
        if self.genesis_hash != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.genesis_hash))?; }
        if self.note != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.note))?; }
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(34, |w| w.write_bytes(&**&self.private_key))?; }
        if self.first_round != 0u64 { w.write_with_tag(40, |w| w.write_uint64(*&self.first_round))?; }
        if self.last_round != 0u64 { w.write_with_tag(48, |w| w.write_uint64(*&self.last_round))?; }
        if self.fee != 0u64 { w.write_with_tag(56, |w| w.write_uint64(*&self.fee))?; }
        match self.message_oneof {            TW::Algorand::Proto::mod_SigningInput::OneOfmessage_oneof::transfer(ref m) => { w.write_with_tag(82, |w| w.write_message(m))? },
            TW::Algorand::Proto::mod_SigningInput::OneOfmessage_oneof::asset_transfer(ref m) => { w.write_with_tag(90, |w| w.write_message(m))? },
            TW::Algorand::Proto::mod_SigningInput::OneOfmessage_oneof::asset_opt_in(ref m) => { w.write_with_tag(98, |w| w.write_message(m))? },
            TW::Algorand::Proto::mod_SigningInput::OneOfmessage_oneof::None => {},
    }        Ok(())
    }
}

pub mod mod_SigningInput {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfmessage_oneof<'a> {
    transfer(TW::Algorand::Proto::Transfer<'a>),
    asset_transfer(TW::Algorand::Proto::AssetTransfer<'a>),
    asset_opt_in(TW::Algorand::Proto::AssetOptIn),
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
    pub encoded: Cow<'a, [u8]>,
    pub signature: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.encoded = r.read_bytes(bytes).map(Cow::Borrowed)?,
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
        + if self.encoded == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.encoded).len()) }
        + if self.signature == "" { 0 } else { 1 + sizeof_len((&self.signature).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.encoded != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.encoded))?; }
        if self.signature != "" { w.write_with_tag(18, |w| w.write_string(&**&self.signature))?; }
        Ok(())
    }
}

