// Automatically generated rust module for 'Harmony.proto' file

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
    pub chain_id: Cow<'a, [u8]>,
    pub private_key: Cow<'a, [u8]>,
    pub message_oneof: TW::Harmony::Proto::mod_SigningInput::OneOfmessage_oneof<'a>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.chain_id = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.message_oneof = TW::Harmony::Proto::mod_SigningInput::OneOfmessage_oneof::transaction_message(r.read_message::<TW::Harmony::Proto::TransactionMessage>(bytes)?),
                Ok(34) => msg.message_oneof = TW::Harmony::Proto::mod_SigningInput::OneOfmessage_oneof::staking_message(r.read_message::<TW::Harmony::Proto::StakingMessage>(bytes)?),
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
        + if self.chain_id == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.chain_id).len()) }
        + if self.private_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.private_key).len()) }
        + match self.message_oneof {
            TW::Harmony::Proto::mod_SigningInput::OneOfmessage_oneof::transaction_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Harmony::Proto::mod_SigningInput::OneOfmessage_oneof::staking_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Harmony::Proto::mod_SigningInput::OneOfmessage_oneof::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.chain_id != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.chain_id))?; }
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.private_key))?; }
        match self.message_oneof {            TW::Harmony::Proto::mod_SigningInput::OneOfmessage_oneof::transaction_message(ref m) => { w.write_with_tag(26, |w| w.write_message(m))? },
            TW::Harmony::Proto::mod_SigningInput::OneOfmessage_oneof::staking_message(ref m) => { w.write_with_tag(34, |w| w.write_message(m))? },
            TW::Harmony::Proto::mod_SigningInput::OneOfmessage_oneof::None => {},
    }        Ok(())
    }
}

pub mod mod_SigningInput {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfmessage_oneof<'a> {
    transaction_message(TW::Harmony::Proto::TransactionMessage<'a>),
    staking_message(TW::Harmony::Proto::StakingMessage<'a>),
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
    pub v: Cow<'a, [u8]>,
    pub r: Cow<'a, [u8]>,
    pub s: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.encoded = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.v = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.r = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.s = r.read_bytes(bytes).map(Cow::Borrowed)?,
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
        + if self.v == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.v).len()) }
        + if self.r == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.r).len()) }
        + if self.s == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.s).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.encoded != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.encoded))?; }
        if self.v != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.v))?; }
        if self.r != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.r))?; }
        if self.s != Cow::Borrowed(b"") { w.write_with_tag(34, |w| w.write_bytes(&**&self.s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TransactionMessage<'a> {
    pub nonce: Cow<'a, [u8]>,
    pub gas_price: Cow<'a, [u8]>,
    pub gas_limit: Cow<'a, [u8]>,
    pub to_address: Cow<'a, str>,
    pub amount: Cow<'a, [u8]>,
    pub payload: Cow<'a, [u8]>,
    pub from_shard_id: Cow<'a, [u8]>,
    pub to_shard_id: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for TransactionMessage<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.nonce = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.gas_price = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.gas_limit = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.to_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.amount = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(50) => msg.payload = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(58) => msg.from_shard_id = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(66) => msg.to_shard_id = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TransactionMessage<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.nonce == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.nonce).len()) }
        + if self.gas_price == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.gas_price).len()) }
        + if self.gas_limit == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.gas_limit).len()) }
        + if self.to_address == "" { 0 } else { 1 + sizeof_len((&self.to_address).len()) }
        + if self.amount == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.amount).len()) }
        + if self.payload == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.payload).len()) }
        + if self.from_shard_id == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.from_shard_id).len()) }
        + if self.to_shard_id == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.to_shard_id).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.nonce != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.nonce))?; }
        if self.gas_price != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.gas_price))?; }
        if self.gas_limit != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.gas_limit))?; }
        if self.to_address != "" { w.write_with_tag(34, |w| w.write_string(&**&self.to_address))?; }
        if self.amount != Cow::Borrowed(b"") { w.write_with_tag(42, |w| w.write_bytes(&**&self.amount))?; }
        if self.payload != Cow::Borrowed(b"") { w.write_with_tag(50, |w| w.write_bytes(&**&self.payload))?; }
        if self.from_shard_id != Cow::Borrowed(b"") { w.write_with_tag(58, |w| w.write_bytes(&**&self.from_shard_id))?; }
        if self.to_shard_id != Cow::Borrowed(b"") { w.write_with_tag(66, |w| w.write_bytes(&**&self.to_shard_id))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct StakingMessage<'a> {
    pub nonce: Cow<'a, [u8]>,
    pub gas_price: Cow<'a, [u8]>,
    pub gas_limit: Cow<'a, [u8]>,
    pub stake_msg: TW::Harmony::Proto::mod_StakingMessage::OneOfstake_msg<'a>,
}

impl<'a> MessageRead<'a> for StakingMessage<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(50) => msg.nonce = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(58) => msg.gas_price = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(66) => msg.gas_limit = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(10) => msg.stake_msg = TW::Harmony::Proto::mod_StakingMessage::OneOfstake_msg::create_validator_message(r.read_message::<TW::Harmony::Proto::DirectiveCreateValidator>(bytes)?),
                Ok(18) => msg.stake_msg = TW::Harmony::Proto::mod_StakingMessage::OneOfstake_msg::edit_validator_message(r.read_message::<TW::Harmony::Proto::DirectiveEditValidator>(bytes)?),
                Ok(26) => msg.stake_msg = TW::Harmony::Proto::mod_StakingMessage::OneOfstake_msg::delegate_message(r.read_message::<TW::Harmony::Proto::DirectiveDelegate>(bytes)?),
                Ok(34) => msg.stake_msg = TW::Harmony::Proto::mod_StakingMessage::OneOfstake_msg::undelegate_message(r.read_message::<TW::Harmony::Proto::DirectiveUndelegate>(bytes)?),
                Ok(42) => msg.stake_msg = TW::Harmony::Proto::mod_StakingMessage::OneOfstake_msg::collect_rewards(r.read_message::<TW::Harmony::Proto::DirectiveCollectRewards>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for StakingMessage<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.nonce == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.nonce).len()) }
        + if self.gas_price == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.gas_price).len()) }
        + if self.gas_limit == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.gas_limit).len()) }
        + match self.stake_msg {
            TW::Harmony::Proto::mod_StakingMessage::OneOfstake_msg::create_validator_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Harmony::Proto::mod_StakingMessage::OneOfstake_msg::edit_validator_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Harmony::Proto::mod_StakingMessage::OneOfstake_msg::delegate_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Harmony::Proto::mod_StakingMessage::OneOfstake_msg::undelegate_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Harmony::Proto::mod_StakingMessage::OneOfstake_msg::collect_rewards(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Harmony::Proto::mod_StakingMessage::OneOfstake_msg::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.nonce != Cow::Borrowed(b"") { w.write_with_tag(50, |w| w.write_bytes(&**&self.nonce))?; }
        if self.gas_price != Cow::Borrowed(b"") { w.write_with_tag(58, |w| w.write_bytes(&**&self.gas_price))?; }
        if self.gas_limit != Cow::Borrowed(b"") { w.write_with_tag(66, |w| w.write_bytes(&**&self.gas_limit))?; }
        match self.stake_msg {            TW::Harmony::Proto::mod_StakingMessage::OneOfstake_msg::create_validator_message(ref m) => { w.write_with_tag(10, |w| w.write_message(m))? },
            TW::Harmony::Proto::mod_StakingMessage::OneOfstake_msg::edit_validator_message(ref m) => { w.write_with_tag(18, |w| w.write_message(m))? },
            TW::Harmony::Proto::mod_StakingMessage::OneOfstake_msg::delegate_message(ref m) => { w.write_with_tag(26, |w| w.write_message(m))? },
            TW::Harmony::Proto::mod_StakingMessage::OneOfstake_msg::undelegate_message(ref m) => { w.write_with_tag(34, |w| w.write_message(m))? },
            TW::Harmony::Proto::mod_StakingMessage::OneOfstake_msg::collect_rewards(ref m) => { w.write_with_tag(42, |w| w.write_message(m))? },
            TW::Harmony::Proto::mod_StakingMessage::OneOfstake_msg::None => {},
    }        Ok(())
    }
}

pub mod mod_StakingMessage {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfstake_msg<'a> {
    create_validator_message(TW::Harmony::Proto::DirectiveCreateValidator<'a>),
    edit_validator_message(TW::Harmony::Proto::DirectiveEditValidator<'a>),
    delegate_message(TW::Harmony::Proto::DirectiveDelegate<'a>),
    undelegate_message(TW::Harmony::Proto::DirectiveUndelegate<'a>),
    collect_rewards(TW::Harmony::Proto::DirectiveCollectRewards<'a>),
    None,
}

impl<'a> Default for OneOfstake_msg<'a> {
    fn default() -> Self {
        OneOfstake_msg::None
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Description<'a> {
    pub name: Cow<'a, str>,
    pub identity: Cow<'a, str>,
    pub website: Cow<'a, str>,
    pub security_contact: Cow<'a, str>,
    pub details: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for Description<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.name = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.identity = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.website = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.security_contact = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.details = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Description<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.name == "" { 0 } else { 1 + sizeof_len((&self.name).len()) }
        + if self.identity == "" { 0 } else { 1 + sizeof_len((&self.identity).len()) }
        + if self.website == "" { 0 } else { 1 + sizeof_len((&self.website).len()) }
        + if self.security_contact == "" { 0 } else { 1 + sizeof_len((&self.security_contact).len()) }
        + if self.details == "" { 0 } else { 1 + sizeof_len((&self.details).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.name != "" { w.write_with_tag(10, |w| w.write_string(&**&self.name))?; }
        if self.identity != "" { w.write_with_tag(18, |w| w.write_string(&**&self.identity))?; }
        if self.website != "" { w.write_with_tag(26, |w| w.write_string(&**&self.website))?; }
        if self.security_contact != "" { w.write_with_tag(34, |w| w.write_string(&**&self.security_contact))?; }
        if self.details != "" { w.write_with_tag(42, |w| w.write_string(&**&self.details))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Decimal<'a> {
    pub value: Cow<'a, [u8]>,
    pub precision: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for Decimal<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.value = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.precision = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Decimal<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.value == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.value).len()) }
        + if self.precision == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.precision).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.value != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.value))?; }
        if self.precision != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.precision))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct CommissionRate<'a> {
    pub rate: Option<TW::Harmony::Proto::Decimal<'a>>,
    pub max_rate: Option<TW::Harmony::Proto::Decimal<'a>>,
    pub max_change_rate: Option<TW::Harmony::Proto::Decimal<'a>>,
}

impl<'a> MessageRead<'a> for CommissionRate<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.rate = Some(r.read_message::<TW::Harmony::Proto::Decimal>(bytes)?),
                Ok(18) => msg.max_rate = Some(r.read_message::<TW::Harmony::Proto::Decimal>(bytes)?),
                Ok(26) => msg.max_change_rate = Some(r.read_message::<TW::Harmony::Proto::Decimal>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for CommissionRate<'a> {
    fn get_size(&self) -> usize {
        0
        + self.rate.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + self.max_rate.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + self.max_change_rate.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.rate { w.write_with_tag(10, |w| w.write_message(s))?; }
        if let Some(ref s) = self.max_rate { w.write_with_tag(18, |w| w.write_message(s))?; }
        if let Some(ref s) = self.max_change_rate { w.write_with_tag(26, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct DirectiveCreateValidator<'a> {
    pub validator_address: Cow<'a, str>,
    pub description: Option<TW::Harmony::Proto::Description<'a>>,
    pub commission_rates: Option<TW::Harmony::Proto::CommissionRate<'a>>,
    pub min_self_delegation: Cow<'a, [u8]>,
    pub max_total_delegation: Cow<'a, [u8]>,
    pub slot_pub_keys: Vec<Cow<'a, [u8]>>,
    pub slot_key_sigs: Vec<Cow<'a, [u8]>>,
    pub amount: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for DirectiveCreateValidator<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.validator_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.description = Some(r.read_message::<TW::Harmony::Proto::Description>(bytes)?),
                Ok(26) => msg.commission_rates = Some(r.read_message::<TW::Harmony::Proto::CommissionRate>(bytes)?),
                Ok(34) => msg.min_self_delegation = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.max_total_delegation = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(50) => msg.slot_pub_keys.push(r.read_bytes(bytes).map(Cow::Borrowed)?),
                Ok(58) => msg.slot_key_sigs.push(r.read_bytes(bytes).map(Cow::Borrowed)?),
                Ok(66) => msg.amount = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for DirectiveCreateValidator<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.validator_address == "" { 0 } else { 1 + sizeof_len((&self.validator_address).len()) }
        + self.description.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + self.commission_rates.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.min_self_delegation == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.min_self_delegation).len()) }
        + if self.max_total_delegation == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.max_total_delegation).len()) }
        + self.slot_pub_keys.iter().map(|s| 1 + sizeof_len((s).len())).sum::<usize>()
        + self.slot_key_sigs.iter().map(|s| 1 + sizeof_len((s).len())).sum::<usize>()
        + if self.amount == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.amount).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.validator_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.validator_address))?; }
        if let Some(ref s) = self.description { w.write_with_tag(18, |w| w.write_message(s))?; }
        if let Some(ref s) = self.commission_rates { w.write_with_tag(26, |w| w.write_message(s))?; }
        if self.min_self_delegation != Cow::Borrowed(b"") { w.write_with_tag(34, |w| w.write_bytes(&**&self.min_self_delegation))?; }
        if self.max_total_delegation != Cow::Borrowed(b"") { w.write_with_tag(42, |w| w.write_bytes(&**&self.max_total_delegation))?; }
        for s in &self.slot_pub_keys { w.write_with_tag(50, |w| w.write_bytes(&**s))?; }
        for s in &self.slot_key_sigs { w.write_with_tag(58, |w| w.write_bytes(&**s))?; }
        if self.amount != Cow::Borrowed(b"") { w.write_with_tag(66, |w| w.write_bytes(&**&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct DirectiveEditValidator<'a> {
    pub validator_address: Cow<'a, str>,
    pub description: Option<TW::Harmony::Proto::Description<'a>>,
    pub commission_rate: Option<TW::Harmony::Proto::Decimal<'a>>,
    pub min_self_delegation: Cow<'a, [u8]>,
    pub max_total_delegation: Cow<'a, [u8]>,
    pub slot_key_to_remove: Cow<'a, [u8]>,
    pub slot_key_to_add: Cow<'a, [u8]>,
    pub slot_key_to_add_sig: Cow<'a, [u8]>,
    pub active: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for DirectiveEditValidator<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.validator_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.description = Some(r.read_message::<TW::Harmony::Proto::Description>(bytes)?),
                Ok(26) => msg.commission_rate = Some(r.read_message::<TW::Harmony::Proto::Decimal>(bytes)?),
                Ok(34) => msg.min_self_delegation = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.max_total_delegation = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(50) => msg.slot_key_to_remove = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(58) => msg.slot_key_to_add = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(66) => msg.slot_key_to_add_sig = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(74) => msg.active = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for DirectiveEditValidator<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.validator_address == "" { 0 } else { 1 + sizeof_len((&self.validator_address).len()) }
        + self.description.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + self.commission_rate.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.min_self_delegation == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.min_self_delegation).len()) }
        + if self.max_total_delegation == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.max_total_delegation).len()) }
        + if self.slot_key_to_remove == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.slot_key_to_remove).len()) }
        + if self.slot_key_to_add == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.slot_key_to_add).len()) }
        + if self.slot_key_to_add_sig == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.slot_key_to_add_sig).len()) }
        + if self.active == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.active).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.validator_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.validator_address))?; }
        if let Some(ref s) = self.description { w.write_with_tag(18, |w| w.write_message(s))?; }
        if let Some(ref s) = self.commission_rate { w.write_with_tag(26, |w| w.write_message(s))?; }
        if self.min_self_delegation != Cow::Borrowed(b"") { w.write_with_tag(34, |w| w.write_bytes(&**&self.min_self_delegation))?; }
        if self.max_total_delegation != Cow::Borrowed(b"") { w.write_with_tag(42, |w| w.write_bytes(&**&self.max_total_delegation))?; }
        if self.slot_key_to_remove != Cow::Borrowed(b"") { w.write_with_tag(50, |w| w.write_bytes(&**&self.slot_key_to_remove))?; }
        if self.slot_key_to_add != Cow::Borrowed(b"") { w.write_with_tag(58, |w| w.write_bytes(&**&self.slot_key_to_add))?; }
        if self.slot_key_to_add_sig != Cow::Borrowed(b"") { w.write_with_tag(66, |w| w.write_bytes(&**&self.slot_key_to_add_sig))?; }
        if self.active != Cow::Borrowed(b"") { w.write_with_tag(74, |w| w.write_bytes(&**&self.active))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct DirectiveDelegate<'a> {
    pub delegator_address: Cow<'a, str>,
    pub validator_address: Cow<'a, str>,
    pub amount: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for DirectiveDelegate<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.delegator_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.validator_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.amount = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for DirectiveDelegate<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.delegator_address == "" { 0 } else { 1 + sizeof_len((&self.delegator_address).len()) }
        + if self.validator_address == "" { 0 } else { 1 + sizeof_len((&self.validator_address).len()) }
        + if self.amount == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.amount).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.delegator_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.delegator_address))?; }
        if self.validator_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.validator_address))?; }
        if self.amount != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct DirectiveUndelegate<'a> {
    pub delegator_address: Cow<'a, str>,
    pub validator_address: Cow<'a, str>,
    pub amount: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for DirectiveUndelegate<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.delegator_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.validator_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.amount = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for DirectiveUndelegate<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.delegator_address == "" { 0 } else { 1 + sizeof_len((&self.delegator_address).len()) }
        + if self.validator_address == "" { 0 } else { 1 + sizeof_len((&self.validator_address).len()) }
        + if self.amount == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.amount).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.delegator_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.delegator_address))?; }
        if self.validator_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.validator_address))?; }
        if self.amount != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct DirectiveCollectRewards<'a> {
    pub delegator_address: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for DirectiveCollectRewards<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.delegator_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for DirectiveCollectRewards<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.delegator_address == "" { 0 } else { 1 + sizeof_len((&self.delegator_address).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.delegator_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.delegator_address))?; }
        Ok(())
    }
}

