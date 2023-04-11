// Automatically generated rust module for 'Stellar.proto' file

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
pub enum ClaimPredicate {
    Predicate_unconditional = 0,
}

impl Default for ClaimPredicate {
    fn default() -> Self {
        ClaimPredicate::Predicate_unconditional
    }
}

impl From<i32> for ClaimPredicate {
    fn from(i: i32) -> Self {
        match i {
            0 => ClaimPredicate::Predicate_unconditional,
            _ => Self::default(),
        }
    }
}

impl<'a> From<&'a str> for ClaimPredicate {
    fn from(s: &'a str) -> Self {
        match s {
            "Predicate_unconditional" => ClaimPredicate::Predicate_unconditional,
            _ => Self::default(),
        }
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Asset<'a> {
    pub issuer: Cow<'a, str>,
    pub alphanum4: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for Asset<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.issuer = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.alphanum4 = r.read_string(bytes).map(Cow::Borrowed)?,
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
        + if self.issuer == "" { 0 } else { 1 + sizeof_len((&self.issuer).len()) }
        + if self.alphanum4 == "" { 0 } else { 1 + sizeof_len((&self.alphanum4).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.issuer != "" { w.write_with_tag(10, |w| w.write_string(&**&self.issuer))?; }
        if self.alphanum4 != "" { w.write_with_tag(18, |w| w.write_string(&**&self.alphanum4))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct OperationCreateAccount<'a> {
    pub destination: Cow<'a, str>,
    pub amount: i64,
}

impl<'a> MessageRead<'a> for OperationCreateAccount<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.destination = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.amount = r.read_int64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for OperationCreateAccount<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.destination == "" { 0 } else { 1 + sizeof_len((&self.destination).len()) }
        + if self.amount == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.destination != "" { w.write_with_tag(10, |w| w.write_string(&**&self.destination))?; }
        if self.amount != 0i64 { w.write_with_tag(16, |w| w.write_int64(*&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct OperationPayment<'a> {
    pub destination: Cow<'a, str>,
    pub asset: Option<TW::Stellar::Proto::Asset<'a>>,
    pub amount: i64,
}

impl<'a> MessageRead<'a> for OperationPayment<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.destination = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.asset = Some(r.read_message::<TW::Stellar::Proto::Asset>(bytes)?),
                Ok(24) => msg.amount = r.read_int64(bytes)?,
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
        + self.asset.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.amount == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.destination != "" { w.write_with_tag(10, |w| w.write_string(&**&self.destination))?; }
        if let Some(ref s) = self.asset { w.write_with_tag(18, |w| w.write_message(s))?; }
        if self.amount != 0i64 { w.write_with_tag(24, |w| w.write_int64(*&self.amount))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct OperationChangeTrust<'a> {
    pub asset: Option<TW::Stellar::Proto::Asset<'a>>,
    pub valid_before: i64,
}

impl<'a> MessageRead<'a> for OperationChangeTrust<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.asset = Some(r.read_message::<TW::Stellar::Proto::Asset>(bytes)?),
                Ok(16) => msg.valid_before = r.read_int64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for OperationChangeTrust<'a> {
    fn get_size(&self) -> usize {
        0
        + self.asset.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.valid_before == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.valid_before) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.asset { w.write_with_tag(10, |w| w.write_message(s))?; }
        if self.valid_before != 0i64 { w.write_with_tag(16, |w| w.write_int64(*&self.valid_before))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Claimant<'a> {
    pub account: Cow<'a, str>,
    pub predicate: TW::Stellar::Proto::ClaimPredicate,
}

impl<'a> MessageRead<'a> for Claimant<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.account = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.predicate = r.read_enum(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Claimant<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.account == "" { 0 } else { 1 + sizeof_len((&self.account).len()) }
        + if self.predicate == TW::Stellar::Proto::ClaimPredicate::Predicate_unconditional { 0 } else { 1 + sizeof_varint(*(&self.predicate) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.account != "" { w.write_with_tag(10, |w| w.write_string(&**&self.account))?; }
        if self.predicate != TW::Stellar::Proto::ClaimPredicate::Predicate_unconditional { w.write_with_tag(16, |w| w.write_enum(*&self.predicate as i32))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct OperationCreateClaimableBalance<'a> {
    pub asset: Option<TW::Stellar::Proto::Asset<'a>>,
    pub amount: i64,
    pub claimants: Vec<TW::Stellar::Proto::Claimant<'a>>,
}

impl<'a> MessageRead<'a> for OperationCreateClaimableBalance<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.asset = Some(r.read_message::<TW::Stellar::Proto::Asset>(bytes)?),
                Ok(16) => msg.amount = r.read_int64(bytes)?,
                Ok(26) => msg.claimants.push(r.read_message::<TW::Stellar::Proto::Claimant>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for OperationCreateClaimableBalance<'a> {
    fn get_size(&self) -> usize {
        0
        + self.asset.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.amount == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
        + self.claimants.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.asset { w.write_with_tag(10, |w| w.write_message(s))?; }
        if self.amount != 0i64 { w.write_with_tag(16, |w| w.write_int64(*&self.amount))?; }
        for s in &self.claimants { w.write_with_tag(26, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct OperationClaimClaimableBalance<'a> {
    pub balance_id: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for OperationClaimClaimableBalance<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.balance_id = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for OperationClaimClaimableBalance<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.balance_id == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.balance_id).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.balance_id != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.balance_id))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct MemoVoid { }

impl<'a> MessageRead<'a> for MemoVoid {
    fn from_reader(r: &mut BytesReader, _: &[u8]) -> Result<Self> {
        r.read_to_end();
        Ok(Self::default())
    }
}

impl MessageWrite for MemoVoid { }

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct MemoText<'a> {
    pub text: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for MemoText<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.text = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for MemoText<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.text == "" { 0 } else { 1 + sizeof_len((&self.text).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.text != "" { w.write_with_tag(10, |w| w.write_string(&**&self.text))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct MemoId {
    pub id: i64,
}

impl<'a> MessageRead<'a> for MemoId {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.id = r.read_int64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl MessageWrite for MemoId {
    fn get_size(&self) -> usize {
        0
        + if self.id == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.id) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.id != 0i64 { w.write_with_tag(8, |w| w.write_int64(*&self.id))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct MemoHash<'a> {
    pub hash: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for MemoHash<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.hash = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for MemoHash<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.hash == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.hash).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.hash != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.hash))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningInput<'a> {
    pub fee: i32,
    pub sequence: i64,
    pub account: Cow<'a, str>,
    pub private_key: Cow<'a, [u8]>,
    pub passphrase: Cow<'a, str>,
    pub operation_oneof: TW::Stellar::Proto::mod_SigningInput::OneOfoperation_oneof<'a>,
    pub memo_type_oneof: TW::Stellar::Proto::mod_SigningInput::OneOfmemo_type_oneof<'a>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.fee = r.read_int32(bytes)?,
                Ok(16) => msg.sequence = r.read_int64(bytes)?,
                Ok(26) => msg.account = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.passphrase = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(50) => msg.operation_oneof = TW::Stellar::Proto::mod_SigningInput::OneOfoperation_oneof::op_create_account(r.read_message::<TW::Stellar::Proto::OperationCreateAccount>(bytes)?),
                Ok(58) => msg.operation_oneof = TW::Stellar::Proto::mod_SigningInput::OneOfoperation_oneof::op_payment(r.read_message::<TW::Stellar::Proto::OperationPayment>(bytes)?),
                Ok(66) => msg.operation_oneof = TW::Stellar::Proto::mod_SigningInput::OneOfoperation_oneof::op_change_trust(r.read_message::<TW::Stellar::Proto::OperationChangeTrust>(bytes)?),
                Ok(114) => msg.operation_oneof = TW::Stellar::Proto::mod_SigningInput::OneOfoperation_oneof::op_create_claimable_balance(r.read_message::<TW::Stellar::Proto::OperationCreateClaimableBalance>(bytes)?),
                Ok(122) => msg.operation_oneof = TW::Stellar::Proto::mod_SigningInput::OneOfoperation_oneof::op_claim_claimable_balance(r.read_message::<TW::Stellar::Proto::OperationClaimClaimableBalance>(bytes)?),
                Ok(74) => msg.memo_type_oneof = TW::Stellar::Proto::mod_SigningInput::OneOfmemo_type_oneof::memo_void(r.read_message::<TW::Stellar::Proto::MemoVoid>(bytes)?),
                Ok(82) => msg.memo_type_oneof = TW::Stellar::Proto::mod_SigningInput::OneOfmemo_type_oneof::memo_text(r.read_message::<TW::Stellar::Proto::MemoText>(bytes)?),
                Ok(90) => msg.memo_type_oneof = TW::Stellar::Proto::mod_SigningInput::OneOfmemo_type_oneof::memo_id(r.read_message::<TW::Stellar::Proto::MemoId>(bytes)?),
                Ok(98) => msg.memo_type_oneof = TW::Stellar::Proto::mod_SigningInput::OneOfmemo_type_oneof::memo_hash(r.read_message::<TW::Stellar::Proto::MemoHash>(bytes)?),
                Ok(106) => msg.memo_type_oneof = TW::Stellar::Proto::mod_SigningInput::OneOfmemo_type_oneof::memo_return_hash(r.read_message::<TW::Stellar::Proto::MemoHash>(bytes)?),
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
        + if self.fee == 0i32 { 0 } else { 1 + sizeof_varint(*(&self.fee) as u64) }
        + if self.sequence == 0i64 { 0 } else { 1 + sizeof_varint(*(&self.sequence) as u64) }
        + if self.account == "" { 0 } else { 1 + sizeof_len((&self.account).len()) }
        + if self.private_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.private_key).len()) }
        + if self.passphrase == "" { 0 } else { 1 + sizeof_len((&self.passphrase).len()) }
        + match self.operation_oneof {
            TW::Stellar::Proto::mod_SigningInput::OneOfoperation_oneof::op_create_account(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Stellar::Proto::mod_SigningInput::OneOfoperation_oneof::op_payment(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Stellar::Proto::mod_SigningInput::OneOfoperation_oneof::op_change_trust(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Stellar::Proto::mod_SigningInput::OneOfoperation_oneof::op_create_claimable_balance(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Stellar::Proto::mod_SigningInput::OneOfoperation_oneof::op_claim_claimable_balance(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Stellar::Proto::mod_SigningInput::OneOfoperation_oneof::None => 0,
    }        + match self.memo_type_oneof {
            TW::Stellar::Proto::mod_SigningInput::OneOfmemo_type_oneof::memo_void(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Stellar::Proto::mod_SigningInput::OneOfmemo_type_oneof::memo_text(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Stellar::Proto::mod_SigningInput::OneOfmemo_type_oneof::memo_id(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Stellar::Proto::mod_SigningInput::OneOfmemo_type_oneof::memo_hash(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Stellar::Proto::mod_SigningInput::OneOfmemo_type_oneof::memo_return_hash(ref m) => 1 + sizeof_len((m).get_size()),
            TW::Stellar::Proto::mod_SigningInput::OneOfmemo_type_oneof::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.fee != 0i32 { w.write_with_tag(8, |w| w.write_int32(*&self.fee))?; }
        if self.sequence != 0i64 { w.write_with_tag(16, |w| w.write_int64(*&self.sequence))?; }
        if self.account != "" { w.write_with_tag(26, |w| w.write_string(&**&self.account))?; }
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(34, |w| w.write_bytes(&**&self.private_key))?; }
        if self.passphrase != "" { w.write_with_tag(42, |w| w.write_string(&**&self.passphrase))?; }
        match self.operation_oneof {            TW::Stellar::Proto::mod_SigningInput::OneOfoperation_oneof::op_create_account(ref m) => { w.write_with_tag(50, |w| w.write_message(m))? },
            TW::Stellar::Proto::mod_SigningInput::OneOfoperation_oneof::op_payment(ref m) => { w.write_with_tag(58, |w| w.write_message(m))? },
            TW::Stellar::Proto::mod_SigningInput::OneOfoperation_oneof::op_change_trust(ref m) => { w.write_with_tag(66, |w| w.write_message(m))? },
            TW::Stellar::Proto::mod_SigningInput::OneOfoperation_oneof::op_create_claimable_balance(ref m) => { w.write_with_tag(114, |w| w.write_message(m))? },
            TW::Stellar::Proto::mod_SigningInput::OneOfoperation_oneof::op_claim_claimable_balance(ref m) => { w.write_with_tag(122, |w| w.write_message(m))? },
            TW::Stellar::Proto::mod_SigningInput::OneOfoperation_oneof::None => {},
    }        match self.memo_type_oneof {            TW::Stellar::Proto::mod_SigningInput::OneOfmemo_type_oneof::memo_void(ref m) => { w.write_with_tag(74, |w| w.write_message(m))? },
            TW::Stellar::Proto::mod_SigningInput::OneOfmemo_type_oneof::memo_text(ref m) => { w.write_with_tag(82, |w| w.write_message(m))? },
            TW::Stellar::Proto::mod_SigningInput::OneOfmemo_type_oneof::memo_id(ref m) => { w.write_with_tag(90, |w| w.write_message(m))? },
            TW::Stellar::Proto::mod_SigningInput::OneOfmemo_type_oneof::memo_hash(ref m) => { w.write_with_tag(98, |w| w.write_message(m))? },
            TW::Stellar::Proto::mod_SigningInput::OneOfmemo_type_oneof::memo_return_hash(ref m) => { w.write_with_tag(106, |w| w.write_message(m))? },
            TW::Stellar::Proto::mod_SigningInput::OneOfmemo_type_oneof::None => {},
    }        Ok(())
    }
}

pub mod mod_SigningInput {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfoperation_oneof<'a> {
    op_create_account(TW::Stellar::Proto::OperationCreateAccount<'a>),
    op_payment(TW::Stellar::Proto::OperationPayment<'a>),
    op_change_trust(TW::Stellar::Proto::OperationChangeTrust<'a>),
    op_create_claimable_balance(TW::Stellar::Proto::OperationCreateClaimableBalance<'a>),
    op_claim_claimable_balance(TW::Stellar::Proto::OperationClaimClaimableBalance<'a>),
    None,
}

impl<'a> Default for OneOfoperation_oneof<'a> {
    fn default() -> Self {
        OneOfoperation_oneof::None
    }
}

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfmemo_type_oneof<'a> {
    memo_void(TW::Stellar::Proto::MemoVoid),
    memo_text(TW::Stellar::Proto::MemoText<'a>),
    memo_id(TW::Stellar::Proto::MemoId),
    memo_hash(TW::Stellar::Proto::MemoHash<'a>),
    memo_return_hash(TW::Stellar::Proto::MemoHash<'a>),
    None,
}

impl<'a> Default for OneOfmemo_type_oneof<'a> {
    fn default() -> Self {
        OneOfmemo_type_oneof::None
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningOutput<'a> {
    pub signature: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.signature = r.read_string(bytes).map(Cow::Borrowed)?,
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
        + if self.signature == "" { 0 } else { 1 + sizeof_len((&self.signature).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.signature != "" { w.write_with_tag(10, |w| w.write_string(&**&self.signature))?; }
        Ok(())
    }
}

