// Automatically generated rust module for 'Cosmos.proto' file

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
pub enum BroadcastMode {
    BLOCK = 0,
    SYNC = 1,
    ASYNC = 2,
}

impl Default for BroadcastMode {
    fn default() -> Self {
        BroadcastMode::BLOCK
    }
}

impl From<i32> for BroadcastMode {
    fn from(i: i32) -> Self {
        match i {
            0 => BroadcastMode::BLOCK,
            1 => BroadcastMode::SYNC,
            2 => BroadcastMode::ASYNC,
            _ => Self::default(),
        }
    }
}

impl<'a> From<&'a str> for BroadcastMode {
    fn from(s: &'a str) -> Self {
        match s {
            "BLOCK" => BroadcastMode::BLOCK,
            "SYNC" => BroadcastMode::SYNC,
            "ASYNC" => BroadcastMode::ASYNC,
            _ => Self::default(),
        }
    }
}

#[derive(Debug, PartialEq, Eq, Clone, Copy)]
pub enum SigningMode {
    JSON = 0,
    Protobuf = 1,
}

impl Default for SigningMode {
    fn default() -> Self {
        SigningMode::JSON
    }
}

impl From<i32> for SigningMode {
    fn from(i: i32) -> Self {
        match i {
            0 => SigningMode::JSON,
            1 => SigningMode::Protobuf,
            _ => Self::default(),
        }
    }
}

impl<'a> From<&'a str> for SigningMode {
    fn from(s: &'a str) -> Self {
        match s {
            "JSON" => SigningMode::JSON,
            "Protobuf" => SigningMode::Protobuf,
            _ => Self::default(),
        }
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Amount<'a> {
    pub denom: Cow<'a, str>,
    pub amount: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for Amount<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.denom = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.amount = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Amount<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.denom == "" { 0 } else { 1 + sizeof_len((&self.denom).len()) }
        + if self.amount == "" { 0 } else { 1 + sizeof_len((&self.amount).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.denom != "" { w.write_with_tag(10, |w| w.write_string(&**&self.denom))?; }
        if self.amount != "" { w.write_with_tag(18, |w| w.write_string(&**&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Fee<'a> {
    pub amounts: Vec<TW::Cosmos::Proto::Amount<'a>>,
    pub gas: u64,
}

impl<'a> MessageRead<'a> for Fee<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.amounts.push(r.read_message::<TW::Cosmos::Proto::Amount>(bytes)?),
                Ok(16) => msg.gas = r.read_uint64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Fee<'a> {
    fn get_size(&self) -> usize {
        0
        + self.amounts.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + if self.gas == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.gas) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        for s in &self.amounts { w.write_with_tag(10, |w| w.write_message(s))?; }
        if self.gas != 0u64 { w.write_with_tag(16, |w| w.write_uint64(*&self.gas))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Height {
    pub revision_number: u64,
    pub revision_height: u64,
}

impl<'a> MessageRead<'a> for Height {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.revision_number = r.read_uint64(bytes)?,
                Ok(16) => msg.revision_height = r.read_uint64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl MessageWrite for Height {
    fn get_size(&self) -> usize {
        0
        + if self.revision_number == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.revision_number) as u64) }
        + if self.revision_height == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.revision_height) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.revision_number != 0u64 { w.write_with_tag(8, |w| w.write_uint64(*&self.revision_number))?; }
        if self.revision_height != 0u64 { w.write_with_tag(16, |w| w.write_uint64(*&self.revision_height))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Message<'a> {
    pub message_oneof: TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof<'a>,
}

impl<'a> MessageRead<'a> for Message<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.message_oneof = TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::send_coins_message(r.read_message::<TW::Cosmos::Proto::mod_Message::Send>(bytes)?),
                Ok(18) => msg.message_oneof = TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::transfer_tokens_message(r.read_message::<TW::Cosmos::Proto::mod_Message::Transfer>(bytes)?),
                Ok(26) => msg.message_oneof = TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::stake_message(r.read_message::<TW::Cosmos::Proto::mod_Message::Delegate>(bytes)?),
                Ok(34) => msg.message_oneof = TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::unstake_message(r.read_message::<TW::Cosmos::Proto::mod_Message::Undelegate>(bytes)?),
                Ok(42) => msg.message_oneof = TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::restake_message(r.read_message::<TW::Cosmos::Proto::mod_Message::BeginRedelegate>(bytes)?),
                Ok(50) => msg.message_oneof = TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::withdraw_stake_reward_message(r.read_message::<TW::Cosmos::Proto::mod_Message::WithdrawDelegationReward>(bytes)?),
                Ok(58) => msg.message_oneof = TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::raw_json_message(r.read_message::<TW::Cosmos::Proto::mod_Message::RawJSON>(bytes)?),
                Ok(66) => msg.message_oneof = TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::wasm_terra_execute_contract_transfer_message(r.read_message::<TW::Cosmos::Proto::mod_Message::WasmTerraExecuteContractTransfer>(bytes)?),
                Ok(74) => msg.message_oneof = TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::wasm_terra_execute_contract_send_message(r.read_message::<TW::Cosmos::Proto::mod_Message::WasmTerraExecuteContractSend>(bytes)?),
                Ok(82) => msg.message_oneof = TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::thorchain_send_message(r.read_message::<TW::Cosmos::Proto::mod_Message::THORChainSend>(bytes)?),
                Ok(90) => msg.message_oneof = TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::wasm_terra_execute_contract_generic(r.read_message::<TW::Cosmos::Proto::mod_Message::WasmTerraExecuteContractGeneric>(bytes)?),
                Ok(98) => msg.message_oneof = TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::wasm_execute_contract_transfer_message(r.read_message::<TW::Cosmos::Proto::mod_Message::WasmExecuteContractTransfer>(bytes)?),
                Ok(106) => msg.message_oneof = TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::wasm_execute_contract_send_message(r.read_message::<TW::Cosmos::Proto::mod_Message::WasmExecuteContractSend>(bytes)?),
                Ok(114) => msg.message_oneof = TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::wasm_execute_contract_generic(r.read_message::<TW::Cosmos::Proto::mod_Message::WasmExecuteContractGeneric>(bytes)?),
                Ok(122) => msg.message_oneof = TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::sign_direct_message(r.read_message::<TW::Cosmos::Proto::mod_Message::SignDirect>(bytes)?),
                Ok(130) => msg.message_oneof = TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::auth_grant(r.read_message::<TW::Cosmos::Proto::mod_Message::AuthGrant>(bytes)?),
                Ok(138) => msg.message_oneof = TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::auth_revoke(r.read_message::<TW::Cosmos::Proto::mod_Message::AuthRevoke>(bytes)?),
                Ok(146) => msg.message_oneof = TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::msg_vote(r.read_message::<TW::Cosmos::Proto::mod_Message::MsgVote>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Message<'a> {
    fn get_size(&self) -> usize {
        0
        + match self.message_oneof {
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::send_coins_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::transfer_tokens_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::stake_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::unstake_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::restake_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::withdraw_stake_reward_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::raw_json_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::wasm_terra_execute_contract_transfer_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::wasm_terra_execute_contract_send_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::thorchain_send_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::wasm_terra_execute_contract_generic(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::wasm_execute_contract_transfer_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::wasm_execute_contract_send_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::wasm_execute_contract_generic(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::sign_direct_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::auth_grant(ref m) => 2 + sizeof_len((m).get_size()),
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::auth_revoke(ref m) => 2 + sizeof_len((m).get_size()),
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::msg_vote(ref m) => 2 + sizeof_len((m).get_size()),
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        match self.message_oneof {            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::send_coins_message(ref m) => { w.write_with_tag(10, |w| w.write_message(m))? },
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::transfer_tokens_message(ref m) => { w.write_with_tag(18, |w| w.write_message(m))? },
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::stake_message(ref m) => { w.write_with_tag(26, |w| w.write_message(m))? },
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::unstake_message(ref m) => { w.write_with_tag(34, |w| w.write_message(m))? },
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::restake_message(ref m) => { w.write_with_tag(42, |w| w.write_message(m))? },
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::withdraw_stake_reward_message(ref m) => { w.write_with_tag(50, |w| w.write_message(m))? },
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::raw_json_message(ref m) => { w.write_with_tag(58, |w| w.write_message(m))? },
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::wasm_terra_execute_contract_transfer_message(ref m) => { w.write_with_tag(66, |w| w.write_message(m))? },
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::wasm_terra_execute_contract_send_message(ref m) => { w.write_with_tag(74, |w| w.write_message(m))? },
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::thorchain_send_message(ref m) => { w.write_with_tag(82, |w| w.write_message(m))? },
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::wasm_terra_execute_contract_generic(ref m) => { w.write_with_tag(90, |w| w.write_message(m))? },
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::wasm_execute_contract_transfer_message(ref m) => { w.write_with_tag(98, |w| w.write_message(m))? },
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::wasm_execute_contract_send_message(ref m) => { w.write_with_tag(106, |w| w.write_message(m))? },
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::wasm_execute_contract_generic(ref m) => { w.write_with_tag(114, |w| w.write_message(m))? },
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::sign_direct_message(ref m) => { w.write_with_tag(122, |w| w.write_message(m))? },
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::auth_grant(ref m) => { w.write_with_tag(130, |w| w.write_message(m))? },
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::auth_revoke(ref m) => { w.write_with_tag(138, |w| w.write_message(m))? },
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::msg_vote(ref m) => { w.write_with_tag(146, |w| w.write_message(m))? },
            TW::Cosmos::Proto::mod_Message::OneOfmessage_oneof::None => {},
    }        Ok(())
    }
}

pub mod mod_Message {

use std::borrow::Cow;
use super::*;

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Send<'a> {
    pub from_address: Cow<'a, str>,
    pub to_address: Cow<'a, str>,
    pub amounts: Vec<TW::Cosmos::Proto::Amount<'a>>,
    pub type_prefix: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for Send<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.from_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.to_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.amounts.push(r.read_message::<TW::Cosmos::Proto::Amount>(bytes)?),
                Ok(34) => msg.type_prefix = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Send<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.from_address == "" { 0 } else { 1 + sizeof_len((&self.from_address).len()) }
        + if self.to_address == "" { 0 } else { 1 + sizeof_len((&self.to_address).len()) }
        + self.amounts.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + if self.type_prefix == "" { 0 } else { 1 + sizeof_len((&self.type_prefix).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.from_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.from_address))?; }
        if self.to_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.to_address))?; }
        for s in &self.amounts { w.write_with_tag(26, |w| w.write_message(s))?; }
        if self.type_prefix != "" { w.write_with_tag(34, |w| w.write_string(&**&self.type_prefix))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Transfer<'a> {
    pub source_port: Cow<'a, str>,
    pub source_channel: Cow<'a, str>,
    pub token: Option<TW::Cosmos::Proto::Amount<'a>>,
    pub sender: Cow<'a, str>,
    pub receiver: Cow<'a, str>,
    pub timeout_height: Option<TW::Cosmos::Proto::Height>,
    pub timeout_timestamp: u64,
}

impl<'a> MessageRead<'a> for Transfer<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.source_port = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.source_channel = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.token = Some(r.read_message::<TW::Cosmos::Proto::Amount>(bytes)?),
                Ok(34) => msg.sender = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.receiver = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(50) => msg.timeout_height = Some(r.read_message::<TW::Cosmos::Proto::Height>(bytes)?),
                Ok(56) => msg.timeout_timestamp = r.read_uint64(bytes)?,
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
        + if self.source_port == "" { 0 } else { 1 + sizeof_len((&self.source_port).len()) }
        + if self.source_channel == "" { 0 } else { 1 + sizeof_len((&self.source_channel).len()) }
        + self.token.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.sender == "" { 0 } else { 1 + sizeof_len((&self.sender).len()) }
        + if self.receiver == "" { 0 } else { 1 + sizeof_len((&self.receiver).len()) }
        + self.timeout_height.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.timeout_timestamp == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.timeout_timestamp) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.source_port != "" { w.write_with_tag(10, |w| w.write_string(&**&self.source_port))?; }
        if self.source_channel != "" { w.write_with_tag(18, |w| w.write_string(&**&self.source_channel))?; }
        if let Some(ref s) = self.token { w.write_with_tag(26, |w| w.write_message(s))?; }
        if self.sender != "" { w.write_with_tag(34, |w| w.write_string(&**&self.sender))?; }
        if self.receiver != "" { w.write_with_tag(42, |w| w.write_string(&**&self.receiver))?; }
        if let Some(ref s) = self.timeout_height { w.write_with_tag(50, |w| w.write_message(s))?; }
        if self.timeout_timestamp != 0u64 { w.write_with_tag(56, |w| w.write_uint64(*&self.timeout_timestamp))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Delegate<'a> {
    pub delegator_address: Cow<'a, str>,
    pub validator_address: Cow<'a, str>,
    pub amount: Option<TW::Cosmos::Proto::Amount<'a>>,
    pub type_prefix: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for Delegate<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.delegator_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.validator_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.amount = Some(r.read_message::<TW::Cosmos::Proto::Amount>(bytes)?),
                Ok(34) => msg.type_prefix = r.read_string(bytes).map(Cow::Borrowed)?,
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
        + if self.delegator_address == "" { 0 } else { 1 + sizeof_len((&self.delegator_address).len()) }
        + if self.validator_address == "" { 0 } else { 1 + sizeof_len((&self.validator_address).len()) }
        + self.amount.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.type_prefix == "" { 0 } else { 1 + sizeof_len((&self.type_prefix).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.delegator_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.delegator_address))?; }
        if self.validator_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.validator_address))?; }
        if let Some(ref s) = self.amount { w.write_with_tag(26, |w| w.write_message(s))?; }
        if self.type_prefix != "" { w.write_with_tag(34, |w| w.write_string(&**&self.type_prefix))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Undelegate<'a> {
    pub delegator_address: Cow<'a, str>,
    pub validator_address: Cow<'a, str>,
    pub amount: Option<TW::Cosmos::Proto::Amount<'a>>,
    pub type_prefix: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for Undelegate<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.delegator_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.validator_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.amount = Some(r.read_message::<TW::Cosmos::Proto::Amount>(bytes)?),
                Ok(34) => msg.type_prefix = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Undelegate<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.delegator_address == "" { 0 } else { 1 + sizeof_len((&self.delegator_address).len()) }
        + if self.validator_address == "" { 0 } else { 1 + sizeof_len((&self.validator_address).len()) }
        + self.amount.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.type_prefix == "" { 0 } else { 1 + sizeof_len((&self.type_prefix).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.delegator_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.delegator_address))?; }
        if self.validator_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.validator_address))?; }
        if let Some(ref s) = self.amount { w.write_with_tag(26, |w| w.write_message(s))?; }
        if self.type_prefix != "" { w.write_with_tag(34, |w| w.write_string(&**&self.type_prefix))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct BeginRedelegate<'a> {
    pub delegator_address: Cow<'a, str>,
    pub validator_src_address: Cow<'a, str>,
    pub validator_dst_address: Cow<'a, str>,
    pub amount: Option<TW::Cosmos::Proto::Amount<'a>>,
    pub type_prefix: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for BeginRedelegate<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.delegator_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.validator_src_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.validator_dst_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.amount = Some(r.read_message::<TW::Cosmos::Proto::Amount>(bytes)?),
                Ok(42) => msg.type_prefix = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for BeginRedelegate<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.delegator_address == "" { 0 } else { 1 + sizeof_len((&self.delegator_address).len()) }
        + if self.validator_src_address == "" { 0 } else { 1 + sizeof_len((&self.validator_src_address).len()) }
        + if self.validator_dst_address == "" { 0 } else { 1 + sizeof_len((&self.validator_dst_address).len()) }
        + self.amount.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.type_prefix == "" { 0 } else { 1 + sizeof_len((&self.type_prefix).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.delegator_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.delegator_address))?; }
        if self.validator_src_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.validator_src_address))?; }
        if self.validator_dst_address != "" { w.write_with_tag(26, |w| w.write_string(&**&self.validator_dst_address))?; }
        if let Some(ref s) = self.amount { w.write_with_tag(34, |w| w.write_message(s))?; }
        if self.type_prefix != "" { w.write_with_tag(42, |w| w.write_string(&**&self.type_prefix))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct WithdrawDelegationReward<'a> {
    pub delegator_address: Cow<'a, str>,
    pub validator_address: Cow<'a, str>,
    pub type_prefix: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for WithdrawDelegationReward<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.delegator_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.validator_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.type_prefix = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for WithdrawDelegationReward<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.delegator_address == "" { 0 } else { 1 + sizeof_len((&self.delegator_address).len()) }
        + if self.validator_address == "" { 0 } else { 1 + sizeof_len((&self.validator_address).len()) }
        + if self.type_prefix == "" { 0 } else { 1 + sizeof_len((&self.type_prefix).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.delegator_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.delegator_address))?; }
        if self.validator_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.validator_address))?; }
        if self.type_prefix != "" { w.write_with_tag(26, |w| w.write_string(&**&self.type_prefix))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct WasmTerraExecuteContractTransfer<'a> {
    pub sender_address: Cow<'a, str>,
    pub contract_address: Cow<'a, str>,
    pub amount: Cow<'a, [u8]>,
    pub recipient_address: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for WasmTerraExecuteContractTransfer<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.sender_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.contract_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.amount = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.recipient_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for WasmTerraExecuteContractTransfer<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.sender_address == "" { 0 } else { 1 + sizeof_len((&self.sender_address).len()) }
        + if self.contract_address == "" { 0 } else { 1 + sizeof_len((&self.contract_address).len()) }
        + if self.amount == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.amount).len()) }
        + if self.recipient_address == "" { 0 } else { 1 + sizeof_len((&self.recipient_address).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.sender_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.sender_address))?; }
        if self.contract_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.contract_address))?; }
        if self.amount != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.amount))?; }
        if self.recipient_address != "" { w.write_with_tag(34, |w| w.write_string(&**&self.recipient_address))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct WasmTerraExecuteContractSend<'a> {
    pub sender_address: Cow<'a, str>,
    pub contract_address: Cow<'a, str>,
    pub amount: Cow<'a, [u8]>,
    pub recipient_contract_address: Cow<'a, str>,
    pub msg: Cow<'a, str>,
    pub coin: Vec<Cow<'a, str>>,
}

impl<'a> MessageRead<'a> for WasmTerraExecuteContractSend<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.sender_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.contract_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.amount = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.recipient_contract_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.msg = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(50) => msg.coin.push(r.read_string(bytes).map(Cow::Borrowed)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for WasmTerraExecuteContractSend<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.sender_address == "" { 0 } else { 1 + sizeof_len((&self.sender_address).len()) }
        + if self.contract_address == "" { 0 } else { 1 + sizeof_len((&self.contract_address).len()) }
        + if self.amount == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.amount).len()) }
        + if self.recipient_contract_address == "" { 0 } else { 1 + sizeof_len((&self.recipient_contract_address).len()) }
        + if self.msg == "" { 0 } else { 1 + sizeof_len((&self.msg).len()) }
        + self.coin.iter().map(|s| 1 + sizeof_len((s).len())).sum::<usize>()
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.sender_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.sender_address))?; }
        if self.contract_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.contract_address))?; }
        if self.amount != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.amount))?; }
        if self.recipient_contract_address != "" { w.write_with_tag(34, |w| w.write_string(&**&self.recipient_contract_address))?; }
        if self.msg != "" { w.write_with_tag(42, |w| w.write_string(&**&self.msg))?; }
        for s in &self.coin { w.write_with_tag(50, |w| w.write_string(&**s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct THORChainSend<'a> {
    pub from_address: Cow<'a, [u8]>,
    pub to_address: Cow<'a, [u8]>,
    pub amounts: Vec<TW::Cosmos::Proto::Amount<'a>>,
}

impl<'a> MessageRead<'a> for THORChainSend<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.from_address = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.to_address = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.amounts.push(r.read_message::<TW::Cosmos::Proto::Amount>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for THORChainSend<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.from_address == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.from_address).len()) }
        + if self.to_address == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.to_address).len()) }
        + self.amounts.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.from_address != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.from_address))?; }
        if self.to_address != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.to_address))?; }
        for s in &self.amounts { w.write_with_tag(26, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct WasmTerraExecuteContractGeneric<'a> {
    pub sender_address: Cow<'a, str>,
    pub contract_address: Cow<'a, str>,
    pub execute_msg: Cow<'a, str>,
    pub coins: Vec<TW::Cosmos::Proto::Amount<'a>>,
}

impl<'a> MessageRead<'a> for WasmTerraExecuteContractGeneric<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.sender_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.contract_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.execute_msg = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.coins.push(r.read_message::<TW::Cosmos::Proto::Amount>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for WasmTerraExecuteContractGeneric<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.sender_address == "" { 0 } else { 1 + sizeof_len((&self.sender_address).len()) }
        + if self.contract_address == "" { 0 } else { 1 + sizeof_len((&self.contract_address).len()) }
        + if self.execute_msg == "" { 0 } else { 1 + sizeof_len((&self.execute_msg).len()) }
        + self.coins.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.sender_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.sender_address))?; }
        if self.contract_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.contract_address))?; }
        if self.execute_msg != "" { w.write_with_tag(26, |w| w.write_string(&**&self.execute_msg))?; }
        for s in &self.coins { w.write_with_tag(42, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct WasmExecuteContractTransfer<'a> {
    pub sender_address: Cow<'a, str>,
    pub contract_address: Cow<'a, str>,
    pub amount: Cow<'a, [u8]>,
    pub recipient_address: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for WasmExecuteContractTransfer<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.sender_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.contract_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.amount = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.recipient_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for WasmExecuteContractTransfer<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.sender_address == "" { 0 } else { 1 + sizeof_len((&self.sender_address).len()) }
        + if self.contract_address == "" { 0 } else { 1 + sizeof_len((&self.contract_address).len()) }
        + if self.amount == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.amount).len()) }
        + if self.recipient_address == "" { 0 } else { 1 + sizeof_len((&self.recipient_address).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.sender_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.sender_address))?; }
        if self.contract_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.contract_address))?; }
        if self.amount != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.amount))?; }
        if self.recipient_address != "" { w.write_with_tag(34, |w| w.write_string(&**&self.recipient_address))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct WasmExecuteContractSend<'a> {
    pub sender_address: Cow<'a, str>,
    pub contract_address: Cow<'a, str>,
    pub amount: Cow<'a, [u8]>,
    pub recipient_contract_address: Cow<'a, str>,
    pub msg: Cow<'a, str>,
    pub coin: Vec<Cow<'a, str>>,
}

impl<'a> MessageRead<'a> for WasmExecuteContractSend<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.sender_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.contract_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.amount = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.recipient_contract_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.msg = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(50) => msg.coin.push(r.read_string(bytes).map(Cow::Borrowed)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for WasmExecuteContractSend<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.sender_address == "" { 0 } else { 1 + sizeof_len((&self.sender_address).len()) }
        + if self.contract_address == "" { 0 } else { 1 + sizeof_len((&self.contract_address).len()) }
        + if self.amount == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.amount).len()) }
        + if self.recipient_contract_address == "" { 0 } else { 1 + sizeof_len((&self.recipient_contract_address).len()) }
        + if self.msg == "" { 0 } else { 1 + sizeof_len((&self.msg).len()) }
        + self.coin.iter().map(|s| 1 + sizeof_len((s).len())).sum::<usize>()
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.sender_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.sender_address))?; }
        if self.contract_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.contract_address))?; }
        if self.amount != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.amount))?; }
        if self.recipient_contract_address != "" { w.write_with_tag(34, |w| w.write_string(&**&self.recipient_contract_address))?; }
        if self.msg != "" { w.write_with_tag(42, |w| w.write_string(&**&self.msg))?; }
        for s in &self.coin { w.write_with_tag(50, |w| w.write_string(&**s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct WasmExecuteContractGeneric<'a> {
    pub sender_address: Cow<'a, str>,
    pub contract_address: Cow<'a, str>,
    pub execute_msg: Cow<'a, str>,
    pub coins: Vec<TW::Cosmos::Proto::Amount<'a>>,
}

impl<'a> MessageRead<'a> for WasmExecuteContractGeneric<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.sender_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.contract_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.execute_msg = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.coins.push(r.read_message::<TW::Cosmos::Proto::Amount>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for WasmExecuteContractGeneric<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.sender_address == "" { 0 } else { 1 + sizeof_len((&self.sender_address).len()) }
        + if self.contract_address == "" { 0 } else { 1 + sizeof_len((&self.contract_address).len()) }
        + if self.execute_msg == "" { 0 } else { 1 + sizeof_len((&self.execute_msg).len()) }
        + self.coins.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.sender_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.sender_address))?; }
        if self.contract_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.contract_address))?; }
        if self.execute_msg != "" { w.write_with_tag(26, |w| w.write_string(&**&self.execute_msg))?; }
        for s in &self.coins { w.write_with_tag(42, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct RawJSON<'a> {
    pub type_pb: Cow<'a, str>,
    pub value: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for RawJSON<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.type_pb = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.value = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for RawJSON<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.type_pb == "" { 0 } else { 1 + sizeof_len((&self.type_pb).len()) }
        + if self.value == "" { 0 } else { 1 + sizeof_len((&self.value).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.type_pb != "" { w.write_with_tag(10, |w| w.write_string(&**&self.type_pb))?; }
        if self.value != "" { w.write_with_tag(18, |w| w.write_string(&**&self.value))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SignDirect<'a> {
    pub body_bytes: Cow<'a, [u8]>,
    pub auth_info_bytes: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for SignDirect<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.body_bytes = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.auth_info_bytes = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for SignDirect<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.body_bytes == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.body_bytes).len()) }
        + if self.auth_info_bytes == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.auth_info_bytes).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.body_bytes != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.body_bytes))?; }
        if self.auth_info_bytes != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.auth_info_bytes))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct StakeAuthorization<'a> {
    pub max_tokens: Option<TW::Cosmos::Proto::Amount<'a>>,
    pub authorization_type: TW::Cosmos::Proto::mod_Message::AuthorizationType,
    pub validators: TW::Cosmos::Proto::mod_Message::mod_StakeAuthorization::OneOfvalidators<'a>,
}

impl<'a> MessageRead<'a> for StakeAuthorization<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.max_tokens = Some(r.read_message::<TW::Cosmos::Proto::Amount>(bytes)?),
                Ok(32) => msg.authorization_type = r.read_enum(bytes)?,
                Ok(18) => msg.validators = TW::Cosmos::Proto::mod_Message::mod_StakeAuthorization::OneOfvalidators::allow_list(r.read_message::<TW::Cosmos::Proto::mod_Message::mod_StakeAuthorization::Validators>(bytes)?),
                Ok(26) => msg.validators = TW::Cosmos::Proto::mod_Message::mod_StakeAuthorization::OneOfvalidators::deny_list(r.read_message::<TW::Cosmos::Proto::mod_Message::mod_StakeAuthorization::Validators>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for StakeAuthorization<'a> {
    fn get_size(&self) -> usize {
        0
        + self.max_tokens.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.authorization_type == TW::Cosmos::Proto::mod_Message::AuthorizationType::UNSPECIFIED { 0 } else { 1 + sizeof_varint(*(&self.authorization_type) as u64) }
        + match self.validators {
            TW::Cosmos::Proto::mod_Message::mod_StakeAuthorization::OneOfvalidators::allow_list(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Cosmos::Proto::mod_Message::mod_StakeAuthorization::OneOfvalidators::deny_list(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Cosmos::Proto::mod_Message::mod_StakeAuthorization::OneOfvalidators::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.max_tokens { w.write_with_tag(10, |w| w.write_message(s))?; }
        if self.authorization_type != TW::Cosmos::Proto::mod_Message::AuthorizationType::UNSPECIFIED { w.write_with_tag(32, |w| w.write_enum(*&self.authorization_type as i32))?; }
        match self.validators {            TW::Cosmos::Proto::mod_Message::mod_StakeAuthorization::OneOfvalidators::allow_list(ref m) => { w.write_with_tag(18, |w| w.write_message(m))? },
            TW::Cosmos::Proto::mod_Message::mod_StakeAuthorization::OneOfvalidators::deny_list(ref m) => { w.write_with_tag(26, |w| w.write_message(m))? },
            TW::Cosmos::Proto::mod_Message::mod_StakeAuthorization::OneOfvalidators::None => {},
    }        Ok(())
    }
}

pub mod mod_StakeAuthorization {

use std::borrow::Cow;
use super::*;

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Validators<'a> {
    pub address: Vec<Cow<'a, str>>,
}

impl<'a> MessageRead<'a> for Validators<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.address.push(r.read_string(bytes).map(Cow::Borrowed)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Validators<'a> {
    fn get_size(&self) -> usize {
        0
        + self.address.iter().map(|s| 1 + sizeof_len((s).len())).sum::<usize>()
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        for s in &self.address { w.write_with_tag(10, |w| w.write_string(&**s))?; }
        Ok(())
    }
}

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfvalidators<'a> {
    allow_list(TW::Cosmos::Proto::mod_Message::mod_StakeAuthorization::Validators<'a>),
    deny_list(TW::Cosmos::Proto::mod_Message::mod_StakeAuthorization::Validators<'a>),
    None,
}

impl<'a> Default for OneOfvalidators<'a> {
    fn default() -> Self {
        OneOfvalidators::None
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct AuthGrant<'a> {
    pub granter: Cow<'a, str>,
    pub grantee: Cow<'a, str>,
    pub expiration: i64,
    pub grant_type: TW::Cosmos::Proto::mod_Message::mod_AuthGrant::OneOfgrant_type<'a>,
}

impl<'a> MessageRead<'a> for AuthGrant<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.granter = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.grantee = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(32) => msg.expiration = r.read_int64(bytes)?,
                Ok(26) => msg.grant_type = TW::Cosmos::Proto::mod_Message::mod_AuthGrant::OneOfgrant_type::grant_stake(r.read_message::<TW::Cosmos::Proto::mod_Message::StakeAuthorization>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for AuthGrant<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.granter == "" { 0 } else { 1 + sizeof_len((&self.granter).len()) }
        + if self.grantee == "" { 0 } else { 1 + sizeof_len((&self.grantee).len()) }
        + if self.expiration == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.expiration) as u64) }
        + match self.grant_type {
            TW::Cosmos::Proto::mod_Message::mod_AuthGrant::OneOfgrant_type::grant_stake(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Cosmos::Proto::mod_Message::mod_AuthGrant::OneOfgrant_type::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.granter != "" { w.write_with_tag(10, |w| w.write_string(&**&self.granter))?; }
        if self.grantee != "" { w.write_with_tag(18, |w| w.write_string(&**&self.grantee))?; }
        if self.expiration != 0i64 { w.write_with_tag(32, |w| w.write_int64(*&self.expiration))?; }
        match self.grant_type {            TW::Cosmos::Proto::mod_Message::mod_AuthGrant::OneOfgrant_type::grant_stake(ref m) => { w.write_with_tag(26, |w| w.write_message(m))? },
            TW::Cosmos::Proto::mod_Message::mod_AuthGrant::OneOfgrant_type::None => {},
    }        Ok(())
    }
}

pub mod mod_AuthGrant {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfgrant_type<'a> {
    grant_stake(TW::Cosmos::Proto::mod_Message::StakeAuthorization<'a>),
    None,
}

impl<'a> Default for OneOfgrant_type<'a> {
    fn default() -> Self {
        OneOfgrant_type::None
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct AuthRevoke<'a> {
    pub granter: Cow<'a, str>,
    pub grantee: Cow<'a, str>,
    pub msg_type_url: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for AuthRevoke<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.granter = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.grantee = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.msg_type_url = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for AuthRevoke<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.granter == "" { 0 } else { 1 + sizeof_len((&self.granter).len()) }
        + if self.grantee == "" { 0 } else { 1 + sizeof_len((&self.grantee).len()) }
        + if self.msg_type_url == "" { 0 } else { 1 + sizeof_len((&self.msg_type_url).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.granter != "" { w.write_with_tag(10, |w| w.write_string(&**&self.granter))?; }
        if self.grantee != "" { w.write_with_tag(18, |w| w.write_string(&**&self.grantee))?; }
        if self.msg_type_url != "" { w.write_with_tag(26, |w| w.write_string(&**&self.msg_type_url))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct MsgVote<'a> {
    pub proposal_id: u64,
    pub voter: Cow<'a, str>,
    pub option: TW::Cosmos::Proto::mod_Message::VoteOption,
}

impl<'a> MessageRead<'a> for MsgVote<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.proposal_id = r.read_uint64(bytes)?,
                Ok(18) => msg.voter = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.option = r.read_enum(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for MsgVote<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.proposal_id == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.proposal_id) as u64) }
        + if self.voter == "" { 0 } else { 1 + sizeof_len((&self.voter).len()) }
        + if self.option == TW::Cosmos::Proto::mod_Message::VoteOption::_UNSPECIFIED { 0 } else { 1 + sizeof_varint(*(&self.option) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.proposal_id != 0u64 { w.write_with_tag(8, |w| w.write_uint64(*&self.proposal_id))?; }
        if self.voter != "" { w.write_with_tag(18, |w| w.write_string(&**&self.voter))?; }
        if self.option != TW::Cosmos::Proto::mod_Message::VoteOption::_UNSPECIFIED { w.write_with_tag(24, |w| w.write_enum(*&self.option as i32))?; }
        Ok(())
    }
}

#[derive(Debug, PartialEq, Eq, Clone, Copy)]
pub enum AuthorizationType {
    UNSPECIFIED = 0,
    DELEGATE = 1,
    UNDELEGATE = 2,
    REDELEGATE = 3,
}

impl Default for AuthorizationType {
    fn default() -> Self {
        AuthorizationType::UNSPECIFIED
    }
}

impl From<i32> for AuthorizationType {
    fn from(i: i32) -> Self {
        match i {
            0 => AuthorizationType::UNSPECIFIED,
            1 => AuthorizationType::DELEGATE,
            2 => AuthorizationType::UNDELEGATE,
            3 => AuthorizationType::REDELEGATE,
            _ => Self::default(),
        }
    }
}

impl<'a> From<&'a str> for AuthorizationType {
    fn from(s: &'a str) -> Self {
        match s {
            "UNSPECIFIED" => AuthorizationType::UNSPECIFIED,
            "DELEGATE" => AuthorizationType::DELEGATE,
            "UNDELEGATE" => AuthorizationType::UNDELEGATE,
            "REDELEGATE" => AuthorizationType::REDELEGATE,
            _ => Self::default(),
        }
    }
}

#[derive(Debug, PartialEq, Eq, Clone, Copy)]
pub enum VoteOption {
    _UNSPECIFIED = 0,
    YES = 1,
    ABSTAIN = 2,
    NO = 3,
    NO_WITH_VETO = 4,
}

impl Default for VoteOption {
    fn default() -> Self {
        VoteOption::_UNSPECIFIED
    }
}

impl From<i32> for VoteOption {
    fn from(i: i32) -> Self {
        match i {
            0 => VoteOption::_UNSPECIFIED,
            1 => VoteOption::YES,
            2 => VoteOption::ABSTAIN,
            3 => VoteOption::NO,
            4 => VoteOption::NO_WITH_VETO,
            _ => Self::default(),
        }
    }
}

impl<'a> From<&'a str> for VoteOption {
    fn from(s: &'a str) -> Self {
        match s {
            "_UNSPECIFIED" => VoteOption::_UNSPECIFIED,
            "YES" => VoteOption::YES,
            "ABSTAIN" => VoteOption::ABSTAIN,
            "NO" => VoteOption::NO,
            "NO_WITH_VETO" => VoteOption::NO_WITH_VETO,
            _ => Self::default(),
        }
    }
}

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfmessage_oneof<'a> {
    send_coins_message(TW::Cosmos::Proto::mod_Message::Send<'a>),
    transfer_tokens_message(TW::Cosmos::Proto::mod_Message::Transfer<'a>),
    stake_message(TW::Cosmos::Proto::mod_Message::Delegate<'a>),
    unstake_message(TW::Cosmos::Proto::mod_Message::Undelegate<'a>),
    restake_message(TW::Cosmos::Proto::mod_Message::BeginRedelegate<'a>),
    withdraw_stake_reward_message(TW::Cosmos::Proto::mod_Message::WithdrawDelegationReward<'a>),
    raw_json_message(TW::Cosmos::Proto::mod_Message::RawJSON<'a>),
    wasm_terra_execute_contract_transfer_message(TW::Cosmos::Proto::mod_Message::WasmTerraExecuteContractTransfer<'a>),
    wasm_terra_execute_contract_send_message(TW::Cosmos::Proto::mod_Message::WasmTerraExecuteContractSend<'a>),
    thorchain_send_message(TW::Cosmos::Proto::mod_Message::THORChainSend<'a>),
    wasm_terra_execute_contract_generic(TW::Cosmos::Proto::mod_Message::WasmTerraExecuteContractGeneric<'a>),
    wasm_execute_contract_transfer_message(TW::Cosmos::Proto::mod_Message::WasmExecuteContractTransfer<'a>),
    wasm_execute_contract_send_message(TW::Cosmos::Proto::mod_Message::WasmExecuteContractSend<'a>),
    wasm_execute_contract_generic(TW::Cosmos::Proto::mod_Message::WasmExecuteContractGeneric<'a>),
    sign_direct_message(TW::Cosmos::Proto::mod_Message::SignDirect<'a>),
    auth_grant(TW::Cosmos::Proto::mod_Message::AuthGrant<'a>),
    auth_revoke(TW::Cosmos::Proto::mod_Message::AuthRevoke<'a>),
    msg_vote(TW::Cosmos::Proto::mod_Message::MsgVote<'a>),
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
    pub signing_mode: TW::Cosmos::Proto::SigningMode,
    pub account_number: u64,
    pub chain_id: Cow<'a, str>,
    pub fee: Option<TW::Cosmos::Proto::Fee<'a>>,
    pub memo: Cow<'a, str>,
    pub sequence: u64,
    pub private_key: Cow<'a, [u8]>,
    pub messages: Vec<TW::Cosmos::Proto::Message<'a>>,
    pub mode: TW::Cosmos::Proto::BroadcastMode,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.signing_mode = r.read_enum(bytes)?,
                Ok(16) => msg.account_number = r.read_uint64(bytes)?,
                Ok(26) => msg.chain_id = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.fee = Some(r.read_message::<TW::Cosmos::Proto::Fee>(bytes)?),
                Ok(42) => msg.memo = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(48) => msg.sequence = r.read_uint64(bytes)?,
                Ok(58) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(66) => msg.messages.push(r.read_message::<TW::Cosmos::Proto::Message>(bytes)?),
                Ok(72) => msg.mode = r.read_enum(bytes)?,
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
        + if self.signing_mode == TW::Cosmos::Proto::SigningMode::JSON { 0 } else { 1 + sizeof_varint(*(&self.signing_mode) as u64) }
        + if self.account_number == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.account_number) as u64) }
        + if self.chain_id == "" { 0 } else { 1 + sizeof_len((&self.chain_id).len()) }
        + self.fee.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.memo == "" { 0 } else { 1 + sizeof_len((&self.memo).len()) }
        + if self.sequence == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.sequence) as u64) }
        + if self.private_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.private_key).len()) }
        + self.messages.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + if self.mode == TW::Cosmos::Proto::BroadcastMode::BLOCK { 0 } else { 1 + sizeof_varint(*(&self.mode) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.signing_mode != TW::Cosmos::Proto::SigningMode::JSON { w.write_with_tag(8, |w| w.write_enum(*&self.signing_mode as i32))?; }
        if self.account_number != 0u64 { w.write_with_tag(16, |w| w.write_uint64(*&self.account_number))?; }
        if self.chain_id != "" { w.write_with_tag(26, |w| w.write_string(&**&self.chain_id))?; }
        if let Some(ref s) = self.fee { w.write_with_tag(34, |w| w.write_message(s))?; }
        if self.memo != "" { w.write_with_tag(42, |w| w.write_string(&**&self.memo))?; }
        if self.sequence != 0u64 { w.write_with_tag(48, |w| w.write_uint64(*&self.sequence))?; }
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(58, |w| w.write_bytes(&**&self.private_key))?; }
        for s in &self.messages { w.write_with_tag(66, |w| w.write_message(s))?; }
        if self.mode != TW::Cosmos::Proto::BroadcastMode::BLOCK { w.write_with_tag(72, |w| w.write_enum(*&self.mode as i32))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningOutput<'a> {
    pub signature: Cow<'a, [u8]>,
    pub json: Cow<'a, str>,
    pub serialized: Cow<'a, str>,
    pub error: Cow<'a, str>,
    pub signature_json: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.signature = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.json = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.serialized = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.error = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.signature_json = r.read_string(bytes).map(Cow::Borrowed)?,
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
        + if self.signature == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.signature).len()) }
        + if self.json == "" { 0 } else { 1 + sizeof_len((&self.json).len()) }
        + if self.serialized == "" { 0 } else { 1 + sizeof_len((&self.serialized).len()) }
        + if self.error == "" { 0 } else { 1 + sizeof_len((&self.error).len()) }
        + if self.signature_json == "" { 0 } else { 1 + sizeof_len((&self.signature_json).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.signature != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.signature))?; }
        if self.json != "" { w.write_with_tag(18, |w| w.write_string(&**&self.json))?; }
        if self.serialized != "" { w.write_with_tag(26, |w| w.write_string(&**&self.serialized))?; }
        if self.error != "" { w.write_with_tag(34, |w| w.write_string(&**&self.error))?; }
        if self.signature_json != "" { w.write_with_tag(42, |w| w.write_string(&**&self.signature_json))?; }
        Ok(())
    }
}

