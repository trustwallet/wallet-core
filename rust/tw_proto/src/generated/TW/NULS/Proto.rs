// Automatically generated rust module for 'NULS.proto' file

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
pub struct TransactionCoinFrom<'a> {
    pub from_address: Cow<'a, str>,
    pub assets_chainid: u32,
    pub assets_id: u32,
    pub id_amount: Cow<'a, [u8]>,
    pub nonce: Cow<'a, [u8]>,
    pub locked: u32,
}

impl<'a> MessageRead<'a> for TransactionCoinFrom<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.from_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.assets_chainid = r.read_uint32(bytes)?,
                Ok(24) => msg.assets_id = r.read_uint32(bytes)?,
                Ok(34) => msg.id_amount = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.nonce = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(48) => msg.locked = r.read_uint32(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TransactionCoinFrom<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.from_address == "" { 0 } else { 1 + sizeof_len((&self.from_address).len()) }
        + if self.assets_chainid == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.assets_chainid) as u64) }
        + if self.assets_id == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.assets_id) as u64) }
        + if self.id_amount == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.id_amount).len()) }
        + if self.nonce == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.nonce).len()) }
        + if self.locked == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.locked) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.from_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.from_address))?; }
        if self.assets_chainid != 0u32 { w.write_with_tag(16, |w| w.write_uint32(*&self.assets_chainid))?; }
        if self.assets_id != 0u32 { w.write_with_tag(24, |w| w.write_uint32(*&self.assets_id))?; }
        if self.id_amount != Cow::Borrowed(b"") { w.write_with_tag(34, |w| w.write_bytes(&**&self.id_amount))?; }
        if self.nonce != Cow::Borrowed(b"") { w.write_with_tag(42, |w| w.write_bytes(&**&self.nonce))?; }
        if self.locked != 0u32 { w.write_with_tag(48, |w| w.write_uint32(*&self.locked))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TransactionCoinTo<'a> {
    pub to_address: Cow<'a, str>,
    pub assets_chainid: u32,
    pub assets_id: u32,
    pub id_amount: Cow<'a, [u8]>,
    pub lock_time: u32,
}

impl<'a> MessageRead<'a> for TransactionCoinTo<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.to_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.assets_chainid = r.read_uint32(bytes)?,
                Ok(24) => msg.assets_id = r.read_uint32(bytes)?,
                Ok(34) => msg.id_amount = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(40) => msg.lock_time = r.read_uint32(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TransactionCoinTo<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.to_address == "" { 0 } else { 1 + sizeof_len((&self.to_address).len()) }
        + if self.assets_chainid == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.assets_chainid) as u64) }
        + if self.assets_id == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.assets_id) as u64) }
        + if self.id_amount == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.id_amount).len()) }
        + if self.lock_time == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.lock_time) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.to_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.to_address))?; }
        if self.assets_chainid != 0u32 { w.write_with_tag(16, |w| w.write_uint32(*&self.assets_chainid))?; }
        if self.assets_id != 0u32 { w.write_with_tag(24, |w| w.write_uint32(*&self.assets_id))?; }
        if self.id_amount != Cow::Borrowed(b"") { w.write_with_tag(34, |w| w.write_bytes(&**&self.id_amount))?; }
        if self.lock_time != 0u32 { w.write_with_tag(40, |w| w.write_uint32(*&self.lock_time))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Signature<'a> {
    pub pkey_len: u32,
    pub public_key: Cow<'a, [u8]>,
    pub sig_len: u32,
    pub signature: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for Signature<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.pkey_len = r.read_uint32(bytes)?,
                Ok(18) => msg.public_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.sig_len = r.read_uint32(bytes)?,
                Ok(34) => msg.signature = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Signature<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.pkey_len == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.pkey_len) as u64) }
        + if self.public_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.public_key).len()) }
        + if self.sig_len == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.sig_len) as u64) }
        + if self.signature == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.signature).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.pkey_len != 0u32 { w.write_with_tag(8, |w| w.write_uint32(*&self.pkey_len))?; }
        if self.public_key != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.public_key))?; }
        if self.sig_len != 0u32 { w.write_with_tag(24, |w| w.write_uint32(*&self.sig_len))?; }
        if self.signature != Cow::Borrowed(b"") { w.write_with_tag(34, |w| w.write_bytes(&**&self.signature))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Transaction<'a> {
    pub type_pb: u32,
    pub timestamp: u32,
    pub remark: Cow<'a, str>,
    pub tx_data: Cow<'a, [u8]>,
    pub input: Option<TW::NULS::Proto::TransactionCoinFrom<'a>>,
    pub output: Option<TW::NULS::Proto::TransactionCoinTo<'a>>,
    pub tx_sigs: Option<TW::NULS::Proto::Signature<'a>>,
    pub hash: u32,
}

impl<'a> MessageRead<'a> for Transaction<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.type_pb = r.read_uint32(bytes)?,
                Ok(16) => msg.timestamp = r.read_uint32(bytes)?,
                Ok(26) => msg.remark = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.tx_data = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.input = Some(r.read_message::<TW::NULS::Proto::TransactionCoinFrom>(bytes)?),
                Ok(50) => msg.output = Some(r.read_message::<TW::NULS::Proto::TransactionCoinTo>(bytes)?),
                Ok(58) => msg.tx_sigs = Some(r.read_message::<TW::NULS::Proto::Signature>(bytes)?),
                Ok(64) => msg.hash = r.read_uint32(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Transaction<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.type_pb == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.type_pb) as u64) }
        + if self.timestamp == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.timestamp) as u64) }
        + if self.remark == "" { 0 } else { 1 + sizeof_len((&self.remark).len()) }
        + if self.tx_data == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.tx_data).len()) }
        + self.input.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + self.output.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + self.tx_sigs.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.hash == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.hash) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.type_pb != 0u32 { w.write_with_tag(8, |w| w.write_uint32(*&self.type_pb))?; }
        if self.timestamp != 0u32 { w.write_with_tag(16, |w| w.write_uint32(*&self.timestamp))?; }
        if self.remark != "" { w.write_with_tag(26, |w| w.write_string(&**&self.remark))?; }
        if self.tx_data != Cow::Borrowed(b"") { w.write_with_tag(34, |w| w.write_bytes(&**&self.tx_data))?; }
        if let Some(ref s) = self.input { w.write_with_tag(42, |w| w.write_message(s))?; }
        if let Some(ref s) = self.output { w.write_with_tag(50, |w| w.write_message(s))?; }
        if let Some(ref s) = self.tx_sigs { w.write_with_tag(58, |w| w.write_message(s))?; }
        if self.hash != 0u32 { w.write_with_tag(64, |w| w.write_uint32(*&self.hash))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningInput<'a> {
    pub private_key: Cow<'a, [u8]>,
    pub from: Cow<'a, str>,
    pub to: Cow<'a, str>,
    pub amount: Cow<'a, [u8]>,
    pub chain_id: u32,
    pub idassets_id: u32,
    pub nonce: Cow<'a, [u8]>,
    pub remark: Cow<'a, str>,
    pub balance: Cow<'a, [u8]>,
    pub timestamp: u32,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.from = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.to = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.amount = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(40) => msg.chain_id = r.read_uint32(bytes)?,
                Ok(48) => msg.idassets_id = r.read_uint32(bytes)?,
                Ok(58) => msg.nonce = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(66) => msg.remark = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(74) => msg.balance = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(80) => msg.timestamp = r.read_uint32(bytes)?,
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
        + if self.from == "" { 0 } else { 1 + sizeof_len((&self.from).len()) }
        + if self.to == "" { 0 } else { 1 + sizeof_len((&self.to).len()) }
        + if self.amount == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.amount).len()) }
        + if self.chain_id == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.chain_id) as u64) }
        + if self.idassets_id == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.idassets_id) as u64) }
        + if self.nonce == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.nonce).len()) }
        + if self.remark == "" { 0 } else { 1 + sizeof_len((&self.remark).len()) }
        + if self.balance == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.balance).len()) }
        + if self.timestamp == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.timestamp) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.private_key))?; }
        if self.from != "" { w.write_with_tag(18, |w| w.write_string(&**&self.from))?; }
        if self.to != "" { w.write_with_tag(26, |w| w.write_string(&**&self.to))?; }
        if self.amount != Cow::Borrowed(b"") { w.write_with_tag(34, |w| w.write_bytes(&**&self.amount))?; }
        if self.chain_id != 0u32 { w.write_with_tag(40, |w| w.write_uint32(*&self.chain_id))?; }
        if self.idassets_id != 0u32 { w.write_with_tag(48, |w| w.write_uint32(*&self.idassets_id))?; }
        if self.nonce != Cow::Borrowed(b"") { w.write_with_tag(58, |w| w.write_bytes(&**&self.nonce))?; }
        if self.remark != "" { w.write_with_tag(66, |w| w.write_string(&**&self.remark))?; }
        if self.balance != Cow::Borrowed(b"") { w.write_with_tag(74, |w| w.write_bytes(&**&self.balance))?; }
        if self.timestamp != 0u32 { w.write_with_tag(80, |w| w.write_uint32(*&self.timestamp))?; }
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

