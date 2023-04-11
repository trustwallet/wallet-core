// Automatically generated rust module for 'Aptos.proto' file

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
pub struct TransferMessage<'a> {
    pub to: Cow<'a, str>,
    pub amount: u64,
}

impl<'a> MessageRead<'a> for TransferMessage<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.to = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.amount = r.read_uint64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TransferMessage<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.to == "" { 0 } else { 1 + sizeof_len((&self.to).len()) }
        + if self.amount == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.to != "" { w.write_with_tag(10, |w| w.write_string(&**&self.to))?; }
        if self.amount != 0u64 { w.write_with_tag(16, |w| w.write_uint64(*&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct StructTag<'a> {
    pub account_address: Cow<'a, str>,
    pub module: Cow<'a, str>,
    pub name: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for StructTag<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.account_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.module = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.name = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for StructTag<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.account_address == "" { 0 } else { 1 + sizeof_len((&self.account_address).len()) }
        + if self.module == "" { 0 } else { 1 + sizeof_len((&self.module).len()) }
        + if self.name == "" { 0 } else { 1 + sizeof_len((&self.name).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.account_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.account_address))?; }
        if self.module != "" { w.write_with_tag(18, |w| w.write_string(&**&self.module))?; }
        if self.name != "" { w.write_with_tag(26, |w| w.write_string(&**&self.name))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TokenTransferMessage<'a> {
    pub to: Cow<'a, str>,
    pub amount: u64,
    pub function: Option<TW::Aptos::Proto::StructTag<'a>>,
}

impl<'a> MessageRead<'a> for TokenTransferMessage<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.to = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.amount = r.read_uint64(bytes)?,
                Ok(26) => msg.function = Some(r.read_message::<TW::Aptos::Proto::StructTag>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TokenTransferMessage<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.to == "" { 0 } else { 1 + sizeof_len((&self.to).len()) }
        + if self.amount == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
        + self.function.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.to != "" { w.write_with_tag(10, |w| w.write_string(&**&self.to))?; }
        if self.amount != 0u64 { w.write_with_tag(16, |w| w.write_uint64(*&self.amount))?; }
        if let Some(ref s) = self.function { w.write_with_tag(26, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct ManagedTokensRegisterMessage<'a> {
    pub function: Option<TW::Aptos::Proto::StructTag<'a>>,
}

impl<'a> MessageRead<'a> for ManagedTokensRegisterMessage<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.function = Some(r.read_message::<TW::Aptos::Proto::StructTag>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for ManagedTokensRegisterMessage<'a> {
    fn get_size(&self) -> usize {
        0
        + self.function.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.function { w.write_with_tag(10, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct CreateAccountMessage<'a> {
    pub auth_key: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for CreateAccountMessage<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.auth_key = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for CreateAccountMessage<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.auth_key == "" { 0 } else { 1 + sizeof_len((&self.auth_key).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.auth_key != "" { w.write_with_tag(10, |w| w.write_string(&**&self.auth_key))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct OfferNftMessage<'a> {
    pub receiver: Cow<'a, str>,
    pub creator: Cow<'a, str>,
    pub collectionName: Cow<'a, str>,
    pub name: Cow<'a, str>,
    pub property_version: u64,
    pub amount: u64,
}

impl<'a> MessageRead<'a> for OfferNftMessage<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.receiver = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.creator = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.collectionName = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.name = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(40) => msg.property_version = r.read_uint64(bytes)?,
                Ok(48) => msg.amount = r.read_uint64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for OfferNftMessage<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.receiver == "" { 0 } else { 1 + sizeof_len((&self.receiver).len()) }
        + if self.creator == "" { 0 } else { 1 + sizeof_len((&self.creator).len()) }
        + if self.collectionName == "" { 0 } else { 1 + sizeof_len((&self.collectionName).len()) }
        + if self.name == "" { 0 } else { 1 + sizeof_len((&self.name).len()) }
        + if self.property_version == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.property_version) as u64) }
        + if self.amount == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.receiver != "" { w.write_with_tag(10, |w| w.write_string(&**&self.receiver))?; }
        if self.creator != "" { w.write_with_tag(18, |w| w.write_string(&**&self.creator))?; }
        if self.collectionName != "" { w.write_with_tag(26, |w| w.write_string(&**&self.collectionName))?; }
        if self.name != "" { w.write_with_tag(34, |w| w.write_string(&**&self.name))?; }
        if self.property_version != 0u64 { w.write_with_tag(40, |w| w.write_uint64(*&self.property_version))?; }
        if self.amount != 0u64 { w.write_with_tag(48, |w| w.write_uint64(*&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct CancelOfferNftMessage<'a> {
    pub receiver: Cow<'a, str>,
    pub creator: Cow<'a, str>,
    pub collectionName: Cow<'a, str>,
    pub name: Cow<'a, str>,
    pub property_version: u64,
}

impl<'a> MessageRead<'a> for CancelOfferNftMessage<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.receiver = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.creator = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.collectionName = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.name = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(40) => msg.property_version = r.read_uint64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for CancelOfferNftMessage<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.receiver == "" { 0 } else { 1 + sizeof_len((&self.receiver).len()) }
        + if self.creator == "" { 0 } else { 1 + sizeof_len((&self.creator).len()) }
        + if self.collectionName == "" { 0 } else { 1 + sizeof_len((&self.collectionName).len()) }
        + if self.name == "" { 0 } else { 1 + sizeof_len((&self.name).len()) }
        + if self.property_version == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.property_version) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.receiver != "" { w.write_with_tag(10, |w| w.write_string(&**&self.receiver))?; }
        if self.creator != "" { w.write_with_tag(18, |w| w.write_string(&**&self.creator))?; }
        if self.collectionName != "" { w.write_with_tag(26, |w| w.write_string(&**&self.collectionName))?; }
        if self.name != "" { w.write_with_tag(34, |w| w.write_string(&**&self.name))?; }
        if self.property_version != 0u64 { w.write_with_tag(40, |w| w.write_uint64(*&self.property_version))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct ClaimNftMessage<'a> {
    pub sender: Cow<'a, str>,
    pub creator: Cow<'a, str>,
    pub collectionName: Cow<'a, str>,
    pub name: Cow<'a, str>,
    pub property_version: u64,
}

impl<'a> MessageRead<'a> for ClaimNftMessage<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.sender = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.creator = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.collectionName = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.name = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(40) => msg.property_version = r.read_uint64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for ClaimNftMessage<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.sender == "" { 0 } else { 1 + sizeof_len((&self.sender).len()) }
        + if self.creator == "" { 0 } else { 1 + sizeof_len((&self.creator).len()) }
        + if self.collectionName == "" { 0 } else { 1 + sizeof_len((&self.collectionName).len()) }
        + if self.name == "" { 0 } else { 1 + sizeof_len((&self.name).len()) }
        + if self.property_version == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.property_version) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.sender != "" { w.write_with_tag(10, |w| w.write_string(&**&self.sender))?; }
        if self.creator != "" { w.write_with_tag(18, |w| w.write_string(&**&self.creator))?; }
        if self.collectionName != "" { w.write_with_tag(26, |w| w.write_string(&**&self.collectionName))?; }
        if self.name != "" { w.write_with_tag(34, |w| w.write_string(&**&self.name))?; }
        if self.property_version != 0u64 { w.write_with_tag(40, |w| w.write_uint64(*&self.property_version))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct NftMessage<'a> {
    pub nft_transaction_payload: TW::Aptos::Proto::mod_NftMessage::OneOfnft_transaction_payload<'a>,
}

impl<'a> MessageRead<'a> for NftMessage<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.nft_transaction_payload = TW::Aptos::Proto::mod_NftMessage::OneOfnft_transaction_payload::offer_nft(r.read_message::<TW::Aptos::Proto::OfferNftMessage>(bytes)?),
                Ok(18) => msg.nft_transaction_payload = TW::Aptos::Proto::mod_NftMessage::OneOfnft_transaction_payload::cancel_offer_nft(r.read_message::<TW::Aptos::Proto::CancelOfferNftMessage>(bytes)?),
                Ok(26) => msg.nft_transaction_payload = TW::Aptos::Proto::mod_NftMessage::OneOfnft_transaction_payload::claim_nft(r.read_message::<TW::Aptos::Proto::ClaimNftMessage>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for NftMessage<'a> {
    fn get_size(&self) -> usize {
        0
        + match self.nft_transaction_payload {
            TW::Aptos::Proto::mod_NftMessage::OneOfnft_transaction_payload::offer_nft(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Aptos::Proto::mod_NftMessage::OneOfnft_transaction_payload::cancel_offer_nft(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Aptos::Proto::mod_NftMessage::OneOfnft_transaction_payload::claim_nft(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Aptos::Proto::mod_NftMessage::OneOfnft_transaction_payload::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        match self.nft_transaction_payload {            TW::Aptos::Proto::mod_NftMessage::OneOfnft_transaction_payload::offer_nft(ref m) => { w.write_with_tag(10, |w| w.write_message(m))? },
            TW::Aptos::Proto::mod_NftMessage::OneOfnft_transaction_payload::cancel_offer_nft(ref m) => { w.write_with_tag(18, |w| w.write_message(m))? },
            TW::Aptos::Proto::mod_NftMessage::OneOfnft_transaction_payload::claim_nft(ref m) => { w.write_with_tag(26, |w| w.write_message(m))? },
            TW::Aptos::Proto::mod_NftMessage::OneOfnft_transaction_payload::None => {},
    }        Ok(())
    }
}

pub mod mod_NftMessage {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfnft_transaction_payload<'a> {
    offer_nft(TW::Aptos::Proto::OfferNftMessage<'a>),
    cancel_offer_nft(TW::Aptos::Proto::CancelOfferNftMessage<'a>),
    claim_nft(TW::Aptos::Proto::ClaimNftMessage<'a>),
    None,
}

impl<'a> Default for OneOfnft_transaction_payload<'a> {
    fn default() -> Self {
        OneOfnft_transaction_payload::None
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningInput<'a> {
    pub sender: Cow<'a, str>,
    pub sequence_number: i64,
    pub max_gas_amount: u64,
    pub gas_unit_price: u64,
    pub expiration_timestamp_secs: u64,
    pub chain_id: u32,
    pub private_key: Cow<'a, [u8]>,
    pub any_encoded: Cow<'a, str>,
    pub transaction_payload: TW::Aptos::Proto::mod_SigningInput::OneOftransaction_payload<'a>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.sender = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.sequence_number = r.read_int64(bytes)?,
                Ok(24) => msg.max_gas_amount = r.read_uint64(bytes)?,
                Ok(32) => msg.gas_unit_price = r.read_uint64(bytes)?,
                Ok(40) => msg.expiration_timestamp_secs = r.read_uint64(bytes)?,
                Ok(48) => msg.chain_id = r.read_uint32(bytes)?,
                Ok(58) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(66) => msg.any_encoded = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(74) => msg.transaction_payload = TW::Aptos::Proto::mod_SigningInput::OneOftransaction_payload::transfer(r.read_message::<TW::Aptos::Proto::TransferMessage>(bytes)?),
                Ok(82) => msg.transaction_payload = TW::Aptos::Proto::mod_SigningInput::OneOftransaction_payload::token_transfer(r.read_message::<TW::Aptos::Proto::TokenTransferMessage>(bytes)?),
                Ok(90) => msg.transaction_payload = TW::Aptos::Proto::mod_SigningInput::OneOftransaction_payload::create_account(r.read_message::<TW::Aptos::Proto::CreateAccountMessage>(bytes)?),
                Ok(98) => msg.transaction_payload = TW::Aptos::Proto::mod_SigningInput::OneOftransaction_payload::nft_message(r.read_message::<TW::Aptos::Proto::NftMessage>(bytes)?),
                Ok(106) => msg.transaction_payload = TW::Aptos::Proto::mod_SigningInput::OneOftransaction_payload::register_token(r.read_message::<TW::Aptos::Proto::ManagedTokensRegisterMessage>(bytes)?),
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
        + if self.sender == "" { 0 } else { 1 + sizeof_len((&self.sender).len()) }
        + if self.sequence_number == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.sequence_number) as u64) }
        + if self.max_gas_amount == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.max_gas_amount) as u64) }
        + if self.gas_unit_price == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.gas_unit_price) as u64) }
        + if self.expiration_timestamp_secs == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.expiration_timestamp_secs) as u64) }
        + if self.chain_id == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.chain_id) as u64) }
        + if self.private_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.private_key).len()) }
        + if self.any_encoded == "" { 0 } else { 1 + sizeof_len((&self.any_encoded).len()) }
        + match self.transaction_payload {
            TW::Aptos::Proto::mod_SigningInput::OneOftransaction_payload::transfer(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Aptos::Proto::mod_SigningInput::OneOftransaction_payload::token_transfer(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Aptos::Proto::mod_SigningInput::OneOftransaction_payload::create_account(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Aptos::Proto::mod_SigningInput::OneOftransaction_payload::nft_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Aptos::Proto::mod_SigningInput::OneOftransaction_payload::register_token(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Aptos::Proto::mod_SigningInput::OneOftransaction_payload::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.sender != "" { w.write_with_tag(10, |w| w.write_string(&**&self.sender))?; }
        if self.sequence_number != 0i64 { w.write_with_tag(16, |w| w.write_int64(*&self.sequence_number))?; }
        if self.max_gas_amount != 0u64 { w.write_with_tag(24, |w| w.write_uint64(*&self.max_gas_amount))?; }
        if self.gas_unit_price != 0u64 { w.write_with_tag(32, |w| w.write_uint64(*&self.gas_unit_price))?; }
        if self.expiration_timestamp_secs != 0u64 { w.write_with_tag(40, |w| w.write_uint64(*&self.expiration_timestamp_secs))?; }
        if self.chain_id != 0u32 { w.write_with_tag(48, |w| w.write_uint32(*&self.chain_id))?; }
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(58, |w| w.write_bytes(&**&self.private_key))?; }
        if self.any_encoded != "" { w.write_with_tag(66, |w| w.write_string(&**&self.any_encoded))?; }
        match self.transaction_payload {            TW::Aptos::Proto::mod_SigningInput::OneOftransaction_payload::transfer(ref m) => { w.write_with_tag(74, |w| w.write_message(m))? },
            TW::Aptos::Proto::mod_SigningInput::OneOftransaction_payload::token_transfer(ref m) => { w.write_with_tag(82, |w| w.write_message(m))? },
            TW::Aptos::Proto::mod_SigningInput::OneOftransaction_payload::create_account(ref m) => { w.write_with_tag(90, |w| w.write_message(m))? },
            TW::Aptos::Proto::mod_SigningInput::OneOftransaction_payload::nft_message(ref m) => { w.write_with_tag(98, |w| w.write_message(m))? },
            TW::Aptos::Proto::mod_SigningInput::OneOftransaction_payload::register_token(ref m) => { w.write_with_tag(106, |w| w.write_message(m))? },
            TW::Aptos::Proto::mod_SigningInput::OneOftransaction_payload::None => {},
    }        Ok(())
    }
}

pub mod mod_SigningInput {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOftransaction_payload<'a> {
    transfer(TW::Aptos::Proto::TransferMessage<'a>),
    token_transfer(TW::Aptos::Proto::TokenTransferMessage<'a>),
    create_account(TW::Aptos::Proto::CreateAccountMessage<'a>),
    nft_message(TW::Aptos::Proto::NftMessage<'a>),
    register_token(TW::Aptos::Proto::ManagedTokensRegisterMessage<'a>),
    None,
}

impl<'a> Default for OneOftransaction_payload<'a> {
    fn default() -> Self {
        OneOftransaction_payload::None
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct TransactionAuthenticator<'a> {
    pub signature: Cow<'a, [u8]>,
    pub public_key: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for TransactionAuthenticator<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.signature = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.public_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for TransactionAuthenticator<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.signature == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.signature).len()) }
        + if self.public_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.public_key).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.signature != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.signature))?; }
        if self.public_key != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.public_key))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningOutput<'a> {
    pub raw_txn: Cow<'a, [u8]>,
    pub authenticator: Option<TW::Aptos::Proto::TransactionAuthenticator<'a>>,
    pub encoded: Cow<'a, [u8]>,
    pub json: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.raw_txn = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.authenticator = Some(r.read_message::<TW::Aptos::Proto::TransactionAuthenticator>(bytes)?),
                Ok(26) => msg.encoded = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.json = r.read_string(bytes).map(Cow::Borrowed)?,
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
        + if self.raw_txn == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.raw_txn).len()) }
        + self.authenticator.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.encoded == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.encoded).len()) }
        + if self.json == "" { 0 } else { 1 + sizeof_len((&self.json).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.raw_txn != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.raw_txn))?; }
        if let Some(ref s) = self.authenticator { w.write_with_tag(18, |w| w.write_message(s))?; }
        if self.encoded != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.encoded))?; }
        if self.json != "" { w.write_with_tag(34, |w| w.write_string(&**&self.json))?; }
        Ok(())
    }
}

