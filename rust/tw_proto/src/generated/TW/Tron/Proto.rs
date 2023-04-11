// Automatically generated rust module for 'Tron.proto' file

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
pub struct TransferContract<'a> {
    pub owner_address: Cow<'a, str>,
    pub to_address: Cow<'a, str>,
    pub amount: i64,
}

impl<'a> MessageRead<'a> for TransferContract<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.owner_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.to_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.amount = r.read_int64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TransferContract<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.owner_address == "" { 0 } else { 1 + sizeof_len((&self.owner_address).len()) }
        + if self.to_address == "" { 0 } else { 1 + sizeof_len((&self.to_address).len()) }
        + if self.amount == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.owner_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.owner_address))?; }
        if self.to_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.to_address))?; }
        if self.amount != 0i64 { w.write_with_tag(24, |w| w.write_int64(*&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TransferAssetContract<'a> {
    pub asset_name: Cow<'a, str>,
    pub owner_address: Cow<'a, str>,
    pub to_address: Cow<'a, str>,
    pub amount: i64,
}

impl<'a> MessageRead<'a> for TransferAssetContract<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.asset_name = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.owner_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.to_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(32) => msg.amount = r.read_int64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TransferAssetContract<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.asset_name == "" { 0 } else { 1 + sizeof_len((&self.asset_name).len()) }
        + if self.owner_address == "" { 0 } else { 1 + sizeof_len((&self.owner_address).len()) }
        + if self.to_address == "" { 0 } else { 1 + sizeof_len((&self.to_address).len()) }
        + if self.amount == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.asset_name != "" { w.write_with_tag(10, |w| w.write_string(&**&self.asset_name))?; }
        if self.owner_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.owner_address))?; }
        if self.to_address != "" { w.write_with_tag(26, |w| w.write_string(&**&self.to_address))?; }
        if self.amount != 0i64 { w.write_with_tag(32, |w| w.write_int64(*&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TransferTRC20Contract<'a> {
    pub contract_address: Cow<'a, str>,
    pub owner_address: Cow<'a, str>,
    pub to_address: Cow<'a, str>,
    pub amount: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for TransferTRC20Contract<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.contract_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.owner_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.to_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.amount = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TransferTRC20Contract<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.contract_address == "" { 0 } else { 1 + sizeof_len((&self.contract_address).len()) }
        + if self.owner_address == "" { 0 } else { 1 + sizeof_len((&self.owner_address).len()) }
        + if self.to_address == "" { 0 } else { 1 + sizeof_len((&self.to_address).len()) }
        + if self.amount == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.amount).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.contract_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.contract_address))?; }
        if self.owner_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.owner_address))?; }
        if self.to_address != "" { w.write_with_tag(26, |w| w.write_string(&**&self.to_address))?; }
        if self.amount != Cow::Borrowed(b"") { w.write_with_tag(34, |w| w.write_bytes(&**&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct FreezeBalanceContract<'a> {
    pub owner_address: Cow<'a, str>,
    pub frozen_balance: i64,
    pub frozen_duration: i64,
    pub resource: Cow<'a, str>,
    pub receiver_address: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for FreezeBalanceContract<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.owner_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.frozen_balance = r.read_int64(bytes)?,
                Ok(24) => msg.frozen_duration = r.read_int64(bytes)?,
                Ok(82) => msg.resource = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(122) => msg.receiver_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for FreezeBalanceContract<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.owner_address == "" { 0 } else { 1 + sizeof_len((&self.owner_address).len()) }
        + if self.frozen_balance == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.frozen_balance) as u64) }
        + if self.frozen_duration == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.frozen_duration) as u64) }
        + if self.resource == "" { 0 } else { 1 + sizeof_len((&self.resource).len()) }
        + if self.receiver_address == "" { 0 } else { 1 + sizeof_len((&self.receiver_address).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.owner_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.owner_address))?; }
        if self.frozen_balance != 0i64 { w.write_with_tag(16, |w| w.write_int64(*&self.frozen_balance))?; }
        if self.frozen_duration != 0i64 { w.write_with_tag(24, |w| w.write_int64(*&self.frozen_duration))?; }
        if self.resource != "" { w.write_with_tag(82, |w| w.write_string(&**&self.resource))?; }
        if self.receiver_address != "" { w.write_with_tag(122, |w| w.write_string(&**&self.receiver_address))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct FreezeBalanceV2Contract<'a> {
    pub owner_address: Cow<'a, str>,
    pub frozen_balance: i64,
    pub resource: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for FreezeBalanceV2Contract<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.owner_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.frozen_balance = r.read_int64(bytes)?,
                Ok(26) => msg.resource = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for FreezeBalanceV2Contract<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.owner_address == "" { 0 } else { 1 + sizeof_len((&self.owner_address).len()) }
        + if self.frozen_balance == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.frozen_balance) as u64) }
        + if self.resource == "" { 0 } else { 1 + sizeof_len((&self.resource).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.owner_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.owner_address))?; }
        if self.frozen_balance != 0i64 { w.write_with_tag(16, |w| w.write_int64(*&self.frozen_balance))?; }
        if self.resource != "" { w.write_with_tag(26, |w| w.write_string(&**&self.resource))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct UnfreezeBalanceV2Contract<'a> {
    pub owner_address: Cow<'a, str>,
    pub unfreeze_balance: i64,
    pub resource: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for UnfreezeBalanceV2Contract<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.owner_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.unfreeze_balance = r.read_int64(bytes)?,
                Ok(26) => msg.resource = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for UnfreezeBalanceV2Contract<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.owner_address == "" { 0 } else { 1 + sizeof_len((&self.owner_address).len()) }
        + if self.unfreeze_balance == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.unfreeze_balance) as u64) }
        + if self.resource == "" { 0 } else { 1 + sizeof_len((&self.resource).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.owner_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.owner_address))?; }
        if self.unfreeze_balance != 0i64 { w.write_with_tag(16, |w| w.write_int64(*&self.unfreeze_balance))?; }
        if self.resource != "" { w.write_with_tag(26, |w| w.write_string(&**&self.resource))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct WithdrawExpireUnfreezeContract<'a> {
    pub owner_address: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for WithdrawExpireUnfreezeContract<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.owner_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for WithdrawExpireUnfreezeContract<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.owner_address == "" { 0 } else { 1 + sizeof_len((&self.owner_address).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.owner_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.owner_address))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct DelegateResourceContract<'a> {
    pub owner_address: Cow<'a, str>,
    pub resource: Cow<'a, str>,
    pub balance: i64,
    pub receiver_address: Cow<'a, str>,
    pub lock: bool,
}

impl<'a> MessageRead<'a> for DelegateResourceContract<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.owner_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.resource = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.balance = r.read_int64(bytes)?,
                Ok(34) => msg.receiver_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(40) => msg.lock = r.read_bool(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for DelegateResourceContract<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.owner_address == "" { 0 } else { 1 + sizeof_len((&self.owner_address).len()) }
        + if self.resource == "" { 0 } else { 1 + sizeof_len((&self.resource).len()) }
        + if self.balance == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.balance) as u64) }
        + if self.receiver_address == "" { 0 } else { 1 + sizeof_len((&self.receiver_address).len()) }
        + if self.lock == false { 0 } else { 1 + sizeof_varint(*(&self.lock) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.owner_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.owner_address))?; }
        if self.resource != "" { w.write_with_tag(18, |w| w.write_string(&**&self.resource))?; }
        if self.balance != 0i64 { w.write_with_tag(24, |w| w.write_int64(*&self.balance))?; }
        if self.receiver_address != "" { w.write_with_tag(34, |w| w.write_string(&**&self.receiver_address))?; }
        if self.lock != false { w.write_with_tag(40, |w| w.write_bool(*&self.lock))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct UnDelegateResourceContract<'a> {
    pub owner_address: Cow<'a, str>,
    pub resource: Cow<'a, str>,
    pub balance: i64,
    pub receiver_address: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for UnDelegateResourceContract<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.owner_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.resource = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.balance = r.read_int64(bytes)?,
                Ok(34) => msg.receiver_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for UnDelegateResourceContract<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.owner_address == "" { 0 } else { 1 + sizeof_len((&self.owner_address).len()) }
        + if self.resource == "" { 0 } else { 1 + sizeof_len((&self.resource).len()) }
        + if self.balance == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.balance) as u64) }
        + if self.receiver_address == "" { 0 } else { 1 + sizeof_len((&self.receiver_address).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.owner_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.owner_address))?; }
        if self.resource != "" { w.write_with_tag(18, |w| w.write_string(&**&self.resource))?; }
        if self.balance != 0i64 { w.write_with_tag(24, |w| w.write_int64(*&self.balance))?; }
        if self.receiver_address != "" { w.write_with_tag(34, |w| w.write_string(&**&self.receiver_address))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct UnfreezeBalanceContract<'a> {
    pub owner_address: Cow<'a, str>,
    pub resource: Cow<'a, str>,
    pub receiver_address: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for UnfreezeBalanceContract<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.owner_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(82) => msg.resource = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(122) => msg.receiver_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for UnfreezeBalanceContract<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.owner_address == "" { 0 } else { 1 + sizeof_len((&self.owner_address).len()) }
        + if self.resource == "" { 0 } else { 1 + sizeof_len((&self.resource).len()) }
        + if self.receiver_address == "" { 0 } else { 1 + sizeof_len((&self.receiver_address).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.owner_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.owner_address))?; }
        if self.resource != "" { w.write_with_tag(82, |w| w.write_string(&**&self.resource))?; }
        if self.receiver_address != "" { w.write_with_tag(122, |w| w.write_string(&**&self.receiver_address))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct UnfreezeAssetContract<'a> {
    pub owner_address: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for UnfreezeAssetContract<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.owner_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for UnfreezeAssetContract<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.owner_address == "" { 0 } else { 1 + sizeof_len((&self.owner_address).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.owner_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.owner_address))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct VoteAssetContract<'a> {
    pub owner_address: Cow<'a, str>,
    pub vote_address: Vec<Cow<'a, str>>,
    pub support: bool,
    pub count: i32,
}

impl<'a> MessageRead<'a> for VoteAssetContract<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.owner_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.vote_address.push(r.read_string(bytes).map(Cow::Borrowed)?),
                Ok(24) => msg.support = r.read_bool(bytes)?,
                Ok(40) => msg.count = r.read_int32(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for VoteAssetContract<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.owner_address == "" { 0 } else { 1 + sizeof_len((&self.owner_address).len()) }
        + self.vote_address.iter().map(|s| 1 + sizeof_len((s).len())).sum::<usize>()
        + if self.support == false { 0 } else { 1 + sizeof_varint(*(&self.support) as u64) }
        + if self.count == 0i32 { 0 } else { 1 + sizeof_varint(*(&self.count) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.owner_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.owner_address))?; }
        for s in &self.vote_address { w.write_with_tag(18, |w| w.write_string(&**s))?; }
        if self.support != false { w.write_with_tag(24, |w| w.write_bool(*&self.support))?; }
        if self.count != 0i32 { w.write_with_tag(40, |w| w.write_int32(*&self.count))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct VoteWitnessContract<'a> {
    pub owner_address: Cow<'a, str>,
    pub votes: Vec<TW::Tron::Proto::mod_VoteWitnessContract::Vote<'a>>,
    pub support: bool,
}

impl<'a> MessageRead<'a> for VoteWitnessContract<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.owner_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.votes.push(r.read_message::<TW::Tron::Proto::mod_VoteWitnessContract::Vote>(bytes)?),
                Ok(24) => msg.support = r.read_bool(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for VoteWitnessContract<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.owner_address == "" { 0 } else { 1 + sizeof_len((&self.owner_address).len()) }
        + self.votes.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + if self.support == false { 0 } else { 1 + sizeof_varint(*(&self.support) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.owner_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.owner_address))?; }
        for s in &self.votes { w.write_with_tag(18, |w| w.write_message(s))?; }
        if self.support != false { w.write_with_tag(24, |w| w.write_bool(*&self.support))?; }
        Ok(())
    }
}

pub mod mod_VoteWitnessContract {

use std::borrow::Cow;
use super::*;

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Vote<'a> {
    pub vote_address: Cow<'a, str>,
    pub vote_count: i64,
}

impl<'a> MessageRead<'a> for Vote<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.vote_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.vote_count = r.read_int64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Vote<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.vote_address == "" { 0 } else { 1 + sizeof_len((&self.vote_address).len()) }
        + if self.vote_count == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.vote_count) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.vote_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.vote_address))?; }
        if self.vote_count != 0i64 { w.write_with_tag(16, |w| w.write_int64(*&self.vote_count))?; }
        Ok(())
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct WithdrawBalanceContract<'a> {
    pub owner_address: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for WithdrawBalanceContract<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.owner_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for WithdrawBalanceContract<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.owner_address == "" { 0 } else { 1 + sizeof_len((&self.owner_address).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.owner_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.owner_address))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TriggerSmartContract<'a> {
    pub owner_address: Cow<'a, str>,
    pub contract_address: Cow<'a, str>,
    pub call_value: i64,
    pub data: Cow<'a, [u8]>,
    pub call_token_value: i64,
    pub token_id: i64,
}

impl<'a> MessageRead<'a> for TriggerSmartContract<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.owner_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.contract_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.call_value = r.read_int64(bytes)?,
                Ok(34) => msg.data = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(40) => msg.call_token_value = r.read_int64(bytes)?,
                Ok(48) => msg.token_id = r.read_int64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TriggerSmartContract<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.owner_address == "" { 0 } else { 1 + sizeof_len((&self.owner_address).len()) }
        + if self.contract_address == "" { 0 } else { 1 + sizeof_len((&self.contract_address).len()) }
        + if self.call_value == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.call_value) as u64) }
        + if self.data == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.data).len()) }
        + if self.call_token_value == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.call_token_value) as u64) }
        + if self.token_id == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.token_id) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.owner_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.owner_address))?; }
        if self.contract_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.contract_address))?; }
        if self.call_value != 0i64 { w.write_with_tag(24, |w| w.write_int64(*&self.call_value))?; }
        if self.data != Cow::Borrowed(b"") { w.write_with_tag(34, |w| w.write_bytes(&**&self.data))?; }
        if self.call_token_value != 0i64 { w.write_with_tag(40, |w| w.write_int64(*&self.call_token_value))?; }
        if self.token_id != 0i64 { w.write_with_tag(48, |w| w.write_int64(*&self.token_id))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct BlockHeader<'a> {
    pub timestamp: i64,
    pub tx_trie_root: Cow<'a, [u8]>,
    pub parent_hash: Cow<'a, [u8]>,
    pub number: i64,
    pub witness_address: Cow<'a, [u8]>,
    pub version: i32,
}

impl<'a> MessageRead<'a> for BlockHeader<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.timestamp = r.read_int64(bytes)?,
                Ok(18) => msg.tx_trie_root = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.parent_hash = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(56) => msg.number = r.read_int64(bytes)?,
                Ok(74) => msg.witness_address = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(80) => msg.version = r.read_int32(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for BlockHeader<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.timestamp == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.timestamp) as u64) }
        + if self.tx_trie_root == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.tx_trie_root).len()) }
        + if self.parent_hash == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.parent_hash).len()) }
        + if self.number == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.number) as u64) }
        + if self.witness_address == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.witness_address).len()) }
        + if self.version == 0i32 { 0 } else { 1 + sizeof_varint(*(&self.version) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.timestamp != 0i64 { w.write_with_tag(8, |w| w.write_int64(*&self.timestamp))?; }
        if self.tx_trie_root != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.tx_trie_root))?; }
        if self.parent_hash != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.parent_hash))?; }
        if self.number != 0i64 { w.write_with_tag(56, |w| w.write_int64(*&self.number))?; }
        if self.witness_address != Cow::Borrowed(b"") { w.write_with_tag(74, |w| w.write_bytes(&**&self.witness_address))?; }
        if self.version != 0i32 { w.write_with_tag(80, |w| w.write_int32(*&self.version))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Transaction<'a> {
    pub timestamp: i64,
    pub expiration: i64,
    pub block_header: Option<TW::Tron::Proto::BlockHeader<'a>>,
    pub fee_limit: i64,
    pub contract_oneof: TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof<'a>,
}

impl<'a> MessageRead<'a> for Transaction<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.timestamp = r.read_int64(bytes)?,
                Ok(16) => msg.expiration = r.read_int64(bytes)?,
                Ok(26) => msg.block_header = Some(r.read_message::<TW::Tron::Proto::BlockHeader>(bytes)?),
                Ok(32) => msg.fee_limit = r.read_int64(bytes)?,
                Ok(82) => msg.contract_oneof = TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::transfer(r.read_message::<TW::Tron::Proto::TransferContract>(bytes)?),
                Ok(90) => msg.contract_oneof = TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::transfer_asset(r.read_message::<TW::Tron::Proto::TransferAssetContract>(bytes)?),
                Ok(98) => msg.contract_oneof = TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::freeze_balance(r.read_message::<TW::Tron::Proto::FreezeBalanceContract>(bytes)?),
                Ok(106) => msg.contract_oneof = TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::unfreeze_balance(r.read_message::<TW::Tron::Proto::UnfreezeBalanceContract>(bytes)?),
                Ok(114) => msg.contract_oneof = TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::unfreeze_asset(r.read_message::<TW::Tron::Proto::UnfreezeAssetContract>(bytes)?),
                Ok(122) => msg.contract_oneof = TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::withdraw_balance(r.read_message::<TW::Tron::Proto::WithdrawBalanceContract>(bytes)?),
                Ok(130) => msg.contract_oneof = TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::vote_asset(r.read_message::<TW::Tron::Proto::VoteAssetContract>(bytes)?),
                Ok(138) => msg.contract_oneof = TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::vote_witness(r.read_message::<TW::Tron::Proto::VoteWitnessContract>(bytes)?),
                Ok(146) => msg.contract_oneof = TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::trigger_smart_contract(r.read_message::<TW::Tron::Proto::TriggerSmartContract>(bytes)?),
                Ok(154) => msg.contract_oneof = TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::transfer_trc20_contract(r.read_message::<TW::Tron::Proto::TransferTRC20Contract>(bytes)?),
                Ok(162) => msg.contract_oneof = TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::freeze_balance_v2(r.read_message::<TW::Tron::Proto::FreezeBalanceV2Contract>(bytes)?),
                Ok(170) => msg.contract_oneof = TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::unfreeze_balance_v2(r.read_message::<TW::Tron::Proto::UnfreezeBalanceV2Contract>(bytes)?),
                Ok(186) => msg.contract_oneof = TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::withdraw_expire_unfreeze(r.read_message::<TW::Tron::Proto::WithdrawExpireUnfreezeContract>(bytes)?),
                Ok(194) => msg.contract_oneof = TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::delegate_resource(r.read_message::<TW::Tron::Proto::DelegateResourceContract>(bytes)?),
                Ok(202) => msg.contract_oneof = TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::undelegate_resource(r.read_message::<TW::Tron::Proto::UnDelegateResourceContract>(bytes)?),
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
        + if self.timestamp == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.timestamp) as u64) }
        + if self.expiration == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.expiration) as u64) }
        + self.block_header.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.fee_limit == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.fee_limit) as u64) }
        + match self.contract_oneof {
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::transfer(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::transfer_asset(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::freeze_balance(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::unfreeze_balance(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::unfreeze_asset(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::withdraw_balance(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::vote_asset(ref m) => 2 + sizeof_len((m).get_size()),
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::vote_witness(ref m) => 2 + sizeof_len((m).get_size()),
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::trigger_smart_contract(ref m) => 2 + sizeof_len((m).get_size()),
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::transfer_trc20_contract(ref m) => 2 + sizeof_len((m).get_size()),
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::freeze_balance_v2(ref m) => 2 + sizeof_len((m).get_size()),
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::unfreeze_balance_v2(ref m) => 2 + sizeof_len((m).get_size()),
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::withdraw_expire_unfreeze(ref m) => 2 + sizeof_len((m).get_size()),
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::delegate_resource(ref m) => 2 + sizeof_len((m).get_size()),
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::undelegate_resource(ref m) => 2 + sizeof_len((m).get_size()),
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.timestamp != 0i64 { w.write_with_tag(8, |w| w.write_int64(*&self.timestamp))?; }
        if self.expiration != 0i64 { w.write_with_tag(16, |w| w.write_int64(*&self.expiration))?; }
        if let Some(ref s) = self.block_header { w.write_with_tag(26, |w| w.write_message(s))?; }
        if self.fee_limit != 0i64 { w.write_with_tag(32, |w| w.write_int64(*&self.fee_limit))?; }
        match self.contract_oneof {            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::transfer(ref m) => { w.write_with_tag(82, |w| w.write_message(m))? },
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::transfer_asset(ref m) => { w.write_with_tag(90, |w| w.write_message(m))? },
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::freeze_balance(ref m) => { w.write_with_tag(98, |w| w.write_message(m))? },
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::unfreeze_balance(ref m) => { w.write_with_tag(106, |w| w.write_message(m))? },
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::unfreeze_asset(ref m) => { w.write_with_tag(114, |w| w.write_message(m))? },
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::withdraw_balance(ref m) => { w.write_with_tag(122, |w| w.write_message(m))? },
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::vote_asset(ref m) => { w.write_with_tag(130, |w| w.write_message(m))? },
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::vote_witness(ref m) => { w.write_with_tag(138, |w| w.write_message(m))? },
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::trigger_smart_contract(ref m) => { w.write_with_tag(146, |w| w.write_message(m))? },
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::transfer_trc20_contract(ref m) => { w.write_with_tag(154, |w| w.write_message(m))? },
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::freeze_balance_v2(ref m) => { w.write_with_tag(162, |w| w.write_message(m))? },
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::unfreeze_balance_v2(ref m) => { w.write_with_tag(170, |w| w.write_message(m))? },
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::withdraw_expire_unfreeze(ref m) => { w.write_with_tag(186, |w| w.write_message(m))? },
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::delegate_resource(ref m) => { w.write_with_tag(194, |w| w.write_message(m))? },
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::undelegate_resource(ref m) => { w.write_with_tag(202, |w| w.write_message(m))? },
            TW::Tron::Proto::mod_Transaction::OneOfcontract_oneof::None => {},
    }        Ok(())
    }
}

pub mod mod_Transaction {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfcontract_oneof<'a> {
    transfer(TW::Tron::Proto::TransferContract<'a>),
    transfer_asset(TW::Tron::Proto::TransferAssetContract<'a>),
    freeze_balance(TW::Tron::Proto::FreezeBalanceContract<'a>),
    unfreeze_balance(TW::Tron::Proto::UnfreezeBalanceContract<'a>),
    unfreeze_asset(TW::Tron::Proto::UnfreezeAssetContract<'a>),
    withdraw_balance(TW::Tron::Proto::WithdrawBalanceContract<'a>),
    vote_asset(TW::Tron::Proto::VoteAssetContract<'a>),
    vote_witness(TW::Tron::Proto::VoteWitnessContract<'a>),
    trigger_smart_contract(TW::Tron::Proto::TriggerSmartContract<'a>),
    transfer_trc20_contract(TW::Tron::Proto::TransferTRC20Contract<'a>),
    freeze_balance_v2(TW::Tron::Proto::FreezeBalanceV2Contract<'a>),
    unfreeze_balance_v2(TW::Tron::Proto::UnfreezeBalanceV2Contract<'a>),
    withdraw_expire_unfreeze(TW::Tron::Proto::WithdrawExpireUnfreezeContract<'a>),
    delegate_resource(TW::Tron::Proto::DelegateResourceContract<'a>),
    undelegate_resource(TW::Tron::Proto::UnDelegateResourceContract<'a>),
    None,
}

impl<'a> Default for OneOfcontract_oneof<'a> {
    fn default() -> Self {
        OneOfcontract_oneof::None
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningInput<'a> {
    pub transaction: Option<TW::Tron::Proto::Transaction<'a>>,
    pub private_key: Cow<'a, [u8]>,
    pub txId: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.transaction = Some(r.read_message::<TW::Tron::Proto::Transaction>(bytes)?),
                Ok(18) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.txId = r.read_string(bytes).map(Cow::Borrowed)?,
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
        + self.transaction.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.private_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.private_key).len()) }
        + if self.txId == "" { 0 } else { 1 + sizeof_len((&self.txId).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.transaction { w.write_with_tag(10, |w| w.write_message(s))?; }
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.private_key))?; }
        if self.txId != "" { w.write_with_tag(26, |w| w.write_string(&**&self.txId))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningOutput<'a> {
    pub id: Cow<'a, [u8]>,
    pub signature: Cow<'a, [u8]>,
    pub ref_block_bytes: Cow<'a, [u8]>,
    pub ref_block_hash: Cow<'a, [u8]>,
    pub json: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.id = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.signature = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.ref_block_bytes = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.ref_block_hash = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.json = r.read_string(bytes).map(Cow::Borrowed)?,
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
        + if self.id == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.id).len()) }
        + if self.signature == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.signature).len()) }
        + if self.ref_block_bytes == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.ref_block_bytes).len()) }
        + if self.ref_block_hash == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.ref_block_hash).len()) }
        + if self.json == "" { 0 } else { 1 + sizeof_len((&self.json).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.id != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.id))?; }
        if self.signature != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.signature))?; }
        if self.ref_block_bytes != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.ref_block_bytes))?; }
        if self.ref_block_hash != Cow::Borrowed(b"") { w.write_with_tag(34, |w| w.write_bytes(&**&self.ref_block_hash))?; }
        if self.json != "" { w.write_with_tag(42, |w| w.write_string(&**&self.json))?; }
        Ok(())
    }
}

