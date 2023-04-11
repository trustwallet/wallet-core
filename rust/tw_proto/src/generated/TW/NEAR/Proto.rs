// Automatically generated rust module for 'NEAR.proto' file

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
pub struct PublicKey<'a> {
    pub key_type: u32,
    pub data: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for PublicKey<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.key_type = r.read_uint32(bytes)?,
                Ok(18) => msg.data = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for PublicKey<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.key_type == 0u32 { 0 } else { 1 + sizeof_varint(*(&self.key_type) as u64) }
        + if self.data == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.data).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.key_type != 0u32 { w.write_with_tag(8, |w| w.write_uint32(*&self.key_type))?; }
        if self.data != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.data))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct FunctionCallPermission<'a> {
    pub allowance: Cow<'a, [u8]>,
    pub receiver_id: Cow<'a, str>,
    pub method_names: Vec<Cow<'a, str>>,
}

impl<'a> MessageRead<'a> for FunctionCallPermission<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.allowance = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.receiver_id = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(26) => msg.method_names.push(r.read_string(bytes).map(Cow::Borrowed)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for FunctionCallPermission<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.allowance == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.allowance).len()) }
        + if self.receiver_id == "" { 0 } else { 1 + sizeof_len((&self.receiver_id).len()) }
        + self.method_names.iter().map(|s| 1 + sizeof_len((s).len())).sum::<usize>()
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.allowance != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.allowance))?; }
        if self.receiver_id != "" { w.write_with_tag(18, |w| w.write_string(&**&self.receiver_id))?; }
        for s in &self.method_names { w.write_with_tag(26, |w| w.write_string(&**s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct FullAccessPermission { }

impl<'a> MessageRead<'a> for FullAccessPermission {
    fn from_reader(r: &mut BytesReader, _: &[u8]) -> Result<Self> {
        r.read_to_end();
        Ok(Self::default())
    }
}

impl MessageWrite for FullAccessPermission { }

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct AccessKey<'a> {
    pub nonce: u64,
    pub permission: TW::NEAR::Proto::mod_AccessKey::OneOfpermission<'a>,
}

impl<'a> MessageRead<'a> for AccessKey<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(8) => msg.nonce = r.read_uint64(bytes)?,
                Ok(18) => msg.permission = TW::NEAR::Proto::mod_AccessKey::OneOfpermission::function_call(r.read_message::<TW::NEAR::Proto::FunctionCallPermission>(bytes)?),
                Ok(26) => msg.permission = TW::NEAR::Proto::mod_AccessKey::OneOfpermission::full_access(r.read_message::<TW::NEAR::Proto::FullAccessPermission>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for AccessKey<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.nonce == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.nonce) as u64) }
        + match self.permission {
            TW::NEAR::Proto::mod_AccessKey::OneOfpermission::function_call(ref m) => 1 + sizeof_len((m).get_size()),
            TW::NEAR::Proto::mod_AccessKey::OneOfpermission::full_access(ref m) => 1 + sizeof_len((m).get_size()),
            TW::NEAR::Proto::mod_AccessKey::OneOfpermission::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.nonce != 0u64 { w.write_with_tag(8, |w| w.write_uint64(*&self.nonce))?; }
        match self.permission {            TW::NEAR::Proto::mod_AccessKey::OneOfpermission::function_call(ref m) => { w.write_with_tag(18, |w| w.write_message(m))? },
            TW::NEAR::Proto::mod_AccessKey::OneOfpermission::full_access(ref m) => { w.write_with_tag(26, |w| w.write_message(m))? },
            TW::NEAR::Proto::mod_AccessKey::OneOfpermission::None => {},
    }        Ok(())
    }
}

pub mod mod_AccessKey {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfpermission<'a> {
    function_call(TW::NEAR::Proto::FunctionCallPermission<'a>),
    full_access(TW::NEAR::Proto::FullAccessPermission),
    None,
}

impl<'a> Default for OneOfpermission<'a> {
    fn default() -> Self {
        OneOfpermission::None
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct CreateAccount { }

impl<'a> MessageRead<'a> for CreateAccount {
    fn from_reader(r: &mut BytesReader, _: &[u8]) -> Result<Self> {
        r.read_to_end();
        Ok(Self::default())
    }
}

impl MessageWrite for CreateAccount { }

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct DeployContract<'a> {
    pub code: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for DeployContract<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.code = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for DeployContract<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.code == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.code).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.code != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.code))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct FunctionCall<'a> {
    pub method_name: Cow<'a, str>,
    pub args: Cow<'a, [u8]>,
    pub gas: u64,
    pub deposit: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for FunctionCall<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.method_name = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.args = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(24) => msg.gas = r.read_uint64(bytes)?,
                Ok(34) => msg.deposit = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for FunctionCall<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.method_name == "" { 0 } else { 1 + sizeof_len((&self.method_name).len()) }
        + if self.args == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.args).len()) }
        + if self.gas == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.gas) as u64) }
        + if self.deposit == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.deposit).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.method_name != "" { w.write_with_tag(10, |w| w.write_string(&**&self.method_name))?; }
        if self.args != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.args))?; }
        if self.gas != 0u64 { w.write_with_tag(24, |w| w.write_uint64(*&self.gas))?; }
        if self.deposit != Cow::Borrowed(b"") { w.write_with_tag(34, |w| w.write_bytes(&**&self.deposit))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Transfer<'a> {
    pub deposit: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for Transfer<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.deposit = r.read_bytes(bytes).map(Cow::Borrowed)?,
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
        + if self.deposit == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.deposit).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.deposit != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.deposit))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Stake<'a> {
    pub stake: Cow<'a, [u8]>,
    pub public_key: Option<TW::NEAR::Proto::PublicKey<'a>>,
}

impl<'a> MessageRead<'a> for Stake<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.stake = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.public_key = Some(r.read_message::<TW::NEAR::Proto::PublicKey>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for Stake<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.stake == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.stake).len()) }
        + self.public_key.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.stake != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.stake))?; }
        if let Some(ref s) = self.public_key { w.write_with_tag(18, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct AddKey<'a> {
    pub public_key: Option<TW::NEAR::Proto::PublicKey<'a>>,
    pub access_key: Option<TW::NEAR::Proto::AccessKey<'a>>,
}

impl<'a> MessageRead<'a> for AddKey<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.public_key = Some(r.read_message::<TW::NEAR::Proto::PublicKey>(bytes)?),
                Ok(18) => msg.access_key = Some(r.read_message::<TW::NEAR::Proto::AccessKey>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for AddKey<'a> {
    fn get_size(&self) -> usize {
        0
        + self.public_key.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
        + self.access_key.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.public_key { w.write_with_tag(10, |w| w.write_message(s))?; }
        if let Some(ref s) = self.access_key { w.write_with_tag(18, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct DeleteKey<'a> {
    pub public_key: Option<TW::NEAR::Proto::PublicKey<'a>>,
}

impl<'a> MessageRead<'a> for DeleteKey<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.public_key = Some(r.read_message::<TW::NEAR::Proto::PublicKey>(bytes)?),
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for DeleteKey<'a> {
    fn get_size(&self) -> usize {
        0
        + self.public_key.as_ref().map_or(0, |m| 1 + sizeof_len((m).get_size()))
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if let Some(ref s) = self.public_key { w.write_with_tag(10, |w| w.write_message(s))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct DeleteAccount<'a> {
    pub beneficiary_id: Cow<'a, str>,
}

impl<'a> MessageRead<'a> for DeleteAccount<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.beneficiary_id = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(t) => { r.read_unknown(bytes, t)?; }
                Err(e) => return Err(e),
            }
        }
        Ok(msg)
    }
}

impl<'a> MessageWrite for DeleteAccount<'a> {
    fn get_size(&self) -> usize {
        0
        + if self.beneficiary_id == "" { 0 } else { 1 + sizeof_len((&self.beneficiary_id).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.beneficiary_id != "" { w.write_with_tag(10, |w| w.write_string(&**&self.beneficiary_id))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct Action<'a> {
    pub payload: TW::NEAR::Proto::mod_Action::OneOfpayload<'a>,
}

impl<'a> MessageRead<'a> for Action<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.payload = TW::NEAR::Proto::mod_Action::OneOfpayload::create_account(r.read_message::<TW::NEAR::Proto::CreateAccount>(bytes)?),
                Ok(18) => msg.payload = TW::NEAR::Proto::mod_Action::OneOfpayload::deploy_contract(r.read_message::<TW::NEAR::Proto::DeployContract>(bytes)?),
                Ok(26) => msg.payload = TW::NEAR::Proto::mod_Action::OneOfpayload::function_call(r.read_message::<TW::NEAR::Proto::FunctionCall>(bytes)?),
                Ok(34) => msg.payload = TW::NEAR::Proto::mod_Action::OneOfpayload::transfer(r.read_message::<TW::NEAR::Proto::Transfer>(bytes)?),
                Ok(42) => msg.payload = TW::NEAR::Proto::mod_Action::OneOfpayload::stake(r.read_message::<TW::NEAR::Proto::Stake>(bytes)?),
                Ok(50) => msg.payload = TW::NEAR::Proto::mod_Action::OneOfpayload::add_key(r.read_message::<TW::NEAR::Proto::AddKey>(bytes)?),
                Ok(58) => msg.payload = TW::NEAR::Proto::mod_Action::OneOfpayload::delete_key(r.read_message::<TW::NEAR::Proto::DeleteKey>(bytes)?),
                Ok(66) => msg.payload = TW::NEAR::Proto::mod_Action::OneOfpayload::delete_account(r.read_message::<TW::NEAR::Proto::DeleteAccount>(bytes)?),
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
        + match self.payload {
            TW::NEAR::Proto::mod_Action::OneOfpayload::create_account(ref m) => 1 + sizeof_len((m).get_size()),
            TW::NEAR::Proto::mod_Action::OneOfpayload::deploy_contract(ref m) => 1 + sizeof_len((m).get_size()),
            TW::NEAR::Proto::mod_Action::OneOfpayload::function_call(ref m) => 1 + sizeof_len((m).get_size()),
            TW::NEAR::Proto::mod_Action::OneOfpayload::transfer(ref m) => 1 + sizeof_len((m).get_size()),
            TW::NEAR::Proto::mod_Action::OneOfpayload::stake(ref m) => 1 + sizeof_len((m).get_size()),
            TW::NEAR::Proto::mod_Action::OneOfpayload::add_key(ref m) => 1 + sizeof_len((m).get_size()),
            TW::NEAR::Proto::mod_Action::OneOfpayload::delete_key(ref m) => 1 + sizeof_len((m).get_size()),
            TW::NEAR::Proto::mod_Action::OneOfpayload::delete_account(ref m) => 1 + sizeof_len((m).get_size()),
            TW::NEAR::Proto::mod_Action::OneOfpayload::None => 0,
    }    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        match self.payload {            TW::NEAR::Proto::mod_Action::OneOfpayload::create_account(ref m) => { w.write_with_tag(10, |w| w.write_message(m))? },
            TW::NEAR::Proto::mod_Action::OneOfpayload::deploy_contract(ref m) => { w.write_with_tag(18, |w| w.write_message(m))? },
            TW::NEAR::Proto::mod_Action::OneOfpayload::function_call(ref m) => { w.write_with_tag(26, |w| w.write_message(m))? },
            TW::NEAR::Proto::mod_Action::OneOfpayload::transfer(ref m) => { w.write_with_tag(34, |w| w.write_message(m))? },
            TW::NEAR::Proto::mod_Action::OneOfpayload::stake(ref m) => { w.write_with_tag(42, |w| w.write_message(m))? },
            TW::NEAR::Proto::mod_Action::OneOfpayload::add_key(ref m) => { w.write_with_tag(50, |w| w.write_message(m))? },
            TW::NEAR::Proto::mod_Action::OneOfpayload::delete_key(ref m) => { w.write_with_tag(58, |w| w.write_message(m))? },
            TW::NEAR::Proto::mod_Action::OneOfpayload::delete_account(ref m) => { w.write_with_tag(66, |w| w.write_message(m))? },
            TW::NEAR::Proto::mod_Action::OneOfpayload::None => {},
    }        Ok(())
    }
}

pub mod mod_Action {

use super::*;

#[derive(Debug, PartialEq, Clone)]
pub enum OneOfpayload<'a> {
    create_account(TW::NEAR::Proto::CreateAccount),
    deploy_contract(TW::NEAR::Proto::DeployContract<'a>),
    function_call(TW::NEAR::Proto::FunctionCall<'a>),
    transfer(TW::NEAR::Proto::Transfer<'a>),
    stake(TW::NEAR::Proto::Stake<'a>),
    add_key(TW::NEAR::Proto::AddKey<'a>),
    delete_key(TW::NEAR::Proto::DeleteKey<'a>),
    delete_account(TW::NEAR::Proto::DeleteAccount<'a>),
    None,
}

impl<'a> Default for OneOfpayload<'a> {
    fn default() -> Self {
        OneOfpayload::None
    }
}

}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningInput<'a> {
    pub signer_id: Cow<'a, str>,
    pub nonce: u64,
    pub receiver_id: Cow<'a, str>,
    pub block_hash: Cow<'a, [u8]>,
    pub actions: Vec<TW::NEAR::Proto::Action<'a>>,
    pub private_key: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for SigningInput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.signer_id = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(16) => msg.nonce = r.read_uint64(bytes)?,
                Ok(26) => msg.receiver_id = r.read_string(bytes).map(Cow::Borrowed)?,
                Ok(34) => msg.block_hash = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(42) => msg.actions.push(r.read_message::<TW::NEAR::Proto::Action>(bytes)?),
                Ok(50) => msg.private_key = r.read_bytes(bytes).map(Cow::Borrowed)?,
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
        + if self.signer_id == "" { 0 } else { 1 + sizeof_len((&self.signer_id).len()) }
        + if self.nonce == 0u64 { 0 } else { 1 + sizeof_varint(*(&self.nonce) as u64) }
        + if self.receiver_id == "" { 0 } else { 1 + sizeof_len((&self.receiver_id).len()) }
        + if self.block_hash == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.block_hash).len()) }
        + self.actions.iter().map(|s| 1 + sizeof_len((s).get_size())).sum::<usize>()
        + if self.private_key == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.private_key).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.signer_id != "" { w.write_with_tag(10, |w| w.write_string(&**&self.signer_id))?; }
        if self.nonce != 0u64 { w.write_with_tag(16, |w| w.write_uint64(*&self.nonce))?; }
        if self.receiver_id != "" { w.write_with_tag(26, |w| w.write_string(&**&self.receiver_id))?; }
        if self.block_hash != Cow::Borrowed(b"") { w.write_with_tag(34, |w| w.write_bytes(&**&self.block_hash))?; }
        for s in &self.actions { w.write_with_tag(42, |w| w.write_message(s))?; }
        if self.private_key != Cow::Borrowed(b"") { w.write_with_tag(50, |w| w.write_bytes(&**&self.private_key))?; }
        Ok(())
    }
}

#[allow(clippy::derive_partial_eq_without_eq)]
#[derive(Debug, Default, PartialEq, Clone)]
pub struct SigningOutput<'a> {
    pub signed_transaction: Cow<'a, [u8]>,
    pub hash: Cow<'a, [u8]>,
}

impl<'a> MessageRead<'a> for SigningOutput<'a> {
    fn from_reader(r: &mut BytesReader, bytes: &'a [u8]) -> Result<Self> {
        let mut msg = Self::default();
        while !r.is_eof() {
            match r.next_tag(bytes) {
                Ok(10) => msg.signed_transaction = r.read_bytes(bytes).map(Cow::Borrowed)?,
                Ok(18) => msg.hash = r.read_bytes(bytes).map(Cow::Borrowed)?,
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
        + if self.signed_transaction == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.signed_transaction).len()) }
        + if self.hash == Cow::Borrowed(b"") { 0 } else { 1 + sizeof_len((&self.hash).len()) }
    }

    fn write_message<W: WriterBackend>(&self, w: &mut Writer<W>) -> Result<()> {
        if self.signed_transaction != Cow::Borrowed(b"") { w.write_with_tag(10, |w| w.write_bytes(&**&self.signed_transaction))?; }
        if self.hash != Cow::Borrowed(b"") { w.write_with_tag(18, |w| w.write_bytes(&**&self.hash))?; }
        Ok(())
    }
}

