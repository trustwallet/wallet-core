// Automatically generated rust module for 'FIO.proto' file

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
pub struct PublicAddress<'a> {
    pub coin_symbol: Cow<'a, str>,
    pub address: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for PublicAddress<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.coin_symbol = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for PublicAddress<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.coin_symbol == "" { 0 } else { 1 + sizeof_len((&self.coin_symbol).len()) }
        + if self.address == "" { 0 } else { 1 + sizeof_len((&self.address).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.coin_symbol != "" { w.write_with_tag(10, |w| w.write_string(&**&self.coin_symbol))?; }
        if self.address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.address))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct NewFundsContent<'a> {
    pub payee_public_address: Cow<'a, str>,
    pub amount: Cow<'a, str>,
    pub coin_symbol: Cow<'a, str>,
    pub memo: Cow<'a, str>,
    pub hash: Cow<'a, str>,
    pub offline_url: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for NewFundsContent<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.payee_public_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.amount = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.coin_symbol = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.memo = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.hash = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(50) => msg.offline_url = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for NewFundsContent<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.payee_public_address == "" { 0 } else { 1 + sizeof_len((&self.payee_public_address).len()) }
        + if self.amount == "" { 0 } else { 1 + sizeof_len((&self.amount).len()) }
        + if self.coin_symbol == "" { 0 } else { 1 + sizeof_len((&self.coin_symbol).len()) }
        + if self.memo == "" { 0 } else { 1 + sizeof_len((&self.memo).len()) }
        + if self.hash == "" { 0 } else { 1 + sizeof_len((&self.hash).len()) }
        + if self.offline_url == "" { 0 } else { 1 + sizeof_len((&self.offline_url).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.payee_public_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.payee_public_address))?; }
        if self.amount != "" { w.write_with_tag(18, |w| w.write_string(&**&self.amount))?; }
        if self.coin_symbol != "" { w.write_with_tag(26, |w| w.write_string(&**&self.coin_symbol))?; }
        if self.memo != "" { w.write_with_tag(34, |w| w.write_string(&**&self.memo))?; }
        if self.hash != "" { w.write_with_tag(42, |w| w.write_string(&**&self.hash))?; }
        if self.offline_url != "" { w.write_with_tag(50, |w| w.write_string(&**&self.offline_url))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Action<'a> {
    pub message_oneof: TW::FIO::Proto::mod_Action::OneOfmessage_oneof<'a>,
}

impl<'a> MessageRead<'a> for Action<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.message_oneof = TW::FIO::Proto::mod_Action::OneOfmessage_oneof::register_fio_address_message(r.read_message::<TW::FIO::Proto::mod_Action::RegisterFioAddress>(bytes)?),
                Ok(18) => msg.message_oneof = TW::FIO::Proto::mod_Action::OneOfmessage_oneof::add_pub_address_message(r.read_message::<TW::FIO::Proto::mod_Action::AddPubAddress>(bytes)?),
                Ok(26) => msg.message_oneof = TW::FIO::Proto::mod_Action::OneOfmessage_oneof::transfer_message(r.read_message::<TW::FIO::Proto::mod_Action::Transfer>(bytes)?),
                Ok(34) => msg.message_oneof = TW::FIO::Proto::mod_Action::OneOfmessage_oneof::renew_fio_address_message(r.read_message::<TW::FIO::Proto::mod_Action::RenewFioAddress>(bytes)?),
                Ok(42) => msg.message_oneof = TW::FIO::Proto::mod_Action::OneOfmessage_oneof::new_funds_request_message(r.read_message::<TW::FIO::Proto::mod_Action::NewFundsRequest>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Action<'a> {
    fn get_size(&self) -> usize {
        0
        + match self.message_oneof {
            TW::FIO::Proto::mod_Action::OneOfmessage_oneof::register_fio_address_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::FIO::Proto::mod_Action::OneOfmessage_oneof::add_pub_address_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::FIO::Proto::mod_Action::OneOfmessage_oneof::transfer_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::FIO::Proto::mod_Action::OneOfmessage_oneof::renew_fio_address_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::FIO::Proto::mod_Action::OneOfmessage_oneof::new_funds_request_message(ref m) => 1 + sizeof_len((m).get_size()),
            TW::FIO::Proto::mod_Action::OneOfmessage_oneof::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        match self.message_oneof {            TW::FIO::Proto::mod_Action::OneOfmessage_oneof::register_fio_address_message(ref m) => { w.write_with_tag(10, |w| w.write_message(m))? },
            TW::FIO::Proto::mod_Action::OneOfmessage_oneof::add_pub_address_message(ref m) => { w.write_with_tag(18, |w| w.write_message(m))? },
            TW::FIO::Proto::mod_Action::OneOfmessage_oneof::transfer_message(ref m) => { w.write_with_tag(26, |w| w.write_message(m))? },
            TW::FIO::Proto::mod_Action::OneOfmessage_oneof::renew_fio_address_message(ref m) => { w.write_with_tag(34, |w| w.write_message(m))? },
            TW::FIO::Proto::mod_Action::OneOfmessage_oneof::new_funds_request_message(ref m) => { w.write_with_tag(42, |w| w.write_message(m))? },
            TW::FIO::Proto::mod_Action::OneOfmessage_oneof::None => {},
    }        Ok(())
    }
}

pub mod mod_Action {

use std::borrow::Cow;
use super::*;

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct RegisterFioAddress<'a> {
    pub fio_address: Cow<'a, str>,
    pub owner_fio_public_key: Cow<'a, str>,
    pub fee: u64,
}

impl<'a> MessageRead<'a> for RegisterFioAddress<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.fio_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.owner_fio_public_key = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.fee = r.read_uint64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for RegisterFioAddress<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.fio_address == "" { 0 } else { 1 + sizeof_len((&self.fio_address).len()) }
        + if self.owner_fio_public_key == "" { 0 } else { 1 + sizeof_len((&self.owner_fio_public_key).len()) }
        + if self.fee == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.fee) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.fio_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.fio_address))?; }
        if self.owner_fio_public_key != "" { w.write_with_tag(18, |w| w.write_string(&**&self.owner_fio_public_key))?; }
        if self.fee != 0u64 { w.write_with_tag(24, |w| w.write_uint64(*&self.fee))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct AddPubAddress<'a> {
    pub fio_address: Cow<'a, str>,
    pub public_addresses: Vec<TW::FIO::Proto::PublicAddress<'a>>,
    pub fee: u64,
}

impl<'a> MessageRead<'a> for AddPubAddress<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.fio_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.public_addresses.push(r.read_message::<TW::FIO::Proto::PublicAddress>(bytes)?),
                Ok(24) => msg.fee = r.read_uint64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for AddPubAddress<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.fio_address == "" { 0 } else { 1 + sizeof_len((&self.fio_address).len()) }
        + self.public_addresses.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + if self.fee == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.fee) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.fio_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.fio_address))?; }
        for s in &self.public_addresses { w.write_with_tag(18, |w| w.write_message(s))?; }
        if self.fee != 0u64 { w.write_with_tag(24, |w| w.write_uint64(*&self.fee))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Transfer<'a> {
    pub payee_public_key: Cow<'a, str>,
    pub amount: u64,
    pub fee: u64,
}

impl<'a> MessageRead<'a> for Transfer<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.payee_public_key = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.amount = r.read_uint64(bytes)?,
                Ok(24) => msg.fee = r.read_uint64(bytes)?,
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
        + if self.payee_public_key == "" { 0 } else { 1 + sizeof_len((&self.payee_public_key).len()) }
        + if self.amount == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.amount) as u64) }
        + if self.fee == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.fee) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.payee_public_key != "" { w.write_with_tag(10, |w| w.write_string(&**&self.payee_public_key))?; }
        if self.amount != 0u64 { w.write_with_tag(16, |w| w.write_uint64(*&self.amount))?; }
        if self.fee != 0u64 { w.write_with_tag(24, |w| w.write_uint64(*&self.fee))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct RenewFioAddress<'a> {
    pub fio_address: Cow<'a, str>,
    pub owner_fio_public_key: Cow<'a, str>,
    pub fee: u64,
}

impl<'a> MessageRead<'a> for RenewFioAddress<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.fio_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.owner_fio_public_key = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.fee = r.read_uint64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for RenewFioAddress<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.fio_address == "" { 0 } else { 1 + sizeof_len((&self.fio_address).len()) }
        + if self.owner_fio_public_key == "" { 0 } else { 1 + sizeof_len((&self.owner_fio_public_key).len()) }
        + if self.fee == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.fee) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.fio_address != "" { w.write_with_tag(10, |w| w.write_string(&**&self.fio_address))?; }
        if self.owner_fio_public_key != "" { w.write_with_tag(18, |w| w.write_string(&**&self.owner_fio_public_key))?; }
        if self.fee != 0u64 { w.write_with_tag(24, |w| w.write_uint64(*&self.fee))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct NewFundsRequest<'a> {
    pub payer_fio_name: Cow<'a, str>,
    pub payer_fio_address: Cow<'a, str>,
    pub payee_fio_name: Cow<'a, str>,
    pub content: Option<TW::FIO::Proto::NewFundsContent<'a>>,
    pub fee: u64,
}

impl<'a> MessageRead<'a> for NewFundsRequest<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.payer_fio_name = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.payer_fio_address = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.payee_fio_name = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.content = Some(r.read_message::<TW::FIO::Proto::NewFundsContent>(bytes)?),
                Ok(40) => msg.fee = r.read_uint64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for NewFundsRequest<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.payer_fio_name == "" { 0 } else { 1 + sizeof_len((&self.payer_fio_name).len()) }
        + if self.payer_fio_address == "" { 0 } else { 1 + sizeof_len((&self.payer_fio_address).len()) }
        + if self.payee_fio_name == "" { 0 } else { 1 + sizeof_len((&self.payee_fio_name).len()) }
        + self.content.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.fee == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.fee) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.payer_fio_name != "" { w.write_with_tag(10, |w| w.write_string(&**&self.payer_fio_name))?; }
        if self.payer_fio_address != "" { w.write_with_tag(18, |w| w.write_string(&**&self.payer_fio_address))?; }
        if self.payee_fio_name != "" { w.write_with_tag(26, |w| w.write_string(&**&self.payee_fio_name))?; }
        if let Some(ref s) = self.content { w.write_with_tag(34, |w| w.write_message(s))?; }
        if self.fee != 0u64 { w.write_with_tag(40, |w| w.write_uint64(*&self.fee))?; }
        Ok(())
    }
}

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfmessage_oneof<'a> {
    register_fio_address_message(TW::FIO::Proto::mod_Action::RegisterFioAddress<'a>),
    add_pub_address_message(TW::FIO::Proto::mod_Action::AddPubAddress<'a>),
    transfer_message(TW::FIO::Proto::mod_Action::Transfer<'a>),
    renew_fio_address_message(TW::FIO::Proto::mod_Action::RenewFioAddress<'a>),
    new_funds_request_message(TW::FIO::Proto::mod_Action::NewFundsRequest<'a>),
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
pub struct ChainParams<'a> {
    pub chain_id: Cow<'a, [u8]>,
    pub head_block_number: u64,
    pub ref_block_prefix: u64,
}

impl<'a> MessageRead<'a> for ChainParams<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.chain_id = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.head_block_number = r.read_uint64(bytes)?,
                Ok(24) => msg.ref_block_prefix = r.read_uint64(bytes)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for ChainParams<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.chain_id == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.chain_id).len()) }
        + if self.head_block_number == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.head_block_number) as u64) }
        + if self.ref_block_prefix == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.ref_block_prefix) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.chain_id != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.chain_id))?; }
        if self.head_block_number != 0u64 { w.write_with_tag(16, |w| w.write_uint64(*&self.head_block_number))?; }
        if self.ref_block_prefix != 0u64 { w.write_with_tag(24, |w| w.write_uint64(*&self.ref_block_prefix))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningInput<'a> {
    pub expiry: u32,
    pub chain_params: Option<TW::FIO::Proto::ChainParams<'a>>,
    pub private_key: Cow<'a, [u8]>,
    pub tpid: Cow<'a, str>,
    pub action: Option<TW::FIO::Proto::Action<'a>>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.expiry = r.read_uint32(bytes)?,
                Ok(18) => msg.chain_params = Some(r.read_message::<TW::FIO::Proto::ChainParams>(bytes)?),
                Ok(26) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.tpid = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.action = Some(r.read_message::<TW::FIO::Proto::Action>(bytes)?),
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
        + if self.expiry == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.expiry) as u64) }
        + self.chain_params.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + if self.private_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.private_key).len()) }
        + if self.tpid == "" { 0 } else { 1 + sizeof_len((&self.tpid).len()) }
        + self.action.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.expiry != 0u32 { w.write_with_tag(8, |w| w.write_uint32(*&self.expiry))?; }
        if let Some(ref s) = self.chain_params { w.write_with_tag(18, |w| w.write_message(s))?; }
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(26, |w| w.write_bytes(&**&self.private_key))?; }
        if self.tpid != "" { w.write_with_tag(34, |w| w.write_string(&**&self.tpid))?; }
        if let Some(ref s) = self.action { w.write_with_tag(42, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningOutput<'a> {
    pub json: Cow<'a, str>,
    pub error: TW::Common::Proto::SigningError,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.json = r.read_string(bytes).map(Cow::Borrowed)?,
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
        + if self.json == "" { 0 } else { 1 + sizeof_len((&self.json).len()) }
        + if self.error == TW::Common::Proto::SigningError::OK { 0 } else { 1 + sizeof_varint(*(&self.error) as u64) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.json != "" { w.write_with_tag(10, |w| w.write_string(&**&self.json))?; }
        if self.error != TW::Common::Proto::SigningError::OK { w.write_with_tag(16, |w| w.write_enum(*&self.error as i32))?; }
        Ok(())
    }
}

