// Automatically generated rust module for 'Binance.proto' file

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
pub struct Transaction<'a> {
    pub msgs: Vec<Cow<'a, [u8]>>,
    pub signatures: Vec<Cow<'a, [u8]>>,
    pub memo: Cow<'a, str>,
    pub source: i64,
    pub data: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for Transaction<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.msgs.push(r.read_bytes(bytes).map(Cow::Borrowed)?),
                Ok(18) => msg.signatures.push(r.read_bytes(bytes).map(Cow::Borrowed)?),
                Ok(26) => msg.memo = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(32) => msg.source = r.read_int64(bytes)?,
                Ok(42) => msg.data = r.read_bytes(bytes).map(Cow::Borrowed)?,
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
        + self.msgs.iter().map(|s| 1 + sizeof_len((s).len())).sum::<usize>()
        + self.signatures.iter().map(|s| 1 + sizeof_len((s).len())).sum::<usize>()
        + if self.memo == "" { 0 } else { 1 + sizeof_len((&self.memo).len()) }
        + if self.source == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.source) as u64) }
        + if self.data == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.data).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        for s in &self.msgs { w.write_with_tag(10, |w| w.write_bytes(&**s))?; }
        for s in &self.signatures { w.write_with_tag(18, |w| w.write_bytes(&**s))?; }
        if self.memo != "" { w.write_with_tag(26, |w| w.write_string(&**&self.memo))?; }
        if self.source != 0i64 { w.write_with_tag(32, |w| w.write_int64(*&self.source))?; }
        if self.data != Cow::Borrowed(b"") { w.write_with_tag(42, |w| w.write_bytes(&**&self.data))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Signature<'a> {
    pub pub_key: Cow<'a, [u8]>,
    pub signature: Cow<'a, [u8]>,
    pub account_number: i64,
    pub sequence: i64,
}

impl<'a> MessageRead<'a> for Signature<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.pub_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.signature = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.account_number = r.read_int64(bytes)?,
                Ok(32) => msg.sequence = r.read_int64(bytes)?,
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
        + if self.pub_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.pub_key).len()) }
        + if self.signature == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.signature).len()) }
        + if self.account_number == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.account_number) as u64) }
        + if self.sequence == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.sequence) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.pub_key != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.pub_key))?; }
        if self.signature != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.signature))?; }
        if self.account_number != 0i64 { w.write_with_tag(24, |w| w.write_int64(*&self.account_number))?; }
        if self.sequence != 0i64 { w.write_with_tag(32, |w| w.write_int64(*&self.sequence))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TradeOrder<'a> {
    pub sender: Cow<'a, [u8]>,
    pub id: Cow<'a, str>,
    pub symbol: Cow<'a, str>,
    pub ordertype: i64,
    pub side: i64,
    pub price: i64,
    pub quantity: i64,
    pub timeinforce: i64,
}

impl<'a> MessageRead<'a> for TradeOrder<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.sender = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.id = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.symbol = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(32) => msg.ordertype = r.read_int64(bytes)?,
                Ok(40) => msg.side = r.read_int64(bytes)?,
                Ok(48) => msg.price = r.read_int64(bytes)?,
                Ok(56) => msg.quantity = r.read_int64(bytes)?,
                Ok(64) => msg.timeinforce = r.read_int64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TradeOrder<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.sender == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.sender).len()) }
        + if self.id == "" { 0 } else { 1 + sizeof_len((&self.id).len()) }
        + if self.symbol == "" { 0 } else { 1 + sizeof_len((&self.symbol).len()) }
        + if self.ordertype == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.ordertype) as u64) }
        + if self.side == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.side) as u64) }
        + if self.price == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.price) as u64) }
        + if self.quantity == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.quantity) as u64) }
        + if self.timeinforce == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.timeinforce) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.sender != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.sender))?; }
        if self.id != "" { w.write_with_tag(18, |w| w.write_string(&**&self.id))?; }
        if self.symbol != "" { w.write_with_tag(26, |w| w.write_string(&**&self.symbol))?; }
        if self.ordertype != 0i64 { w.write_with_tag(32, |w| w.write_int64(*&self.ordertype))?; }
        if self.side != 0i64 { w.write_with_tag(40, |w| w.write_int64(*&self.side))?; }
        if self.price != 0i64 { w.write_with_tag(48, |w| w.write_int64(*&self.price))?; }
        if self.quantity != 0i64 { w.write_with_tag(56, |w| w.write_int64(*&self.quantity))?; }
        if self.timeinforce != 0i64 { w.write_with_tag(64, |w| w.write_int64(*&self.timeinforce))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct CancelTradeOrder<'a> {
    pub sender: Cow<'a, [u8]>,
    pub symbol: Cow<'a, str>,
    pub refid: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for CancelTradeOrder<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.sender = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.symbol = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.refid = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for CancelTradeOrder<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.sender == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.sender).len()) }
        + if self.symbol == "" { 0 } else { 1 + sizeof_len((&self.symbol).len()) }
        + if self.refid == "" { 0 } else { 1 + sizeof_len((&self.refid).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.sender != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.sender))?; }
        if self.symbol != "" { w.write_with_tag(18, |w| w.write_string(&**&self.symbol))?; }
        if self.refid != "" { w.write_with_tag(26, |w| w.write_string(&**&self.refid))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SendOrder<'a> {
    pub inputs: Vec<TW::Binance::Proto::mod_SendOrder::Input<'a>>,
    pub outputs: Vec<TW::Binance::Proto::mod_SendOrder::Output<'a>>,
}

impl<'a> MessageRead<'a> for SendOrder<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.inputs.push(r.read_message::<TW::Binance::Proto::mod_SendOrder::Input>(bytes)?),
                Ok(18) => msg.outputs.push(r.read_message::<TW::Binance::Proto::mod_SendOrder::Output>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for SendOrder<'a> {
    fn get_size(&self) -> usize {
        0
        + self.inputs.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + self.outputs.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        for s in &self.inputs { w.write_with_tag(10, |w| w.write_message(s))?; }
        for s in &self.outputs { w.write_with_tag(18, |w| w.write_message(s))?; }
        Ok(())
    }
}

pub mod mod_SendOrder {

use std::borrow::Cow;
use super::*;

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Token<'a> {
    pub denom: Cow<'a, str>,
    pub amount: i64,
}

impl<'a> MessageRead<'a> for Token<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.denom = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.amount = r.read_int64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Token<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.denom == "" { 0 } else { 1 + sizeof_len((&self.denom).len()) }
        + if self.amount == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.denom != "" { w.write_with_tag(10, |w| w.write_string(&**&self.denom))?; }
        if self.amount != 0i64 { w.write_with_tag(16, |w| w.write_int64(*&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Input<'a> {
    pub address: Cow<'a, [u8]>,
    pub coins: Vec<TW::Binance::Proto::mod_SendOrder::Token<'a>>,
}

impl<'a> MessageRead<'a> for Input<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.address = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.coins.push(r.read_message::<TW::Binance::Proto::mod_SendOrder::Token>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Input<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.address == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.address).len()) }
        + self.coins.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.address != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.address))?; }
        for s in &self.coins { w.write_with_tag(18, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Output<'a> {
    pub address: Cow<'a, [u8]>,
    pub coins: Vec<TW::Binance::Proto::mod_SendOrder::Token<'a>>,
}

impl<'a> MessageRead<'a> for Output<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.address = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.coins.push(r.read_message::<TW::Binance::Proto::mod_SendOrder::Token>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Output<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.address == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.address).len()) }
        + self.coins.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.address != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.address))?; }
        for s in &self.coins { w.write_with_tag(18, |w| w.write_message(s))?; }
        Ok(())
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TokenIssueOrder<'a> {
    pub from: Cow<'a, [u8]>,
    pub name: Cow<'a, str>,
    pub symbol: Cow<'a, str>,
    pub total_supply: i64,
    pub mintable: bool,
}

impl<'a> MessageRead<'a> for TokenIssueOrder<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.from = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.name = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.symbol = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(32) => msg.total_supply = r.read_int64(bytes)?,
                Ok(40) => msg.mintable = r.read_bool(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TokenIssueOrder<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.from == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.from).len()) }
        + if self.name == "" { 0 } else { 1 + sizeof_len((&self.name).len()) }
        + if self.symbol == "" { 0 } else { 1 + sizeof_len((&self.symbol).len()) }
        + if self.total_supply == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.total_supply) as u64) }
        + if self.mintable == false { 0 } else { 1 + sizeof_varint(*(&self.mintable) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.from != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.from))?; }
        if self.name != "" { w.write_with_tag(18, |w| w.write_string(&**&self.name))?; }
        if self.symbol != "" { w.write_with_tag(26, |w| w.write_string(&**&self.symbol))?; }
        if self.total_supply != 0i64 { w.write_with_tag(32, |w| w.write_int64(*&self.total_supply))?; }
        if self.mintable != false { w.write_with_tag(40, |w| w.write_bool(*&self.mintable))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TokenMintOrder<'a> {
    pub from: Cow<'a, [u8]>,
    pub symbol: Cow<'a, str>,
    pub amount: i64,
}

impl<'a> MessageRead<'a> for TokenMintOrder<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.from = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.symbol = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.amount = r.read_int64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TokenMintOrder<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.from == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.from).len()) }
        + if self.symbol == "" { 0 } else { 1 + sizeof_len((&self.symbol).len()) }
        + if self.amount == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.from != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.from))?; }
        if self.symbol != "" { w.write_with_tag(18, |w| w.write_string(&**&self.symbol))?; }
        if self.amount != 0i64 { w.write_with_tag(24, |w| w.write_int64(*&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TokenBurnOrder<'a> {
    pub from: Cow<'a, [u8]>,
    pub symbol: Cow<'a, str>,
    pub amount: i64,
}

impl<'a> MessageRead<'a> for TokenBurnOrder<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.from = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.symbol = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.amount = r.read_int64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TokenBurnOrder<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.from == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.from).len()) }
        + if self.symbol == "" { 0 } else { 1 + sizeof_len((&self.symbol).len()) }
        + if self.amount == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.from != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.from))?; }
        if self.symbol != "" { w.write_with_tag(18, |w| w.write_string(&**&self.symbol))?; }
        if self.amount != 0i64 { w.write_with_tag(24, |w| w.write_int64(*&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TokenFreezeOrder<'a> {
    pub from: Cow<'a, [u8]>,
    pub symbol: Cow<'a, str>,
    pub amount: i64,
}

impl<'a> MessageRead<'a> for TokenFreezeOrder<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.from = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.symbol = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.amount = r.read_int64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TokenFreezeOrder<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.from == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.from).len()) }
        + if self.symbol == "" { 0 } else { 1 + sizeof_len((&self.symbol).len()) }
        + if self.amount == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.from != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.from))?; }
        if self.symbol != "" { w.write_with_tag(18, |w| w.write_string(&**&self.symbol))?; }
        if self.amount != 0i64 { w.write_with_tag(24, |w| w.write_int64(*&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TokenUnfreezeOrder<'a> {
    pub from: Cow<'a, [u8]>,
    pub symbol: Cow<'a, str>,
    pub amount: i64,
}

impl<'a> MessageRead<'a> for TokenUnfreezeOrder<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.from = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.symbol = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.amount = r.read_int64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TokenUnfreezeOrder<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.from == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.from).len()) }
        + if self.symbol == "" { 0 } else { 1 + sizeof_len((&self.symbol).len()) }
        + if self.amount == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.from != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.from))?; }
        if self.symbol != "" { w.write_with_tag(18, |w| w.write_string(&**&self.symbol))?; }
        if self.amount != 0i64 { w.write_with_tag(24, |w| w.write_int64(*&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct HTLTOrder<'a> {
    pub from: Cow<'a, [u8]>,
    pub to: Cow<'a, [u8]>,
    pub recipient_other_chain: Cow<'a, str>,
    pub sender_other_chain: Cow<'a, str>,
    pub random_number_hash: Cow<'a, [u8]>,
    pub timestamp: i64,
    pub amount: Vec<TW::Binance::Proto::mod_SendOrder::Token<'a>>,
    pub expected_income: Cow<'a, str>,
    pub height_span: i64,
    pub cross_chain: bool,
}

impl<'a> MessageRead<'a> for HTLTOrder<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.from = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.to = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.recipient_other_chain = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.sender_other_chain = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.random_number_hash = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(48) => msg.timestamp = r.read_int64(bytes)?,
                Ok(58) => msg.amount.push(r.read_message::<TW::Binance::Proto::mod_SendOrder::Token>(bytes)?),
                Ok(66) => msg.expected_income = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(72) => msg.height_span = r.read_int64(bytes)?,
                Ok(80) => msg.cross_chain = r.read_bool(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for HTLTOrder<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.from == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.from).len()) }
        + if self.to == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.to).len()) }
        + if self.recipient_other_chain == "" { 0 } else { 1 + sizeof_len((&self.recipient_other_chain).len()) }
        + if self.sender_other_chain == "" { 0 } else { 1 + sizeof_len((&self.sender_other_chain).len()) }
        + if self.random_number_hash == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.random_number_hash).len()) }
        + if self.timestamp == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.timestamp) as u64) }
        + self.amount.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + if self.expected_income == "" { 0 } else { 1 + sizeof_len((&self.expected_income).len()) }
        + if self.height_span == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.height_span) as u64) }
        + if self.cross_chain == false { 0 } else { 1 + sizeof_varint(*(&self.cross_chain) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.from != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.from))?; }
        if self.to != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.to))?; }
        if self.recipient_other_chain != "" { w.write_with_tag(26, |w| w.write_string(&**&self.recipient_other_chain))?; }
        if self.sender_other_chain != "" { w.write_with_tag(34, |w| w.write_string(&**&self.sender_other_chain))?; }
        if self.random_number_hash != Cow::Borrowed(b"") { w.write_with_tag(42, |w| w.write_bytes(&**&self.random_number_hash))?; }
        if self.timestamp != 0i64 { w.write_with_tag(48, |w| w.write_int64(*&self.timestamp))?; }
        for s in &self.amount { w.write_with_tag(58, |w| w.write_message(s))?; }
        if self.expected_income != "" { w.write_with_tag(66, |w| w.write_string(&**&self.expected_income))?; }
        if self.height_span != 0i64 { w.write_with_tag(72, |w| w.write_int64(*&self.height_span))?; }
        if self.cross_chain != false { w.write_with_tag(80, |w| w.write_bool(*&self.cross_chain))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct DepositHTLTOrder<'a> {
    pub from: Cow<'a, [u8]>,
    pub amount: Vec<TW::Binance::Proto::mod_SendOrder::Token<'a>>,
    pub swap_id: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for DepositHTLTOrder<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.from = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.amount.push(r.read_message::<TW::Binance::Proto::mod_SendOrder::Token>(bytes)?),
                Ok(26) => msg.swap_id = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for DepositHTLTOrder<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.from == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.from).len()) }
        + self.amount.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + if self.swap_id == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.swap_id).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.from != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.from))?; }
        for s in &self.amount { w.write_with_tag(18, |w| w.write_message(s))?; }
        if self.swap_id != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.swap_id))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct ClaimHTLOrder<'a> {
    pub from: Cow<'a, [u8]>,
    pub swap_id: Cow<'a, [u8]>,
    pub random_number: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for ClaimHTLOrder<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.from = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.swap_id = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.random_number = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for ClaimHTLOrder<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.from == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.from).len()) }
        + if self.swap_id == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.swap_id).len()) }
        + if self.random_number == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.random_number).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.from != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.from))?; }
        if self.swap_id != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.swap_id))?; }
        if self.random_number != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.random_number))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct RefundHTLTOrder<'a> {
    pub from: Cow<'a, [u8]>,
    pub swap_id: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for RefundHTLTOrder<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.from = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.swap_id = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for RefundHTLTOrder<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.from == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.from).len()) }
        + if self.swap_id == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.swap_id).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.from != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.from))?; }
        if self.swap_id != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.swap_id))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TransferOut<'a> {
    pub from: Cow<'a, [u8]>,
    pub to: Cow<'a, [u8]>,
    pub amount: Option<TW::Binance::Proto::mod_SendOrder::Token<'a>>,
    pub expire_time: i64,
}

impl<'a> MessageRead<'a> for TransferOut<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.from = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.to = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.amount = Some(r.read_message::<TW::Binance::Proto::mod_SendOrder::Token>(bytes)?),
                Ok(32) => msg.expire_time = r.read_int64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TransferOut<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.from == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.from).len()) }
        + if self.to == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.to).len()) }
        + self.amount.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.expire_time == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.expire_time) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.from != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.from))?; }
        if self.to != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.to))?; }
        if let Some(ref s) = self.amount { w.write_with_tag(26, |w| w.write_message(s))?; }
        if self.expire_time != 0i64 { w.write_with_tag(32, |w| w.write_int64(*&self.expire_time))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SideChainDelegate<'a> {
    pub delegator_addr: Cow<'a, [u8]>,
    pub validator_addr: Cow<'a, [u8]>,
    pub delegation: Option<TW::Binance::Proto::mod_SendOrder::Token<'a>>,
    pub chain_id: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for SideChainDelegate<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.delegator_addr = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.validator_addr = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.delegation = Some(r.read_message::<TW::Binance::Proto::mod_SendOrder::Token>(bytes)?),
                Ok(34) => msg.chain_id = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for SideChainDelegate<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.delegator_addr == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.delegator_addr).len()) }
        + if self.validator_addr == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.validator_addr).len()) }
        + self.delegation.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.chain_id == "" { 0 } else { 1 + sizeof_len((&self.chain_id).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.delegator_addr != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.delegator_addr))?; }
        if self.validator_addr != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.validator_addr))?; }
        if let Some(ref s) = self.delegation { w.write_with_tag(26, |w| w.write_message(s))?; }
        if self.chain_id != "" { w.write_with_tag(34, |w| w.write_string(&**&self.chain_id))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SideChainRedelegate<'a> {
    pub delegator_addr: Cow<'a, [u8]>,
    pub validator_src_addr: Cow<'a, [u8]>,
    pub validator_dst_addr: Cow<'a, [u8]>,
    pub amount: Option<TW::Binance::Proto::mod_SendOrder::Token<'a>>,
    pub chain_id: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for SideChainRedelegate<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.delegator_addr = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.validator_src_addr = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.validator_dst_addr = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.amount = Some(r.read_message::<TW::Binance::Proto::mod_SendOrder::Token>(bytes)?),
                Ok(42) => msg.chain_id = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for SideChainRedelegate<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.delegator_addr == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.delegator_addr).len()) }
        + if self.validator_src_addr == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.validator_src_addr).len()) }
        + if self.validator_dst_addr == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.validator_dst_addr).len()) }
        + self.amount.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.chain_id == "" { 0 } else { 1 + sizeof_len((&self.chain_id).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.delegator_addr != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.delegator_addr))?; }
        if self.validator_src_addr != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.validator_src_addr))?; }
        if self.validator_dst_addr != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.validator_dst_addr))?; }
        if let Some(ref s) = self.amount { w.write_with_tag(34, |w| w.write_message(s))?; }
        if self.chain_id != "" { w.write_with_tag(42, |w| w.write_string(&**&self.chain_id))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SideChainUndelegate<'a> {
    pub delegator_addr: Cow<'a, [u8]>,
    pub validator_addr: Cow<'a, [u8]>,
    pub amount: Option<TW::Binance::Proto::mod_SendOrder::Token<'a>>,
    pub chain_id: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for SideChainUndelegate<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.delegator_addr = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.validator_addr = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.amount = Some(r.read_message::<TW::Binance::Proto::mod_SendOrder::Token>(bytes)?),
                Ok(34) => msg.chain_id = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for SideChainUndelegate<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.delegator_addr == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.delegator_addr).len()) }
        + if self.validator_addr == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.validator_addr).len()) }
        + self.amount.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.chain_id == "" { 0 } else { 1 + sizeof_len((&self.chain_id).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.delegator_addr != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.delegator_addr))?; }
        if self.validator_addr != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.validator_addr))?; }
        if let Some(ref s) = self.amount { w.write_with_tag(26, |w| w.write_message(s))?; }
        if self.chain_id != "" { w.write_with_tag(34, |w| w.write_string(&**&self.chain_id))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TimeLockOrder<'a> {
    pub from_address: Cow<'a, [u8]>,
    pub description: Cow<'a, str>,
    pub amount: Vec<TW::Binance::Proto::mod_SendOrder::Token<'a>>,
    pub lock_time: i64,
}

impl<'a> MessageRead<'a> for TimeLockOrder<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.from_address = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.description = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.amount.push(r.read_message::<TW::Binance::Proto::mod_SendOrder::Token>(bytes)?),
                Ok(32) => msg.lock_time = r.read_int64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TimeLockOrder<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.from_address == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.from_address).len()) }
        + if self.description == "" { 0 } else { 1 + sizeof_len((&self.description).len()) }
        + self.amount.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + if self.lock_time == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.lock_time) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.from_address != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.from_address))?; }
        if self.description != "" { w.write_with_tag(18, |w| w.write_string(&**&self.description))?; }
        for s in &self.amount { w.write_with_tag(26, |w| w.write_message(s))?; }
        if self.lock_time != 0i64 { w.write_with_tag(32, |w| w.write_int64(*&self.lock_time))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TimeRelockOrder<'a> {
    pub from_address: Cow<'a, [u8]>,
    pub id: i64,
    pub description: Cow<'a, str>,
    pub amount: Vec<TW::Binance::Proto::mod_SendOrder::Token<'a>>,
    pub lock_time: i64,
}

impl<'a> MessageRead<'a> for TimeRelockOrder<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.from_address = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.id = r.read_int64(bytes)?,
                Ok(26) => msg.description = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.amount.push(r.read_message::<TW::Binance::Proto::mod_SendOrder::Token>(bytes)?),
                Ok(40) => msg.lock_time = r.read_int64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TimeRelockOrder<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.from_address == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.from_address).len()) }
        + if self.id == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.id) as u64) }
        + if self.description == "" { 0 } else { 1 + sizeof_len((&self.description).len()) }
        + self.amount.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + if self.lock_time == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.lock_time) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.from_address != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.from_address))?; }
        if self.id != 0i64 { w.write_with_tag(16, |w| w.write_int64(*&self.id))?; }
        if self.description != "" { w.write_with_tag(26, |w| w.write_string(&**&self.description))?; }
        for s in &self.amount { w.write_with_tag(34, |w| w.write_message(s))?; }
        if self.lock_time != 0i64 { w.write_with_tag(40, |w| w.write_int64(*&self.lock_time))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TimeUnlockOrder<'a> {
    pub from_address: Cow<'a, [u8]>,
    pub id: i64,
}

impl<'a> MessageRead<'a> for TimeUnlockOrder<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.from_address = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.id = r.read_int64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TimeUnlockOrder<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.from_address == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.from_address).len()) }
        + if self.id == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.id) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.from_address != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.from_address))?; }
        if self.id != 0i64 { w.write_with_tag(16, |w| w.write_int64(*&self.id))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningInput<'a> {
    pub chain_id: Cow<'a, str>,
    pub account_number: i64,
    pub sequence: i64,
    pub source: i64,
    pub memo: Cow<'a, str>,
    pub private_key: Cow<'a, [u8]>,
    pub order_oneof: TW::Binance::Proto::mod_SigningInput::OneOforder_oneof<'a>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.chain_id = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.account_number = r.read_int64(bytes)?,
                Ok(24) => msg.sequence = r.read_int64(bytes)?,
                Ok(32) => msg.source = r.read_int64(bytes)?,
                Ok(42) => msg.memo = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(50) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(66) => msg.order_oneof = TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::trade_order(r.read_message::<TW::Binance::Proto::TradeOrder>(bytes)?),
                Ok(74) => msg.order_oneof = TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::cancel_trade_order(r.read_message::<TW::Binance::Proto::CancelTradeOrder>(bytes)?),
                Ok(82) => msg.order_oneof = TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::send_order(r.read_message::<TW::Binance::Proto::SendOrder>(bytes)?),
                Ok(90) => msg.order_oneof = TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::freeze_order(r.read_message::<TW::Binance::Proto::TokenFreezeOrder>(bytes)?),
                Ok(98) => msg.order_oneof = TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::unfreeze_order(r.read_message::<TW::Binance::Proto::TokenUnfreezeOrder>(bytes)?),
                Ok(106) => msg.order_oneof = TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::htlt_order(r.read_message::<TW::Binance::Proto::HTLTOrder>(bytes)?),
                Ok(114) => msg.order_oneof = TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::depositHTLT_order(r.read_message::<TW::Binance::Proto::DepositHTLTOrder>(bytes)?),
                Ok(122) => msg.order_oneof = TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::claimHTLT_order(r.read_message::<TW::Binance::Proto::ClaimHTLOrder>(bytes)?),
                Ok(130) => msg.order_oneof = TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::refundHTLT_order(r.read_message::<TW::Binance::Proto::RefundHTLTOrder>(bytes)?),
                Ok(138) => msg.order_oneof = TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::issue_order(r.read_message::<TW::Binance::Proto::TokenIssueOrder>(bytes)?),
                Ok(146) => msg.order_oneof = TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::mint_order(r.read_message::<TW::Binance::Proto::TokenMintOrder>(bytes)?),
                Ok(154) => msg.order_oneof = TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::burn_order(r.read_message::<TW::Binance::Proto::TokenBurnOrder>(bytes)?),
                Ok(162) => msg.order_oneof = TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::transfer_out_order(r.read_message::<TW::Binance::Proto::TransferOut>(bytes)?),
                Ok(170) => msg.order_oneof = TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::side_delegate_order(r.read_message::<TW::Binance::Proto::SideChainDelegate>(bytes)?),
                Ok(178) => msg.order_oneof = TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::side_redelegate_order(r.read_message::<TW::Binance::Proto::SideChainRedelegate>(bytes)?),
                Ok(186) => msg.order_oneof = TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::side_undelegate_order(r.read_message::<TW::Binance::Proto::SideChainUndelegate>(bytes)?),
                Ok(194) => msg.order_oneof = TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::time_lock_order(r.read_message::<TW::Binance::Proto::TimeLockOrder>(bytes)?),
                Ok(202) => msg.order_oneof = TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::time_relock_order(r.read_message::<TW::Binance::Proto::TimeRelockOrder>(bytes)?),
                Ok(210) => msg.order_oneof = TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::time_unlock_order(r.read_message::<TW::Binance::Proto::TimeUnlockOrder>(bytes)?),
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
        + if self.account_number == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.account_number) as u64) }
        + if self.sequence == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.sequence) as u64) }
        + if self.source == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.source) as u64) }
        + if self.memo == "" { 0 } else { 1 + sizeof_len((&self.memo).len()) }
        + if self.private_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.private_key).len()) }
        + match self.order_oneof {
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::trade_order(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::cancel_trade_order(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::send_order(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::freeze_order(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::unfreeze_order(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::htlt_order(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::depositHTLT_order(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::claimHTLT_order(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::refundHTLT_order(ref m) => 2 + sizeof_len((m).get_size()),
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::issue_order(ref m) => 2 + sizeof_len((m).get_size()),
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::mint_order(ref m) => 2 + sizeof_len((m).get_size()),
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::burn_order(ref m) => 2 + sizeof_len((m).get_size()),
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::transfer_out_order(ref m) => 2 + sizeof_len((m).get_size()),
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::side_delegate_order(ref m) => 2 + sizeof_len((m).get_size()),
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::side_redelegate_order(ref m) => 2 + sizeof_len((m).get_size()),
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::side_undelegate_order(ref m) => 2 + sizeof_len((m).get_size()),
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::time_lock_order(ref m) => 2 + sizeof_len((m).get_size()),
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::time_relock_order(ref m) => 2 + sizeof_len((m).get_size()),
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::time_unlock_order(ref m) => 2 + sizeof_len((m).get_size()),
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.chain_id != "" { w.write_with_tag(10, |w| w.write_string(&**&self.chain_id))?; }
        if self.account_number != 0i64 { w.write_with_tag(16, |w| w.write_int64(*&self.account_number))?; }
        if self.sequence != 0i64 { w.write_with_tag(24, |w| w.write_int64(*&self.sequence))?; }
        if self.source != 0i64 { w.write_with_tag(32, |w| w.write_int64(*&self.source))?; }
        if self.memo != "" { w.write_with_tag(42, |w| w.write_string(&**&self.memo))?; }
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(50, |w| w.write_bytes(&**&self.private_key))?; }
        match self.order_oneof {            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::trade_order(ref m) => { w.write_with_tag(66, |w| w.write_message(m))? },
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::cancel_trade_order(ref m) => { w.write_with_tag(74, |w| w.write_message(m))? },
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::send_order(ref m) => { w.write_with_tag(82, |w| w.write_message(m))? },
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::freeze_order(ref m) => { w.write_with_tag(90, |w| w.write_message(m))? },
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::unfreeze_order(ref m) => { w.write_with_tag(98, |w| w.write_message(m))? },
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::htlt_order(ref m) => { w.write_with_tag(106, |w| w.write_message(m))? },
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::depositHTLT_order(ref m) => { w.write_with_tag(114, |w| w.write_message(m))? },
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::claimHTLT_order(ref m) => { w.write_with_tag(122, |w| w.write_message(m))? },
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::refundHTLT_order(ref m) => { w.write_with_tag(130, |w| w.write_message(m))? },
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::issue_order(ref m) => { w.write_with_tag(138, |w| w.write_message(m))? },
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::mint_order(ref m) => { w.write_with_tag(146, |w| w.write_message(m))? },
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::burn_order(ref m) => { w.write_with_tag(154, |w| w.write_message(m))? },
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::transfer_out_order(ref m) => { w.write_with_tag(162, |w| w.write_message(m))? },
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::side_delegate_order(ref m) => { w.write_with_tag(170, |w| w.write_message(m))? },
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::side_redelegate_order(ref m) => { w.write_with_tag(178, |w| w.write_message(m))? },
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::side_undelegate_order(ref m) => { w.write_with_tag(186, |w| w.write_message(m))? },
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::time_lock_order(ref m) => { w.write_with_tag(194, |w| w.write_message(m))? },
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::time_relock_order(ref m) => { w.write_with_tag(202, |w| w.write_message(m))? },
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::time_unlock_order(ref m) => { w.write_with_tag(210, |w| w.write_message(m))? },
            TW::Binance::Proto::mod_SigningInput::OneOforder_oneof::None => {},
    }        Ok(())
    }
}

pub mod mod_SigningInput {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOforder_oneof<'a> {
    trade_order(TW::Binance::Proto::TradeOrder<'a>),
    cancel_trade_order(TW::Binance::Proto::CancelTradeOrder<'a>),
    send_order(TW::Binance::Proto::SendOrder<'a>),
    freeze_order(TW::Binance::Proto::TokenFreezeOrder<'a>),
    unfreeze_order(TW::Binance::Proto::TokenUnfreezeOrder<'a>),
    htlt_order(TW::Binance::Proto::HTLTOrder<'a>),
    depositHTLT_order(TW::Binance::Proto::DepositHTLTOrder<'a>),
    claimHTLT_order(TW::Binance::Proto::ClaimHTLOrder<'a>),
    refundHTLT_order(TW::Binance::Proto::RefundHTLTOrder<'a>),
    issue_order(TW::Binance::Proto::TokenIssueOrder<'a>),
    mint_order(TW::Binance::Proto::TokenMintOrder<'a>),
    burn_order(TW::Binance::Proto::TokenBurnOrder<'a>),
    transfer_out_order(TW::Binance::Proto::TransferOut<'a>),
    side_delegate_order(TW::Binance::Proto::SideChainDelegate<'a>),
    side_redelegate_order(TW::Binance::Proto::SideChainRedelegate<'a>),
    side_undelegate_order(TW::Binance::Proto::SideChainUndelegate<'a>),
    time_lock_order(TW::Binance::Proto::TimeLockOrder<'a>),
    time_relock_order(TW::Binance::Proto::TimeRelockOrder<'a>),
    time_unlock_order(TW::Binance::Proto::TimeUnlockOrder<'a>),
    None,
}

impl<'a> Default for OneOforder_oneof<'a> {
    fn default() -> Self {
        OneOforder_oneof::None
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningOutput<'a> {
    pub encoded: Cow<'a, [u8]>,
    pub error: TW::Common::Proto::SigningError,
    pub error_message: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.encoded = r.read_bytes(bytes).map(Cow::Borrowed)?,
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
        + if self.encoded == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.encoded).len()) }
        + if self.error == TW::Common::Proto::SigningError::OK { 0 } else { 1 + sizeof_varint(*(&self.error) as u64) }
        + if self.error_message == "" { 0 } else { 1 + sizeof_len((&self.error_message).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.encoded != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.encoded))?; }
        if self.error != TW::Common::Proto::SigningError::OK { w.write_with_tag(16, |w| w.write_enum(*&self.error as i32))?; }
        if self.error_message != "" { w.write_with_tag(26, |w| w.write_string(&**&self.error_message))?; }
        Ok(())
    }
}

