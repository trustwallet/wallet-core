// Automatically generated rust module for 'Hedera.proto' file

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
pub struct Timestamp {
    pub seconds: i64,
    pub nanos: i32,
}

impl<'a> MessageRead<'a> for Timestamp {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.seconds = r.read_int64(bytes)?,
                Ok(16) => msg.nanos = r.read_int32(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl MessageWrite for Timestamp {
    fn get_size(&self) -> usize {
        0
        + if self.seconds == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.seconds) as u64) }
        + if self.nanos == 0i32 { 0 } else { 1 + sizeof_varint(*(&self.nanos) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.seconds != 0i64 { w.write_with_tag(8, |w| w.write_int64(*&self.seconds))?; }
        if self.nanos != 0i32 { w.write_with_tag(16, |w| w.write_int32(*&self.nanos))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TransactionID<'a> {
    pub transactionValidStart: Option<TW::Hedera::Proto::Timestamp>,
    pub accountID: Cow<'a, str>,
    pub scheduled: bool,
    pub nonce: i32,
}

impl<'a> MessageRead<'a> for TransactionID<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.transactionValidStart = Some(r.read_message::<TW::Hedera::Proto::Timestamp>(bytes)?),
                Ok(18) => msg.accountID = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.scheduled = r.read_bool(bytes)?,
                Ok(32) => msg.nonce = r.read_int32(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TransactionID<'a> {
    fn get_size(&self) -> usize {
        0
        + self.transactionValidStart.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.accountID == "" { 0 } else { 1 + sizeof_len((&self.accountID).len()) }
        + if self.scheduled == false { 0 } else { 1 + sizeof_varint(*(&self.scheduled) as u64) }
        + if self.nonce == 0i32 { 0 } else { 1 + sizeof_varint(*(&self.nonce) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.transactionValidStart { w.write_with_tag(10, |w| w.write_message(s))?; }
        if self.accountID != "" { w.write_with_tag(18, |w| w.write_string(&**&self.accountID))?; }
        if self.scheduled != false { w.write_with_tag(24, |w| w.write_bool(*&self.scheduled))?; }
        if self.nonce != 0i32 { w.write_with_tag(32, |w| w.write_int32(*&self.nonce))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TransferMessage<'a> {
    pub from: Cow<'a, str>,
    pub to: Cow<'a, str>,
    pub amount: i64,
}

impl<'a> MessageRead<'a> for TransferMessage<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.from = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.to = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.amount = r.read_sint64(bytes)?,
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
        + if self.from == "" { 0 } else { 1 + sizeof_len((&self.from).len()) }
        + if self.to == "" { 0 } else { 1 + sizeof_len((&self.to).len()) }
        + if self.amount == 0i64 { 0 } else { 1 + sizeof_sint64(*(&self.amount)) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.from != "" { w.write_with_tag(10, |w| w.write_string(&**&self.from))?; }
        if self.to != "" { w.write_with_tag(18, |w| w.write_string(&**&self.to))?; }
        if self.amount != 0i64 { w.write_with_tag(24, |w| w.write_sint64(*&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TransactionBody<'a> {
    pub transactionID: Option<TW::Hedera::Proto::TransactionID<'a>>,
    pub nodeAccountID: Cow<'a, str>,
    pub transactionFee: u64,
    pub transactionValidDuration: i64,
    pub memo: Cow<'a, str>,
    pub data: TW::Hedera::Proto::mod_TransactionBody::OneOfdata<'a>,
}

impl<'a> MessageRead<'a> for TransactionBody<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.transactionID = Some(r.read_message::<TW::Hedera::Proto::TransactionID>(bytes)?),
                Ok(18) => msg.nodeAccountID = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.transactionFee = r.read_uint64(bytes)?,
                Ok(32) => msg.transactionValidDuration = r.read_int64(bytes)?,
                Ok(42) => msg.memo = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(50) => msg.data = TW::Hedera::Proto::mod_TransactionBody::OneOfdata::transfer(r.read_message::<TW::Hedera::Proto::TransferMessage>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TransactionBody<'a> {
    fn get_size(&self) -> usize {
        0
        + self.transactionID.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.nodeAccountID == "" { 0 } else { 1 + sizeof_len((&self.nodeAccountID).len()) }
        + if self.transactionFee == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.transactionFee) as u64) }
        + if self.transactionValidDuration == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.transactionValidDuration) as u64) }
        + if self.memo == "" { 0 } else { 1 + sizeof_len((&self.memo).len()) }
        + match self.data {
            TW::Hedera::Proto::mod_TransactionBody::OneOfdata::transfer(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Hedera::Proto::mod_TransactionBody::OneOfdata::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.transactionID { w.write_with_tag(10, |w| w.write_message(s))?; }
        if self.nodeAccountID != "" { w.write_with_tag(18, |w| w.write_string(&**&self.nodeAccountID))?; }
        if self.transactionFee != 0u64 { w.write_with_tag(24, |w| w.write_uint64(*&self.transactionFee))?; }
        if self.transactionValidDuration != 0i64 { w.write_with_tag(32, |w| w.write_int64(*&self.transactionValidDuration))?; }
        if self.memo != "" { w.write_with_tag(42, |w| w.write_string(&**&self.memo))?; }
        match self.data {            TW::Hedera::Proto::mod_TransactionBody::OneOfdata::transfer(ref m) => { w.write_with_tag(50, |w| w.write_message(m))? },
            TW::Hedera::Proto::mod_TransactionBody::OneOfdata::None => {},
    }        Ok(())
    }
}

pub mod mod_TransactionBody {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfdata<'a> {
    transfer(TW::Hedera::Proto::TransferMessage<'a>),
    None,
}

impl<'a> Default for OneOfdata<'a> {
    fn default() -> Self {
        OneOfdata::None
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningInput<'a> {
    pub private_key: Cow<'a, [u8]>,
    pub body: Option<TW::Hedera::Proto::TransactionBody<'a>>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.body = Some(r.read_message::<TW::Hedera::Proto::TransactionBody>(bytes)?),
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
        + self.body.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.private_key))?; }
        if let Some(ref s) = self.body { w.write_with_tag(18, |w| w.write_message(s))?; }
        Ok(())
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

