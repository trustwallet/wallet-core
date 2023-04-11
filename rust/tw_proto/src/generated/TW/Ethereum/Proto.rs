// Automatically generated rust module for 'Ethereum.proto' file

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
pub enum TransactionMode {
    Legacy = 0,
    Enveloped = 1,
    UserOp = 2,
}

impl Default for TransactionMode {
    fn default() -> Self {
        TransactionMode::Legacy
    }
}

impl From<i32> for TransactionMode {
    fn from(i: i32) -> Self {
        match i {
            0 => TransactionMode::Legacy,
            1 => TransactionMode::Enveloped,
            2 => TransactionMode::UserOp,
            _ => Self::default(),
        }
    }
}

impl<'a> From<&'a str> for TransactionMode {
    fn from(s: &'a str) -> Self {
        match s {
            "Legacy" => TransactionMode::Legacy,
            "Enveloped" => TransactionMode::Enveloped,
            "UserOp" => TransactionMode::UserOp,
            _ => Self::default(),
        }
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Transaction<'a> {
    pub transaction_oneof: TW::Ethereum::Proto::mod_Transaction::OneOftransaction_oneof<'a>,
}

impl<'a> MessageRead<'a> for Transaction<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.transaction_oneof = TW::Ethereum::Proto::mod_Transaction::OneOftransaction_oneof::transfer(r.read_message::<TW::Ethereum::Proto::mod_Transaction::Transfer>(bytes)?),
                Ok(18) => msg.transaction_oneof = TW::Ethereum::Proto::mod_Transaction::OneOftransaction_oneof::erc20_transfer(r.read_message::<TW::Ethereum::Proto::mod_Transaction::ERC20Transfer>(bytes)?),
                Ok(26) => msg.transaction_oneof = TW::Ethereum::Proto::mod_Transaction::OneOftransaction_oneof::erc20_approve(r.read_message::<TW::Ethereum::Proto::mod_Transaction::ERC20Approve>(bytes)?),
                Ok(34) => msg.transaction_oneof = TW::Ethereum::Proto::mod_Transaction::OneOftransaction_oneof::erc721_transfer(r.read_message::<TW::Ethereum::Proto::mod_Transaction::ERC721Transfer>(bytes)?),
                Ok(42) => msg.transaction_oneof = TW::Ethereum::Proto::mod_Transaction::OneOftransaction_oneof::erc1155_transfer(r.read_message::<TW::Ethereum::Proto::mod_Transaction::ERC1155Transfer>(bytes)?),
                Ok(50) => msg.transaction_oneof = TW::Ethereum::Proto::mod_Transaction::OneOftransaction_oneof::contract_generic(r.read_message::<TW::Ethereum::Proto::mod_Transaction::ContractGeneric>(bytes)?),
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
        + match self.transaction_oneof {
            TW::Ethereum::Proto::mod_Transaction::OneOftransaction_oneof::transfer(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Ethereum::Proto::mod_Transaction::OneOftransaction_oneof::erc20_transfer(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Ethereum::Proto::mod_Transaction::OneOftransaction_oneof::erc20_approve(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Ethereum::Proto::mod_Transaction::OneOftransaction_oneof::erc721_transfer(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Ethereum::Proto::mod_Transaction::OneOftransaction_oneof::erc1155_transfer(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Ethereum::Proto::mod_Transaction::OneOftransaction_oneof::contract_generic(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Ethereum::Proto::mod_Transaction::OneOftransaction_oneof::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        match self.transaction_oneof {            TW::Ethereum::Proto::mod_Transaction::OneOftransaction_oneof::transfer(ref m) => { w.write_with_tag(10, |w| w.write_message(m))? },
            TW::Ethereum::Proto::mod_Transaction::OneOftransaction_oneof::erc20_transfer(ref m) => { w.write_with_tag(18, |w| w.write_message(m))? },
            TW::Ethereum::Proto::mod_Transaction::OneOftransaction_oneof::erc20_approve(ref m) => { w.write_with_tag(26, |w| w.write_message(m))? },
            TW::Ethereum::Proto::mod_Transaction::OneOftransaction_oneof::erc721_transfer(ref m) => { w.write_with_tag(34, |w| w.write_message(m))? },
            TW::Ethereum::Proto::mod_Transaction::OneOftransaction_oneof::erc1155_transfer(ref m) => { w.write_with_tag(42, |w| w.write_message(m))? },
            TW::Ethereum::Proto::mod_Transaction::OneOftransaction_oneof::contract_generic(ref m) => { w.write_with_tag(50, |w| w.write_message(m))? },
            TW::Ethereum::Proto::mod_Transaction::OneOftransaction_oneof::None => {},
    }        Ok(())
    }
}

pub mod mod_Transaction {

use std::borrow::Cow;
use super::*;

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Transfer<'a> {
    pub amount: Cow<'a, [u8]>,
    pub data: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for Transfer<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.amount = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.data = r.read_bytes(bytes).map(Cow::Borrowed)?,
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
        + if self.amount == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.amount).len()) }
        + if self.data == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.data).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.amount != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.amount))?; }
        if self.data != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.data))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct ERC20Transfer<'a> {
    pub to: Cow<'a, str>,
    pub amount: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for ERC20Transfer<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.to = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.amount = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for ERC20Transfer<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.to == "" { 0 } else { 1 + sizeof_len((&self.to).len()) }
        + if self.amount == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.amount).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.to != "" { w.write_with_tag(10, |w| w.write_string(&**&self.to))?; }
        if self.amount != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct ERC20Approve<'a> {
    pub spender: Cow<'a, str>,
    pub amount: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for ERC20Approve<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.spender = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.amount = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for ERC20Approve<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.spender == "" { 0 } else { 1 + sizeof_len((&self.spender).len()) }
        + if self.amount == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.amount).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.spender != "" { w.write_with_tag(10, |w| w.write_string(&**&self.spender))?; }
        if self.amount != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct ERC721Transfer<'a> {
    pub from: Cow<'a, str>,
    pub to: Cow<'a, str>,
    pub token_id: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for ERC721Transfer<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.from = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.to = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.token_id = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for ERC721Transfer<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.from == "" { 0 } else { 1 + sizeof_len((&self.from).len()) }
        + if self.to == "" { 0 } else { 1 + sizeof_len((&self.to).len()) }
        + if self.token_id == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.token_id).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.from != "" { w.write_with_tag(10, |w| w.write_string(&**&self.from))?; }
        if self.to != "" { w.write_with_tag(18, |w| w.write_string(&**&self.to))?; }
        if self.token_id != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.token_id))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct ERC1155Transfer<'a> {
    pub from: Cow<'a, str>,
    pub to: Cow<'a, str>,
    pub token_id: Cow<'a, [u8]>,
    pub value: Cow<'a, [u8]>,
    pub data: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for ERC1155Transfer<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.from = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.to = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.token_id = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.value = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.data = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for ERC1155Transfer<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.from == "" { 0 } else { 1 + sizeof_len((&self.from).len()) }
        + if self.to == "" { 0 } else { 1 + sizeof_len((&self.to).len()) }
        + if self.token_id == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.token_id).len()) }
        + if self.value == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.value).len()) }
        + if self.data == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.data).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.from != "" { w.write_with_tag(10, |w| w.write_string(&**&self.from))?; }
        if self.to != "" { w.write_with_tag(18, |w| w.write_string(&**&self.to))?; }
        if self.token_id != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.token_id))?; }
        if self.value != Cow::Borrowed(b"") { w.write_with_tag(34, |w| w.write_bytes(&**&self.value))?; }
        if self.data != Cow::Borrowed(b"") { w.write_with_tag(42, |w| w.write_bytes(&**&self.data))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct ContractGeneric<'a> {
    pub amount: Cow<'a, [u8]>,
    pub data: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for ContractGeneric<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.amount = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.data = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for ContractGeneric<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.amount == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.amount).len()) }
        + if self.data == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.data).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.amount != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.amount))?; }
        if self.data != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.data))?; }
        Ok(())
    }
}

#[derive(Debug, PartialEq, Clone)]
pub enum OneOftransaction_oneof<'a> {
    transfer(TW::Ethereum::Proto::mod_Transaction::Transfer<'a>),
    erc20_transfer(TW::Ethereum::Proto::mod_Transaction::ERC20Transfer<'a>),
    erc20_approve(TW::Ethereum::Proto::mod_Transaction::ERC20Approve<'a>),
    erc721_transfer(TW::Ethereum::Proto::mod_Transaction::ERC721Transfer<'a>),
    erc1155_transfer(TW::Ethereum::Proto::mod_Transaction::ERC1155Transfer<'a>),
    contract_generic(TW::Ethereum::Proto::mod_Transaction::ContractGeneric<'a>),
    None,
}

impl<'a> Default for OneOftransaction_oneof<'a> {
    fn default() -> Self {
        OneOftransaction_oneof::None
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct UserOperation<'a> {
    pub entry_point: Cow<'a, str>,
    pub account_factory: Cow<'a, str>,
    pub account_logic: Cow<'a, str>,
    pub owner: Cow<'a, str>,
    pub is_account_deployed: bool,
    pub pre_verification_gas: Cow<'a, [u8]>,
    pub verification_gas_limit: Cow<'a, [u8]>,
    pub paymaster_and_data: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for UserOperation<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.entry_point = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.account_factory = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.account_logic = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.owner = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(40) => msg.is_account_deployed = r.read_bool(bytes)?,
                Ok(50) => msg.pre_verification_gas = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(58) => msg.verification_gas_limit = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(66) => msg.paymaster_and_data = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for UserOperation<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.entry_point == "" { 0 } else { 1 + sizeof_len((&self.entry_point).len()) }
        + if self.account_factory == "" { 0 } else { 1 + sizeof_len((&self.account_factory).len()) }
        + if self.account_logic == "" { 0 } else { 1 + sizeof_len((&self.account_logic).len()) }
        + if self.owner == "" { 0 } else { 1 + sizeof_len((&self.owner).len()) }
        + if self.is_account_deployed == false { 0 } else { 1 + sizeof_varint(*(&self.is_account_deployed) as u64) }
        + if self.pre_verification_gas == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.pre_verification_gas).len()) }
        + if self.verification_gas_limit == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.verification_gas_limit).len()) }
        + if self.paymaster_and_data == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.paymaster_and_data).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.entry_point != "" { w.write_with_tag(10, |w| w.write_string(&**&self.entry_point))?; }
        if self.account_factory != "" { w.write_with_tag(18, |w| w.write_string(&**&self.account_factory))?; }
        if self.account_logic != "" { w.write_with_tag(26, |w| w.write_string(&**&self.account_logic))?; }
        if self.owner != "" { w.write_with_tag(34, |w| w.write_string(&**&self.owner))?; }
        if self.is_account_deployed != false { w.write_with_tag(40, |w| w.write_bool(*&self.is_account_deployed))?; }
        if self.pre_verification_gas != Cow::Borrowed(b"") { w.write_with_tag(50, |w| w.write_bytes(&**&self.pre_verification_gas))?; }
        if self.verification_gas_limit != Cow::Borrowed(b"") { w.write_with_tag(58, |w| w.write_bytes(&**&self.verification_gas_limit))?; }
        if self.paymaster_and_data != Cow::Borrowed(b"") { w.write_with_tag(66, |w| w.write_bytes(&**&self.paymaster_and_data))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningInput<'a> {
    pub chain_id: Cow<'a, [u8]>,
    pub nonce: Cow<'a, [u8]>,
    pub tx_mode: TW::Ethereum::Proto::TransactionMode,
    pub gas_price: Cow<'a, [u8]>,
    pub gas_limit: Cow<'a, [u8]>,
    pub max_inclusion_fee_per_gas: Cow<'a, [u8]>,
    pub max_fee_per_gas: Cow<'a, [u8]>,
    pub to_address: Cow<'a, str>,
    pub private_key: Cow<'a, [u8]>,
    pub transaction: Option<TW::Ethereum::Proto::Transaction<'a>>,
    pub user_operation: Option<TW::Ethereum::Proto::UserOperation<'a>>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.chain_id = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.nonce = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.tx_mode = r.read_enum(bytes)?,
                Ok(34) => msg.gas_price = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.gas_limit = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(50) => msg.max_inclusion_fee_per_gas = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(58) => msg.max_fee_per_gas = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(66) => msg.to_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(74) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(82) => msg.transaction = Some(r.read_message::<TW::Ethereum::Proto::Transaction>(bytes)?),
                Ok(90) => msg.user_operation = Some(r.read_message::<TW::Ethereum::Proto::UserOperation>(bytes)?),
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
        + if self.nonce == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.nonce).len()) }
        + if self.tx_mode == TW::Ethereum::Proto::TransactionMode::Legacy { 0 } else { 1 + sizeof_varint(*(&self.tx_mode) as u64) }
        + if self.gas_price == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.gas_price).len()) }
        + if self.gas_limit == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.gas_limit).len()) }
        + if self.max_inclusion_fee_per_gas == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.max_inclusion_fee_per_gas).len()) }
        + if self.max_fee_per_gas == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.max_fee_per_gas).len()) }
        + if self.to_address == "" { 0 } else { 1 + sizeof_len((&self.to_address).len()) }
        + if self.private_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.private_key).len()) }
        + self.transaction.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + self.user_operation.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.chain_id != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.chain_id))?; }
        if self.nonce != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.nonce))?; }
        if self.tx_mode != TW::Ethereum::Proto::TransactionMode::Legacy { w.write_with_tag(24, |w| w.write_enum(*&self.tx_mode as i32))?; }
        if self.gas_price != Cow::Borrowed(b"") { w.write_with_tag(34, |w| w.write_bytes(&**&self.gas_price))?; }
        if self.gas_limit != Cow::Borrowed(b"") { w.write_with_tag(42, |w| w.write_bytes(&**&self.gas_limit))?; }
        if self.max_inclusion_fee_per_gas != Cow::Borrowed(b"") { w.write_with_tag(50, |w| w.write_bytes(&**&self.max_inclusion_fee_per_gas))?; }
        if self.max_fee_per_gas != Cow::Borrowed(b"") { w.write_with_tag(58, |w| w.write_bytes(&**&self.max_fee_per_gas))?; }
        if self.to_address != "" { w.write_with_tag(66, |w| w.write_string(&**&self.to_address))?; }
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(74, |w| w.write_bytes(&**&self.private_key))?; }
        if let Some(ref s) = self.transaction { w.write_with_tag(82, |w| w.write_message(s))?; }
        if let Some(ref s) = self.user_operation { w.write_with_tag(90, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningOutput<'a> {
    pub encoded: Cow<'a, [u8]>,
    pub v: Cow<'a, [u8]>,
    pub r: Cow<'a, [u8]>,
    pub s: Cow<'a, [u8]>,
    pub data: Cow<'a, [u8]>,
    pub error: TW::Common::Proto::SigningError,
    pub error_message: Cow<'a, str>,
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
                Ok(42) => msg.data = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(48) => msg.error = r.read_enum(bytes)?,
                Ok(58) => msg.error_message = r.read_string(bytes).map(Cow::Borrowed)?,
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
        + if self.data == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.data).len()) }
        + if self.error == TW::Common::Proto::SigningError::OK { 0 } else { 1 + sizeof_varint(*(&self.error) as u64) }
        + if self.error_message == "" { 0 } else { 1 + sizeof_len((&self.error_message).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.encoded != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.encoded))?; }
        if self.v != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.v))?; }
        if self.r != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.r))?; }
        if self.s != Cow::Borrowed(b"") { w.write_with_tag(34, |w| w.write_bytes(&**&self.s))?; }
        if self.data != Cow::Borrowed(b"") { w.write_with_tag(42, |w| w.write_bytes(&**&self.data))?; }
        if self.error != TW::Common::Proto::SigningError::OK { w.write_with_tag(48, |w| w.write_enum(*&self.error as i32))?; }
        if self.error_message != "" { w.write_with_tag(58, |w| w.write_string(&**&self.error_message))?; }
        Ok(())
    }
}

