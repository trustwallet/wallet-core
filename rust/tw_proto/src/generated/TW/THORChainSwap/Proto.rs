// Automatically generated rust module for 'THORChainSwap.proto' file

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
pub enum Chain {
    THOR = 0,
    BTC = 1,
    ETH = 2,
    BNB = 3,
    DOGE = 4,
    BCH = 5,
    LTC = 6,
    ATOM = 7,
    AVAX = 8,
}

impl Default for Chain {
    fn default() -> Self {
        Chain::THOR
    }
}

impl From<i32> for Chain {
    fn from(i: i32) -> Self {
        match i {
            0 => Chain::THOR,
            1 => Chain::BTC,
            2 => Chain::ETH,
            3 => Chain::BNB,
            4 => Chain::DOGE,
            5 => Chain::BCH,
            6 => Chain::LTC,
            7 => Chain::ATOM,
            8 => Chain::AVAX,
            _ => Self::default(),
        }
    }
}

impl<'a> From<&'a str> for Chain {
    fn from(s: &'a str) -> Self {
        match s {
            "THOR" => Chain::THOR,
            "BTC" => Chain::BTC,
            "ETH" => Chain::ETH,
            "BNB" => Chain::BNB,
            "DOGE" => Chain::DOGE,
            "BCH" => Chain::BCH,
            "LTC" => Chain::LTC,
            "ATOM" => Chain::ATOM,
            "AVAX" => Chain::AVAX,
            _ => Self::default(),
        }
    }
}

#[derive(Debug, PartialEq, Eq, Clone, Copy)]
pub enum ErrorCode {
    OK = 0,
    Error_general = 1,
    Error_Input_proto_deserialization = 2,
    Error_Unsupported_from_chain = 13,
    Error_Unsupported_to_chain = 14,
    Error_Invalid_from_address = 15,
    Error_Invalid_to_address = 16,
    Error_Invalid_vault_address = 21,
    Error_Invalid_router_address = 22,
}

impl Default for ErrorCode {
    fn default() -> Self {
        ErrorCode::OK
    }
}

impl From<i32> for ErrorCode {
    fn from(i: i32) -> Self {
        match i {
            0 => ErrorCode::OK,
            1 => ErrorCode::Error_general,
            2 => ErrorCode::Error_Input_proto_deserialization,
            13 => ErrorCode::Error_Unsupported_from_chain,
            14 => ErrorCode::Error_Unsupported_to_chain,
            15 => ErrorCode::Error_Invalid_from_address,
            16 => ErrorCode::Error_Invalid_to_address,
            21 => ErrorCode::Error_Invalid_vault_address,
            22 => ErrorCode::Error_Invalid_router_address,
            _ => Self::default(),
        }
    }
}

impl<'a> From<&'a str> for ErrorCode {
    fn from(s: &'a str) -> Self {
        match s {
            "OK" => ErrorCode::OK,
            "Error_general" => ErrorCode::Error_general,
            "Error_Input_proto_deserialization" => ErrorCode::Error_Input_proto_deserialization,
            "Error_Unsupported_from_chain" => ErrorCode::Error_Unsupported_from_chain,
            "Error_Unsupported_to_chain" => ErrorCode::Error_Unsupported_to_chain,
            "Error_Invalid_from_address" => ErrorCode::Error_Invalid_from_address,
            "Error_Invalid_to_address" => ErrorCode::Error_Invalid_to_address,
            "Error_Invalid_vault_address" => ErrorCode::Error_Invalid_vault_address,
            "Error_Invalid_router_address" => ErrorCode::Error_Invalid_router_address,
            _ => Self::default(),
        }
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Error<'a> {
    pub code: TW::THORChainSwap::Proto::ErrorCode,
    pub message: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for Error<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.code = r.read_enum(bytes)?,
                Ok(18) => msg.message = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Error<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.code == TW::THORChainSwap::Proto::ErrorCode::OK { 0 } else { 1 + sizeof_varint(*(&self.code) as u64) }
        + if self.message == "" { 0 } else { 1 + sizeof_len((&self.message).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.code != TW::THORChainSwap::Proto::ErrorCode::OK { w.write_with_tag(8, |w| w.write_enum(*&self.code as i32))?; }
        if self.message != "" { w.write_with_tag(18, |w| w.write_string(&**&self.message))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Asset<'a> {
    pub chain: TW::THORChainSwap::Proto::Chain,
    pub symbol: Cow<'a, str>,
    pub token_id: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for Asset<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.chain = r.read_enum(bytes)?,
                Ok(18) => msg.symbol = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.token_id = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Asset<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.chain == TW::THORChainSwap::Proto::Chain::THOR { 0 } else { 1 + sizeof_varint(*(&self.chain) as u64) }
        + if self.symbol == "" { 0 } else { 1 + sizeof_len((&self.symbol).len()) }
        + if self.token_id == "" { 0 } else { 1 + sizeof_len((&self.token_id).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.chain != TW::THORChainSwap::Proto::Chain::THOR { w.write_with_tag(8, |w| w.write_enum(*&self.chain as i32))?; }
        if self.symbol != "" { w.write_with_tag(18, |w| w.write_string(&**&self.symbol))?; }
        if self.token_id != "" { w.write_with_tag(26, |w| w.write_string(&**&self.token_id))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SwapInput<'a> {
    pub from_asset: Option<TW::THORChainSwap::Proto::Asset<'a>>,
    pub from_address: Cow<'a, str>,
    pub to_asset: Option<TW::THORChainSwap::Proto::Asset<'a>>,
    pub to_address: Cow<'a, str>,
    pub vault_address: Cow<'a, str>,
    pub router_address: Cow<'a, str>,
    pub from_amount: Cow<'a, str>,
    pub to_amount_limit: Cow<'a, str>,
    pub affiliate_fee_address: Cow<'a, str>,
    pub affiliate_fee_rate_bp: Cow<'a, str>,
    pub extra_memo: Cow<'a, str>,
    pub expiration_time: u64,
}

impl<'a> MessageRead<'a> for SwapInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.from_asset = Some(r.read_message::<TW::THORChainSwap::Proto::Asset>(bytes)?),
                Ok(18) => msg.from_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.to_asset = Some(r.read_message::<TW::THORChainSwap::Proto::Asset>(bytes)?),
                Ok(34) => msg.to_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.vault_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(50) => msg.router_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(58) => msg.from_amount = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(66) => msg.to_amount_limit = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(74) => msg.affiliate_fee_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(82) => msg.affiliate_fee_rate_bp = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(90) => msg.extra_memo = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(96) => msg.expiration_time = r.read_uint64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for SwapInput<'a> {
    fn get_size(&self) -> usize {
        0
        + self.from_asset.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.from_address == "" { 0 } else { 1 + sizeof_len((&self.from_address).len()) }
        + self.to_asset.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.to_address == "" { 0 } else { 1 + sizeof_len((&self.to_address).len()) }
        + if self.vault_address == "" { 0 } else { 1 + sizeof_len((&self.vault_address).len()) }
        + if self.router_address == "" { 0 } else { 1 + sizeof_len((&self.router_address).len()) }
        + if self.from_amount == "" { 0 } else { 1 + sizeof_len((&self.from_amount).len()) }
        + if self.to_amount_limit == "" { 0 } else { 1 + sizeof_len((&self.to_amount_limit).len()) }
        + if self.affiliate_fee_address == "" { 0 } else { 1 + sizeof_len((&self.affiliate_fee_address).len()) }
        + if self.affiliate_fee_rate_bp == "" { 0 } else { 1 + sizeof_len((&self.affiliate_fee_rate_bp).len()) }
        + if self.extra_memo == "" { 0 } else { 1 + sizeof_len((&self.extra_memo).len()) }
        + if self.expiration_time == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.expiration_time) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.from_asset { w.write_with_tag(10, |w| w.write_message(s))?; }
        if self.from_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.from_address))?; }
        if let Some(ref s) = self.to_asset { w.write_with_tag(26, |w| w.write_message(s))?; }
        if self.to_address != "" { w.write_with_tag(34, |w| w.write_string(&**&self.to_address))?; }
        if self.vault_address != "" { w.write_with_tag(42, |w| w.write_string(&**&self.vault_address))?; }
        if self.router_address != "" { w.write_with_tag(50, |w| w.write_string(&**&self.router_address))?; }
        if self.from_amount != "" { w.write_with_tag(58, |w| w.write_string(&**&self.from_amount))?; }
        if self.to_amount_limit != "" { w.write_with_tag(66, |w| w.write_string(&**&self.to_amount_limit))?; }
        if self.affiliate_fee_address != "" { w.write_with_tag(74, |w| w.write_string(&**&self.affiliate_fee_address))?; }
        if self.affiliate_fee_rate_bp != "" { w.write_with_tag(82, |w| w.write_string(&**&self.affiliate_fee_rate_bp))?; }
        if self.extra_memo != "" { w.write_with_tag(90, |w| w.write_string(&**&self.extra_memo))?; }
        if self.expiration_time != 0u64 { w.write_with_tag(96, |w| w.write_uint64(*&self.expiration_time))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SwapOutput<'a> {
    pub from_chain: TW::THORChainSwap::Proto::Chain,
    pub to_chain: TW::THORChainSwap::Proto::Chain,
    pub error: Option<TW::THORChainSwap::Proto::Error<'a>>,
    pub signing_input_oneof: TW::THORChainSwap::Proto::mod_SwapOutput::OneOfsigning_input_oneof<'a>,
}

impl<'a> MessageRead<'a> for SwapOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.from_chain = r.read_enum(bytes)?,
                Ok(16) => msg.to_chain = r.read_enum(bytes)?,
                Ok(26) => msg.error = Some(r.read_message::<TW::THORChainSwap::Proto::Error>(bytes)?),
                Ok(34) => msg.signing_input_oneof = TW::THORChainSwap::Proto::mod_SwapOutput::OneOfsigning_input_oneof::bitcoin(r.read_message::<TW::Bitcoin::Proto::SigningInput>(bytes)?),
                Ok(42) => msg.signing_input_oneof = TW::THORChainSwap::Proto::mod_SwapOutput::OneOfsigning_input_oneof::ethereum(r.read_message::<TW::Ethereum::Proto::SigningInput>(bytes)?),
                Ok(50) => msg.signing_input_oneof = TW::THORChainSwap::Proto::mod_SwapOutput::OneOfsigning_input_oneof::binance(r.read_message::<TW::Binance::Proto::SigningInput>(bytes)?),
                Ok(58) => msg.signing_input_oneof = TW::THORChainSwap::Proto::mod_SwapOutput::OneOfsigning_input_oneof::cosmos(r.read_message::<TW::Cosmos::Proto::SigningInput>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for SwapOutput<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.from_chain == TW::THORChainSwap::Proto::Chain::THOR { 0 } else { 1 + sizeof_varint(*(&self.from_chain) as u64) }
        + if self.to_chain == TW::THORChainSwap::Proto::Chain::THOR { 0 } else { 1 + sizeof_varint(*(&self.to_chain) as u64) }
        + self.error.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + match self.signing_input_oneof {
            TW::THORChainSwap::Proto::mod_SwapOutput::OneOfsigning_input_oneof::bitcoin(ref m) => 1 + sizeof_len((m).get_size()),
            TW::THORChainSwap::Proto::mod_SwapOutput::OneOfsigning_input_oneof::ethereum(ref m) => 1 + sizeof_len((m).get_size()),
            TW::THORChainSwap::Proto::mod_SwapOutput::OneOfsigning_input_oneof::binance(ref m) => 1 + sizeof_len((m).get_size()),
            TW::THORChainSwap::Proto::mod_SwapOutput::OneOfsigning_input_oneof::cosmos(ref m) => 1 + sizeof_len((m).get_size()),
            TW::THORChainSwap::Proto::mod_SwapOutput::OneOfsigning_input_oneof::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.from_chain != TW::THORChainSwap::Proto::Chain::THOR { w.write_with_tag(8, |w| w.write_enum(*&self.from_chain as i32))?; }
        if self.to_chain != TW::THORChainSwap::Proto::Chain::THOR { w.write_with_tag(16, |w| w.write_enum(*&self.to_chain as i32))?; }
        if let Some(ref s) = self.error { w.write_with_tag(26, |w| w.write_message(s))?; }
        match self.signing_input_oneof {            TW::THORChainSwap::Proto::mod_SwapOutput::OneOfsigning_input_oneof::bitcoin(ref m) => { w.write_with_tag(34, |w| w.write_message(m))? },
            TW::THORChainSwap::Proto::mod_SwapOutput::OneOfsigning_input_oneof::ethereum(ref m) => { w.write_with_tag(42, |w| w.write_message(m))? },
            TW::THORChainSwap::Proto::mod_SwapOutput::OneOfsigning_input_oneof::binance(ref m) => { w.write_with_tag(50, |w| w.write_message(m))? },
            TW::THORChainSwap::Proto::mod_SwapOutput::OneOfsigning_input_oneof::cosmos(ref m) => { w.write_with_tag(58, |w| w.write_message(m))? },
            TW::THORChainSwap::Proto::mod_SwapOutput::OneOfsigning_input_oneof::None => {},
    }        Ok(())
    }
}

pub mod mod_SwapOutput {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfsigning_input_oneof<'a> {
    bitcoin(TW::Bitcoin::Proto::SigningInput<'a>),
    ethereum(TW::Ethereum::Proto::SigningInput<'a>),
    binance(TW::Binance::Proto::SigningInput<'a>),
    cosmos(TW::Cosmos::Proto::SigningInput<'a>),
    None,
}

impl<'a> Default for OneOfsigning_input_oneof<'a> {
    fn default() -> Self {
        OneOfsigning_input_oneof::None
    }
}

}

