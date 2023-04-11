// Automatically generated rust module for 'Polkadot.proto' file

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

#[derive(Debug, PartialEq, Eq, Clone, Copy)]
pub enum RewardDestination {
    STAKED = 0,
    STASH = 1,
    CONTROLLER = 2,
}

impl Default for RewardDestination {
    fn default() -> Self {
        RewardDestination::STAKED
    }
}

impl From<i32> for RewardDestination {
    fn from(i: i32) -> Self {
        match i {
            0 => RewardDestination::STAKED,
            1 => RewardDestination::STASH,
            2 => RewardDestination::CONTROLLER,
            _ => Self::default(),
        }
    }
}

impl<'a> From<&'a str> for RewardDestination {
    fn from(s: &'a str) -> Self {
        match s {
            "STAKED" => RewardDestination::STAKED,
            "STASH" => RewardDestination::STASH,
            "CONTROLLER" => RewardDestination::CONTROLLER,
            _ => Self::default(),
        }
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Era {
    pub block_number: u64,
    pub period: u64,
}

impl<'a> MessageRead<'a> for Era {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.block_number = r.read_uint64(bytes)?,
                Ok(16) => msg.period = r.read_uint64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl MessageWrite for Era {
    fn get_size(&self) -> usize {
        0
        + if self.block_number == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.block_number) as u64) }
        + if self.period == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.period) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.block_number != 0u64 { w.write_with_tag(8, |w| w.write_uint64(*&self.block_number))?; }
        if self.period != 0u64 { w.write_with_tag(16, |w| w.write_uint64(*&self.period))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Balance<'a> {
    pub message_oneof: TW::Polkadot::Proto::mod_Balance::OneOfmessage_oneof<'a>,
}

impl<'a> MessageRead<'a> for Balance<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.message_oneof = TW::Polkadot::Proto::mod_Balance::OneOfmessage_oneof::transfer(r.read_message::<TW::Polkadot::Proto::mod_Balance::Transfer>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Balance<'a> {
    fn get_size(&self) -> usize {
        0
        + match self.message_oneof {
            TW::Polkadot::Proto::mod_Balance::OneOfmessage_oneof::transfer(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Polkadot::Proto::mod_Balance::OneOfmessage_oneof::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        match self.message_oneof {            TW::Polkadot::Proto::mod_Balance::OneOfmessage_oneof::transfer(ref m) => { w.write_with_tag(10, |w| w.write_message(m))? },
            TW::Polkadot::Proto::mod_Balance::OneOfmessage_oneof::None => {},
    }        Ok(())
    }
}

pub mod mod_Balance {

use std::borrow::Cow;
use super::*;

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Transfer<'a> {
    pub to_address: Cow<'a, str>,
    pub value: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for Transfer<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.to_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.value = r.read_bytes(bytes).map(Cow::Borrowed)?,
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
        + if self.value == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.value).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.to_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.to_address))?; }
        if self.value != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.value))?; }
        Ok(())
    }
}

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfmessage_oneof<'a> {
    transfer(TW::Polkadot::Proto::mod_Balance::Transfer<'a>),
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
pub struct Staking<'a> {
    pub message_oneof: TW::Polkadot::Proto::mod_Staking::OneOfmessage_oneof<'a>,
}

impl<'a> MessageRead<'a> for Staking<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.message_oneof = TW::Polkadot::Proto::mod_Staking::OneOfmessage_oneof::bond(r.read_message::<TW::Polkadot::Proto::mod_Staking::Bond>(bytes)?),
                Ok(18) => msg.message_oneof = TW::Polkadot::Proto::mod_Staking::OneOfmessage_oneof::bond_and_nominate(r.read_message::<TW::Polkadot::Proto::mod_Staking::BondAndNominate>(bytes)?),
                Ok(26) => msg.message_oneof = TW::Polkadot::Proto::mod_Staking::OneOfmessage_oneof::bond_extra(r.read_message::<TW::Polkadot::Proto::mod_Staking::BondExtra>(bytes)?),
                Ok(34) => msg.message_oneof = TW::Polkadot::Proto::mod_Staking::OneOfmessage_oneof::unbond(r.read_message::<TW::Polkadot::Proto::mod_Staking::Unbond>(bytes)?),
                Ok(42) => msg.message_oneof = TW::Polkadot::Proto::mod_Staking::OneOfmessage_oneof::withdraw_unbonded(r.read_message::<TW::Polkadot::Proto::mod_Staking::WithdrawUnbonded>(bytes)?),
                Ok(50) => msg.message_oneof = TW::Polkadot::Proto::mod_Staking::OneOfmessage_oneof::nominate(r.read_message::<TW::Polkadot::Proto::mod_Staking::Nominate>(bytes)?),
                Ok(58) => msg.message_oneof = TW::Polkadot::Proto::mod_Staking::OneOfmessage_oneof::chill(r.read_message::<TW::Polkadot::Proto::mod_Staking::Chill>(bytes)?),
                Ok(66) => msg.message_oneof = TW::Polkadot::Proto::mod_Staking::OneOfmessage_oneof::chill_and_unbond(r.read_message::<TW::Polkadot::Proto::mod_Staking::ChillAndUnbond>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Staking<'a> {
    fn get_size(&self) -> usize {
        0
        + match self.message_oneof {
            TW::Polkadot::Proto::mod_Staking::OneOfmessage_oneof::bond(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Polkadot::Proto::mod_Staking::OneOfmessage_oneof::bond_and_nominate(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Polkadot::Proto::mod_Staking::OneOfmessage_oneof::bond_extra(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Polkadot::Proto::mod_Staking::OneOfmessage_oneof::unbond(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Polkadot::Proto::mod_Staking::OneOfmessage_oneof::withdraw_unbonded(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Polkadot::Proto::mod_Staking::OneOfmessage_oneof::nominate(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Polkadot::Proto::mod_Staking::OneOfmessage_oneof::chill(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Polkadot::Proto::mod_Staking::OneOfmessage_oneof::chill_and_unbond(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Polkadot::Proto::mod_Staking::OneOfmessage_oneof::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        match self.message_oneof {            TW::Polkadot::Proto::mod_Staking::OneOfmessage_oneof::bond(ref m) => { w.write_with_tag(10, |w| w.write_message(m))? },
            TW::Polkadot::Proto::mod_Staking::OneOfmessage_oneof::bond_and_nominate(ref m) => { w.write_with_tag(18, |w| w.write_message(m))? },
            TW::Polkadot::Proto::mod_Staking::OneOfmessage_oneof::bond_extra(ref m) => { w.write_with_tag(26, |w| w.write_message(m))? },
            TW::Polkadot::Proto::mod_Staking::OneOfmessage_oneof::unbond(ref m) => { w.write_with_tag(34, |w| w.write_message(m))? },
            TW::Polkadot::Proto::mod_Staking::OneOfmessage_oneof::withdraw_unbonded(ref m) => { w.write_with_tag(42, |w| w.write_message(m))? },
            TW::Polkadot::Proto::mod_Staking::OneOfmessage_oneof::nominate(ref m) => { w.write_with_tag(50, |w| w.write_message(m))? },
            TW::Polkadot::Proto::mod_Staking::OneOfmessage_oneof::chill(ref m) => { w.write_with_tag(58, |w| w.write_message(m))? },
            TW::Polkadot::Proto::mod_Staking::OneOfmessage_oneof::chill_and_unbond(ref m) => { w.write_with_tag(66, |w| w.write_message(m))? },
            TW::Polkadot::Proto::mod_Staking::OneOfmessage_oneof::None => {},
    }        Ok(())
    }
}

pub mod mod_Staking {

use std::borrow::Cow;
use super::*;

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Bond<'a> {
    pub controller: Cow<'a, str>,
    pub value: Cow<'a, [u8]>,
    pub reward_destination: TW::Polkadot::Proto::RewardDestination,
}

impl<'a> MessageRead<'a> for Bond<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.controller = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.value = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.reward_destination = r.read_enum(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Bond<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.controller == "" { 0 } else { 1 + sizeof_len((&self.controller).len()) }
        + if self.value == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.value).len()) }
        + if self.reward_destination == TW::Polkadot::Proto::RewardDestination::STAKED { 0 } else { 1 + sizeof_varint(*(&self.reward_destination) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.controller != "" { w.write_with_tag(10, |w| w.write_string(&**&self.controller))?; }
        if self.value != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.value))?; }
        if self.reward_destination != TW::Polkadot::Proto::RewardDestination::STAKED { w.write_with_tag(24, |w| w.write_enum(*&self.reward_destination as i32))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct BondAndNominate<'a> {
    pub controller: Cow<'a, str>,
    pub value: Cow<'a, [u8]>,
    pub reward_destination: TW::Polkadot::Proto::RewardDestination,
    pub nominators: Vec<Cow<'a, str>>,
}

impl<'a> MessageRead<'a> for BondAndNominate<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.controller = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.value = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.reward_destination = r.read_enum(bytes)?,
                Ok(34) => msg.nominators.push(r.read_string(bytes).map(Cow::Borrowed)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for BondAndNominate<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.controller == "" { 0 } else { 1 + sizeof_len((&self.controller).len()) }
        + if self.value == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.value).len()) }
        + if self.reward_destination == TW::Polkadot::Proto::RewardDestination::STAKED { 0 } else { 1 + sizeof_varint(*(&self.reward_destination) as u64) }
        + self.nominators.iter().map(|s| 1 + sizeof_len((s).len())).sum::<usize>()
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.controller != "" { w.write_with_tag(10, |w| w.write_string(&**&self.controller))?; }
        if self.value != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.value))?; }
        if self.reward_destination != TW::Polkadot::Proto::RewardDestination::STAKED { w.write_with_tag(24, |w| w.write_enum(*&self.reward_destination as i32))?; }
        for s in &self.nominators { w.write_with_tag(34, |w| w.write_string(&**s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct BondExtra<'a> {
    pub value: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for BondExtra<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.value = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for BondExtra<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.value == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.value).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.value != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.value))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Unbond<'a> {
    pub value: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for Unbond<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.value = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Unbond<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.value == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.value).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.value != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.value))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct WithdrawUnbonded {
    pub slashing_spans: i32,
}

impl<'a> MessageRead<'a> for WithdrawUnbonded {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.slashing_spans = r.read_int32(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl MessageWrite for WithdrawUnbonded {
    fn get_size(&self) -> usize {
        0
        + if self.slashing_spans == 0i32 { 0 } else { 1 + sizeof_varint(*(&self.slashing_spans) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.slashing_spans != 0i32 { w.write_with_tag(8, |w| w.write_int32(*&self.slashing_spans))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Nominate<'a> {
    pub nominators: Vec<Cow<'a, str>>,
}

impl<'a> MessageRead<'a> for Nominate<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.nominators.push(r.read_string(bytes).map(Cow::Borrowed)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Nominate<'a> {
    fn get_size(&self) -> usize {
        0
        + self.nominators.iter().map(|s| 1 + sizeof_len((s).len())).sum::<usize>()
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        for s in &self.nominators { w.write_with_tag(10, |w| w.write_string(&**s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct ChillAndUnbond<'a> {
    pub value: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for ChillAndUnbond<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.value = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for ChillAndUnbond<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.value == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.value).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.value != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.value))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Chill { }

impl<'a> MessageRead<'a> for Chill {
    fn from_reader(r: &mut BytesReader, _: &[u8]) -> Result<Self> {
        r.read_to_end();
        Ok(Self::default())
    }
}

impl MessageWrite for Chill { }

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfmessage_oneof<'a> {
    bond(TW::Polkadot::Proto::mod_Staking::Bond<'a>),
    bond_and_nominate(TW::Polkadot::Proto::mod_Staking::BondAndNominate<'a>),
    bond_extra(TW::Polkadot::Proto::mod_Staking::BondExtra<'a>),
    unbond(TW::Polkadot::Proto::mod_Staking::Unbond<'a>),
    withdraw_unbonded(TW::Polkadot::Proto::mod_Staking::WithdrawUnbonded),
    nominate(TW::Polkadot::Proto::mod_Staking::Nominate<'a>),
    chill(TW::Polkadot::Proto::mod_Staking::Chill),
    chill_and_unbond(TW::Polkadot::Proto::mod_Staking::ChillAndUnbond<'a>),
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
pub struct SigningInput<'a> {
    pub block_hash: Cow<'a, [u8]>,
    pub genesis_hash: Cow<'a, [u8]>,
    pub nonce: u64,
    pub spec_version: u32,
    pub transaction_version: u32,
    pub tip: Cow<'a, [u8]>,
    pub era: Option<TW::Polkadot::Proto::Era>,
    pub private_key: Cow<'a, [u8]>,
    pub network: u32,
    pub message_oneof: TW::Polkadot::Proto::mod_SigningInput::OneOfmessage_oneof<'a>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.block_hash = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.genesis_hash = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.nonce = r.read_uint64(bytes)?,
                Ok(32) => msg.spec_version = r.read_uint32(bytes)?,
                Ok(40) => msg.transaction_version = r.read_uint32(bytes)?,
                Ok(50) => msg.tip = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(58) => msg.era = Some(r.read_message::<TW::Polkadot::Proto::Era>(bytes)?),
                Ok(66) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(72) => msg.network = r.read_uint32(bytes)?,
                Ok(82) => msg.message_oneof = TW::Polkadot::Proto::mod_SigningInput::OneOfmessage_oneof::balance_call(r.read_message::<TW::Polkadot::Proto::Balance>(bytes)?),
                Ok(90) => msg.message_oneof = TW::Polkadot::Proto::mod_SigningInput::OneOfmessage_oneof::staking_call(r.read_message::<TW::Polkadot::Proto::Staking>(bytes)?),
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
        + if self.block_hash == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.block_hash).len()) }
        + if self.genesis_hash == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.genesis_hash).len()) }
        + if self.nonce == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.nonce) as u64) }
        + if self.spec_version == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.spec_version) as u64) }
        + if self.transaction_version == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.transaction_version) as u64) }
        + if self.tip == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.tip).len()) }
        + self.era.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.private_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.private_key).len()) }
        + if self.network == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.network) as u64) }
        + match self.message_oneof {
            TW::Polkadot::Proto::mod_SigningInput::OneOfmessage_oneof::balance_call(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Polkadot::Proto::mod_SigningInput::OneOfmessage_oneof::staking_call(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Polkadot::Proto::mod_SigningInput::OneOfmessage_oneof::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.block_hash != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.block_hash))?; }
        if self.genesis_hash != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.genesis_hash))?; }
        if self.nonce != 0u64 { w.write_with_tag(24, |w| w.write_uint64(*&self.nonce))?; }
        if self.spec_version != 0u32 { w.write_with_tag(32, |w| w.write_uint32(*&self.spec_version))?; }
        if self.transaction_version != 0u32 { w.write_with_tag(40, |w| w.write_uint32(*&self.transaction_version))?; }
        if self.tip != Cow::Borrowed(b"") { w.write_with_tag(50, |w| w.write_bytes(&**&self.tip))?; }
        if let Some(ref s) = self.era { w.write_with_tag(58, |w| w.write_message(s))?; }
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(66, |w| w.write_bytes(&**&self.private_key))?; }
        if self.network != 0u32 { w.write_with_tag(72, |w| w.write_uint32(*&self.network))?; }
        match self.message_oneof {            TW::Polkadot::Proto::mod_SigningInput::OneOfmessage_oneof::balance_call(ref m) => { w.write_with_tag(82, |w| w.write_message(m))? },
            TW::Polkadot::Proto::mod_SigningInput::OneOfmessage_oneof::staking_call(ref m) => { w.write_with_tag(90, |w| w.write_message(m))? },
            TW::Polkadot::Proto::mod_SigningInput::OneOfmessage_oneof::None => {},
    }        Ok(())
    }
}

pub mod mod_SigningInput {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfmessage_oneof<'a> {
    balance_call(TW::Polkadot::Proto::Balance<'a>),
    staking_call(TW::Polkadot::Proto::Staking<'a>),
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

