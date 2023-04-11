// Automatically generated rust module for 'Theta.proto' file

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
    pub chain_id: Cow<'a, str>,
    pub to_address: Cow<'a, str>,
    pub theta_amount: Cow<'a, [u8]>,
    pub tfuel_amount: Cow<'a, [u8]>,
    pub sequence: u64,
    pub fee: Cow<'a, [u8]>,
    pub private_key: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.chain_id = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.to_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.theta_amount = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.tfuel_amount = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(40) => msg.sequence = r.read_uint64(bytes)?,
                Ok(50) => msg.fee = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(58) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
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
        + if self.chain_id == "" { 0 } else { 1 + sizeof_len((&self.chain_id).len()) }
        + if self.to_address == "" { 0 } else { 1 + sizeof_len((&self.to_address).len()) }
        + if self.theta_amount == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.theta_amount).len()) }
        + if self.tfuel_amount == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.tfuel_amount).len()) }
        + if self.sequence == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.sequence) as u64) }
        + if self.fee == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.fee).len()) }
        + if self.private_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.private_key).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.chain_id != "" { w.write_with_tag(10, |w| w.write_string(&**&self.chain_id))?; }
        if self.to_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.to_address))?; }
        if self.theta_amount != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.theta_amount))?; }
        if self.tfuel_amount != Cow::Borrowed(b"") { w.write_with_tag(34, |w| w.write_bytes(&**&self.tfuel_amount))?; }
        if self.sequence != 0u64 { w.write_with_tag(40, |w| w.write_uint64(*&self.sequence))?; }
        if self.fee != Cow::Borrowed(b"") { w.write_with_tag(50, |w| w.write_bytes(&**&self.fee))?; }
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(58, |w| w.write_bytes(&**&self.private_key))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningOutput<'a> {
    pub encoded: Cow<'a, [u8]>,
    pub signature: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.encoded = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.signature = r.read_bytes(bytes).map(Cow::Borrowed)?,
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
        + if self.signature == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.signature).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.encoded != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.encoded))?; }
        if self.signature != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.signature))?; }
        Ok(())
    }
}

