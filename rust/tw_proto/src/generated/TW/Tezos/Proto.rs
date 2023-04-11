// Automatically generated rust module for 'Tezos.proto' file

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
    pub operation_list: Option<TW::Tezos::Proto::OperationList<'a>>,
    pub encoded_operations: Cow<'a, [u8]>,
    pub private_key: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.operation_list = Some(r.read_message::<TW::Tezos::Proto::OperationList>(bytes)?),
                Ok(18) => msg.encoded_operations = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
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
        + self.operation_list.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.encoded_operations == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.encoded_operations).len()) }
        + if self.private_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.private_key).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.operation_list { w.write_with_tag(10, |w| w.write_message(s))?; }
        if self.encoded_operations != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.encoded_operations))?; }
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.private_key))?; }
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

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct OperationList<'a> {
    pub branch: Cow<'a, str>,
    pub operations: Vec<TW::Tezos::Proto::Operation<'a>>,
}

impl<'a> MessageRead<'a> for OperationList<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.branch = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.operations.push(r.read_message::<TW::Tezos::Proto::Operation>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for OperationList<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.branch == "" { 0 } else { 1 + sizeof_len((&self.branch).len()) }
        + self.operations.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.branch != "" { w.write_with_tag(10, |w| w.write_string(&**&self.branch))?; }
        for s in &self.operations { w.write_with_tag(18, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Operation<'a> {
    pub counter: i64,
    pub source: Cow<'a, str>,
    pub fee: i64,
    pub gas_limit: i64,
    pub storage_limit: i64,
    pub kind: TW::Tezos::Proto::mod_Operation::OperationKind,
    pub operation_data: TW::Tezos::Proto::mod_Operation::OneOfoperation_data<'a>,
}

impl<'a> MessageRead<'a> for Operation<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.counter = r.read_int64(bytes)?,
                Ok(18) => msg.source = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.fee = r.read_int64(bytes)?,
                Ok(32) => msg.gas_limit = r.read_int64(bytes)?,
                Ok(40) => msg.storage_limit = r.read_int64(bytes)?,
                Ok(56) => msg.kind = r.read_enum(bytes)?,
                Ok(66) => msg.operation_data = TW::Tezos::Proto::mod_Operation::OneOfoperation_data::reveal_operation_data(r.read_message::<TW::Tezos::Proto::RevealOperationData>(bytes)?),
                Ok(74) => msg.operation_data = TW::Tezos::Proto::mod_Operation::OneOfoperation_data::transaction_operation_data(r.read_message::<TW::Tezos::Proto::TransactionOperationData>(bytes)?),
                Ok(90) => msg.operation_data = TW::Tezos::Proto::mod_Operation::OneOfoperation_data::delegation_operation_data(r.read_message::<TW::Tezos::Proto::DelegationOperationData>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Operation<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.counter == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.counter) as u64) }
        + if self.source == "" { 0 } else { 1 + sizeof_len((&self.source).len()) }
        + if self.fee == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.fee) as u64) }
        + if self.gas_limit == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.gas_limit) as u64) }
        + if self.storage_limit == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.storage_limit) as u64) }
        + if self.kind == TW::Tezos::Proto::mod_Operation::OperationKind::ENDORSEMENT { 0 } else { 1 + sizeof_varint(*(&self.kind) as u64) }
        + match self.operation_data {
            TW::Tezos::Proto::mod_Operation::OneOfoperation_data::reveal_operation_data(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Tezos::Proto::mod_Operation::OneOfoperation_data::transaction_operation_data(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Tezos::Proto::mod_Operation::OneOfoperation_data::delegation_operation_data(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Tezos::Proto::mod_Operation::OneOfoperation_data::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.counter != 0i64 { w.write_with_tag(8, |w| w.write_int64(*&self.counter))?; }
        if self.source != "" { w.write_with_tag(18, |w| w.write_string(&**&self.source))?; }
        if self.fee != 0i64 { w.write_with_tag(24, |w| w.write_int64(*&self.fee))?; }
        if self.gas_limit != 0i64 { w.write_with_tag(32, |w| w.write_int64(*&self.gas_limit))?; }
        if self.storage_limit != 0i64 { w.write_with_tag(40, |w| w.write_int64(*&self.storage_limit))?; }
        if self.kind != TW::Tezos::Proto::mod_Operation::OperationKind::ENDORSEMENT { w.write_with_tag(56, |w| w.write_enum(*&self.kind as i32))?; }
        match self.operation_data {            TW::Tezos::Proto::mod_Operation::OneOfoperation_data::reveal_operation_data(ref m) => { w.write_with_tag(66, |w| w.write_message(m))? },
            TW::Tezos::Proto::mod_Operation::OneOfoperation_data::transaction_operation_data(ref m) => { w.write_with_tag(74, |w| w.write_message(m))? },
            TW::Tezos::Proto::mod_Operation::OneOfoperation_data::delegation_operation_data(ref m) => { w.write_with_tag(90, |w| w.write_message(m))? },
            TW::Tezos::Proto::mod_Operation::OneOfoperation_data::None => {},
    }        Ok(())
    }
}

pub mod mod_Operation {

use super::*;

#[derive(Debug, PartialEq, Eq, Clone, Copy)]
pub enum OperationKind {
    ENDORSEMENT = 0,
    REVEAL = 107,
    TRANSACTION = 108,
    DELEGATION = 110,
}

impl Default for OperationKind {
    fn default() -> Self {
        OperationKind::ENDORSEMENT
    }
}

impl From<i32> for OperationKind {
    fn from(i: i32) -> Self {
        match i {
            0 => OperationKind::ENDORSEMENT,
            107 => OperationKind::REVEAL,
            108 => OperationKind::TRANSACTION,
            110 => OperationKind::DELEGATION,
            _ => Self::default(),
        }
    }
}

impl<'a> From<&'a str> for OperationKind {
    fn from(s: &'a str) -> Self {
        match s {
            "ENDORSEMENT" => OperationKind::ENDORSEMENT,
            "REVEAL" => OperationKind::REVEAL,
            "TRANSACTION" => OperationKind::TRANSACTION,
            "DELEGATION" => OperationKind::DELEGATION,
            _ => Self::default(),
        }
    }
}

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfoperation_data<'a> {
    reveal_operation_data(TW::Tezos::Proto::RevealOperationData<'a>),
    transaction_operation_data(TW::Tezos::Proto::TransactionOperationData<'a>),
    delegation_operation_data(TW::Tezos::Proto::DelegationOperationData<'a>),
    None,
}

impl<'a> Default for OneOfoperation_data<'a> {
    fn default() -> Self {
        OneOfoperation_data::None
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct FA12Parameters<'a> {
    pub entrypoint: Cow<'a, str>,
    pub from: Cow<'a, str>,
    pub to: Cow<'a, str>,
    pub value: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for FA12Parameters<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.entrypoint = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.from = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.to = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.value = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for FA12Parameters<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.entrypoint == "" { 0 } else { 1 + sizeof_len((&self.entrypoint).len()) }
        + if self.from == "" { 0 } else { 1 + sizeof_len((&self.from).len()) }
        + if self.to == "" { 0 } else { 1 + sizeof_len((&self.to).len()) }
        + if self.value == "" { 0 } else { 1 + sizeof_len((&self.value).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.entrypoint != "" { w.write_with_tag(10, |w| w.write_string(&**&self.entrypoint))?; }
        if self.from != "" { w.write_with_tag(18, |w| w.write_string(&**&self.from))?; }
        if self.to != "" { w.write_with_tag(26, |w| w.write_string(&**&self.to))?; }
        if self.value != "" { w.write_with_tag(34, |w| w.write_string(&**&self.value))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Txs<'a> {
    pub to: Cow<'a, str>,
    pub token_id: Cow<'a, str>,
    pub amount: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for Txs<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.to = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.token_id = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.amount = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Txs<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.to == "" { 0 } else { 1 + sizeof_len((&self.to).len()) }
        + if self.token_id == "" { 0 } else { 1 + sizeof_len((&self.token_id).len()) }
        + if self.amount == "" { 0 } else { 1 + sizeof_len((&self.amount).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.to != "" { w.write_with_tag(10, |w| w.write_string(&**&self.to))?; }
        if self.token_id != "" { w.write_with_tag(18, |w| w.write_string(&**&self.token_id))?; }
        if self.amount != "" { w.write_with_tag(26, |w| w.write_string(&**&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TxObject<'a> {
    pub from: Cow<'a, str>,
    pub txs: Vec<TW::Tezos::Proto::Txs<'a>>,
}

impl<'a> MessageRead<'a> for TxObject<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.from = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.txs.push(r.read_message::<TW::Tezos::Proto::Txs>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TxObject<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.from == "" { 0 } else { 1 + sizeof_len((&self.from).len()) }
        + self.txs.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.from != "" { w.write_with_tag(10, |w| w.write_string(&**&self.from))?; }
        for s in &self.txs { w.write_with_tag(18, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct FA2Parameters<'a> {
    pub entrypoint: Cow<'a, str>,
    pub txs_object: Vec<TW::Tezos::Proto::TxObject<'a>>,
}

impl<'a> MessageRead<'a> for FA2Parameters<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.entrypoint = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.txs_object.push(r.read_message::<TW::Tezos::Proto::TxObject>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for FA2Parameters<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.entrypoint == "" { 0 } else { 1 + sizeof_len((&self.entrypoint).len()) }
        + self.txs_object.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.entrypoint != "" { w.write_with_tag(10, |w| w.write_string(&**&self.entrypoint))?; }
        for s in &self.txs_object { w.write_with_tag(18, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct OperationParameters<'a> {
    pub parameters: TW::Tezos::Proto::mod_OperationParameters::OneOfparameters<'a>,
}

impl<'a> MessageRead<'a> for OperationParameters<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.parameters = TW::Tezos::Proto::mod_OperationParameters::OneOfparameters::fa12_parameters(r.read_message::<TW::Tezos::Proto::FA12Parameters>(bytes)?),
                Ok(18) => msg.parameters = TW::Tezos::Proto::mod_OperationParameters::OneOfparameters::fa2_parameters(r.read_message::<TW::Tezos::Proto::FA2Parameters>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for OperationParameters<'a> {
    fn get_size(&self) -> usize {
        0
        + match self.parameters {
            TW::Tezos::Proto::mod_OperationParameters::OneOfparameters::fa12_parameters(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Tezos::Proto::mod_OperationParameters::OneOfparameters::fa2_parameters(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Tezos::Proto::mod_OperationParameters::OneOfparameters::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        match self.parameters {            TW::Tezos::Proto::mod_OperationParameters::OneOfparameters::fa12_parameters(ref m) => { w.write_with_tag(10, |w| w.write_message(m))? },
            TW::Tezos::Proto::mod_OperationParameters::OneOfparameters::fa2_parameters(ref m) => { w.write_with_tag(18, |w| w.write_message(m))? },
            TW::Tezos::Proto::mod_OperationParameters::OneOfparameters::None => {},
    }        Ok(())
    }
}

pub mod mod_OperationParameters {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfparameters<'a> {
    fa12_parameters(TW::Tezos::Proto::FA12Parameters<'a>),
    fa2_parameters(TW::Tezos::Proto::FA2Parameters<'a>),
    None,
}

impl<'a> Default for OneOfparameters<'a> {
    fn default() -> Self {
        OneOfparameters::None
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TransactionOperationData<'a> {
    pub destination: Cow<'a, str>,
    pub amount: i64,
    pub parameters: Option<TW::Tezos::Proto::OperationParameters<'a>>,
}

impl<'a> MessageRead<'a> for TransactionOperationData<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.destination = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.amount = r.read_int64(bytes)?,
                Ok(26) => msg.parameters = Some(r.read_message::<TW::Tezos::Proto::OperationParameters>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TransactionOperationData<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.destination == "" { 0 } else { 1 + sizeof_len((&self.destination).len()) }
        + if self.amount == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
        + self.parameters.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.destination != "" { w.write_with_tag(10, |w| w.write_string(&**&self.destination))?; }
        if self.amount != 0i64 { w.write_with_tag(16, |w| w.write_int64(*&self.amount))?; }
        if let Some(ref s) = self.parameters { w.write_with_tag(26, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct RevealOperationData<'a> {
    pub public_key: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for RevealOperationData<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.public_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for RevealOperationData<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.public_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.public_key).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.public_key != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.public_key))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct DelegationOperationData<'a> {
    pub delegate: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for DelegationOperationData<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.delegate = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for DelegationOperationData<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.delegate == "" { 0 } else { 1 + sizeof_len((&self.delegate).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.delegate != "" { w.write_with_tag(10, |w| w.write_string(&**&self.delegate))?; }
        Ok(())
    }
}

