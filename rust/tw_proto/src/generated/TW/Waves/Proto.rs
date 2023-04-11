// Automatically generated rust module for 'Waves.proto' file

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
    pub amount: i64,
    pub asset: Cow<'a, str>,
    pub fee: i64,
    pub fee_asset: Cow<'a, str>,
    pub to: Cow<'a, str>,
    pub attachment: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for TransferMessage<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.amount = r.read_int64(bytes)?,
                Ok(18) => msg.asset = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.fee = r.read_int64(bytes)?,
                Ok(34) => msg.fee_asset = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.to = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(50) => msg.attachment = r.read_bytes(bytes).map(Cow::Borrowed)?,
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
        + if self.amount == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
        + if self.asset == "" { 0 } else { 1 + sizeof_len((&self.asset).len()) }
        + if self.fee == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.fee) as u64) }
        + if self.fee_asset == "" { 0 } else { 1 + sizeof_len((&self.fee_asset).len()) }
        + if self.to == "" { 0 } else { 1 + sizeof_len((&self.to).len()) }
        + if self.attachment == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.attachment).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.amount != 0i64 { w.write_with_tag(8, |w| w.write_int64(*&self.amount))?; }
        if self.asset != "" { w.write_with_tag(18, |w| w.write_string(&**&self.asset))?; }
        if self.fee != 0i64 { w.write_with_tag(24, |w| w.write_int64(*&self.fee))?; }
        if self.fee_asset != "" { w.write_with_tag(34, |w| w.write_string(&**&self.fee_asset))?; }
        if self.to != "" { w.write_with_tag(42, |w| w.write_string(&**&self.to))?; }
        if self.attachment != Cow::Borrowed(b"") { w.write_with_tag(50, |w| w.write_bytes(&**&self.attachment))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct LeaseMessage<'a> {
    pub amount: i64,
    pub to: Cow<'a, str>,
    pub fee: i64,
}

impl<'a> MessageRead<'a> for LeaseMessage<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.amount = r.read_int64(bytes)?,
                Ok(18) => msg.to = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.fee = r.read_int64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for LeaseMessage<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.amount == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
        + if self.to == "" { 0 } else { 1 + sizeof_len((&self.to).len()) }
        + if self.fee == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.fee) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.amount != 0i64 { w.write_with_tag(8, |w| w.write_int64(*&self.amount))?; }
        if self.to != "" { w.write_with_tag(18, |w| w.write_string(&**&self.to))?; }
        if self.fee != 0i64 { w.write_with_tag(24, |w| w.write_int64(*&self.fee))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct CancelLeaseMessage<'a> {
    pub lease_id: Cow<'a, str>,
    pub fee: i64,
}

impl<'a> MessageRead<'a> for CancelLeaseMessage<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.lease_id = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.fee = r.read_int64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for CancelLeaseMessage<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.lease_id == "" { 0 } else { 1 + sizeof_len((&self.lease_id).len()) }
        + if self.fee == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.fee) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.lease_id != "" { w.write_with_tag(10, |w| w.write_string(&**&self.lease_id))?; }
        if self.fee != 0i64 { w.write_with_tag(16, |w| w.write_int64(*&self.fee))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningInput<'a> {
    pub timestamp: i64,
    pub private_key: Cow<'a, [u8]>,
    pub message_oneof: TW::Waves::Proto::mod_SigningInput::OneOfmessage_oneof<'a>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.timestamp = r.read_int64(bytes)?,
                Ok(18) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.message_oneof = TW::Waves::Proto::mod_SigningInput::OneOfmessage_oneof::transfer_message(r.read_message::<TW::Waves::Proto::TransferMessage>(bytes)?),
                Ok(34) => msg.message_oneof = TW::Waves::Proto::mod_SigningInput::OneOfmessage_oneof::lease_message(r.read_message::<TW::Waves::Proto::LeaseMessage>(bytes)?),
                Ok(42) => msg.message_oneof = TW::Waves::Proto::mod_SigningInput::OneOfmessage_oneof::cancel_lease_message(r.read_message::<TW::Waves::Proto::CancelLeaseMessage>(bytes)?),
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
        + if self.timestamp == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.timestamp) as u64) }
        + if self.private_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.private_key).len()) }
        + match self.message_oneof {
            TW::Waves::Proto::mod_SigningInput::OneOfmessage_oneof::transfer_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Waves::Proto::mod_SigningInput::OneOfmessage_oneof::lease_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Waves::Proto::mod_SigningInput::OneOfmessage_oneof::cancel_lease_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Waves::Proto::mod_SigningInput::OneOfmessage_oneof::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.timestamp != 0i64 { w.write_with_tag(8, |w| w.write_int64(*&self.timestamp))?; }
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.private_key))?; }
        match self.message_oneof {            TW::Waves::Proto::mod_SigningInput::OneOfmessage_oneof::transfer_message(ref m) => { w.write_with_tag(26, |w| w.write_message(m))? },
            TW::Waves::Proto::mod_SigningInput::OneOfmessage_oneof::lease_message(ref m) => { w.write_with_tag(34, |w| w.write_message(m))? },
            TW::Waves::Proto::mod_SigningInput::OneOfmessage_oneof::cancel_lease_message(ref m) => { w.write_with_tag(42, |w| w.write_message(m))? },
            TW::Waves::Proto::mod_SigningInput::OneOfmessage_oneof::None => {},
    }        Ok(())
    }
}

pub mod mod_SigningInput {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfmessage_oneof<'a> {
    transfer_message(TW::Waves::Proto::TransferMessage<'a>),
    lease_message(TW::Waves::Proto::LeaseMessage<'a>),
    cancel_lease_message(TW::Waves::Proto::CancelLeaseMessage<'a>),
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

