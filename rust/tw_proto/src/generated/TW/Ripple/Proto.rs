// Automatically generated rust module for 'Ripple.proto' file

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
pub struct CurrencyAmount<'a> {
    pub currency: Cow<'a, str>,
    pub value: Cow<'a, str>,
    pub issuer: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for CurrencyAmount<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.currency = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.value = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.issuer = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for CurrencyAmount<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.currency == "" { 0 } else { 1 + sizeof_len((&self.currency).len()) }
        + if self.value == "" { 0 } else { 1 + sizeof_len((&self.value).len()) }
        + if self.issuer == "" { 0 } else { 1 + sizeof_len((&self.issuer).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.currency != "" { w.write_with_tag(10, |w| w.write_string(&**&self.currency))?; }
        if self.value != "" { w.write_with_tag(18, |w| w.write_string(&**&self.value))?; }
        if self.issuer != "" { w.write_with_tag(26, |w| w.write_string(&**&self.issuer))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct OperationTrustSet<'a> {
    pub limit_amount: Option<TW::Ripple::Proto::CurrencyAmount<'a>>,
}

impl<'a> MessageRead<'a> for OperationTrustSet<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.limit_amount = Some(r.read_message::<TW::Ripple::Proto::CurrencyAmount>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for OperationTrustSet<'a> {
    fn get_size(&self) -> usize {
        0
        + self.limit_amount.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.limit_amount { w.write_with_tag(10, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct OperationPayment<'a> {
    pub destination: Cow<'a, str>,
    pub destination_tag: i64,
    pub amount_oneof: TW::Ripple::Proto::mod_OperationPayment::OneOfamount_oneof<'a>,
}

impl<'a> MessageRead<'a> for OperationPayment<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(26) => msg.destination = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(32) => msg.destination_tag = r.read_int64(bytes)?,
                Ok(8) => msg.amount_oneof = TW::Ripple::Proto::mod_OperationPayment::OneOfamount_oneof::amount(r.read_int64(bytes)?),
                Ok(18) => msg.amount_oneof = TW::Ripple::Proto::mod_OperationPayment::OneOfamount_oneof::currency_amount(r.read_message::<TW::Ripple::Proto::CurrencyAmount>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for OperationPayment<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.destination == "" { 0 } else { 1 + sizeof_len((&self.destination).len()) }
        + if self.destination_tag == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.destination_tag) as u64) }
        + match self.amount_oneof {
            TW::Ripple::Proto::mod_OperationPayment::OneOfamount_oneof::amount(ref m) => 1 + sizeof_varint(*(m) as u64),
            TW::Ripple::Proto::mod_OperationPayment::OneOfamount_oneof::currency_amount(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Ripple::Proto::mod_OperationPayment::OneOfamount_oneof::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.destination != "" { w.write_with_tag(26, |w| w.write_string(&**&self.destination))?; }
        if self.destination_tag != 0i64 { w.write_with_tag(32, |w| w.write_int64(*&self.destination_tag))?; }
        match self.amount_oneof {            TW::Ripple::Proto::mod_OperationPayment::OneOfamount_oneof::amount(ref m) => { w.write_with_tag(8, |w| w.write_int64(*m))? },
            TW::Ripple::Proto::mod_OperationPayment::OneOfamount_oneof::currency_amount(ref m) => { w.write_with_tag(18, |w| w.write_message(m))? },
            TW::Ripple::Proto::mod_OperationPayment::OneOfamount_oneof::None => {},
    }        Ok(())
    }
}

pub mod mod_OperationPayment {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfamount_oneof<'a> {
    amount(i64),
    currency_amount(TW::Ripple::Proto::CurrencyAmount<'a>),
    None,
}

impl<'a> Default for OneOfamount_oneof<'a> {
    fn default() -> Self {
        OneOfamount_oneof::None
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct OperationNFTokenBurn<'a> {
    pub nftoken_id: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for OperationNFTokenBurn<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.nftoken_id = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for OperationNFTokenBurn<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.nftoken_id == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.nftoken_id).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.nftoken_id != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.nftoken_id))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct OperationNFTokenCreateOffer<'a> {
    pub nftoken_id: Cow<'a, [u8]>,
    pub destination: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for OperationNFTokenCreateOffer<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.nftoken_id = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.destination = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for OperationNFTokenCreateOffer<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.nftoken_id == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.nftoken_id).len()) }
        + if self.destination == "" { 0 } else { 1 + sizeof_len((&self.destination).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.nftoken_id != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.nftoken_id))?; }
        if self.destination != "" { w.write_with_tag(18, |w| w.write_string(&**&self.destination))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct OperationNFTokenAcceptOffer<'a> {
    pub sell_offer: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for OperationNFTokenAcceptOffer<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.sell_offer = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for OperationNFTokenAcceptOffer<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.sell_offer == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.sell_offer).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.sell_offer != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.sell_offer))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct OperationNFTokenCancelOffer<'a> {
    pub token_offers: Vec<Cow<'a, [u8]>>,
}

impl<'a> MessageRead<'a> for OperationNFTokenCancelOffer<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.token_offers.push(r.read_bytes(bytes).map(Cow::Borrowed)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for OperationNFTokenCancelOffer<'a> {
    fn get_size(&self) -> usize {
        0
        + self.token_offers.iter().map(|s| 1 + sizeof_len((s).len())).sum::<usize>()
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        for s in &self.token_offers { w.write_with_tag(10, |w| w.write_bytes(&**s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningInput<'a> {
    pub fee: i64,
    pub sequence: i32,
    pub last_ledger_sequence: i32,
    pub account: Cow<'a, str>,
    pub flags: i64,
    pub private_key: Cow<'a, [u8]>,
    pub operation_oneof: TW::Ripple::Proto::mod_SigningInput::OneOfoperation_oneof<'a>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.fee = r.read_int64(bytes)?,
                Ok(16) => msg.sequence = r.read_int32(bytes)?,
                Ok(24) => msg.last_ledger_sequence = r.read_int32(bytes)?,
                Ok(34) => msg.account = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(40) => msg.flags = r.read_int64(bytes)?,
                Ok(50) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(58) => msg.operation_oneof = TW::Ripple::Proto::mod_SigningInput::OneOfoperation_oneof::op_trust_set(r.read_message::<TW::Ripple::Proto::OperationTrustSet>(bytes)?),
                Ok(66) => msg.operation_oneof = TW::Ripple::Proto::mod_SigningInput::OneOfoperation_oneof::op_payment(r.read_message::<TW::Ripple::Proto::OperationPayment>(bytes)?),
                Ok(74) => msg.operation_oneof = TW::Ripple::Proto::mod_SigningInput::OneOfoperation_oneof::op_nftoken_burn(r.read_message::<TW::Ripple::Proto::OperationNFTokenBurn>(bytes)?),
                Ok(82) => msg.operation_oneof = TW::Ripple::Proto::mod_SigningInput::OneOfoperation_oneof::op_nftoken_create_offer(r.read_message::<TW::Ripple::Proto::OperationNFTokenCreateOffer>(bytes)?),
                Ok(90) => msg.operation_oneof = TW::Ripple::Proto::mod_SigningInput::OneOfoperation_oneof::op_nftoken_accept_offer(r.read_message::<TW::Ripple::Proto::OperationNFTokenAcceptOffer>(bytes)?),
                Ok(98) => msg.operation_oneof = TW::Ripple::Proto::mod_SigningInput::OneOfoperation_oneof::op_nftoken_cancel_offer(r.read_message::<TW::Ripple::Proto::OperationNFTokenCancelOffer>(bytes)?),
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
        + if self.fee == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.fee) as u64) }
        + if self.sequence == 0i32 { 0 } else { 1 + sizeof_varint(*(&self.sequence) as u64) }
        + if self.last_ledger_sequence == 0i32 { 0 } else { 1 + sizeof_varint(*(&self.last_ledger_sequence) as u64) }
        + if self.account == "" { 0 } else { 1 + sizeof_len((&self.account).len()) }
        + if self.flags == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.flags) as u64) }
        + if self.private_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.private_key).len()) }
        + match self.operation_oneof {
            TW::Ripple::Proto::mod_SigningInput::OneOfoperation_oneof::op_trust_set(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Ripple::Proto::mod_SigningInput::OneOfoperation_oneof::op_payment(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Ripple::Proto::mod_SigningInput::OneOfoperation_oneof::op_nftoken_burn(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Ripple::Proto::mod_SigningInput::OneOfoperation_oneof::op_nftoken_create_offer(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Ripple::Proto::mod_SigningInput::OneOfoperation_oneof::op_nftoken_accept_offer(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Ripple::Proto::mod_SigningInput::OneOfoperation_oneof::op_nftoken_cancel_offer(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Ripple::Proto::mod_SigningInput::OneOfoperation_oneof::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.fee != 0i64 { w.write_with_tag(8, |w| w.write_int64(*&self.fee))?; }
        if self.sequence != 0i32 { w.write_with_tag(16, |w| w.write_int32(*&self.sequence))?; }
        if self.last_ledger_sequence != 0i32 { w.write_with_tag(24, |w| w.write_int32(*&self.last_ledger_sequence))?; }
        if self.account != "" { w.write_with_tag(34, |w| w.write_string(&**&self.account))?; }
        if self.flags != 0i64 { w.write_with_tag(40, |w| w.write_int64(*&self.flags))?; }
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(50, |w| w.write_bytes(&**&self.private_key))?; }
        match self.operation_oneof {            TW::Ripple::Proto::mod_SigningInput::OneOfoperation_oneof::op_trust_set(ref m) => { w.write_with_tag(58, |w| w.write_message(m))? },
            TW::Ripple::Proto::mod_SigningInput::OneOfoperation_oneof::op_payment(ref m) => { w.write_with_tag(66, |w| w.write_message(m))? },
            TW::Ripple::Proto::mod_SigningInput::OneOfoperation_oneof::op_nftoken_burn(ref m) => { w.write_with_tag(74, |w| w.write_message(m))? },
            TW::Ripple::Proto::mod_SigningInput::OneOfoperation_oneof::op_nftoken_create_offer(ref m) => { w.write_with_tag(82, |w| w.write_message(m))? },
            TW::Ripple::Proto::mod_SigningInput::OneOfoperation_oneof::op_nftoken_accept_offer(ref m) => { w.write_with_tag(90, |w| w.write_message(m))? },
            TW::Ripple::Proto::mod_SigningInput::OneOfoperation_oneof::op_nftoken_cancel_offer(ref m) => { w.write_with_tag(98, |w| w.write_message(m))? },
            TW::Ripple::Proto::mod_SigningInput::OneOfoperation_oneof::None => {},
    }        Ok(())
    }
}

pub mod mod_SigningInput {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfoperation_oneof<'a> {
    op_trust_set(TW::Ripple::Proto::OperationTrustSet<'a>),
    op_payment(TW::Ripple::Proto::OperationPayment<'a>),
    op_nftoken_burn(TW::Ripple::Proto::OperationNFTokenBurn<'a>),
    op_nftoken_create_offer(TW::Ripple::Proto::OperationNFTokenCreateOffer<'a>),
    op_nftoken_accept_offer(TW::Ripple::Proto::OperationNFTokenAcceptOffer<'a>),
    op_nftoken_cancel_offer(TW::Ripple::Proto::OperationNFTokenCancelOffer<'a>),
    None,
}

impl<'a> Default for OneOfoperation_oneof<'a> {
    fn default() -> Self {
        OneOfoperation_oneof::None
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningOutput<'a> {
    pub encoded: Cow<'a, [u8]>,
    pub error: TW::Common::Proto::SigningError,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.encoded = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.error = r.read_enum(bytes)?,
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
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.encoded != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.encoded))?; }
        if self.error != TW::Common::Proto::SigningError::OK { w.write_with_tag(16, |w| w.write_enum(*&self.error as i32))?; }
        Ok(())
    }
}

