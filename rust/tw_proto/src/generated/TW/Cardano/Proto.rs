// Automatically generated rust module for 'Cardano.proto' file

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
pub struct OutPoint<'a> {
    pub tx_hash: Cow<'a, [u8]>,
    pub output_index: u64,
}

impl<'a> MessageRead<'a> for OutPoint<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.tx_hash = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.output_index = r.read_uint64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for OutPoint<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.tx_hash == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.tx_hash).len()) }
        + if self.output_index == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.output_index) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.tx_hash != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.tx_hash))?; }
        if self.output_index != 0u64 { w.write_with_tag(16, |w| w.write_uint64(*&self.output_index))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TokenAmount<'a> {
    pub policy_id: Cow<'a, str>,
    pub asset_name: Cow<'a, str>,
    pub amount: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for TokenAmount<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.policy_id = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.asset_name = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.amount = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TokenAmount<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.policy_id == "" { 0 } else { 1 + sizeof_len((&self.policy_id).len()) }
        + if self.asset_name == "" { 0 } else { 1 + sizeof_len((&self.asset_name).len()) }
        + if self.amount == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.amount).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.policy_id != "" { w.write_with_tag(10, |w| w.write_string(&**&self.policy_id))?; }
        if self.asset_name != "" { w.write_with_tag(18, |w| w.write_string(&**&self.asset_name))?; }
        if self.amount != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TxInput<'a> {
    pub out_point: Option<TW::Cardano::Proto::OutPoint<'a>>,
    pub address: Cow<'a, str>,
    pub amount: u64,
    pub token_amount: Vec<TW::Cardano::Proto::TokenAmount<'a>>,
}

impl<'a> MessageRead<'a> for TxInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.out_point = Some(r.read_message::<TW::Cardano::Proto::OutPoint>(bytes)?),
                Ok(18) => msg.address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.amount = r.read_uint64(bytes)?,
                Ok(34) => msg.token_amount.push(r.read_message::<TW::Cardano::Proto::TokenAmount>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TxInput<'a> {
    fn get_size(&self) -> usize {
        0
        + self.out_point.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.address == "" { 0 } else { 1 + sizeof_len((&self.address).len()) }
        + if self.amount == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
        + self.token_amount.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.out_point { w.write_with_tag(10, |w| w.write_message(s))?; }
        if self.address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.address))?; }
        if self.amount != 0u64 { w.write_with_tag(24, |w| w.write_uint64(*&self.amount))?; }
        for s in &self.token_amount { w.write_with_tag(34, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TxOutput<'a> {
    pub address: Cow<'a, str>,
    pub amount: u64,
    pub token_amount: Vec<TW::Cardano::Proto::TokenAmount<'a>>,
}

impl<'a> MessageRead<'a> for TxOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.amount = r.read_uint64(bytes)?,
                Ok(26) => msg.token_amount.push(r.read_message::<TW::Cardano::Proto::TokenAmount>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TxOutput<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.address == "" { 0 } else { 1 + sizeof_len((&self.address).len()) }
        + if self.amount == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
        + self.token_amount.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.address))?; }
        if self.amount != 0u64 { w.write_with_tag(16, |w| w.write_uint64(*&self.amount))?; }
        for s in &self.token_amount { w.write_with_tag(26, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TokenBundle<'a> {
    pub token: Vec<TW::Cardano::Proto::TokenAmount<'a>>,
}

impl<'a> MessageRead<'a> for TokenBundle<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.token.push(r.read_message::<TW::Cardano::Proto::TokenAmount>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TokenBundle<'a> {
    fn get_size(&self) -> usize {
        0
        + self.token.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        for s in &self.token { w.write_with_tag(10, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Transfer<'a> {
    pub to_address: Cow<'a, str>,
    pub change_address: Cow<'a, str>,
    pub amount: u64,
    pub token_amount: Option<TW::Cardano::Proto::TokenBundle<'a>>,
    pub use_max_amount: bool,
    pub force_fee: u64,
}

impl<'a> MessageRead<'a> for Transfer<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.to_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.change_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.amount = r.read_uint64(bytes)?,
                Ok(34) => msg.token_amount = Some(r.read_message::<TW::Cardano::Proto::TokenBundle>(bytes)?),
                Ok(40) => msg.use_max_amount = r.read_bool(bytes)?,
                Ok(48) => msg.force_fee = r.read_uint64(bytes)?,
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
        + if self.change_address == "" { 0 } else { 1 + sizeof_len((&self.change_address).len()) }
        + if self.amount == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
        + self.token_amount.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.use_max_amount == false { 0 } else { 1 + sizeof_varint(*(&self.use_max_amount) as u64) }
        + if self.force_fee == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.force_fee) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.to_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.to_address))?; }
        if self.change_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.change_address))?; }
        if self.amount != 0u64 { w.write_with_tag(24, |w| w.write_uint64(*&self.amount))?; }
        if let Some(ref s) = self.token_amount { w.write_with_tag(34, |w| w.write_message(s))?; }
        if self.use_max_amount != false { w.write_with_tag(40, |w| w.write_bool(*&self.use_max_amount))?; }
        if self.force_fee != 0u64 { w.write_with_tag(48, |w| w.write_uint64(*&self.force_fee))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct RegisterStakingKey<'a> {
    pub staking_address: Cow<'a, str>,
    pub deposit_amount: u64,
}

impl<'a> MessageRead<'a> for RegisterStakingKey<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.staking_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.deposit_amount = r.read_uint64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for RegisterStakingKey<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.staking_address == "" { 0 } else { 1 + sizeof_len((&self.staking_address).len()) }
        + if self.deposit_amount == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.deposit_amount) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.staking_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.staking_address))?; }
        if self.deposit_amount != 0u64 { w.write_with_tag(16, |w| w.write_uint64(*&self.deposit_amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct DeregisterStakingKey<'a> {
    pub staking_address: Cow<'a, str>,
    pub undeposit_amount: u64,
}

impl<'a> MessageRead<'a> for DeregisterStakingKey<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.staking_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.undeposit_amount = r.read_uint64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for DeregisterStakingKey<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.staking_address == "" { 0 } else { 1 + sizeof_len((&self.staking_address).len()) }
        + if self.undeposit_amount == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.undeposit_amount) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.staking_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.staking_address))?; }
        if self.undeposit_amount != 0u64 { w.write_with_tag(16, |w| w.write_uint64(*&self.undeposit_amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Delegate<'a> {
    pub staking_address: Cow<'a, str>,
    pub pool_id: Cow<'a, [u8]>,
    pub deposit_amount: u64,
}

impl<'a> MessageRead<'a> for Delegate<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.staking_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.pool_id = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.deposit_amount = r.read_uint64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Delegate<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.staking_address == "" { 0 } else { 1 + sizeof_len((&self.staking_address).len()) }
        + if self.pool_id == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.pool_id).len()) }
        + if self.deposit_amount == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.deposit_amount) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.staking_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.staking_address))?; }
        if self.pool_id != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.pool_id))?; }
        if self.deposit_amount != 0u64 { w.write_with_tag(24, |w| w.write_uint64(*&self.deposit_amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Withdraw<'a> {
    pub staking_address: Cow<'a, str>,
    pub withdraw_amount: u64,
}

impl<'a> MessageRead<'a> for Withdraw<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.staking_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.withdraw_amount = r.read_uint64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Withdraw<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.staking_address == "" { 0 } else { 1 + sizeof_len((&self.staking_address).len()) }
        + if self.withdraw_amount == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.withdraw_amount) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.staking_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.staking_address))?; }
        if self.withdraw_amount != 0u64 { w.write_with_tag(16, |w| w.write_uint64(*&self.withdraw_amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TransactionPlan<'a> {
    pub available_amount: u64,
    pub amount: u64,
    pub fee: u64,
    pub change: u64,
    pub deposit: u64,
    pub undeposit: u64,
    pub available_tokens: Vec<TW::Cardano::Proto::TokenAmount<'a>>,
    pub output_tokens: Vec<TW::Cardano::Proto::TokenAmount<'a>>,
    pub change_tokens: Vec<TW::Cardano::Proto::TokenAmount<'a>>,
    pub utxos: Vec<TW::Cardano::Proto::TxInput<'a>>,
    pub error: TW::Common::Proto::SigningError,
}

impl<'a> MessageRead<'a> for TransactionPlan<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.available_amount = r.read_uint64(bytes)?,
                Ok(16) => msg.amount = r.read_uint64(bytes)?,
                Ok(24) => msg.fee = r.read_uint64(bytes)?,
                Ok(32) => msg.change = r.read_uint64(bytes)?,
                Ok(80) => msg.deposit = r.read_uint64(bytes)?,
                Ok(88) => msg.undeposit = r.read_uint64(bytes)?,
                Ok(42) => msg.available_tokens.push(r.read_message::<TW::Cardano::Proto::TokenAmount>(bytes)?),
                Ok(50) => msg.output_tokens.push(r.read_message::<TW::Cardano::Proto::TokenAmount>(bytes)?),
                Ok(58) => msg.change_tokens.push(r.read_message::<TW::Cardano::Proto::TokenAmount>(bytes)?),
                Ok(66) => msg.utxos.push(r.read_message::<TW::Cardano::Proto::TxInput>(bytes)?),
                Ok(72) => msg.error = r.read_enum(bytes)?,
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
        + if self.available_amount == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.available_amount) as u64) }
        + if self.amount == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
        + if self.fee == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.fee) as u64) }
        + if self.change == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.change) as u64) }
        + if self.deposit == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.deposit) as u64) }
        + if self.undeposit == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.undeposit) as u64) }
        + self.available_tokens.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + self.output_tokens.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + self.change_tokens.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + self.utxos.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + if self.error == TW::Common::Proto::SigningError::OK { 0 } else { 1 + sizeof_varint(*(&self.error) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.available_amount != 0u64 { w.write_with_tag(8, |w| w.write_uint64(*&self.available_amount))?; }
        if self.amount != 0u64 { w.write_with_tag(16, |w| w.write_uint64(*&self.amount))?; }
        if self.fee != 0u64 { w.write_with_tag(24, |w| w.write_uint64(*&self.fee))?; }
        if self.change != 0u64 { w.write_with_tag(32, |w| w.write_uint64(*&self.change))?; }
        if self.deposit != 0u64 { w.write_with_tag(80, |w| w.write_uint64(*&self.deposit))?; }
        if self.undeposit != 0u64 { w.write_with_tag(88, |w| w.write_uint64(*&self.undeposit))?; }
        for s in &self.available_tokens { w.write_with_tag(42, |w| w.write_message(s))?; }
        for s in &self.output_tokens { w.write_with_tag(50, |w| w.write_message(s))?; }
        for s in &self.change_tokens { w.write_with_tag(58, |w| w.write_message(s))?; }
        for s in &self.utxos { w.write_with_tag(66, |w| w.write_message(s))?; }
        if self.error != TW::Common::Proto::SigningError::OK { w.write_with_tag(72, |w| w.write_enum(*&self.error as i32))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningInput<'a> {
    pub utxos: Vec<TW::Cardano::Proto::TxInput<'a>>,
    pub private_key: Vec<Cow<'a, [u8]>>,
    pub transfer_message: Option<TW::Cardano::Proto::Transfer<'a>>,
    pub register_staking_key: Option<TW::Cardano::Proto::RegisterStakingKey<'a>>,
    pub delegate: Option<TW::Cardano::Proto::Delegate<'a>>,
    pub withdraw: Option<TW::Cardano::Proto::Withdraw<'a>>,
    pub deregister_staking_key: Option<TW::Cardano::Proto::DeregisterStakingKey<'a>>,
    pub ttl: u64,
    pub plan: Option<TW::Cardano::Proto::TransactionPlan<'a>>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.utxos.push(r.read_message::<TW::Cardano::Proto::TxInput>(bytes)?),
                Ok(18) => msg.private_key.push(r.read_bytes(bytes).map(Cow::Borrowed)?),
                Ok(26) => msg.transfer_message = Some(r.read_message::<TW::Cardano::Proto::Transfer>(bytes)?),
                Ok(50) => msg.register_staking_key = Some(r.read_message::<TW::Cardano::Proto::RegisterStakingKey>(bytes)?),
                Ok(58) => msg.delegate = Some(r.read_message::<TW::Cardano::Proto::Delegate>(bytes)?),
                Ok(66) => msg.withdraw = Some(r.read_message::<TW::Cardano::Proto::Withdraw>(bytes)?),
                Ok(74) => msg.deregister_staking_key = Some(r.read_message::<TW::Cardano::Proto::DeregisterStakingKey>(bytes)?),
                Ok(32) => msg.ttl = r.read_uint64(bytes)?,
                Ok(42) => msg.plan = Some(r.read_message::<TW::Cardano::Proto::TransactionPlan>(bytes)?),
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
        + self.utxos.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + self.private_key.iter().map(|s| 1 + sizeof_len((s).len())).sum::<usize>()
        + self.transfer_message.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + self.register_staking_key.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + self.delegate.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + self.withdraw.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + self.deregister_staking_key.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.ttl == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.ttl) as u64) }
        + self.plan.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        for s in &self.utxos { w.write_with_tag(10, |w| w.write_message(s))?; }
        for s in &self.private_key { w.write_with_tag(18, |w| w.write_bytes(&**s))?; }
        if let Some(ref s) = self.transfer_message { w.write_with_tag(26, |w| w.write_message(s))?; }
        if let Some(ref s) = self.register_staking_key { w.write_with_tag(50, |w| w.write_message(s))?; }
        if let Some(ref s) = self.delegate { w.write_with_tag(58, |w| w.write_message(s))?; }
        if let Some(ref s) = self.withdraw { w.write_with_tag(66, |w| w.write_message(s))?; }
        if let Some(ref s) = self.deregister_staking_key { w.write_with_tag(74, |w| w.write_message(s))?; }
        if self.ttl != 0u64 { w.write_with_tag(32, |w| w.write_uint64(*&self.ttl))?; }
        if let Some(ref s) = self.plan { w.write_with_tag(42, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningOutput<'a> {
    pub encoded: Cow<'a, [u8]>,
    pub tx_id: Cow<'a, [u8]>,
    pub error: TW::Common::Proto::SigningError,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.encoded = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.tx_id = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.error = r.read_enum(bytes)?,
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
        + if self.tx_id == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.tx_id).len()) }
        + if self.error == TW::Common::Proto::SigningError::OK { 0 } else { 1 + sizeof_varint(*(&self.error) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.encoded != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.encoded))?; }
        if self.tx_id != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.tx_id))?; }
        if self.error != TW::Common::Proto::SigningError::OK { w.write_with_tag(24, |w| w.write_enum(*&self.error as i32))?; }
        Ok(())
    }
}

