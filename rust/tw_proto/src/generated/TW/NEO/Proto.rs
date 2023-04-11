// Automatically generated rust module for 'NEO.proto' file

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
pub struct TransactionInput<'a> {
    pub prev_hash: Cow<'a, [u8]>,
    pub prev_index: u32,
    pub value: i64,
    pub asset_id: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for TransactionInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.prev_hash = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(21) => msg.prev_index = r.read_fixed32(bytes)?,
                Ok(24) => msg.value = r.read_int64(bytes)?,
                Ok(34) => msg.asset_id = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TransactionInput<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.prev_hash == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.prev_hash).len()) }
        + if self.prev_index == 0u32 { 0 } else { 1 + 4 }
        + if self.value == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.value) as u64) }
        + if self.asset_id == "" { 0 } else { 1 + sizeof_len((&self.asset_id).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.prev_hash != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.prev_hash))?; }
        if self.prev_index != 0u32 { w.write_with_tag(21, |w| w.write_fixed32(*&self.prev_index))?; }
        if self.value != 0i64 { w.write_with_tag(24, |w| w.write_int64(*&self.value))?; }
        if self.asset_id != "" { w.write_with_tag(34, |w| w.write_string(&**&self.asset_id))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TransactionOutput<'a> {
    pub asset_id: Cow<'a, str>,
    pub amount: i64,
    pub to_address: Cow<'a, str>,
    pub change_address: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for TransactionOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.asset_id = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.amount = r.read_sint64(bytes)?,
                Ok(26) => msg.to_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.change_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TransactionOutput<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.asset_id == "" { 0 } else { 1 + sizeof_len((&self.asset_id).len()) }
        + if self.amount == 0i64 { 0 } else { 1 + sizeof_sint64(*(&self.amount)) }
        + if self.to_address == "" { 0 } else { 1 + sizeof_len((&self.to_address).len()) }
        + if self.change_address == "" { 0 } else { 1 + sizeof_len((&self.change_address).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.asset_id != "" { w.write_with_tag(10, |w| w.write_string(&**&self.asset_id))?; }
        if self.amount != 0i64 { w.write_with_tag(16, |w| w.write_sint64(*&self.amount))?; }
        if self.to_address != "" { w.write_with_tag(26, |w| w.write_string(&**&self.to_address))?; }
        if self.change_address != "" { w.write_with_tag(34, |w| w.write_string(&**&self.change_address))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningInput<'a> {
    pub inputs: Vec<TW::NEO::Proto::TransactionInput<'a>>,
    pub outputs: Vec<TW::NEO::Proto::TransactionOutput<'a>>,
    pub private_key: Cow<'a, [u8]>,
    pub fee: i64,
    pub gas_asset_id: Cow<'a, str>,
    pub gas_change_address: Cow<'a, str>,
    pub plan: Option<TW::NEO::Proto::TransactionPlan<'a>>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.inputs.push(r.read_message::<TW::NEO::Proto::TransactionInput>(bytes)?),
                Ok(18) => msg.outputs.push(r.read_message::<TW::NEO::Proto::TransactionOutput>(bytes)?),
                Ok(26) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(32) => msg.fee = r.read_int64(bytes)?,
                Ok(42) => msg.gas_asset_id = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(50) => msg.gas_change_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(58) => msg.plan = Some(r.read_message::<TW::NEO::Proto::TransactionPlan>(bytes)?),
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
        + self.inputs.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + self.outputs.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + if self.private_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.private_key).len()) }
        + if self.fee == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.fee) as u64) }
        + if self.gas_asset_id == "" { 0 } else { 1 + sizeof_len((&self.gas_asset_id).len()) }
        + if self.gas_change_address == "" { 0 } else { 1 + sizeof_len((&self.gas_change_address).len()) }
        + self.plan.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        for s in &self.inputs { w.write_with_tag(10, |w| w.write_message(s))?; }
        for s in &self.outputs { w.write_with_tag(18, |w| w.write_message(s))?; }
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.private_key))?; }
        if self.fee != 0i64 { w.write_with_tag(32, |w| w.write_int64(*&self.fee))?; }
        if self.gas_asset_id != "" { w.write_with_tag(42, |w| w.write_string(&**&self.gas_asset_id))?; }
        if self.gas_change_address != "" { w.write_with_tag(50, |w| w.write_string(&**&self.gas_change_address))?; }
        if let Some(ref s) = self.plan { w.write_with_tag(58, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningOutput<'a> {
    pub encoded: Cow<'a, [u8]>,
    pub error: TW::Common::Proto::SigningError,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.encoded = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.error = r.read_enum(bytes)?,
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
        + if self.error == TW::Common::Proto::SigningError::OK { 0 } else { 1 + sizeof_varint(*(&self.error) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.encoded != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.encoded))?; }
        if self.error != TW::Common::Proto::SigningError::OK { w.write_with_tag(16, |w| w.write_enum(*&self.error as i32))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TransactionOutputPlan<'a> {
    pub amount: i64,
    pub available_amount: i64,
    pub change: i64,
    pub asset_id: Cow<'a, str>,
    pub to_address: Cow<'a, str>,
    pub change_address: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for TransactionOutputPlan<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.amount = r.read_int64(bytes)?,
                Ok(16) => msg.available_amount = r.read_int64(bytes)?,
                Ok(24) => msg.change = r.read_int64(bytes)?,
                Ok(34) => msg.asset_id = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.to_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(50) => msg.change_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TransactionOutputPlan<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.amount == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
        + if self.available_amount == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.available_amount) as u64) }
        + if self.change == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.change) as u64) }
        + if self.asset_id == "" { 0 } else { 1 + sizeof_len((&self.asset_id).len()) }
        + if self.to_address == "" { 0 } else { 1 + sizeof_len((&self.to_address).len()) }
        + if self.change_address == "" { 0 } else { 1 + sizeof_len((&self.change_address).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.amount != 0i64 { w.write_with_tag(8, |w| w.write_int64(*&self.amount))?; }
        if self.available_amount != 0i64 { w.write_with_tag(16, |w| w.write_int64(*&self.available_amount))?; }
        if self.change != 0i64 { w.write_with_tag(24, |w| w.write_int64(*&self.change))?; }
        if self.asset_id != "" { w.write_with_tag(34, |w| w.write_string(&**&self.asset_id))?; }
        if self.to_address != "" { w.write_with_tag(42, |w| w.write_string(&**&self.to_address))?; }
        if self.change_address != "" { w.write_with_tag(50, |w| w.write_string(&**&self.change_address))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TransactionPlan<'a> {
    pub outputs: Vec<TW::NEO::Proto::TransactionOutputPlan<'a>>,
    pub inputs: Vec<TW::NEO::Proto::TransactionInput<'a>>,
    pub fee: i64,
    pub error: TW::Common::Proto::SigningError,
}

impl<'a> MessageRead<'a> for TransactionPlan<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.outputs.push(r.read_message::<TW::NEO::Proto::TransactionOutputPlan>(bytes)?),
                Ok(18) => msg.inputs.push(r.read_message::<TW::NEO::Proto::TransactionInput>(bytes)?),
                Ok(24) => msg.fee = r.read_int64(bytes)?,
                Ok(32) => msg.error = r.read_enum(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TransactionPlan<'a> {
    fn get_size(&self) -> usize {
        0
        + self.outputs.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + self.inputs.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + if self.fee == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.fee) as u64) }
        + if self.error == TW::Common::Proto::SigningError::OK { 0 } else { 1 + sizeof_varint(*(&self.error) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        for s in &self.outputs { w.write_with_tag(10, |w| w.write_message(s))?; }
        for s in &self.inputs { w.write_with_tag(18, |w| w.write_message(s))?; }
        if self.fee != 0i64 { w.write_with_tag(24, |w| w.write_int64(*&self.fee))?; }
        if self.error != TW::Common::Proto::SigningError::OK { w.write_with_tag(32, |w| w.write_enum(*&self.error as i32))?; }
        Ok(())
    }
}

